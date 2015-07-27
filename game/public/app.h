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

class CApp : public Singleton<CApp>, public ITimerEvent
{
public:
	CApp();
	~CApp();

public:
	bool InitNet();
	bool InitDb();

	void Stop();

	bool Run();

	bool PostAsyncEvent(IAsyncEvent *pAsyncEvent);

	int SetTimer(ITimerEvent *pTimerEvent, int nInterval);

	void KillTimer(int nTimerId);

protected:
	// ·µ»Øtrue:¼ÌÐø£»·µ»Øfalse:ÖÕÖ¹
	virtual bool OnTimerEvent(int nTimerId);

public:
	INetService *m_pNetService;
	ITimerManager *m_pTimerManager;
	IEventManager *m_pEventManager;
};

#endif