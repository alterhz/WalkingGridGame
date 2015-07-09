//-------------------------------------------
// author : alterhz
// date : 6/18/2015
// description : 

#ifndef _CONNECTOR_H_
#define _CONNECTOR_H_

#include "netio.h"
#include "boostinc.h"
#include "socket.h"

NS_IO_Header

class CNetConnector : public INetConnector, public IParent
{
public:
	CNetConnector(boost::asio::io_service &ios);
	~CNetConnector();

	virtual bool Connect(INetClient *pNetClient, const char *pIP, unsigned short wPort);
	virtual bool ReConnect();

	virtual bool DoSend(const char *pBuffer, unsigned short wLength);
	virtual void DoClose();

	virtual const char * GetRemoteIP();
	virtual unsigned short GetRemotePort();
	virtual const char * GetLocalIP();
	virtual unsigned short GetLocalPort();

	virtual void OnRelease(INetSocket *pNetSocket);

private:
	void OnConnect(const boost::system::error_code& ec);

private:
	boost::asio::io_service &m_ioService;
	INetClient *m_pNetClient;
	std::string m_strRemoteIP;
	unsigned short m_wRemotePort;
	boost::asio::ip::tcp::socket *m_pConnectSocket;
	CNetSocket *m_pNetSocket;
};

NS_IO_Footer

#endif