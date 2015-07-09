//-------------------------------------------
// author : alterhz
// date : 6/26/2015
// description : 

#ifndef _DB_H_
#define _DB_H_

#ifndef NS_IO

#define NS_IO		dgs
#define NS_IO_Header	namespace dgs {
#define NS_IO_Footer	}

#endif	//NS_IO

NS_IO_Header

class IConnection
{
public:
	virtual ~IConnection() {}

public:
	virtual bool Connect(const char* szServer, const char* szUser, const char* szPasswd, 
		const char* szDb, unsigned int nPort) = 0;

	virtual bool Connected() const = 0;
	virtual void Disconnect() = 0;

	virtual bool ExecuteSQL(const char* szSQL) = 0;
	virtual bool ExecuteSP(const char* szSP) = 0;
};


NS_IO_Footer

#endif