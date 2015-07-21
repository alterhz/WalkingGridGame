#include "msgdispatch.h"
#include "client.h"
#include "debug.h"
#include "app.h"

// 宏定义协议解析，减少代码量
#define DecodeProtoBuf(proto_msg, msg) proto_msg msg; \
	if (!msg.ParseFromArray(pBuffer, nLength)) \
			{ \
				LOGPrint("协议[" + wProtocolID + "]解析失败。"); \
				return false; \
			}

CMsgDispatch::CMsgDispatch()
	: m_pCurrentClient(nullptr)
{
}

CMsgDispatch::~CMsgDispatch()
{
}

bool CMsgDispatch::Dispatch(unsigned short wProtocolID, const char *pBuffer, int nLength)
{
	switch (wProtocolID)
	{
	case gproto::gather::CSID_G2C_HeartBeat:
		{
			DecodeProtoBuf(gproto::gather::MSG_G2C_HeartBeat, msgHeartBeat);
			return OnMsgG2CHeartBeat(msgHeartBeat);
		}
		break;
	case gproto::gather::CSID_C2G_HeartBeat:
		{
			DecodeProtoBuf(gproto::gather::MSG_C2G_HeartBeat, msgHeartBeat);
			return OnMsgC2GHeartBeat(msgHeartBeat);
		}
		break;
	default:
		break;
	}

	return true;
}

bool CMsgDispatch::OnMsgC2GHeartBeat(gproto::gather::MSG_C2G_HeartBeat &msg)
{
	LOGPrint("接收到心跳，并回发。");

	if (nullptr == m_pCurrentClient)
	{
		return false;
	}

	gproto::gather::MSG_G2C_HeartBeat msgHeartBeat;
	m_pCurrentClient->SendMessage(gproto::gather::CSID_G2C_HeartBeat, &msgHeartBeat);

	{
		// 测试代码
		// 投递异步事件
		//CApp::getMe().PostAsyncEvent(new CEvent_TestDb());
		
	}

	return true;
}

bool CMsgDispatch::OnMsgG2CHeartBeat(gproto::gather::MSG_G2C_HeartBeat  &msg)
{
	LOGPrint("收到心跳回应。");

	return true;
}

