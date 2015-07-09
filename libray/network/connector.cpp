#include "connector.h"
#include "debug.h"


NS_IO_Header

CNetConnector::CNetConnector(boost::asio::io_service &ios)
	: m_ioService(ios)
	, m_pNetClient(nullptr)
	, m_pNetSocket(nullptr)
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

	m_pConnectSocket = pNewSocket;

	boost::asio::ip::tcp::endpoint ep(boost::asio::ip::address::from_string(m_strRemoteIP.c_str()), m_wRemotePort);

	m_pConnectSocket->async_connect(ep, 
		bind(&CNetConnector::OnConnect, this, boost::asio::placeholders::error));

	return true;
}

void CNetConnector::OnConnect(const boost::system::error_code& ec)
{
	if (ec)
	{
		LOGPrint("OnAccept:errorid:" + ec.value() + ",message:" + ec.message().c_str());

		delete m_pConnectSocket;
		m_pConnectSocket = nullptr;

		if (ec.value() == boost::asio::error::operation_aborted)
		{

		}
		else
		{
			
		}

		return ;
	}

	CNetSocket *pNewNetSocket = new CNetSocket(m_pConnectSocket);
	if (nullptr == pNewNetSocket)
	{
		return ;
	}

	m_pNetSocket = pNewNetSocket;

	m_pNetSocket->DoInit(m_pNetClient, this);
}

bool CNetConnector::DoSend(const char *pBuffer, unsigned short wLength)
{
	if (m_pNetSocket)
	{
		return m_pNetSocket->DoSend(pBuffer, wLength);
	}
	
	return false;
}

void CNetConnector::DoClose()
{
	if (m_pNetSocket)
	{
		m_pNetSocket->DoClose();
	}
}

const char * CNetConnector::GetRemoteIP()
{
	if (m_pNetSocket)
	{
		return m_pNetSocket->GetRemoteIP();
	}

	return nullptr;
}

unsigned short CNetConnector::GetRemotePort()
{
	if (m_pNetSocket)
	{
		return m_pNetSocket->GetRemotePort();
	}

	return 0;
}

const char * CNetConnector::GetLocalIP()
{
	if (m_pNetSocket)
	{
		return m_pNetSocket->GetLocalIP();
	}

	return nullptr;
}

unsigned short CNetConnector::GetLocalPort()
{
	if (m_pNetSocket)
	{
		return m_pNetSocket->GetLocalPort();
	}

	return 0;
}

void CNetConnector::OnRelease(INetSocket *pNetSocket)
{
	m_pNetSocket = nullptr;
}

NS_IO_Footer
