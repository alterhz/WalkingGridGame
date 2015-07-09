//-------------------------------------------
// author : alterhz
// date : 6/18/2015
// description : 

#ifndef _CLIENTMANAGER_H_
#define _CLIENTMANAGER_H_

#include "netio.h"
#include "UtilityInc.h"
#include "msginc.h"

using namespace NS_IO;

class CClientManager : public INetClientManager, public Singleton<CClientManager>
{
public:
	CClientManager();
	virtual ~CClientManager();

public:	//ÊÂ¼þ
	virtual INetClient * OnAccept();
	// ÊÍ·Å
	virtual void OnRelease() {}
};

#endif