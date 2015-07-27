//-------------------------------------------
// author : alterhz
// date : 6/17/2015
// description : 

#ifndef _SOCKET_H_
#define _SOCKET_H_

#include "netio.h"
#include "boostinc.h"
#include "ringbuffer.h"

NS_IO_Header

class CNetSocket : public INetSocket
{
public:
	CNetSocket();
	virtual ~CNetSocket();

public:
	void DoInit(boost::asio::ip::tcp::socket *pAsioSocket, INetClient *pNetClient);

public:
	virtual bool DoSend(const char *pBuffer, unsigned short wLength);
	virtual void DoClose();

	virtual const char * GetRemoteIP();
	virtual unsigned short GetRemotePort();
	virtual const char * GetLocalIP();
	virtual unsigned short GetLocalPort();

private:
	bool DoRecv();

	// 事件
	void OnRecv(const boost::system::error_code &ec, size_t nByteTransferred);
	void OnSend(const boost::system::error_code &ec, size_t nByteTransferred);

	// 解析消息包
	USHORT ReadPacket(const char *pPacketHead, unsigned short wLength);

protected:
	boost::asio::ip::tcp::socket *m_pAsioSocket;
	int m_nAsyncEventCount;	//异步事件计数

private:
	INetClient *m_pNetClient;

	char m_szRecvBuffer[MAX_RECV_BUFFER_LENGTH];
	int m_nHaveRecvLength;
	char m_szSendBuffer[MAX_SEND_BUFFER_LENGTH];
	int m_nSendLength;

	CRingBuffer m_ringBuffer;
};

NS_IO_Footer

#endif	//_SOCKET_H_