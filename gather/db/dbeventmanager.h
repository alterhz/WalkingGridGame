//-------------------------------------------
// author : alterhz
// date : 6/25/2015
// description : 

#ifndef _DBEVENTMANAGER_H_
#define _DBEVENTMANAGER_H_

#include "event.h"
using namespace NS_IO;

class CDbEventManager
{
public:
	CDbEventManager();
	~CDbEventManager();

	bool Init();

	bool PostEvent(IAsyncEvent *pAsyncEvent);

	bool DoTick(int nExcuteCount = 1);

	


public:
	IEventManager *m_pEventManager;
};

#endif