//-------------------------------------------
// author : alterhz
// date : 8/13/2015
// description : 

#ifndef _GAMEDISPATCHER_H_
#define _GAMEDISPATCHER_H_

#include "memoryleak.h"
#include "observer.h"
#include "msginc.h"

class CClient;

class CGameDispatcher : public Observer3::IDispatcher<CGameDispatcher, const char *, int, CClient *>
{
public:
	virtual void OnInit();

private:
	// ÐÄÌø
	void OnHeartBeat(int nProtoId, const char *pMessage, int nLength, CClient *pClient);
};

#endif