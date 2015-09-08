#include "socket.h"
#include "debug.h"

NS_IO_Header

CNetSocket::CNetSocket() 
	: m_pAsioSocket(nullptr)
	, m_pNetClient(nullptr)
	, m_nAsyncEventCount(0)
	, m_nHaveRecvLength(0)
	, m_nSendLength(0)
	, m_wRemotePort(0)
	, m_wLocalPort(0)
{
}

CNetSocket::~CNetSocket()
{
}

const char * CNetSocket::GetRemoteIP()
{
	return m_strRemoteIP.c_str();
}

unsigned short CNetSocket::GetRemotePort()
{
	return m_wRemotePort;
}

const char * CNetSocket::GetLocalIP()
{
	return m_strLocalIP.c_str();
}

unsigned short CNetSocket::GetLocalPort()
{
	return m_wLocalPort;
}

void CNetSocket::DoInit(boost::asio::ip::tcp::socket *pAsioSocket, INetClient *pNetClient)
{
	if (nullptr == pAsioSocket)
	{
		LOGPrint("nullptr == pAsioSocket");
		return ;
	}

	// 设置nodelay
	boost::asio::ip::tcp::no_delay noDelayOption(true);
	pAsioSocket->set_option(noDelayOption);

	m_pAsioSocket = pAsioSocket;
	m_pNetClient = pNetClient;

	// 记录local和remote的IP和port
	boost::system::error_code ec;
	boost::asio::ip::tcp::endpoint epRemote = m_pAsioSocket->remote_endpoint(ec);
	if (!ec)
	{
		m_strRemoteIP = epRemote.address().to_string();
		m_wRemotePort = epRemote.port();
	}

	boost::asio::ip::tcp::endpoint epLocal = m_pAsioSocket->local_endpoint(ec);	
	if (!ec)
	{
		m_strLocalIP = epLocal.address().to_string();
		m_wLocalPort = epLocal.port();
	}

	// 投递异步Recv
	DoRecv();

	// NetClient触发OnConnect事件
	if (m_pNetClient)
	{
		m_pNetClient->OnConnected(this);
	}
}

bool CNetSocket::DoRecv()
{
	if (nullptr == m_pAsioSocket)
	{
		return false;
	}

	char *pRecvBuffer = m_szRecvBuffer + m_nHaveRecvLength;
	int nFreeRecvLength = MAX_RECV_BUFFER_LENGTH - m_nHaveRecvLength;

	if (0 == nFreeRecvLength)
	{
		// 单个消息包过长，断开连接
		DoClose();

		return false;
	}

	++m_nAsyncEventCount;

	m_pAsioSocket->async_read_some(boost::asio::buffer(pRecvBuffer, nFreeRecvLength), 
		bind(&CNetSocket::OnRecv, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));

	return true;
}

void CNetSocket::OnRecv(const boost::system::error_code& ec, size_t nByteTransferred)
{
	if (ec)
	{
		//ec.value() == boost::asio::error::connection_reset
		//ec.value() == boost::asio::error::eof
		//ec.value() == boost::asio::error::bad_descriptor
		if (ec.value() != boost::asio::error::operation_aborted)
		{
			LOGPrint("OnRecv:errorid:" + ec.value() + ",message:" + ec.message().c_str());
		}
		
		--m_nAsyncEventCount;

		DoClose();

		return ;
	}

	m_nHaveRecvLength += nByteTransferred;

	// 黏包解析
	int nReadLength = 0;	//解析长度修改都是完整消息包

	while (true)
	{
		//读取的位置
		const char *pPacketHead = m_szRecvBuffer + nReadLength;
		//剩余长度
		int nRemainLength = m_nHaveRecvLength - nReadLength;

		USHORT wReadPacketLength = ReadPacket(pPacketHead, nRemainLength);
		if (wReadPacketLength > 0)
		{
			//已读长度
			nReadLength += wReadPacketLength;
		}
		else
		{
			//没读出来，不够一个包，或者已读完
			if ((nRemainLength > 0) && (nRemainLength != m_nHaveRecvLength))
			{
				//已经读取过包，需要移动已经读取的位置到buffer头
				memmove(m_szRecvBuffer, m_szRecvBuffer+nReadLength, nRemainLength);
			}

			m_nHaveRecvLength = nRemainLength;
			break;
		}
	}

	--m_nAsyncEventCount;

	DoRecv();
}

