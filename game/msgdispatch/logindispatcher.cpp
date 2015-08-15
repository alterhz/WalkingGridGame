#include "logindispatcher.h"
#include "debug.h"
using namespace NS_IO;

// 宏定义协议解析，减少代码量
#define DecodeProtoBuf(proto_msg, msg) proto_msg msg; \
	if (!msg.ParseFromArray(pMessage, nLength)) \
			{ \
			LOGPrint("协议[" + nProtoId + "]解析失败。"); \
			return ; \
			}

void CLoginDispatcher::OnInit()
{
	RegMessage(gproto::CSID_C2G_HeartBeat, &CLoginDispatcher::OnHeartBeat, this);
	RegMessage(gproto::CSID_C2G_StartGame, &CLoginDispatcher::OnStartGame, this);
}

void CLoginDispatcher::OnHeartBeat(int nProtoId, const char *pMessage, int nLength, CClient *pClient)
{
	DecodeProtoBuf(gproto::MSG_C2G_HeartBeat, msgHeartBeat);

	LOGDebug("CLoginDispatcher心跳！");
}

void CLoginDispatcher::OnStartGame(int nProtoId, const char *pMessage, int nLength, CClient *pClient)
{
	DecodeProtoBuf(gproto::MSG_C2G_StartGame, msgStartGame);

	LOGDebug("角色[" + msgStartGame.rolename() + "]开始游戏。");
}

