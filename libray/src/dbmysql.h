//-------------------------------------------
// author : alterhz
// date : 6/26/2015
// description : 

#ifndef _DBMYSQL_H_
#define _DBMYSQL_H_

#include "db.h"
#include "mysql++.h"
#include <iostream>
using namespace std;

NS_IO_Header

class CDbConnection : public IConnection, public mysqlpp::Connection
{
public:
	CDbConnection();
	virtual ~CDbConnection();

public:
	virtual bool Connect(const char* szServer, const char* szUser, const char* szPasswd, 
		const char* szDb, unsigned int nPort);

	virtual bool Connected() const;
	virtual void Disconnect();

	virtual bool ExecuteSQL(const char* szSQL);
	virtual bool ExecuteSP(const char* szSP);
};

NS_IO_Footer

#endif