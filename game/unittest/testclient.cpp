#include "testclient.h"
#include "debug.h"

CTestClient::CTestClient()
{

}

CTestClient::~CTestClient()
{

}

bool CTestClient::Init(INetConnector *pNetConnector)
{
	if (nullptr == pNetConnector)
	{
		return false;
	}

	m_pNetConnector = pNetConnector;

	return m_pNetConnector->Connect(this, "127.0.0.1", 8000);
}

bool CTestClient::SendMessage(unsigned short wProtocolId, google::protobuf::Message *pMessage)
{
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

	if (nullptr == m_pNetSocket)
	{
		return false;
	}

	return m_pNetSocket->DoSend(szSendBuffer, sizeof(unsigned short) + pMessage->ByteSize());
}

void CTestClient::Disconnect()
{
	if (m_pNetSocket)
	{
		m_pNetSocket->DoClose();
	}
}

bool CTestClient::OnConnected(INetSocket *pNetSocket)
{
	if (nullptr == pNetSocket)
	{
		return false;
	}

	m_pNetSocket = pNetSocket;

	LOGInfo("Test客户端连接上服务器[localIP:" + m_pNetSocket->GetLocalIP() + ",localPort:"
		+ m_pNetSocket->GetLocalPort() + ", remoteIP:" + m_pNetSocket->GetRemoteIP() + ", remotePort:"
		+ m_pNetSocket->GetRemotePort() + "]。");

	//// 发送测试心跳
	//gproto::MSG_C2G_HeartBeat msgHeartBeat;
	//SendMessage(gproto::CSID_C2G_HeartBeat, &msgHeartBeat);

	//gproto::MSG_C2G_StartGame msgStartGame;
	//msgStartGame.set_rolename("惊天一棍");
	//SendMessage(gproto::CSID_C2G_StartGame, &msgStartGame);

	SendPrepare();

	return true;
}

bool CTestClient::OnDisconnect()
{
	LOGInfo("Test客户端与服务器断开网络连接。");

	m_pNetSocket = nullptr;

	return true;
}

