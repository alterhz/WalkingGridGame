//-------------------------------------------
// author : alterhz
// date : 6/25/2015
// description : 

#ifndef _DBEVENT_H_
#define _DBEVENT_H_

#include <string>
#include "event.h"
using namespace NS_IO;

class CEvent_TestDb : public IAsyncEvent
{
public:
	~CEvent_TestDb() {}

public:
	// 多线程执行
	virtual bool OnExcute();
	// 单线程执行
	virtual bool OnResult();
};

// 充值
class CDbEvent_Deposit : public IAsyncEvent
{
public:
	CDbEvent_Deposit(std::string strPlatform, std::string strAccount, int nRoleId, std::string strRoleName,
		int nRoleSN, int nRoleLevel, int nVipLevel, int nType, int nDiamond)
		: m_nLastInsertId(0)
		, m_strPlatform(strPlatform)
		, m_strAccount(strAccount)
		, m_nRoleId(nRoleId)
		, m_strRoleName(strRoleName)
		, m_nRoleSN(nRoleSN)
		, m_nRoleLevel(nRoleLevel)
		, m_nVipLevel(nVipLevel)
		, m_nType(nType)
		, m_nDiamond(nDiamond)
	{}
	~CDbEvent_Deposit() {}

public:
	// 多线程执行
	virtual bool OnExcute();
	// 单线程执行
	virtual bool OnResult();

private:
	int m_nLastInsertId;
	std::string m_strPlatform;
	std::string m_strAccount;
	int m_nRoleId;
	std::string m_strRoleName;
	int m_nRoleSN;
	int m_nRoleLevel;
	int m_nVipLevel;
	int m_nType;
	int m_nDiamond;
};

// 账号注册
class CDbEvent_AccountReg : public IAsyncEvent
{
public:
	CDbEvent_AccountReg(std::string strPlatform, std::string strAccount)
		: m_nLastInsertId(0)
		, m_strPlatform(strPlatform)
		, m_strAccount(strAccount)
	{}
	~CDbEvent_AccountReg() {}

public:
	// 多线程执行
	virtual bool OnExcute();
	// 单线程执行
	virtual bool OnResult();

private:
	int m_nLastInsertId;
	std::string m_strPlatform;
	std::string m_strAccount;
};

// 账号创建
class CDbEvent_RoleCreate : public IAsyncEvent
{
public:
	CDbEvent_RoleCreate(std::string strPlatform, std::string strAccount, int nRoleSN, std::string strRoleName)
		: m_nLastInsertId(0)
		, m_strPlatform(strPlatform)
		, m_strAccount(strAccount)
		, m_nRoleSN(nRoleSN)
		, m_strRoleName(strRoleName)
	{}
	~CDbEvent_RoleCreate() {}

public:
	// 多线程执行
	virtual bool OnExcute();
	// 单线程执行
	virtual bool OnResult();

private:
	int m_nLastInsertId;
	std::string m_strPlatform;
	std::string m_strAccount;
	int m_nRoleSN;
	std::string m_strRoleName;
};

// 登陆日志
class CDbEvent_Login : public IAsyncEvent
{
public:
	CDbEvent_Login(std::string strPlatform, std::string strAccount, int nRoleSN, int nRoleLevel, std::string strRoleName,
		int nOnlineTime, int nOfflineTime) 
		: m_nLastInsertId(0)
		, m_strPlatform(strPlatform)
		, m_strAccount(strAccount)
		, m_nRoleSN(nRoleSN)
		, m_nRoleLevel(nRoleLevel)
		, m_strRoleName(strRoleName)
		, m_nOnlineTime(nOnlineTime)
		, m_nOfflineTime(nOfflineTime)
	{}
	~CDbEvent_Login() {}

public:
	// 多线程执行
	virtual bool OnExcute();
	// 单线程执行
	virtual bool OnResult();

private:
	int m_nLastInsertId;
	std::string m_strPlatform;
	std::string m_strAccount;
	int m_nRoleSN;
	int m_nRoleLevel;
	std::string m_strRoleName;
	int m_nOnlineTime;
	int m_nOfflineTime;
};

// 背包扩充日志
class CDbEvent_BagExtend : public IAsyncEvent
{
public:
	CDbEvent_BagExtend(int nRoleId, std::string strRoleName, int nRoleSN, int nType, int nExtendNum) 
		: m_nLastInsertId(0)
		, m_nRoleId(nRoleId)
		, m_strRoleName(strRoleName)
		, m_nRoleSN(nRoleSN)
		, m_nType(nType)
		, m_nExtendNum(nExtendNum)
	{}
	~CDbEvent_BagExtend() {}

public:
	// 多线程执行
	virtual bool OnExcute();
	// 单线程执行
	virtual bool OnResult();

private:
	int m_nLastInsertId;
	int m_nRoleId;
	std::string m_strRoleName;
	int m_nRoleSN;
	int m_nType;
	int m_nExtendNum;
};

