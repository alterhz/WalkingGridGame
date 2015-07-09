//-------------------------------------------
// author : alterhz
// date : 6/18/2015
// description : 

#ifndef _CONNECTOR_H_
#define _CONNECTOR_H_

#include "netio.h"
#include "msginc.h"
#include "event.h"
using namespace NS_IO;

class CConnectorTest : public INetClient, ITimerEvent
{
public:
	CConnectorTest();
	virtual ~CConnectorTest();

	void Init(INetConnector *pNetConnector);

	bool SendMessage(unsigned short wProtocolId, google::protobuf::Message *pMessage);

	void Disconnect();

public:
	// 背包扩充日志
	bool GatherBagExtend(int nRoleId, std::string strRoleName, int nRoleSN, int nType, int nExtendNum);
	// 副本进入次数购买日志
	bool GatherBuyInstance(int nRoleId, std::string strRoleName, int nRoleSN, 
		int nRoleLevel, int nVipLevel, int nInstanceSN, int nBuyCount);
	// 游戏币购买日志
	bool GatherBuyCoin(int nRoleId, std::string strRoleName, int nRoleSN, 
		int nRoleLevel, int nVipLevel, int nSpendDiamond, int nCoin);
	// 钻石产出或消耗
	bool GatherDiamond(std::string strAccount, int nRoleId, std::string strRoleName, int nRoleLevel, 
		int nType, int nSubType, int nChangeDiamond, int nTotalDiamond);
	// 金币产生或消耗
	bool GatherCoin(int nRoleId, std::string strRoleName, int nRoleLevel, 
		int nType, int nSubType, int nChangeCoin, int nTotalCoin);
	// 道具产生或消耗日志
	bool GatherItem(int nItemSN, int nItemType, int nNum, int nType, int nWay, 
		int nRoleId, std::string strRoleName, int nRoleLevel, gproto::gather::INFO_Equipment &infoEquipMent);
	// 装备打造日志
	bool GatherEquip(int nRoleId, std::string strRoleName, int nEquipSN, int nEquipId, int nMakeType, 
		gproto::gather::INFO_Equipment &equipMakeBefore, gproto::gather::INFO_Equipment &equipMakeAfter);
	// 复活日志
	bool GatherRelive(int nRoleId, std::string strRoleName, int nRoleLevel, int nInstanceSN, int nWasteDiamond);
	// 副本日志
	bool GatherInstance(int nInstanceSN, int nResult, int nPassTime, int nDeadTimes, std::vector<int> vtRoleId);
	// 竞技场日志
	bool GatherArena(int nRoleId, std::string strRoleName, int nRoleLevel, int nResult);


public:
	// 网络连接事件
	virtual bool OnConnected(INetSocket *pNetSocket);
	// 网络断开事件(此函数调用后INetSocket对象销毁)
	virtual bool OnDisconnect();
	// 接收数据
	virtual bool OnRecvPacket(const char *pPacket, unsigned short wLength);
	// 错误异常
	virtual bool OnError(int nErrCode) { return true; }

	// 返回true:继续；返回false:终止
	virtual bool OnTimerEvent(int nTimerId);
	// 删除
	virtual void OnKilled(int nTimerId);

private:
	INetConnector *m_pNetConnector;
	int m_nTimerId;
};

#endif