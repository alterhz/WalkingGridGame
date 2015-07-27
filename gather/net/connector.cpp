#include "connector.h"
#include "debug.h"
#include "clientmanager.h"
#include "msgdispatch.h"
#include "app.h"

CConnectorTest::CConnectorTest()
	: m_pNetConnector(nullptr)
	, m_pNetSocket(nullptr)
	, m_nTimerId(INVALID_TIMER_ID)
{
	m_nTimerId = CApp::getMe().SetTimer(this, 1500);
}

CConnectorTest::~CConnectorTest()
{
	CApp::getMe().KillTimer(m_nTimerId);
	m_nTimerId = INVALID_TIMER_ID;
}

void CConnectorTest::Init(INetConnector *pNetConnector)
{
	if (nullptr == pNetConnector)
	{
		return ;
	}

	m_pNetConnector = pNetConnector;

	m_pNetConnector->Connect(this, "127.0.0.1", 8000);
}

bool CConnectorTest::OnConnected(INetSocket *pNetSocket)
{
	if (nullptr == pNetSocket)
	{
		return false;
	}

	m_pNetSocket = pNetSocket;

	LOGPrint("客户端与服务器连接");

	// 发送测试心跳
	gproto::gather::MSG_C2G_HeartBeat msgHeartBeat;
	SendMessage(gproto::gather::CSID_C2G_HeartBeat, &msgHeartBeat);

	return true;
}

bool CConnectorTest::OnDisconnect()
{
	LOGPrint("客户端与服务器断开");

	m_pNetSocket = nullptr;

	return true;
}

bool CConnectorTest::OnRecvPacket(const char *pPacket, unsigned short wLength)
{
	//LOGPrint("收到一条长度[" + wLength + "]的消息");

	// 协议解析
	const unsigned short *pProtocolId = reinterpret_cast<const unsigned short *>(pPacket);
	unsigned short wProtocolId = *pProtocolId;

	const char *pMessage = pPacket + sizeof(unsigned short);

	int nMessageLength = wLength - sizeof(unsigned short);

	switch (wProtocolId)
	{
	case gproto::gather::CSID_G2C_HeartBeat:
		{
			Disconnect();
		}
		break;
	default:
		break;
	}

	return true;
}

bool CConnectorTest::SendMessage(unsigned short wProtocolId, google::protobuf::Message *pMessage)
{
	if (nullptr == m_pNetConnector)
	{
		return false;
	}

	char szSendBuffer[MAX_SENDBUFFER_LENGTH] = {0};

	// 协议编号
	unsigned short *pMessageLength = reinterpret_cast<unsigned short *>(szSendBuffer);
	*pMessageLength = wProtocolId;

	void *pData = reinterpret_cast<void *>(szSendBuffer + sizeof(unsigned short));

	// 协议内容编码
	if (!pMessage->SerializePartialToArray(pData, MAX_SENDBUFFER_LENGTH-sizeof(unsigned short)))
	{
		LOGPrint("协议[" + wProtocolId + "]编码失败。");
		return false;
	}

	return m_pNetSocket->DoSend(szSendBuffer, sizeof(unsigned short) + pMessage->ByteSize());
}

void CConnectorTest::Disconnect()
{
	if (m_pNetSocket)
	{
		m_pNetSocket->DoClose();
	}
}

bool CConnectorTest::OnTimerEvent(int nTimerId)
{
	LOGPrint("timer[" + nTimerId + "]计时器触发.");

	static int n = 0;
	++n;
	if (n >= 3)
	{
		CApp::getMe().KillTimer(m_nTimerId);
		m_nTimerId = INVALID_TIMER_ID;
	}

	return true;
}

void CConnectorTest::OnKilled(int nTimerId)
{
	LOGPrint("计时器[" + nTimerId + "]销毁");
}

