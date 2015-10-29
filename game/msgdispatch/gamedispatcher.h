//-------------------------------------------
// author : alterhz
// date : 8/13/2015
// description : 

#ifndef _GAMEDISPATCHER_H_
#define _GAMEDISPATCHER_H_

#include "observer.h"
#include "msginc.h"

class CClient;

class CGameDispatcher : public Observer3::IDispatcher<CGameDispatcher, const char *, int, CClient *>
{
public:
	virtual void OnInit();

private:
	// 心跳
	void OnHeartBeat(int nProtoId, const char *pMessage, int nLength, CClient *pClient);
	// 准备
	void OnPrepare(int nProtoId, const char *pMessage, int nLength, CClient *pClient);
	// 进入场景
	void OnEnterGround(int nProtoId, const char *pMessage, int nLength, CClient *pClient);
	// 离开场景
	void OnLeaveGround(int nProtoId, const char *pMessage, int nLength, CClient *pClient);
	// 获取场景数据
	void OnGetGroundInfo(int nProtoId, const char *pMessage, int nLength, CClient *pClient);
	// 准备完成
	void OnPrepareFinish(int nProtoId, const char *pMessage, int nLength, CClient *pClient);
	// 战斗完成
	void OnBattleFightEnd(int nProtoId, const char *pMessage, int nLength, CClient *pClient);
};

#endif