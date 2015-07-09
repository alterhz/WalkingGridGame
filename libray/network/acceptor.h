//-------------------------------------------
// author : alterhz
// date : 6/17/2015
// description : 

#ifndef _ACCEPTOR_H_
#define _ACCEPTOR_H_

#include "netio.h"
#include "boostinc.h"

NS_IO_Header

class CNetAcceptor : public INetAcceptor
{
	typedef std::map<intptr_t, boost::asio::ip::tcp::socket *> MapSocket;
	typedef std::map<intptr_t, INetSocket *> MapNetSocket;

public:
	CNetAcceptor(boost::asio::io_service &ios);
	virtual ~CNetAcceptor();

public:
	virtual bool Listen(INetClientManager* pNetClientManager, unsigned short wPort);
	virtual void Cancel();

private:
	bool DoAccept();
	void OnAccept(boost::asio::ip::tcp::socket *pAcceptSocket, const boost::system::error_code& ec);

private:
	boost::asio::io_service &m_ioService;
	boost::asio::ip::tcp::acceptor m_acceptor;
	INetClientManager *m_pNetClientManager;
	//MapSocket m_mapAcceptSocket;
	//MapNetSocket m_mapNetSocket;
};

NS_IO_Footer

#endif	//_ACCEPTOR_H_