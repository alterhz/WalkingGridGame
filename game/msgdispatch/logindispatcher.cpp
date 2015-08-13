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
	RegMessage(1001, &CLoginDispatcher::OnRecvTest, this);
	RegMessage(gproto::gather::CSID_C2G_HeartBeat, &CLoginDispatcher::OnHeartBeat, this);
}

void CLoginDispatcher::OnRecvTest(int nProtoId, const char *pMessage, int nLength, CClient *pClient)
{
	std::cout << pMessage << nLength << std::endl;
}

void CLoginDispatcher::OnHeartBeat(int nProtoId, const char *pMessage, int nLength, CClient *pClient)
{
	DecodeProtoBuf(gproto::gather::MSG_C2G_HeartBeat, msgHeartBeat);

}

