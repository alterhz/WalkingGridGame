#include "msgdispatch.h"
#include "client.h"
#include "debug.h"
#include "ProtocolFactory.h"
#include "dbevent.h"
#include "app.h"

CMsgDispatch::CMsgDispatch()
	: m_pCurrentClient(nullptr)
	, m_pProtocolFactory(nullptr)
{
	m_pProtocolFactory = new CProtocolFactory();
}

CMsgDispatch::~CMsgDispatch()
{
	delete m_pProtocolFactory;
	m_pProtocolFactory = nullptr;
}

bool CMsgDispatch::Dispatch(unsigned short wProtocolID, const char *pBuffer, int nLength)
{
	if (nullptr == m_pProtocolFactory)
	{
		return false;
	}

	google::protobuf::Message *pMessage = m_pProtocolFactory->CreateMessage(wProtocolID);

	if (nullptr == pMessage)
	{
		return false;
	}

	// 重置清理协议
	m_pProtocolFactory->ClearMessage(pMessage);

	if (!pMessage->ParseFromArray(pBuffer, nLength))
	{
		LOGPrint("协议[id:" + wProtocolID + ", length:" + nLength + "]解码失败。")
		return false;
	}

	switch (wProtocolID)
	{
	case gproto::gather::CSID_G2C_HeartBeat:
		{
			gproto::gather::MSG_G2C_HeartBeat *pMsgHeartBeat = dynamic_cast<gproto::gather::MSG_G2C_HeartBeat *>(pMessage);
			return OnMsgG2CHeartBeat(pMsgHeartBeat);
		}
		break;
	case gproto::gather::CSID_C2G_HeartBeat:
		{
			gproto::gather::MSG_C2G_HeartBeat *pMsgHeartBeat = dynamic_cast<gproto::gather::MSG_C2G_HeartBeat *>(pMessage);
			return OnMsgC2GHeartBeat(pMsgHeartBeat);
		}
		break;
	case gproto::gather::CSID_O2G_Deposit:
		{
			gproto::gather::MSG_O2G_Deposit *pMsgDeposit = dynamic_cast<gproto::gather::MSG_O2G_Deposit *>(pMessage);
			return OnMsgO2GDeposit(pMsgDeposit);
		}
		break;
	case gproto::gather::CSID_O2G_AccountReg:
		{
			gproto::gather::MSG_O2G_AccountReg *pMsgAccountReg = dynamic_cast<gproto::gather::MSG_O2G_AccountReg *>(pMessage);
			return OnMsgO2GAccountReg(pMsgAccountReg);
		}
		break;
	case gproto::gather::CSID_O2G_RoleCreate:
		{
			gproto::gather::MSG_O2G_RoleCreate *pMsgRoleCreate = dynamic_cast<gproto::gather::MSG_O2G_RoleCreate *>(pMessage);
			return OnMsgO2GRoleCreate(pMsgRoleCreate);
		}
		break;
	case gproto::gather::CSID_O2G_Login:
		{
			gproto::gather::MSG_O2G_Login *pMsg = dynamic_cast<gproto::gather::MSG_O2G_Login *>(pMessage);
			return OnMsgO2GLogin(pMsg);
		}
		break;
	case gproto::gather::CSID_O2G_BagExtend:
		{
			gproto::gather::MSG_O2G_BagExtend *pMsg = dynamic_cast<gproto::gather::MSG_O2G_BagExtend *>(pMessage);
			return OnMsgO2GBagExtend(pMsg);
		}
		break;
	case gproto::gather::CSID_O2G_BuyInstance:
		{
			gproto::gather::MSG_O2G_BuyInstance *pMsg = dynamic_cast<gproto::gather::MSG_O2G_BuyInstance *>(pMessage);
			return OnMsgO2GBuyInstance(pMsg);
		}
		break;
	case gproto::gather::CSID_O2G_BuyCoin:
		{
			gproto::gather::MSG_O2G_BuyCoin *pMsg = dynamic_cast<gproto::gather::MSG_O2G_BuyCoin *>(pMessage);
			return OnMsgO2GBuyCoin(pMsg);
		}
		break;
	case gproto::gather::CSID_O2G_Diamond:
		{
			gproto::gather::MSG_O2G_Diamond *pMsg = dynamic_cast<gproto::gather::MSG_O2G_Diamond *>(pMessage);
			return OnMsgO2GDiamond(pMsg);
		}
		break;
	case gproto::gather::CSID_O2G_Coin:
		{
			gproto::gather::MSG_O2G_Coin *pMsg = dynamic_cast<gproto::gather::MSG_O2G_Coin *>(pMessage);
			return OnMsgO2GCoin(pMsg);
		}
		break;
	case gproto::gather::CSID_O2G_Item:
		{
			gproto::gather::MSG_O2G_Item *pMsg = dynamic_cast<gproto::gather::MSG_O2G_Item *>(pMessage);
			return OnMsgO2GItem(pMsg);
		}
		break;
	case gproto::gather::CSID_O2G_Equip:
		{
			gproto::gather::MSG_O2G_Equip *pMsg = dynamic_cast<gproto::gather::MSG_O2G_Equip *>(pMessage);
			return OnMsgO2GEquip(pMsg);
		}
		break;
	case gproto::gather::CSID_O2G_Relive:
		{
			gproto::gather::MSG_O2G_Relive *pMsg = dynamic_cast<gproto::gather::MSG_O2G_Relive *>(pMessage);
			return OnMsgO2GRelive(pMsg);
		}
		break;
	case gproto::gather::CSID_O2G_Instance:
		{
			gproto::gather::MSG_O2G_Instance *pMsg = dynamic_cast<gproto::gather::MSG_O2G_Instance *>(pMessage);
			return OnMsgO2GInstance(pMsg);
		}
		break;
	case gproto::gather::CSID_O2G_Arena:
		{
			gproto::gather::MSG_O2G_Arena *pMsg = dynamic_cast<gproto::gather::MSG_O2G_Arena *>(pMessage);
			return OnMsgO2GArena(pMsg);
		}
		break;
	default:
		break;
	}

	return true;
}

