#include "dbevent.h"
#include "debug.h"
#include "thread.h"
#include "dbconnection.h"

// 自动释放类
class AutoRelease
{
public:
	AutoRelease(mysqlpp::Connection *pDbConntion)
		: m_pDbConnection(pDbConntion)
	{}
	~AutoRelease()
	{
		// 数据库操作完毕，返还Connection对象
		//CDbConnectionManager::getMe().Recycle(m_pDbConnection);
	}

private:
	mysqlpp::Connection *m_pDbConnection;
};

// QUERY宏（减少重复代码量）
#define ExcuteSQL(sql, dbQuery, dbQueryResult) mysqlpp::Connection *pDbConnection = CDbConnectionManager::getMe().Fetch(); \
	if (nullptr == pDbConnection) {	return false; } \
	AutoRelease ar(pDbConnection); \
	if (!pDbConnection->connected()) { if (!pDbConnection->ping()) { return false; } } \
	mysqlpp::Query dbQuery(pDbConnection, false);	\
	mysqlpp::StoreQueryResult dbQueryResult = dbQuery.store(sql.c_str(), sql.length());	\
	if (dbQuery.errnum() != 0) { LOGPrint("mysql_error[error_code:" + dbQuery.errnum() + "]:" + dbQuery.error()); return false; }

//////////////////////////////////////////////////////////////////////////
// 下面是异步事件

bool CEvent_TestDb::OnExcute()
{
	std::string strSP = "DROP TABLE IF EXISTS `user`; \
		CREATE TABLE `user` (`id` int(11) NOT NULL AUTO_INCREMENT, \
		`name` varchar(255) DEFAULT NULL, \
		PRIMARY KEY (`id`)) ENGINE=InnoDB AUTO_INCREMENT=8 DEFAULT CHARSET=latin1; \
		insert into user(name) values('rose1');";
	//std::string strSP = "insert into user(name) values('rose1');";

	ExcuteSQL(strSP, dbQuery, dbQueryResult);

	LOGPrint("执行数据库多线程执行");

	return true;
}

bool CEvent_TestDb::OnResult()
{
	LOGPrint("数据库执行完毕，返回主线程。");

	delete this;
	return true;
}

bool CDbEvent_Deposit::OnExcute()
{
	AutoStr1024 autoStr;

	autoStr + "insert into deposit(platform, account, roleid, rolename, rolesn, rolelevel, viplevel, type, diamond, time) values('"
		+ m_strPlatform + "', '" + m_strAccount + "', " + m_nRoleId + ", '" 
		+ m_strRoleName + "', " + m_nRoleSN + ", " + m_nRoleLevel + ", " + m_nVipLevel + "," 
		+ m_nType + ", " + m_nDiamond + ", now());";

	std::string strSP = autoStr.GetStr();

	ExcuteSQL(strSP, dbQuery, dbQueryResult);

	// 新插入的id
	m_nLastInsertId = dbQuery.insert_id();

	return true;
}
bool CDbEvent_Deposit::OnResult()
{
	LOGPrint("充值数据插入，返回Id:" + m_nLastInsertId);

	delete this;
	return true;
}

bool CDbEvent_AccountReg::OnExcute()
{
	AutoStr1024 autoStr;

	autoStr + "insert into accountreg(platform, account, time) values('"
		+ m_strPlatform + "', '" + m_strAccount + "', now());";

	std::string strSP = autoStr.GetStr();

	ExcuteSQL(strSP, dbQuery, dbQueryResult);

	// 新插入的id
	m_nLastInsertId = dbQuery.insert_id();

	return true;
}
bool CDbEvent_AccountReg::OnResult()
{
	LOGPrint("账号注册，返回Id:" + m_nLastInsertId);

	delete this;
	return true;
}

bool CDbEvent_RoleCreate::OnExcute()
{
	AutoStr1024 autoStr;

	autoStr + "insert into rolecreate(platform, account, rolesn, rolename, time) values('"
		+ m_strPlatform + "', '" + m_strAccount + "', " + m_nRoleSN + ", '" + m_strRoleName + "', now());";

	std::string strSP = autoStr.GetStr();

	ExcuteSQL(strSP, dbQuery, dbQueryResult);

	// 新插入的id
	m_nLastInsertId = dbQuery.insert_id();

	return true;
}
bool CDbEvent_RoleCreate::OnResult()
{
	LOGPrint("创建角色，返回Id:" + m_nLastInsertId);

	delete this;
	return true;
}