// 副本购买次数
class CDbEvent_BuyInstance : public IAsyncEvent
{
public:
	CDbEvent_BuyInstance(int nRoleId, std::string strRoleName, int nRoleSN, 
		int nRoleLevel, int nVipLevel, int nInstanceSN, int nBuyCount) 
		: m_nLastInsertId(0)
		, m_nRoleId(nRoleId)
		, m_strRoleName(strRoleName)
		, m_nRoleSN(nRoleSN)
		, m_nRoleLevel(nRoleLevel)
		, m_nVipLevel(nVipLevel)
		, m_nInstanceSN(nInstanceSN)
		, m_nBuyCount(nBuyCount)
	{}
	~CDbEvent_BuyInstance() {}

public:
	// 多线程执行
	virtual bool OnExcute();
	// 单线程执行
	virtual bool OnResult();

private:
	int m_nLastInsertId;
	int m_nRoleId;
	std::string m_strRoleName;
	int m_nRoleSN;
	int m_nRoleLevel;
	int m_nVipLevel;
	int m_nInstanceSN;
	int m_nBuyCount;
};

// 游戏币购买
class CDbEvent_BuyCoin : public IAsyncEvent
{
public:
	CDbEvent_BuyCoin(int nRoleId, std::string strRoleName, int nRoleSN, 
		int nRoleLevel, int nVipLevel, int nSpendDiamon, int nCoin) 
		: m_nLastInsertId(0)
		, m_nRoleId(nRoleId)
		, m_strRoleName(strRoleName)
		, m_nRoleSN(nRoleSN)
		, m_nRoleLevel(nRoleLevel)
		, m_nVipLevel(nVipLevel)	
		, m_nSpendDiamond(nSpendDiamon)
		, m_nCoin(nCoin)
	{}
	~CDbEvent_BuyCoin() {}

public:
	// 多线程执行
	virtual bool OnExcute();
	// 单线程执行
	virtual bool OnResult();

private:
	int m_nLastInsertId;
	int m_nRoleId;
	std::string m_strRoleName;
	int m_nRoleSN;
	int m_nRoleLevel;
	int m_nVipLevel;
	int m_nSpendDiamond;
	int m_nCoin;
};

// 钻石产出和消耗
class CDbEvent_Diamond : public IAsyncEvent
{
public:
	CDbEvent_Diamond(std::string strAccount, int nRoleId, std::string strRoleName, int nRoleLevel, 
		int nType, int nSubType, int nChangeDiamond, int nTotalDiamond) 
		: m_nLastInsertId(0)
		, m_strAccount(strAccount)
		, m_nRoleId(nRoleId)
		, m_strRoleName(strRoleName)
		, m_nRoleLevel(nRoleLevel)
		, m_nType(nType)
		, m_nSubType(nSubType)
		, m_nChangeDiamond(nChangeDiamond)
		, m_nTotalDiamond(nTotalDiamond)
	{}
	~CDbEvent_Diamond() {}

public:
	// 多线程执行
	virtual bool OnExcute();
	// 单线程执行
	virtual bool OnResult();

private:
	int m_nLastInsertId;
	std::string m_strAccount;
	int m_nRoleId;
	std::string m_strRoleName;
	int m_nRoleLevel;
	int m_nType;
	int m_nSubType;
	int m_nChangeDiamond;
	int m_nTotalDiamond;
};

// 金币产生和消耗
class CDbEvent_Coin : public IAsyncEvent
{
public:
	CDbEvent_Coin(int nRoleId, std::string strRoleName, int nRoleLevel, 
		int nType, int nSubType, int nChangeCoin, int nTotalCoin)
		: m_nLastInsertId(0)
		, m_nRoleId(nRoleId)
		, m_strRoleName(strRoleName)
		, m_nRoleLevel(nRoleLevel)
		, m_nType(nType)
		, m_nSubType(nSubType)
		, m_nChangeCoin(nChangeCoin)
		, m_nTotalCoin(nTotalCoin)
	{}
	~CDbEvent_Coin() {}

public:
	// 多线程执行
	virtual bool OnExcute();
	// 单线程执行
	virtual bool OnResult();

private:
	int m_nLastInsertId;
	int m_nRoleId;
	std::string m_strRoleName;
	int m_nRoleLevel;
	int m_nType;
	int m_nSubType;
	int m_nChangeCoin;
	int m_nTotalCoin;
};

