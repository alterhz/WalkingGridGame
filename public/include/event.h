//-------------------------------------------
// author : alterhz
// date : 6/25/2015
// description : 

#ifndef _EVENT_H_
#define _EVENT_H_

#ifndef NS_IO

#define NS_IO		dgs
#define NS_IO_Header	namespace dgs {
#define NS_IO_Footer	}

#endif	//NS_IO

NS_IO_Header

class IAsyncEvent
{
public:
	virtual ~IAsyncEvent() {}

public:
	// 多线程执行
	virtual bool OnExcute() = 0;
	// 单线程执行
	virtual bool OnResult() = 0;
};

class IEventManager
{
public:
	virtual ~IEventManager() {}

	// 停止
	virtual void Stop() = 0;

	// 投递异步事件
	virtual bool PostAsyncEvent(IAsyncEvent *pAsyncEvent) = 0;
};

class ITimerEvent
{
public:
	virtual ~ITimerEvent() {}

public:
	// 返回true:继续；返回false:终止
	virtual bool OnTimerEvent(int nTimerId) = 0;
	// 删除
	virtual void OnKilled(int nTimerId) {}
};

#define INVALID_TIMER_ID	(0)

class ITimerManager
{
public:
	virtual ~ITimerManager() {}

	// 设置Timer事件,返回TimerId，设置失败，返回INVALID_TIMER_ID
	virtual int SetTimer(ITimerEvent *pTimerEvent, int nInterval) = 0;
	// 删除Timer事件
	virtual void KillTimer(int nTimerId) = 0;
};

NS_IO_Footer

#endif