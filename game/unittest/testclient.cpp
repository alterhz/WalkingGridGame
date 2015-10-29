#include "testclient.h"
#include "debug.h"

CTestClient::CTestClient()
	: m_nId(0)
	, m_pNetConnector(nullptr)
	, m_pNetSocket(nullptr)
	, m_tBegin(0)
	, m_nCountryIndexId(0)
{
	static int g_nID = 0;
	m_nId = (++g_nID);
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
			gproto::MSG_G2C_PrepareGround msg;
			if (!msg.ParseFromArray(pMessage, nMessageLength))
			{
				LOGError("协议CSID_G2C_PrepareGround解码失败。");
				return false;
			}
			
			m_nCountryIndexId = msg.countryindexid();

			LOGDebug("[" + m_nId + "]CountryIndexId[" + m_nCountryIndexId + "]匹配成功，准备场景。");

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
				LOGDebug("[" + m_nId + "]进入场景失败。");
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

			LOGPrint("[" + m_nId + "]场景尺寸W:" + msgGetGroundInfo.wgcount() + ",H:" + msgGetGroundInfo.hgcount());

			/*
			for (int i=0; i<msgGetGroundInfo.grids_size(); ++i)
			{
				const gproto::Info_Grid &infoGrid = msgGetGroundInfo.grids(i);

				LOGPrint("SN:" + infoGrid.sn() + ",X:" + infoGrid.x() + ",Y:" + infoGrid.y());
			}
			*/

			for (int i=0; i<msgGetGroundInfo.gobjects_size(); ++i)
			{
				const gproto::Info_GObject &infoGObject = msgGetGroundInfo.gobjects(i);

				if (gproto::Info_GObject_EType_Still == infoGObject.type())
				{
					LOGPrint("[" + m_nId + "]Still -- SN:" + infoGObject.sn() + ",IndexId:" + infoGObject.indexid() + ",X:" + infoGObject.x()
						+ ",Y:" + infoGObject.y() + ",HP:" + infoGObject.hp() + ",maxhp:" + infoGObject.maxhp() + ",SP:" + infoGObject.sp() 
						+ ",level:" + infoGObject.level() + ",campid:" + infoGObject.campid());
				}
				else
				{
					LOGPrint("[" + m_nId + "]Walkable -- SN:" + infoGObject.sn() + ",IndexId:" + infoGObject.indexid() + ",X:" + infoGObject.x()
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
				LOGPrint("[" + m_nId + "]Still EnterGround -- SN:" + infoGObject.sn() + ",IndexId:" + infoGObject.indexid() + ",X:" + infoGObject.x()
					+ ",Y:" + infoGObject.y() + ",HP:" + infoGObject.hp() + ",maxhp:" + infoGObject.maxhp() + ",SP:" + infoGObject.sp() 
					+ ",level:" + infoGObject.level() + ",campid:" + infoGObject.campid());
			}
			else
			{
				LOGPrint("[" + m_nId + "]Walkable EnterGround -- SN:" + infoGObject.sn() + ",IndexId:" + infoGObject.indexid() + ",X:" + infoGObject.x()
					+ ",Y:" + infoGObject.y() + ",HP:" + infoGObject.hp() + ",maxhp:" + infoGObject.maxhp() + ",SP:" + infoGObject.sp()
					+ ",level:" + infoGObject.level() + ",campid:" + infoGObject.campid());
			}
		}
		break;
	case gproto::CSID_G2C_BattlePrepare:
		{
			// 准备
			SendPrepareFinish();
		}
		break;
	case gproto::CSID_G2C_BattleStart:
		{
			// 战斗开始
			LOGPrint("[" + m_nId + "]战斗开始");
		}
		break;
	case gproto::CSID_G2C_BattleReward:
		{
			// 奖励
			gproto::MSG_G2C_BattleReward msg;
			if (!msg.ParseFromArray(pMessage, nMessageLength))
			{
				LOGError("协议CSID_G2C_BattleReward解码失败。");
				return false;
			}

			LOGDebug("[" + m_nId + "]胜利的Country：" + msg.wincountryindexid());
		}
		break;
	case gproto::CSID_G2C_BattleFight:
		{
			// 战斗轮流
			gproto::MSG_G2C_BattleFight msg;
			if (!msg.ParseFromArray(pMessage, nMessageLength))
			{
				LOGError("协议CSID_G2C_BattleFight解码失败。");
				return false;
			}

			if (msg.countryindexid() == m_nCountryIndexId)
			{
				// 轮到自己的回合
				LOGDebug("[" + m_nId + "]Country[" + msg.countryindexid() + "]自己的回合，可以攻击。");

				{
					// 攻击逻辑
				}

				static int n = 0;
				++n;
				if (n < 10)
				{
					// 攻击完毕
					SendBattleFightEnd();
				}
			}
			else
			{
				LOGDebug("[" + m_nId + "]Country[" + msg.countryindexid() + "]其他人的回合，等待。");
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

bool CTestClient::SendPrepareFinish()
{
	gproto::MSG_C2G_PrepareFinish msg;
	return SendMessage(gproto::CSID_C2G_PrepareFinish, &msg);
}

bool CTestClient::SendBattleFightEnd()
{
	gproto::MSG_C2G_BattleFightEnd msg;
	return SendMessage(gproto::CSID_C2G_BattleFightEnd, &msg);
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
