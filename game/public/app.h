//-------------------------------------------
// author : alterhz
// date : 6/18/2015
// description : 

#ifndef _APP_H_
#define _APP_H_

#include "netio.h"
#include "event.h"
#include "utilityinc.h"
#include "thread.h"

using namespace NS_IO;

class CApp : public Singleton<CApp>, public ITimerEvent, public IThread
{
public:
	CApp();
	~CApp();

public:
	// 运行
	bool Run();
	// 停止
	void Stop();

	// 投递异步事件
	bool PostAsyncEvent(IAsyncEvent *pAsyncEvent);

	// 计时器设置和取消
	int SetTimer(ITimerEvent *pTimerEvent, int nInterval);
	void KillTimer(int nTimerId);

private:
	bool InitNet();
	bool InitDb();

protected:
	// 初始化
	bool OnInit();
	// 线程循环
	virtual bool OnThreadRun();
	// 返回true:继续；返回false:终止
	virtual bool OnTimerEvent(int nTimerId);

public:
	INetService *m_pNetService;
	INetAcceptor *m_pNetAcceptor;
	ITimerManager *m_pTimerManager;
	IEventManager *m_pEventManager;
	int m_nRunTimerId;
};

#endif