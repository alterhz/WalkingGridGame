//-------------------------------------------
// author : alterhz
// date : 6/17/2015
// description : 

#ifndef _NETIO_H_
#define _NETIO_H_

#ifndef NS_IO

#define NS_IO		dgs
#define NS_IO_Header	namespace dgs {
#define NS_IO_Footer	}

#endif	//NS_IO

NS_IO_Header

class INetSocket;
class INetAcceptor;

class INetClient
{
public:
	virtual ~INetClient() {}

public:
	// 网络连接事件
	virtual bool OnConnected(INetSocket *pNetSocket) { return true; }
	// 网络断开事件(此函数调用后INetSocket对象销毁)
	virtual bool OnDisconnect() { return true; }
	// 接收数据
	virtual bool OnRecvPacket(const char *pPacket, unsigned short wLength) { return true; }
	// 错误异常
	virtual bool OnError(int nErrCode) { return true; }
};

class INetClientManager
{
public:
	virtual ~INetClientManager() { }

public:	//事件
	virtual INetClient * OnAccept() = 0;
	// 释放(此函数调用INetAcceptor对象销毁)
	virtual void OnRelease() {}
};

class INetSocket
{
public:
	virtual ~INetSocket() {}

public:
	virtual bool DoSend(const char *pBuffer, unsigned short wLength) = 0;
	virtual void DoClose() = 0;

	virtual const char * GetRemoteIP() = 0;
	virtual unsigned short GetRemotePort() = 0;
	virtual const char * GetLocalIP() = 0;
	virtual unsigned short GetLocalPort() = 0;
};

class INetConnector
{
public:
	virtual ~INetConnector() {}

public:
	virtual bool Connect(INetClient *pNetClient, const char *pIP, unsigned short wPort) = 0;
	virtual bool ReConnect() = 0;
};

class INetAcceptor
{
public:
	virtual ~INetAcceptor() {}

public:
	virtual bool Listen(INetClientManager* pNetClientManager, unsigned short wPort) = 0;
	virtual void Cancel() = 0;
};

class IEventManager;

class INetService
{
public:
	virtual ~INetService() {}
	
public:
	// 触发逻辑循环(队列中数据处理完毕返回false)
	virtual bool DoTick(int nExcuteCount = 1) = 0;
	// Run(阻塞,直到调用stop)
	virtual bool Run() = 0;
	// Pool(非阻塞)参数为0:执行所有事件
	virtual bool Poll(int nExcuteCount = 0) = 0;
	// 创建网络监听对象
	virtual INetAcceptor * CreateListener() = 0;
	// 创建网络连接器
	virtual INetConnector * CreateConnector() = 0;
	// 创建EventManager
	virtual IEventManager * CreateEventManager(int nThreadNumber = 1) = 0;
};

INetService * CreateNetService();
void DestoryNetService(INetService *pNetService);

NS_IO_Footer

#endif	//_NETIO_H_