// 物品产出或消耗日志
class CDbEvent_Item : public IAsyncEvent
{
public:
	CDbEvent_Item(int nItemSN, int nItemType, int nNum, 
		int nType, int nWay, int nRoleId, std::string strRoleName, int nRoleLevel, std::string strItemAttr) 
		: m_nItemSN(nItemSN)
		, m_nItemType(nItemType)
		, m_nNum(nNum)
		, m_nType(nType)
		, m_nWay(nWay)
		, m_nRoleId(nRoleId)
		, m_strRoleName(strRoleName)
		, m_nRoleLevel(nRoleLevel)
		, m_strItemAttr(strItemAttr)
	{}
	~CDbEvent_Item() {}

public:
	// 多线程执行
	virtual bool OnExcute();
	// 单线程执行
	virtual bool OnResult();

private:
	int m_nLastInsertId;
	int m_nItemSN;
	int m_nItemType;
	int m_nNum;
	int m_nType;
	int m_nWay;
	int m_nRoleId;
	std::string m_strRoleName;
	int m_nRoleLevel;
	std::string m_strItemAttr;
};

// 装备打造日志
class CDbEvent_Equip : public IAsyncEvent
{
public:
	CDbEvent_Equip(int nRoleId, std::string strRoleName, int nEquipSN, int nEquipId, int nMakeType,
		std::string strDataBefore, std::string strDataAfter) 
		: m_nLastInsertId(0)
		, m_nRoleId(nRoleId)
		, m_strRoleName(strRoleName)
		, m_nEquipSN(nEquipSN)
		, m_nEquipId(nMakeType)
		, m_nMakeType(nMakeType)
		, m_strDataBefore(strDataBefore)
		, m_strDataAfter(strDataAfter)
	{}
	~CDbEvent_Equip() {}

public:
	// 多线程执行
	virtual bool OnExcute();
	// 单线程执行
	virtual bool OnResult();

private:
	int m_nLastInsertId;
	int m_nRoleId;
	std::string m_strRoleName;
	int m_nEquipSN;
	int m_nEquipId;
	int m_nMakeType;
	std::string m_strDataBefore;
	std::string m_strDataAfter;
};

// 原地复活日志
class CDbEvent_Relive : public IAsyncEvent
{
public:
	CDbEvent_Relive(int nRoleId, std::string strRoleName, int nRoleLevel, 
		int nInstanceSN, int nWasteDiamond) 
		: m_nLastInsertId(0)
		, m_nRoleId(nRoleId)
		, m_strRoleName(strRoleName)
		, m_nRoleLevel(nRoleLevel)
		, m_nInstanceSN(nInstanceSN)
		, m_nWasteDiamond(nWasteDiamond)
	{}
	~CDbEvent_Relive() {}

public:
	// 多线程执行
	virtual bool OnExcute();
	// 单线程执行
	virtual bool OnResult();

private:
	int m_nLastInsertId;
	int m_nRoleId;
	std::string m_strRoleName;
	int m_nRoleLevel;
	int m_nInstanceSN;
	int m_nWasteDiamond;
};

// 副本日志
class CDbEvent_Instance : public IAsyncEvent
{
public:
	CDbEvent_Instance(int nInstanceSN, int nResult, int nPassTime, int nDeadTimes, std::string strRoleIdList) 
		: m_nLastInsertId(0)
		, m_nInstanceSN(nInstanceSN)
		, m_nResult(nResult)
		, m_nPassTime(nPassTime)
		, m_nDeadTimes(nDeadTimes)
		, m_strRoleIdList(strRoleIdList)
	{}
	~CDbEvent_Instance() {}

public:
	// 多线程执行
	virtual bool OnExcute();
	// 单线程执行
	virtual bool OnResult();

private:
	int m_nLastInsertId;
	int m_nInstanceSN;
	int m_nResult;
	int m_nPassTime;
	int m_nDeadTimes;
	std::string m_strRoleIdList;
};

// 竞技场日志
class CDbEvent_Arena : public IAsyncEvent
{
public:
	CDbEvent_Arena(int nRoleId, std::string strRoleName, int nRoleLevel, int nResult) 
		: m_nLastInsertId(0)
		, m_nRoleId(nRoleId)
		, m_strRoleName(strRoleName)
		, m_nRoleLevel(nRoleLevel)
		, m_nResult(nResult)
	{}
	~CDbEvent_Arena() {}

public:
	// 多线程执行
	virtual bool OnExcute();
	// 单线程执行
	virtual bool OnResult();

private:
	int m_nLastInsertId;
	int m_nRoleId;
	std::string m_strRoleName;
	int m_nRoleLevel;
	int m_nResult;
};

#endif