bool CDbEvent_Login::OnExcute()
{
	AutoStr1024 autoStr;

	autoStr + "insert into login(platform, account, rolesn, rolelevel, rolename, onlinetime, offlinetime) values('"
		+ m_strPlatform + "', '" + m_strAccount + "', " + m_nRoleSN + ", " + m_nRoleLevel + ", '" + m_strRoleName 
		+ "', from_unixtime(" + m_nOnlineTime + "), from_unixtime(" + m_nOfflineTime + "));";

	std::string strSP = autoStr.GetStr();

	ExcuteSQL(strSP, dbQuery, dbQueryResult);

	// 新插入的id
	m_nLastInsertId = dbQuery.insert_id();

	return true;
}

bool CDbEvent_Login::OnResult()
{
	LOGPrint("登陆记录，返回Id:" + m_nLastInsertId);

	delete this;
	return true;
}

bool CDbEvent_BagExtend::OnExcute()
{
	AutoStr1024 autoStr;

	autoStr + "insert into bagextend(roleid, rolename, rolesn, type, extendnum, time) values("
		+ m_nRoleId + ", '" + m_strRoleName + "', " + m_nRoleSN + ", " + m_nType + ", " + m_nExtendNum 
		+ ", now());";

	std::string strSP = autoStr.GetStr();

	ExcuteSQL(strSP, dbQuery, dbQueryResult);

	// 新插入的id
	m_nLastInsertId = dbQuery.insert_id();

	return true;
}

bool CDbEvent_BagExtend::OnResult()
{
	LOGPrint("背包扩充日志，返回Id:" + m_nLastInsertId);

	delete this;
	return true;
}

bool CDbEvent_BuyInstance::OnExcute()
{
	AutoStr1024 autoStr;

	autoStr + "insert into buyinstance(roleid, rolename, rolesn, rolelevel, viplevel, instancesn, buycount, time) values("
		+ m_nRoleId + ", '" + m_strRoleName + "', " + m_nRoleSN + ", " + m_nRoleLevel + ", " + m_nVipLevel 
		+ ", " + m_nInstanceSN + ", " + m_nBuyCount + ", now());";

	std::string strSP = autoStr.GetStr();

	ExcuteSQL(strSP, dbQuery, dbQueryResult);

	// 新插入的id
	m_nLastInsertId = dbQuery.insert_id();

	return true;
}

bool CDbEvent_BuyInstance::OnResult()
{
	LOGPrint("副本进入次数购买日志，返回Id:" + m_nLastInsertId);

	delete this;
	return true;
}

bool CDbEvent_BuyCoin::OnExcute()
{
	AutoStr1024 autoStr;

	autoStr + "insert into buycoin(roleid, rolename, rolesn, rolelevel, viplevel, spenddiamond, coin, time) values("
		+ m_nRoleId + ", '" + m_strRoleName + "', " + m_nRoleSN + ", " + m_nRoleLevel + ", " + m_nVipLevel 
		+ ", " + m_nSpendDiamond + ", " + m_nCoin + ", now());";

	std::string strSP = autoStr.GetStr();

	ExcuteSQL(strSP, dbQuery, dbQueryResult);

	// 新插入的id
	m_nLastInsertId = dbQuery.insert_id();

	return true;
}

bool CDbEvent_BuyCoin::OnResult()
{
	LOGPrint("购买游戏币日志，返回Id:" + m_nLastInsertId);

	delete this;
	return true;
}

bool CDbEvent_Diamond::OnExcute()
{
	AutoStr1024 autoStr;

	autoStr + "insert into diamond(account, roleid, rolename, rolelevel, type, subtype, changediamond, totaldiamond, time)" 
		+ " values('" + m_strAccount + "', " + m_nRoleId + ", '" + m_strRoleName + "', " + m_nRoleLevel + ", " + m_nType 
		+ ", " + m_nSubType + ", " + m_nChangeDiamond + ", " + m_nTotalDiamond + ", now());";

	std::string strSP = autoStr.GetStr();

	ExcuteSQL(strSP, dbQuery, dbQueryResult);

	// 新插入的id
	m_nLastInsertId = dbQuery.insert_id();

	return true;
}

bool CDbEvent_Diamond::OnResult()
{
	LOGPrint("钻石产出或消耗日志，返回Id:" + m_nLastInsertId);

	delete this;
	return true;
}

bool CDbEvent_Coin::OnExcute()
{
	AutoStr1024 autoStr;

	autoStr + "insert into coin(roleid, rolename, rolelevel, type, subtype, changecoin, totalcoin, time)" 
		+ " values(" + m_nRoleId + ", '" + m_strRoleName + "', " + m_nRoleLevel + ", " + m_nType 
		+ ", " + m_nSubType + ", " + m_nChangeCoin + ", " + m_nTotalCoin + ", now());";

	std::string strSP = autoStr.GetStr();

	ExcuteSQL(strSP, dbQuery, dbQueryResult);

	// 新插入的id
	m_nLastInsertId = dbQuery.insert_id();

	return true;
}

