#include "connector.h"
#include "debug.h"
#include "clientmanager.h"
#include "msgdispatch.h"
#include "app.h"

CConnectorTest::CConnectorTest()
	: m_pNetConnector(nullptr)
	, m_nTimerId(INVALID_TIMER_ID)
{
	m_nTimerId = CApp::getMe().SetTimer(this, 1500);

	CApp::getMe().KillTimer(m_nTimerId);
	m_nTimerId = INVALID_TIMER_ID;
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
	LOGPrint("客户端与服务器连接");

	// 发送测试心跳
	gproto::gather::MSG_C2G_HeartBeat msgHeartBeat;
	SendMessage(gproto::gather::CSID_C2G_HeartBeat, &msgHeartBeat);

	if (0)
	{
		// 背包扩充
		GatherBagExtend(100012, "sexter", 140003, 100033, 4);
		// 副本进入次数购买日志
		GatherBuyInstance(100012, "sexter", 140003, 100033, 3, 10000, 3);
		// 游戏币购买日志
		GatherBuyCoin(100012, "sexter", 140003, 100033, 2, 20, 200000);
	}

	if (0)
	{
		// 钻石产生或销毁
		GatherDiamond("122", 100012, "sexter", 22, 1, 5, 20, 245);
		// 金币产生或销毁
		GatherCoin(100012, "sexter", 22, 1, 3, 21, 233);
		// 道具产生或消耗
		gproto::gather::INFO_Equipment infoEquipment;
		gproto::gather::INFO_ItemMes *pItemMes = infoEquipment.mutable_basemes();
		if (pItemMes)
		{
			pItemMes->set_id(1);
			pItemMes->set_itemsn(100014);
			pItemMes->set_num(12);
		}
		infoEquipment.set_strelevel(1);
		infoEquipment.set_curholenum(2);
		GatherItem(100033, 1, 2, 1, 1, 100024, "sexter", 22, infoEquipment);
	}

	if (0)
	{
		// 道具产生或消耗
		gproto::gather::INFO_Equipment infoEquipmentBefore;
		gproto::gather::INFO_ItemMes *pItemMes1 = infoEquipmentBefore.mutable_basemes();
		if (pItemMes1)
		{
			pItemMes1->set_id(1);
			pItemMes1->set_itemsn(100014);
			pItemMes1->set_num(12);
		}
		infoEquipmentBefore.set_strelevel(1);
		infoEquipmentBefore.set_curholenum(2);

		gproto::gather::INFO_Equipment infoEquipmentAfter;
		gproto::gather::INFO_ItemMes *pItemMes2 = infoEquipmentAfter.mutable_basemes();
		if (pItemMes2)
		{
			pItemMes2->set_id(2);
			pItemMes2->set_itemsn(100016);
			pItemMes2->set_num(13);
		}
		infoEquipmentAfter.set_strelevel(2);
		infoEquipmentAfter.set_curholenum(3);

		GatherEquip(100033, "sexter", 100011, 1002, 2, infoEquipmentBefore, infoEquipmentAfter);
	}

	if (0)
	{
		// 死亡复活日志
		GatherRelive(100033, "sexter", 12, 1000040, 20);

		// 副本日志
		std::vector<int> vtRoleId;
		vtRoleId.push_back(3333);
		vtRoleId.push_back(2323);
		GatherInstance(100022, 3, 122, 3, vtRoleId);

		// 竞技场日志
		GatherArena(102323, "sexter", 23, 1);
	}

	return true;
}

bool CConnectorTest::OnDisconnect()
{
	LOGPrint("客户端与服务器断开");

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

	return CMsgDispatch::getMe().Dispatch(wProtocolId, pMessage, nMessageLength);
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

	return m_pNetConnector->DoSend(szSendBuffer, sizeof(unsigned short) + pMessage->ByteSize());
}

void CConnectorTest::Disconnect()
{
	if (m_pNetConnector)
	{
		m_pNetConnector->DoClose();
	}
}

bool CConnectorTest::OnTimerEvent(int nTimerId)
{
	LOGPrint("timer[" + nTimerId + "]计时器触发.");

	static int n = 0;
	++n;
	if (n > 5)
	{
		
	}

	return true;
}

void CConnectorTest::OnKilled(int nTimerId)
{
	LOGPrint("计时器[" + nTimerId + "]销毁");
}

bool CConnectorTest::GatherBagExtend(int nRoleId, std::string strRoleName, int nRoleSN, int nType, int nExtendNum)
{
	gproto::gather::MSG_O2G_BagExtend msg;
	msg.set_roleid(nRoleId);
	msg.set_rolename(strRoleName);
	msg.set_rolesn(nRoleSN);
	msg.set_type(nType);
	msg.set_extendnum(nExtendNum);

	return SendMessage(gproto::gather::CSID_O2G_BagExtend, &msg);
}

bool CConnectorTest::GatherBuyInstance(int nRoleId, std::string strRoleName, int nRoleSN, 
									   int nRoleLevel, int nVipLevel, int nInstanceSN, int nBuyCount)
{
	gproto::gather::MSG_O2G_BuyInstance msg;
	msg.set_roleid(nRoleId);
	msg.set_rolename(strRoleName);
	msg.set_rolesn(nRoleSN);	
	msg.set_rolelevel(nRoleLevel);
	msg.set_viplevel(nVipLevel);
	msg.set_instancesn(nInstanceSN);
	msg.set_buycount(nBuyCount);

	return SendMessage(gproto::gather::CSID_O2G_BuyInstance, &msg);
}

