﻿//-------------------------------------------
// author : alterhz
// date : 6/17/2015
// description : 

#ifndef _ACCEPTOR_H_
#define _ACCEPTOR_H_

#include "netio.h"
#include "boostinc.h"

NS_IO_Header

//计时器检测频率
#define TIMER_CHECK_INTERVAL	(1000)

class CNetAcceptor : public INetAcceptor
{
public:
	CNetAcceptor(boost::asio::io_service &ios);
	virtual ~CNetAcceptor();

public:
	virtual bool Listen(INetClientManager* pNetClientManager, unsigned short wPort);
	virtual void Cancel();

private:
	bool DoAccept();
	void OnAccept(boost::asio::ip::tcp::socket *pAcceptSocket, const boost::system::error_code& ec);
	void OnTimerCheck(const boost::system::error_code& ec);

private:
	boost::asio::io_service &m_ioService;
	boost::asio::ip::tcp::acceptor m_acceptor;
	boost::asio::deadline_timer m_timerCheck;
	INetClientManager *m_pNetClientManager;
	int m_nAsyncAcceptErrorCount;
};

NS_IO_Footer

#endif	//_ACCEPTOR_H_