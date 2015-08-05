#include "dbmysql.h"
#include "mysqld_error.h"

NS_IO_Header

CDbConnection::CDbConnection() : mysqlpp::Connection(false)
{
}

CDbConnection::~CDbConnection()
{

}

bool CDbConnection::Connect(const char* szServer, const char* szUser, const char* szPasswd, const char* szDb, unsigned int nPort)
{
	//非常重要，首先要设置字符集，否则中文会变成乱码
	mysqlpp::Connection::set_option(new mysqlpp::SetCharsetNameOption("utf8"));
	//非常重要，必须设置，否则存储过程无法执行
	mysqlpp::Connection::set_option(new mysqlpp::MultiStatementsOption(true));
	mysqlpp::Connection::set_option(new mysqlpp::MultiResultsOption(true));

	bool ret = mysqlpp::Connection::connect(szDb, szServer, szUser, szPasswd, nPort);

	return ret;
}

bool CDbConnection::Connected() const
{
	return mysqlpp::Connection::connected();
}

void CDbConnection::Disconnect()
{
	mysqlpp::Connection::disconnect();
}

bool CDbConnection::ExecuteSQL(const char* szSQL)
{
	try
	{
		mysqlpp::Query q = mysqlpp::Connection::query(szSQL);
	}
	catch (mysqlpp::BadQuery e) {
		if (e.errnum() == ER_LOCK_DEADLOCK) {
			cerr << "Transaction deadlock detected!" << endl;
			cerr << "Connection::errnum = " << errnum() <<
				", BadQuery::errnum = " << e.errnum() << endl;
		}
		else {
			cerr << "Unexpected query error: " << e.what() << endl;
		}
		return false;
	}
	catch (mysqlpp::Exception e) {
		cerr << "General error: " << e.what() << endl;
		return false;
	}

	return true;
}

bool CDbConnection::ExecuteSP(const char* szSP)
{
	try
	{
		mysqlpp::Query q = mysqlpp::Connection::query(szSP);
	}
	catch (mysqlpp::BadQuery e) {
		if (e.errnum() == ER_LOCK_DEADLOCK) {
			cerr << "Transaction deadlock detected!" << endl;
			cerr << "Connection::errnum = " << errnum() <<
				", BadQuery::errnum = " << e.errnum() << endl;
		}
		else {
			cerr << "Unexpected query error: " << e.what() << endl;
		}
		return false;
	}
	catch (mysqlpp::Exception e) {
		cerr << "General error: " << e.what() << endl;
		return false;
	}

	return true;
}

NS_IO_Footer