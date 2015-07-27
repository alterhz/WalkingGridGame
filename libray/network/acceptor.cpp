#include "acceptor.h"
#include "socket.h"
#include "debug.h"

NS_IO_Header

CNetAcceptor::CNetAcceptor(boost::asio::io_service &ios)
	: m_ioService(ios)
	, m_acceptor(ios)
	, m_pNetClientManager(nullptr)
{
}

CNetAcceptor::~CNetAcceptor()
{
}

bool CNetAcceptor::Listen(INetClientManager* pNetClientManager, unsigned short wPort)
{
	if (nullptr == pNetClientManager)
	{
		return false;
	}

	m_pNetClientManager = pNetClientManager;

	// 设置协议
	boost::asio::ip::tcp::endpoint ep(boost::asio::ip::tcp::v4(), wPort);
	m_acceptor.open(ep.protocol());
	// 设置端口重用
	m_acceptor.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));

	// 绑定端口
	boost::system::error_code ec;
	m_acceptor.bind(ep, ec);

	if (ec)
	{
		LOGPrint("绑定端口[" + wPort + "]失败。");
		// 端口绑定事变
		return false;
	}

	// 监听端口
	m_acceptor.listen();

	for (int i=0; i<3; ++i)
	{
		DoAccept();
	}

	return true;
}

void CNetAcceptor::Cancel()
{
	m_acceptor.close();

	if (m_pNetClientManager)
	{
		m_pNetClientManager->OnRelease();
	}

	delete this;
}

bool CNetAcceptor::DoAccept()
{
	boost::asio::ip::tcp::socket *pNewSocket = new boost::asio::ip::tcp::socket(m_ioService);
	if (nullptr == pNewSocket)
	{
		return false;
	}

	m_acceptor.async_accept(*pNewSocket,
		bind(&CNetAcceptor::OnAccept, this, pNewSocket, boost::asio::placeholders::error));

	return true;
}

void CNetAcceptor::OnAccept(boost::asio::ip::tcp::socket *pAcceptSocket, const boost::system::error_code& ec)
{
	if (ec)
	{
		LOGPrint("OnAccept:errorid:" + ec.value() + ",message:" + ec.message().c_str());

		delete pAcceptSocket;
		pAcceptSocket = nullptr;

		if (ec.value() == boost::asio::error::operation_aborted)
		{
			
		}
		else
		{
			DoAccept();
		}

		return ;
	}

	// 增加一个监听
	DoAccept();

	if (nullptr == m_pNetClientManager)
	{
		return ;
	}

	// 获取NetClient对象
	INetClient *pNewNetClient = m_pNetClientManager->OnAccept();
	if (nullptr == pNewNetClient)
	{
		// 逻辑层创建NetClient失败，断开网络连接
		pAcceptSocket->close();
		delete pAcceptSocket;
		pAcceptSocket = nullptr;

		return ;
	}

	CNetSocket *pNewNetSocket = new CNetSocket();
	if (nullptr == pNewNetSocket)
	{
		return ;
	}

	pNewNetSocket->DoInit(pAcceptSocket, pNewNetClient);
}



NS_IO_Footer