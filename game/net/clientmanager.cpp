#include "clientmanager.h"
#include "client.h"
#include "debug.h"

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




