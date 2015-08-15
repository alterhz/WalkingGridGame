#include "gamedispatcher.h"
#include "debug.h"

using namespace NS_IO;

void CGameDispatcher::OnInit()
{
	RegMessage(gproto::CSID_C2G_HeartBeat, &CGameDispatcher::OnHeartBeat, this);
}

void CGameDispatcher::OnHeartBeat(int nProtoId, const char *pMessage, int nLength, CClient *pClient)
{
	LOGDebug("CGameDispatcherÐÄÌø£¡");
}