bool CMsgDispatch::OnMsgC2GHeartBeat(gproto::gather::MSG_C2G_HeartBeat *pMsg)
{
	LOGPrint("接收到心跳，并回发。");

	if (nullptr == m_pCurrentClient)
	{
		return false;
	}

	gproto::gather::MSG_G2C_HeartBeat msgHeartBeat;
	m_pCurrentClient->SendMessage(gproto::gather::CSID_G2C_HeartBeat, &msgHeartBeat);

	{
		// 测试代码
		// 投递异步事件
		//CApp::getMe().PostAsyncEvent(new CEvent_TestDb());
		
	}

	return true;
}

bool CMsgDispatch::OnMsgG2CHeartBeat(gproto::gather::MSG_G2C_HeartBeat *pMsg)
{
	LOGPrint("收到心跳回应。");

	return true;
}

bool CMsgDispatch::OnMsgO2GDeposit(gproto::gather::MSG_O2G_Deposit *pMsg)
{
	if (nullptr == pMsg)
	{
		return false;
	}

	// 充值测试
	CDbEvent_Deposit *pDbEvent_Deposit = new CDbEvent_Deposit(pMsg->platform(), pMsg->account(), 
		pMsg->roleid(), pMsg->rolename(), pMsg->rolesn(), pMsg->rolelevel(), pMsg->viplevel(), 
		pMsg->type(), pMsg->diamond());

	CApp::getMe().PostAsyncEvent(pDbEvent_Deposit);

	LOGPrint("接收到充值消息.");

	return true;
}

bool CMsgDispatch::OnMsgO2GAccountReg(gproto::gather::MSG_O2G_AccountReg *pMsg)
{
	if (nullptr == pMsg)
	{
		return false;
	}

	CDbEvent_AccountReg *pDbEvent_AccountReg = new CDbEvent_AccountReg(pMsg->platform(), pMsg->account());
	CApp::getMe().PostAsyncEvent(pDbEvent_AccountReg);

	LOGPrint("接收到账号注册消息.");

	return true;
}

bool CMsgDispatch::OnMsgO2GRoleCreate(gproto::gather::MSG_O2G_RoleCreate *pMsg)
{
	if (nullptr == pMsg)
	{
		return false;
	}

	CDbEvent_RoleCreate *pDbEvent_RoleCreate = new CDbEvent_RoleCreate(pMsg->platform(), pMsg->account(), 
		pMsg->rolesn(), pMsg->rolename());
	CApp::getMe().PostAsyncEvent(pDbEvent_RoleCreate);

	LOGPrint("接收到角色创建消息.");

	return true;
}

