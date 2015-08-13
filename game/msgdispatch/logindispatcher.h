//-------------------------------------------
// author : alterhz
// date : 8/13/2015
// description : 

#ifndef _LOGINDISPATCHER_H_
#define _LOGINDISPATCHER_H_

#include "observer.h"

class CLoginDispatcher : public Observer2::IDispatcher<CLoginDispatcher, const char *, int>
{
public:
	virtual void OnInit();

private:
	void OnRecvTest(const char *pMessage, int nLength);

};

#endif