//-------------------------------------------
// author : alterhz
// date : 6/18/2015
// description : 

#ifndef _CLIENTMANAGER_H_
#define _CLIENTMANAGER_H_

#include "memoryleak.h"
#include "netio.h"
#include "utilityinc.h"
#include "msginc.h"

#include "observer.h"

using namespace NS_IO;

class CClient;

class CClientManager : public INetClientManager, public Singleton<CClientManager>
{
public:
	CClientManager();
	virtual ~CClientManager();

	bool InitProto();

public:	//ÊÂ¼þ
	virtual INetClient * OnAccept();
	// ÊÍ·Å
	virtual void OnRelease() {}

public:
	Observer3::IDispatcherManager<const char *, int, CClient *> gDM;
};

#endif