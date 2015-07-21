//-------------------------------------------
// author : alterhz
// date : 6/18/2015
// description : 

#ifndef _APP_H_
#define _APP_H_

#include "netio.h"
#include "event.h"
#include "UtilityInc.h"

using namespace NS_IO;

class CApp : public Singleton<CApp>
{
public:
	CApp();
	~CApp();

public:
	bool InitNet();
	bool InitDb();

	void Stop();

	bool DoTick();

	bool PostAsyncEvent(IAsyncEvent *pAsyncEvent);

	int SetTimer(ITimerEvent *pTimerEvent, int nInterval);

	void KillTimer(int nTimerId);

public:
	INetService *m_pNetService;
	IEventManager *m_pEventManager;
};

#endif