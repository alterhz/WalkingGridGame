#include "netio.h"
#include "service.h"

NS_IO_Header

INetService * CreateNetService()
{
	return new CNetService();
}

void DestoryNetService(INetService *pNetService)
{
	delete pNetService;
	pNetService = nullptr;
}

NS_IO_Footer