bool CMsgDispatch::OnMsgO2GLogin(gproto::gather::MSG_O2G_Login *pMsg)
{
	if (nullptr == pMsg)
	{
		return false;
	}

	CDbEvent_Login *pDbEvent_Login = new CDbEvent_Login(pMsg->platform(), pMsg->account(), 
		pMsg->rolesn(), pMsg->rolelevel(), pMsg->rolename(), pMsg->onlinetime(), pMsg->offlinetime());
	CApp::getMe().PostAsyncEvent(pDbEvent_Login);

	LOGPrint("接收到登陆日志消息.");

	return true;
}

bool CMsgDispatch::OnMsgO2GBagExtend(gproto::gather::MSG_O2G_BagExtend *pMsg)
{
	if (nullptr == pMsg)
	{
		return false;
	}

	CDbEvent_BagExtend *pDbEvent_BagExtend = new CDbEvent_BagExtend(pMsg->roleid(), 
		pMsg->rolename(), pMsg->rolesn(), pMsg->type(), pMsg->extendnum());
	CApp::getMe().PostAsyncEvent(pDbEvent_BagExtend);

	LOGPrint("接收背包扩展日志.");

	return true;
}

bool CMsgDispatch::OnMsgO2GBuyInstance(gproto::gather::MSG_O2G_BuyInstance *pMsg)
{
	if (nullptr == pMsg)
	{
		return false;
	}

	CDbEvent_BuyInstance *pDbEvent_BuyInstance = new CDbEvent_BuyInstance(pMsg->roleid(), 
		pMsg->rolename(), pMsg->rolesn(), pMsg->rolelevel(), pMsg->viplevel(), pMsg->instancesn(), pMsg->buycount());
	CApp::getMe().PostAsyncEvent(pDbEvent_BuyInstance);

	LOGPrint("接收购买副本进入次数日志.");

	return true;
}

bool CMsgDispatch::OnMsgO2GBuyCoin(gproto::gather::MSG_O2G_BuyCoin *pMsg)
{
	if (nullptr == pMsg)
	{
		return false;
	}

	CDbEvent_BuyCoin *pDbEvent_BuyCoin = new CDbEvent_BuyCoin(pMsg->roleid(), 
		pMsg->rolename(), pMsg->rolesn(), pMsg->rolelevel(), pMsg->viplevel(), pMsg->spenddiamond(), pMsg->coin());
	CApp::getMe().PostAsyncEvent(pDbEvent_BuyCoin);

	LOGPrint("接收购买游戏币日志.");

	return true;
}

bool CMsgDispatch::OnMsgO2GDiamond(gproto::gather::MSG_O2G_Diamond *pMsg)
{
	if (nullptr == pMsg)
	{
		return false;
	}

	CDbEvent_Diamond *pDbEvent_Diamond = new CDbEvent_Diamond(pMsg->account(), pMsg->roleid(), pMsg->rolename(),
		pMsg->rolelevel(), pMsg->type(), pMsg->subtype(), pMsg->changediamond(), pMsg->totaldiamond());
	CApp::getMe().PostAsyncEvent(pDbEvent_Diamond);

	LOGPrint("接收钻石产生或消耗日志.");

	return true;
}

bool CMsgDispatch::OnMsgO2GCoin(gproto::gather::MSG_O2G_Coin *pMsg)
{
	if (nullptr == pMsg)
	{
		return false;
	}

	CDbEvent_Coin *pDbEvent_Coin = new CDbEvent_Coin(pMsg->roleid(), pMsg->rolename(), pMsg->rolelevel(), 
		pMsg->type(), pMsg->subtype(), pMsg->changecoin(), pMsg->totalcoin());
	CApp::getMe().PostAsyncEvent(pDbEvent_Coin);

	LOGPrint("接收金币产生或消耗日志.");

	return true;
}

