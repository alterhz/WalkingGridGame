#include "connector.h"
#include "debug.h"


NS_IO_Header

CNetConnector::CNetConnector(boost::asio::io_service &ios)
	: m_ioService(ios)
	, m_pNetClient(nullptr)
	, m_wRemotePort(0)
{
}

CNetConnector::~CNetConnector()
{

}

bool CNetConnector::Connect(INetClient *pNetClient, const char *pIP, unsigned short wPort)
{
	if (nullptr == pNetClient)
	{
		return false;
	}

	m_pNetClient = pNetClient;
	m_strRemoteIP = pIP;
	m_wRemotePort = wPort;

	ReConnect();
	
	return true;
}

bool CNetConnector::ReConnect()
{
	boost::asio::ip::tcp::socket *pNewSocket = new boost::asio::ip::tcp::socket(m_ioService);
	if (nullptr == pNewSocket)
	{
		return false;
	}

	m_pAsioSocket = pNewSocket;

	boost::asio::ip::tcp::endpoint ep(boost::asio::ip::address::from_string(m_strRemoteIP.c_str()), m_wRemotePort);

	m_pAsioSocket->async_connect(ep, 
		bind(&CNetConnector::OnConnect, this, boost::asio::placeholders::error));

	return true;
}

void CNetConnector::OnConnect(const boost::system::error_code& ec)
{
	if (ec)
	{
		if (ec.value() != boost::asio::error::operation_aborted)
		{
			//if (ec.value() != boost::asio::error::connection_refused)
			LOGPrint("OnAccept:errorid:" + ec.value() + ",message:" + ec.message().c_str());
		}

		if (m_pNetClient)
		{
			m_pNetClient->OnError(ec.value());
		}

		delete m_pAsioSocket;
		m_pAsioSocket = nullptr;

		return ;
	}

	CNetSocket::DoInit(m_pAsioSocket, m_pNetClient);
}

void CNetConnector::DoClose()
{
	LOGPrint("DoClose-- m_pAsioSocket addr:" + reinterpret_cast<intptr_t>(m_pAsioSocket));

	// 断开连接
	if (m_pAsioSocket->is_open())
	{
		m_pAsioSocket->shutdown(boost::asio::socket_base::shutdown_both);
		m_pAsioSocket->close();
	}

	if (m_nAsyncEventCount > 0)
	{
		return ;
	}

	if (m_pNetClient)
	{
		m_pNetClient->OnDisconnect();
	}

	LOGPrint("下面释放CNetSocket");

	// 删除网络socket对象
	delete m_pAsioSocket;
	m_pAsioSocket = nullptr;
}

NS_IO_Footer