USHORT CNetSocket::ReadPacket(const char *pPacketHead, unsigned short wLength)
{
	if (wLength < sizeof(USHORT))
	{
		// 消息包头长度不足
		return 0;
	}

	USHORT wPacketLength = static_cast<USHORT>(*(USHORT*)(pPacketHead));

	if (sizeof(USHORT) + wPacketLength > wLength)
	{
		// 不足一个完整消息包
		return 0;
	}

	// 一个完整消息包
	const char *pPacketData = (pPacketHead + sizeof(USHORT));

	// 接收数据
	if (m_pNetClient)
	{
		m_pNetClient->OnRecvPacket(pPacketData, wPacketLength);
	}

	return sizeof(USHORT) + wPacketLength;
}

bool CNetSocket::DoSend(const char *pBuffer, unsigned short wLength)
{
	if (nullptr == m_pAsioSocket)
	{
		return false;
	}

	char szData[8] = {0};
	unsigned short *pMessageLength = reinterpret_cast<unsigned short *>(szData);
	*pMessageLength = static_cast<unsigned short>(wLength);

	if (!m_ringBuffer.Write(szData, sizeof(unsigned short)))
	{
		return false;
	}

	if (!m_ringBuffer.Write(pBuffer, wLength))
	{
		return false;
	}

	if (0 == m_nSendLength)
	{
		int nReadLength = MAX_SEND_BUFFER_LENGTH - m_nSendLength;
		m_ringBuffer.Read(m_szSendBuffer, nReadLength);

		m_ringBuffer.DoAutoRelease();

		m_nSendLength = nReadLength;

		unsigned short wLength = *(unsigned short *)m_szSendBuffer;

		// 发送数据
		if (m_nSendLength > 0)
		{
			++m_nAsyncEventCount;

			m_pAsioSocket->async_write_some(boost::asio::buffer(m_szSendBuffer, m_nSendLength), 
				bind(&CNetSocket::OnSend, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
		}
	}
	return true;
}

void CNetSocket::OnSend(const boost::system::error_code &ec, size_t nByteTransferred)
{
	if (nullptr == m_pAsioSocket)
	{
		return ;
	}

	--m_nAsyncEventCount;

	if (ec)
	{
		//ec.value() == boost::asio::error::connection_reset
		//ec.value() == boost::asio::error::eof
		//ec.value() == boost::asio::error::bad_descriptor
		if (ec.value() != boost::asio::error::operation_aborted)
		{
			LOGPrint("OnSend:errorid:" + ec.value() + ",message:" + ec.message().c_str());
		}

		DoClose();

		return ;
	}

	m_nSendLength -= nByteTransferred;

	if (m_nSendLength > 0)
	{
		// 经验来说，只有linux才会进来
		memmove(m_szSendBuffer, m_szSendBuffer + nByteTransferred, m_nSendLength);

		++m_nAsyncEventCount;

		// 发送数据
		m_pAsioSocket->async_write_some(boost::asio::buffer(m_szSendBuffer, m_nSendLength), 
			bind(&CNetSocket::OnSend, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
	}
	else if (0 == m_nSendLength)
	{
		int nReadLength = MAX_SEND_BUFFER_LENGTH - m_nSendLength;
		m_ringBuffer.Read(m_szSendBuffer, nReadLength);

		m_ringBuffer.DoAutoRelease();

		m_nSendLength = nReadLength;

		unsigned short wLength = *(unsigned short *)m_szSendBuffer;

		// 发送数据
		if (m_nSendLength > 0)
		{
			++m_nAsyncEventCount;

			m_pAsioSocket->async_write_some(boost::asio::buffer(m_szSendBuffer, m_nSendLength), 
				bind(&CNetSocket::OnSend, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
		}
	}
	else
	{
		LOGPrint("发送数据长度异常：" + m_nSendLength);
	}
}

void CNetSocket::DoClose()
{
	if (nullptr == m_pAsioSocket)
	{
		return ;
	}

	LOGPrint("DoClose-- m_pAsioSocket addr:" + reinterpret_cast<intptr_t>(m_pAsioSocket));

	// 断开连接
	if (m_pAsioSocket->is_open())
	{
		boost::system::error_code ec;
		m_pAsioSocket->shutdown(boost::asio::socket_base::shutdown_both, ec);
		if (ec)
		{
			LOGPrint("shutdown error[" + ec.value() + "]:" + ec.message());
		}
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

	// 删除网络socket对象
	delete m_pAsioSocket;
	m_pAsioSocket = nullptr;

	LOGPrint("下面释放CNetSocket");

	// 删除自身
	delete this;
}

NS_IO_Footer