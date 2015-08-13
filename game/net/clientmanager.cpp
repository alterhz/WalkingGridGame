#include "clientmanager.h"
#include "client.h"
#include "debug.h"

#include "logindispatcher.h"
#include "gamedispatcher.h"

CClientManager::CClientManager()
{
}

CClientManager::~CClientManager()
{
}

INetClient * CClientManager::OnAccept()
{
	CClient *pClient = new CClient();

	return pClient;
}

bool CClientManager::InitProto()
{
	gDM.AddDispatcher(new CLoginDispatcher);
	gDM.AddDispatcher(new CGameDispatcher);

	return true;
}