bool CTestClient::OnRecvPacket(const char *pPacket, unsigned short wLength)
{
	// 协议解析
	const unsigned short *pProtocolId = reinterpret_cast<const unsigned short *>(pPacket);
	unsigned short wProtocolId = *pProtocolId;

	const char *pMessage = pPacket + sizeof(unsigned short);

	int nMessageLength = wLength - sizeof(unsigned short);

	switch (wProtocolId)
	{
	case gproto::CSID_G2C_HeartBeat:
		{
			gproto::MSG_G2C_HeartBeat msgHeartBeat;
			if (!msgHeartBeat.ParseFromArray(pMessage, nMessageLength))
			{
				LOGError("协议CSID_G2C_HeartBeat解码失败。");
				return false;
			}

			LOGDebug("回发心跳CSID_G2C_HeartBeat");
		}
		break;
	case gproto::CSID_G2C_PrepareGround:
		{
			// 准备场景
			SendEnterGround();
		}
		break;
	case gproto::CSID_G2C_EnterGround:
		{
			gproto::MSG_G2C_EnterGround msgEnterGround;
			if (!msgEnterGround.ParseFromArray(pMessage, nMessageLength))
			{
				LOGError("协议CSID_G2C_EnterGround解码失败。");
				return false;
			}

			// 进入场景成功
			if (gproto::MSG_G2C_EnterGround_EResult_ERR == msgEnterGround.ret())
			{
				LOGDebug("进入场景失败。");
			}
			else
			{
				SendGetGroundInfo();
			}
		}
		break;
	case gproto::CSID_G2C_GetGroundInfo:
		{
			gproto::MSG_G2C_GetGroundInfo msgGetGroundInfo;
			if (!msgGetGroundInfo.ParseFromArray(pMessage, nMessageLength))
			{
				LOGError("协议CSID_G2C_GetGroundInfo解码失败。");
				return false;
			}

			LOGPrint("场景尺寸W:" + msgGetGroundInfo.wgcount() + ",H:" + msgGetGroundInfo.hgcount());

			for (int i=0; i<msgGetGroundInfo.grids_size(); ++i)
			{
				const gproto::Info_Grid &infoGrid = msgGetGroundInfo.grids(i);

				LOGPrint("SN:" + infoGrid.sn() + ",X:" + infoGrid.x() + ",Y:" + infoGrid.y());
			}

			for (int i=0; i<msgGetGroundInfo.gobjects_size(); ++i)
			{
				const gproto::Info_GObject &infoGObject = msgGetGroundInfo.gobjects(i);

				if (gproto::Info_GObject_EType_Still == infoGObject.type())
				{
					LOGPrint("Still -- SN:" + infoGObject.sn() + ",IndexId:" + infoGObject.indexid() + ",X:" + infoGObject.x()
						+ ",Y:" + infoGObject.y() + ",HP:" + infoGObject.hp() + ",maxhp:" + infoGObject.maxhp() + ",SP:" + infoGObject.sp() 
						+ ",level:" + infoGObject.level() + ",campid:" + infoGObject.campid());
				}
				else
				{
					LOGPrint("Walkable -- SN:" + infoGObject.sn() + ",IndexId:" + infoGObject.indexid() + ",X:" + infoGObject.x()
						+ ",Y:" + infoGObject.y() + ",HP:" + infoGObject.hp() + ",maxhp:" + infoGObject.maxhp() + ",SP:" + infoGObject.sp()
						+ ",level:" + infoGObject.level() + ",campid:" + infoGObject.campid());
				}
			}
		}
		break;
	case gproto::CSID_G2C_GObjectEnterGround:
		{
			gproto::MSG_G2C_GObjectEnterGround msgGObjectEnterGround;
			if (!msgGObjectEnterGround.ParseFromArray(pMessage, nMessageLength))
			{
				LOGError("协议CSID_G2C_GObjectEnterGround解码失败。");
				return false;
			}

			const gproto::Info_GObject &infoGObject = msgGObjectEnterGround.gobject();

			if (gproto::Info_GObject_EType_Still == infoGObject.type())
			{
				LOGPrint("Still EnterGround -- SN:" + infoGObject.sn() + ",IndexId:" + infoGObject.indexid() + ",X:" + infoGObject.x()
					+ ",Y:" + infoGObject.y() + ",HP:" + infoGObject.hp() + ",maxhp:" + infoGObject.maxhp() + ",SP:" + infoGObject.sp() 
					+ ",level:" + infoGObject.level() + ",campid:" + infoGObject.campid());
			}
			else
			{
				LOGPrint("Walkable EnterGround -- SN:" + infoGObject.sn() + ",IndexId:" + infoGObject.indexid() + ",X:" + infoGObject.x()
					+ ",Y:" + infoGObject.y() + ",HP:" + infoGObject.hp() + ",maxhp:" + infoGObject.maxhp() + ",SP:" + infoGObject.sp()
					+ ",level:" + infoGObject.level() + ",campid:" + infoGObject.campid());
			}	
		}
		break;
	default:
		break;
	}

	return true;
}

bool CTestClient::OnError(int nErrCode)
{
	LOGInfo("连接服务器失败。");

	return true;
}

bool CTestClient::SendPrepare()
{
	gproto::MSG_C2G_Prepare msg;
	return SendMessage(gproto::CSID_C2G_Prepare, &msg);
}

bool CTestClient::SendEnterGround()
{
	gproto::MSG_C2G_EnterGround msg;
	return SendMessage(gproto::CSID_C2G_EnterGround, &msg);
}

bool CTestClient::SendGetGroundInfo()
{
	gproto::MSG_C2G_GetGroundInfo msg;
	return SendMessage(gproto::CSID_C2G_GetGroundInfo, &msg);
}


CTestClientManager::CTestClientManager()
	: m_pTestClientA(new CTestClient)
	, m_pTestClientB(new CTestClient)
{
}

CTestClientManager::~CTestClientManager()
{
	delete m_pTestClientA;
	m_pTestClientA = nullptr;

	delete m_pTestClientB;
	m_pTestClientB = nullptr;
}

void CTestClientManager::Init(NS_IO::INetService *pNetService)
{
	if (nullptr == pNetService)
	{
		LOGError("nullptr == pNetService");
		return ;
	}

	if (nullptr == m_pTestClientA || nullptr == m_pTestClientB)
	{
		LOGError("nullptr == m_pTestClientA || nullptr == m_pTestClientB");
		return ;
	}

	INetConnector *pNetConnectorA = pNetService->CreateConnector();
	if (pNetConnectorA)
	{
		m_pTestClientA->Init(pNetConnectorA);
	}

	INetConnector *pNetConnectorB = pNetService->CreateConnector();
	if (pNetConnectorB)
	{
		m_pTestClientB->Init(pNetConnectorB);
	}
}