bool CConnectorTest::GatherBuyCoin(int nRoleId, std::string strRoleName, int nRoleSN, 
								   int nRoleLevel, int nVipLevel, int nSpendDiamond, int nCoin)
{
	gproto::gather::MSG_O2G_BuyCoin msg;
	msg.set_roleid(nRoleId);
	msg.set_rolename(strRoleName);
	msg.set_rolesn(nRoleSN);	
	msg.set_rolelevel(nRoleLevel);
	msg.set_viplevel(nVipLevel);
	msg.set_spenddiamond(nSpendDiamond);
	msg.set_coin(nCoin);

	return SendMessage(gproto::gather::CSID_O2G_BuyCoin, &msg);
}

bool CConnectorTest::GatherDiamond(std::string strAccount, int nRoleId, std::string strRoleName, 
								   int nRoleLevel, int nType, int nSubType, int nChangeDiamond, int nTotalDiamond)
{
	gproto::gather::MSG_O2G_Diamond msg;
	msg.set_account(strAccount);
	msg.set_roleid(nRoleId);
	msg.set_rolename(strRoleName);
	msg.set_rolelevel(nRoleLevel);
	msg.set_type(nType);
	msg.set_subtype(nSubType);
	msg.set_changediamond(nChangeDiamond);
	msg.set_totaldiamond(nTotalDiamond);

	return SendMessage(gproto::gather::CSID_O2G_Diamond, &msg);
}

bool CConnectorTest::GatherCoin(int nRoleId, std::string strRoleName, int nRoleLevel, int nType, 
								int nSubType, int nChangeCoin, int nTotalCoin)
{
	gproto::gather::MSG_O2G_Coin msg;
	msg.set_roleid(nRoleId);
	msg.set_rolename(strRoleName);
	msg.set_rolelevel(nRoleLevel);
	msg.set_type(nType);
	msg.set_subtype(nSubType);
	msg.set_changecoin(nChangeCoin);
	msg.set_totalcoin(nTotalCoin);

	return SendMessage(gproto::gather::CSID_O2G_Coin, &msg);
}

bool CConnectorTest::GatherItem(int nItemSN, int nItemType, int nNum, int nType, int nWay, 
								int nRoleId, std::string strRoleName, int nRoleLevel, gproto::gather::INFO_Equipment &infoEquipMent)
{
	gproto::gather::MSG_O2G_Item msg;
	msg.set_itemsn(nItemSN);
	msg.set_itemtype(nItemType);
	msg.set_num(nNum);
	msg.set_type(nType);
	msg.set_way(nWay);
	msg.set_roleid(nRoleId);
	msg.set_rolename(strRoleName);
	msg.set_rolelevel(nRoleLevel);
	gproto::gather::INFO_Equipment *pInfoEquipment = msg.mutable_equip();
	if (pInfoEquipment)
	{
		*pInfoEquipment = infoEquipMent;
	}

	return SendMessage(gproto::gather::CSID_O2G_Item, &msg);
}

bool CConnectorTest::GatherEquip(int nRoleId, std::string strRoleName, int nEquipSN, int nEquipId, int nMakeType, 
								 gproto::gather::INFO_Equipment &equipMakeBefore, gproto::gather::INFO_Equipment &equipMakeAfter)
{
	gproto::gather::MSG_O2G_Equip msg;
	msg.set_roleid(nRoleId);
	msg.set_rolename(strRoleName);
	msg.set_equipsn(nEquipSN);
	msg.set_equipid(nEquipId);
	msg.set_maketype(nMakeType);
	
	gproto::gather::INFO_Equipment *pInfoEquipmentBefore = msg.mutable_equipbefore();
	if (pInfoEquipmentBefore)
	{
		*pInfoEquipmentBefore = equipMakeBefore;
	}

	gproto::gather::INFO_Equipment *pInfoEquipmentAfter = msg.mutable_equipafter();
	if (pInfoEquipmentAfter)
	{
		*pInfoEquipmentAfter = equipMakeAfter;
	}

	return SendMessage(gproto::gather::CSID_O2G_Equip, &msg);
}

bool CConnectorTest::GatherRelive(int nRoleId, std::string strRoleName, int nRoleLevel, int nInstanceSN, int nWasteDiamond)
{
	gproto::gather::MSG_O2G_Relive msg;
	msg.set_roleid(nRoleId);
	msg.set_rolename(strRoleName);
	msg.set_rolelevel(nRoleLevel);
	msg.set_instancesn(nInstanceSN);
	msg.set_wastediamond(nWasteDiamond);

	return SendMessage(gproto::gather::CSID_O2G_Relive, &msg);
}

bool CConnectorTest::GatherInstance(int nInstanceSN, int nResult, int nPassTime, int nDeadTimes, std::vector<int> vtRoleId)
{
	gproto::gather::MSG_O2G_Instance msg;
	msg.set_instancesn(nInstanceSN);
	msg.set_result(nResult);
	msg.set_passtime(nPassTime);
	msg.set_deadtimes(nDeadTimes);
	for (int &nRoleId : vtRoleId)
	{
		msg.add_roleid(nRoleId);
	}

	return SendMessage(gproto::gather::CSID_O2G_Instance, &msg);
}

bool CConnectorTest::GatherArena(int nRoleId, std::string strRoleName, int nRoleLevel, int nResult)
{
	gproto::gather::MSG_O2G_Arena msg;
	msg.set_roleid(nRoleId);
	msg.set_rolename(strRoleName);
	msg.set_rolelevel(nRoleLevel);
	msg.set_result(nResult);

	return SendMessage(gproto::gather::CSID_O2G_Arena, &msg);
}


