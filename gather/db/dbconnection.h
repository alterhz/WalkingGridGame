//-------------------------------------------
// author : alterhz
// date : 6/26/2015
// description : 

#ifndef _DBCONNECTION_H_
#define _DBCONNECTION_H_

#include "UtilityInc.h"
#include "mysql++.h"
#include "thread.h"
using namespace NS_IO;

class CDbConnectionManager : public Singleton<CDbConnectionManager>
{
	typedef std::vector<mysqlpp::Connection *> VtDbConnection;

public:
	CDbConnectionManager();
	~CDbConnectionManager();

public:
	mysqlpp::Connection * Fetch();
	void Recycle(mysqlpp::Connection *pDbConnection);

private:
	CMutex m_mtx;
	VtDbConnection m_vtDbConnection;

	std::string m_strServer;
	std::string m_strUser;
	std::string m_strPwd;
	std::string m_strDb;
	int m_nPort;
};

#endif