#include "client.h"
#include "debug.h"
#include "clientmanager.h"
#include "country.h"

CClient::CClient()
	: m_pNetSocket(nullptr)
	, m_pCountry(nullptr)
{
}

bool CClient::OnConnected(INetSocket *pNetSocket)
{
	if (nullptr == pNetSocket)
	{
		return false;
	}

	m_pNetSocket = pNetSocket;

	LOGInfo("有一个客户端连接[localIP:" + m_pNetSocket->GetLocalIP() + ",localPort:"
		+ m_pNetSocket->GetLocalPort() + ", remoteIP:" + m_pNetSocket->GetRemoteIP() + ", remotePort:"
		+ m_pNetSocket->GetRemotePort() + "]。");

	return true;
}

bool CClient::OnDisconnect()
{
	LOGInfo("有一个客户端断开");

	if (m_pCountry)
	{
		m_pCountry->OnDisconnect();
		m_pCountry = nullptr;
	}
	
	m_pNetSocket = nullptr;

	return true;
}

bool CClient::OnRecvPacket(const char *pPacket, unsigned short wLength)
{
	// 协议解析
	const unsigned short *pProtocolId = reinterpret_cast<const unsigned short *>(pPacket);
	unsigned short wProtocolId = *pProtocolId;

	const char *pMessage = pPacket + sizeof(unsigned short);

	int nMessageLength = wLength - sizeof(unsigned short);

	CClientManager::getMe().gDM.RunExcute(wProtocolId, pMessage, nMessageLength, this);
	return true;
}

bool CClient::SendMessage(unsigned short wProtocolId, google::protobuf::Message *pMessage)
{
	if (nullptr == m_pNetSocket)
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

bool CClient::SendPrepare(bool bOk)
{
	gproto::MSG_G2C_Prepare msg;
	if (bOk)
	{
		msg.set_ret(gproto::MSG_G2C_Prepare_EResult_OK);
	}
	else
	{
		msg.set_ret(gproto::MSG_G2C_Prepare_EResult_ERR);
	}

	return SendMessage(gproto::CSID_G2C_Prepare, &msg);
}
