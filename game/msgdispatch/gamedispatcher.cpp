#include "gamedispatcher.h"
#include "debug.h"

#include "client.h"
#include "country.h"

using namespace NS_IO;

void CGameDispatcher::OnInit()
{
	RegMessage(gproto::CSID_C2G_HeartBeat, &CGameDispatcher::OnHeartBeat, this);
	RegMessage(gproto::CSID_C2G_Prepare, &CGameDispatcher::OnPrepare, this);
}

void CGameDispatcher::OnHeartBeat(int nProtoId, const char *pMessage, int nLength, CClient *pClient)
{
	LOGDebug("CGameDispatcher心跳！");
}

void CGameDispatcher::OnPrepare(int nProtoId, const char *pMessage, int nLength, CClient *pClient)
{
	LOGDebug("准备！");

	if (nullptr == pClient->GetCountry())
	{
		// 创建国家部队
		ICountry *pCountry = CCountryManager::getMe().CreateCountry();
		if (nullptr == pCountry)
		{
			LOGError("nullptr == pCountry");
			return;
		}

		pClient->SetCountry(pCountry);
		// 触发国家的网络连接事件
		pCountry->OnConnected(pClient);
	}

	if (pClient->GetCountry())
	{
		// 添加到准备队列
		if (CCountryManager::getMe().PushPrepareList(pClient->GetCountry()))
		{
			// 准备成功
			pClient->SendPrepare(true);
		}
		else
		{
			// 准备失败
			pClient->SendPrepare(false);
		}
	}
	else
	{
		LOGError("准备时，没有Country对象。");
		// 准备失败
		pClient->SendPrepare(false);
	}
}
