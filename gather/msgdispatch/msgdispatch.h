//-------------------------------------------
// author : alterhz
// date : 6/25/2015
// description : 

#ifndef _MSGDISPATCH_H_
#define _MSGDISPATCH_H_

#include "msginc.h"
#include "utilityinc.h"
#include "event.h"
using namespace NS_IO;

class CClient;

class CMsgDispatch : public Singleton<CMsgDispatch>
{
public:
	CMsgDispatch();
	~CMsgDispatch();
	void SetCurrentClient(CClient *pClient) { m_pCurrentClient = pClient; }

public:
	bool Dispatch(unsigned short wProtocolID, const char *pBuffer, int nLength);

private:
	bool OnMsgC2GHeartBeat(gproto::gather::MSG_C2G_HeartBeat &msg);
	bool OnMsgG2CHeartBeat(gproto::gather::MSG_G2C_HeartBeat &msg);

private:
	CClient *m_pCurrentClient;
};

#endif