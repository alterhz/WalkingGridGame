//-------------------------------------------
// author : alterhz
// date : 6/25/2015
// description : 

#ifndef _MSGDISPATCH_H_
#define _MSGDISPATCH_H_

#include "msginc.h"
#include "UtilityInc.h"
#include "event.h"
using namespace NS_IO;

class CClient;
class CProtocolFactory;

class CMsgDispatch : public Singleton<CMsgDispatch>
{
public:
	CMsgDispatch();
	~CMsgDispatch();
	void SetCurrentClient(CClient *pClient) { m_pCurrentClient = pClient; }

public:
	bool Dispatch(unsigned short wProtocolID, const char *pBuffer, int nLength);

private:
	bool OnMsgC2GHeartBeat(gproto::gather::MSG_C2G_HeartBeat *pMsg);
	bool OnMsgG2CHeartBeat(gproto::gather::MSG_G2C_HeartBeat *pMsg);

	// 充值
	bool OnMsgO2GDeposit(gproto::gather::MSG_O2G_Deposit *pMsg);
	// 账号注册
	bool OnMsgO2GAccountReg(gproto::gather::MSG_O2G_AccountReg *pMsg);
	// 角色创建
	bool OnMsgO2GRoleCreate(gproto::gather::MSG_O2G_RoleCreate *pMsg);
	// 登陆日志
	bool OnMsgO2GLogin(gproto::gather::MSG_O2G_Login *pMsg);
	// 背包扩展日志
	bool OnMsgO2GBagExtend(gproto::gather::MSG_O2G_BagExtend *pMsg);
	// 副本进入次数购买日志
	bool OnMsgO2GBuyInstance(gproto::gather::MSG_O2G_BuyInstance *pMsg);
	// 游戏币购买日志
	bool OnMsgO2GBuyCoin(gproto::gather::MSG_O2G_BuyCoin *pMsg);
	// 钻石产出或消耗
	bool OnMsgO2GDiamond(gproto::gather::MSG_O2G_Diamond *pMsg);
	// 金币产出或消耗
	bool OnMsgO2GCoin(gproto::gather::MSG_O2G_Coin *pMsg);
	// 道具产出或消耗
	bool OnMsgO2GItem(gproto::gather::MSG_O2G_Item *pMsg);
	// 装备打造日志
	bool OnMsgO2GEquip(gproto::gather::MSG_O2G_Equip *pMsg);
	// 复活日志
	bool OnMsgO2GRelive(gproto::gather::MSG_O2G_Relive *pMsg);
	// 副本日志
	bool OnMsgO2GInstance(gproto::gather::MSG_O2G_Instance *pMsg);
	// 竞技场日志
	bool OnMsgO2GArena(gproto::gather::MSG_O2G_Arena *pMsg);

private:
	CClient *m_pCurrentClient;
	CProtocolFactory *m_pProtocolFactory;
};

#endif