bool CDbEvent_Coin::OnResult()
{
	LOGPrint("金币产出或消耗日志，返回Id:" + m_nLastInsertId);

	delete this;
	return true;
}

bool CDbEvent_Item::OnExcute()
{
	AutoStr1024 autoStr;

	autoStr + "insert into item(itemsn, itemtype, itemattr, num, type, way, roleid, rolename, rolelevel, time)" 
		+ " values(" + m_nItemSN + ", " + m_nItemType + ", '" + m_strItemAttr + "', " + m_nNum + ", " + m_nType 
		+ ", " + m_nWay + ", " + m_nRoleId + ", '" + m_strRoleName + "', " + m_nRoleLevel + ", now());";

	std::string strSP = autoStr.GetStr();

	ExcuteSQL(strSP, dbQuery, dbQueryResult);

	// 新插入的id
	m_nLastInsertId = dbQuery.insert_id();

	return true;
}

bool CDbEvent_Item::OnResult()
{
	LOGPrint("道具产出或消耗日志，返回Id:" + m_nLastInsertId);

	delete this;
	return true;
}

bool CDbEvent_Equip::OnExcute()
{
	AutoStr1024 autoStr;

	autoStr + "insert into equip(roleid, rolename, equipsn, equipid, maketype, databefore, dataafter, time)" 
		+ " values(" + m_nRoleId + ", '" + m_strRoleName + "', " + m_nEquipSN + ", " + m_nEquipId 
		+ ", " + m_nMakeType + ", '" + m_strDataBefore + "', '" + m_strDataAfter + "', now());";

	std::string strSP = autoStr.GetStr();

	ExcuteSQL(strSP, dbQuery, dbQueryResult);

	// 新插入的id
	m_nLastInsertId = dbQuery.insert_id();

	return true;
}

bool CDbEvent_Equip::OnResult()
{
	LOGPrint("装备打造日志，返回Id:" + m_nLastInsertId);

	delete this;
	return true;
}

bool CDbEvent_Relive::OnExcute()
{
	AutoStr1024 autoStr;

	autoStr + "insert into relive(roleid, rolename, rolelevel, instancesn, wastediamond, time)" 
		+ " values(" + m_nRoleId + ", '" + m_strRoleName + "', " + m_nRoleLevel + ", " + m_nInstanceSN 
		+ ", " + m_nWasteDiamond + ", now());";

	std::string strSP = autoStr.GetStr();

	ExcuteSQL(strSP, dbQuery, dbQueryResult);

	// 新插入的id
	m_nLastInsertId = dbQuery.insert_id();

	return true;
}

bool CDbEvent_Relive::OnResult()
{
	LOGPrint("复活日志，返回Id:" + m_nLastInsertId);

	delete this;
	return true;
}

bool CDbEvent_Instance::OnExcute()
{
	AutoStr1024 autoStr;

	autoStr + "insert into instance(instancesn, result, passtime, deadtimes, roleidlist, time)" 
		+ " values(" + m_nInstanceSN + ", '" + m_nResult + "', " + m_nPassTime + ", " + m_nDeadTimes 
		+ ", '" + m_strRoleIdList + "', now());";

	std::string strSP = autoStr.GetStr();

	ExcuteSQL(strSP, dbQuery, dbQueryResult);

	// 新插入的id
	m_nLastInsertId = dbQuery.insert_id();

	return true;
}

bool CDbEvent_Instance::OnResult()
{
	LOGPrint("副本日志，返回Id:" + m_nLastInsertId);

	delete this;
	return true;
}

bool CDbEvent_Arena::OnExcute()
{
	AutoStr1024 autoStr;

	autoStr + "insert into arena(roleid, rolename, rolelevel, result, time)" 
		+ " values(" + m_nRoleId + ", '" + m_strRoleName + "', " + m_nRoleLevel + ", " + m_nResult 
		+ ", now());";

	std::string strSP = autoStr.GetStr();

	ExcuteSQL(strSP, dbQuery, dbQueryResult);

	// 新插入的id
	m_nLastInsertId = dbQuery.insert_id();

	return true;
}

bool CDbEvent_Arena::OnResult()
{
	LOGPrint("竞技场日志，返回Id:" + m_nLastInsertId);

	delete this;
	return true;
}