// 装备字段
std::string GetEquipString(const gproto::gather::INFO_Equipment &infoEquipment)
{
	AutoStr1024 autoStr;

	const gproto::gather::INFO_ItemMes &infoItemMes = infoEquipment.basemes();

	autoStr + "{{" + infoItemMes.id() + ", " + infoItemMes.num() + ", " + infoItemMes.itemsn() + "}{" 
		+ infoEquipment.strelevel() + ", " + infoEquipment.curholenum() + "}";

	// 基础属性
	autoStr + "{";
	for (const int &nPropty : infoEquipment.basepropty())
	{
		autoStr + "[" + nPropty + "]";
	}
	autoStr + "}";

	// 附加属性
	autoStr + "{";
	for (const gproto::gather::INFO_Property &infoProperty : infoEquipment.addtional())
	{
		autoStr + "[" + infoProperty.propertytype() + "," + infoProperty.propertyval() + "]";
	}
	autoStr + "}";

	// 镶嵌宝石
	autoStr + "{";
	for (const gproto::gather::INFO_HoleGem &infoHoleGem : infoEquipment.holegem())
	{
		autoStr + "[" + infoHoleGem.holeid() + ", " + infoHoleGem.gemsn() + "]";
	}
	autoStr + "}";

	// 最外层大括号
	autoStr + "}";

	return autoStr.GetStr();
}

bool CMsgDispatch::OnMsgO2GItem(gproto::gather::MSG_O2G_Item *pMsg)
{
	if (nullptr == pMsg)
	{
		return false;
	}

	std::string strItemAttr = "";

	if (pMsg->has_equip())
	{
		strItemAttr = GetEquipString(pMsg->equip());
	}

	CDbEvent_Item *pDbEvent_Item = new CDbEvent_Item(pMsg->itemsn(), pMsg->itemtype(), 
		pMsg->num(), pMsg->type(), pMsg->way(), pMsg->roleid(), pMsg->rolename(), 
		pMsg->rolelevel(), strItemAttr);
	CApp::getMe().PostAsyncEvent(pDbEvent_Item);

	LOGPrint("接收道具产出或消耗日志.");

	return true;
}

bool CMsgDispatch::OnMsgO2GEquip(gproto::gather::MSG_O2G_Equip *pMsg)
{
	if (nullptr == pMsg)
	{
		return false;
	}

	std::string strEquipMakeBefore = GetEquipString(pMsg->equipbefore());
	std::string strEquipMakeAfter = GetEquipString(pMsg->equipafter());

	CDbEvent_Equip *pDbEvent_Equit = new CDbEvent_Equip(pMsg->roleid(), pMsg->rolename(),
		pMsg->equipsn(), pMsg->equipid(), pMsg->maketype(), strEquipMakeBefore, strEquipMakeAfter);
	CApp::getMe().PostAsyncEvent(pDbEvent_Equit);

	LOGPrint("接收装备打造日志");

	return true;
}

bool CMsgDispatch::OnMsgO2GRelive(gproto::gather::MSG_O2G_Relive *pMsg)
{
	if (nullptr == pMsg)
	{
		return false;
	}

	CDbEvent_Relive *pDbEvent_Relive = new CDbEvent_Relive(pMsg->roleid(), pMsg->rolename(), pMsg->rolelevel(),
		pMsg->instancesn(), pMsg->wastediamond());
	CApp::getMe().PostAsyncEvent(pDbEvent_Relive);

	LOGPrint("接收复活日志");

	return true;
}

bool CMsgDispatch::OnMsgO2GInstance(gproto::gather::MSG_O2G_Instance *pMsg)
{
	if (nullptr == pMsg)
	{
		return false;
	}

	AutoStr1024 autoStr;
	autoStr + "{";
	for (const int &nRoleId : pMsg->roleid())
	{
		autoStr + nRoleId + ",";
	}
	autoStr + "}";

	std::string strRoleIdList = autoStr.GetStr();

	CDbEvent_Instance *pDbEvent_Instance = new CDbEvent_Instance(pMsg->instancesn(), pMsg->result(), 
		pMsg->passtime(), pMsg->deadtimes(), strRoleIdList);
	CApp::getMe().PostAsyncEvent(pDbEvent_Instance);

	LOGPrint("接收副本日志");

	return true;
}

bool CMsgDispatch::OnMsgO2GArena(gproto::gather::MSG_O2G_Arena *pMsg)
{
	if (nullptr == pMsg)
	{
		return false;
	}

	CDbEvent_Arena *pDbEvent_Arena = new CDbEvent_Arena(pMsg->roleid(), pMsg->rolename(), pMsg->rolelevel(),
		pMsg->result());
	CApp::getMe().PostAsyncEvent(pDbEvent_Arena);

	LOGPrint("接收竞技场日志");

	return true;
}


