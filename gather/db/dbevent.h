//-------------------------------------------
// author : alterhz
// date : 6/25/2015
// description : 

#ifndef _DBEVENT_H_
#define _DBEVENT_H_

#include <string>
#include "event.h"
using namespace NS_IO;

class CEvent_TestDb : public IAsyncEvent
{
public:
	~CEvent_TestDb() {}

public:
	// 多线程执行
	virtual bool OnExcute();
	// 单线程执行
	virtual bool OnResult();
};

#endif