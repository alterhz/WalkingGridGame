//-------------------------------------------
// author : alterhz
// date : 6/17/2015
// description : 

#ifndef _SERVICE_H_
#define _SERVICE_H_

#include "netio.h"
#include "boostinc.h"

NS_IO_Header

class CNetService : public INetService
{
public:
	CNetService();
	virtual ~CNetService();

public:
	// Run(阻塞,直到调用stop)
	virtual bool Run();
	// 停止(如果队列有异步事件，无法退出时可以使用强制退出)
	virtual void Stop(bool bForceStop = false);
	// Pool(非阻塞)参数为0:执行所有事件
	virtual bool Poll(int nExcuteCount = 0);

	// 创建网络监听对象
	virtual INetAcceptor * CreateListener();
	// 创建网络连接器
	virtual INetConnector * CreateConnector();
	// 创建TimerManager
	virtual ITimerManager * CreateTimerManager();
	// 创建EventManager
	virtual IEventManager * CreateEventManager(int nThreadNumber = 1);

protected:
	// 强制停止
	void OnStop(const boost::system::error_code &ec);

private:
	boost::asio::io_service m_ioService;
	boost::asio::io_service::work *m_pWork;
	boost::asio::deadline_timer *m_pTimerStop;
};

NS_IO_Footer

#endif	//_SERVICE_H_