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
