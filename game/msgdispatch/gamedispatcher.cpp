#include "gamedispatcher.h"
#include "debug.h"

#include "battleground.h"
#include "client.h"
#include "country.h"
#include "gobject.h"

using namespace NS_IO;

// 宏定义协议解析，减少代码量
#define DecodeProtoBuf(proto_msg, msg) proto_msg msg; \
	if (!msg.ParseFromArray(pMessage, nLength)) \
{ \
	LOGPrint("协议[" + nProtoId + "]解析失败。"); \
	return ; \
}

void CGameDispatcher::OnInit()
{
	RegMessage(gproto::CSID_C2G_HeartBeat, &CGameDispatcher::OnHeartBeat, this);
	RegMessage(gproto::CSID_C2G_Prepare, &CGameDispatcher::OnPrepare, this);
	RegMessage(gproto::CSID_C2G_EnterGround, &CGameDispatcher::OnEnterGround, this);
	RegMessage(gproto::CSID_C2G_LeaveGround, &CGameDispatcher::OnLeaveGround, this);
	//RegMessage(gproto::CSID_C2G_GetGroundInfo, &CGameDispatcher::OnGetGroundInfo, this);
	RegMessage(gproto::CSID_C2G_PrepareFinish, &CGameDispatcher::OnPrepareFinish, this);
	RegMessage(gproto::CSID_C2G_BattleFightEnd, &CGameDispatcher::OnBattleFightEnd, this);
	RegMessage(gproto::CSID_C2G_MoveFight, &CGameDispatcher::OnMoveFight, this);
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
			int nCountryIndexId = pClient->GetCountry()->GetIndexId();
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

void CGameDispatcher::OnEnterGround(int nProtoId, const char *pMessage, int nLength, CClient *pClient)
{
	// 进入场景
	if (nullptr == pClient)
	{
		LOGError("nullptr == pClient");
		return ;
	}

	ICountry *pCountry = pClient->GetCountry();
	if (nullptr == pCountry)
	{
		LOGError("nullptr == pCountry");
		return ;
	}

	pCountry->EnterGround();
}

void CGameDispatcher::OnLeaveGround(int nProtoId, const char *pMessage, int nLength, CClient *pClient)
{
	// 离开场景
	if (nullptr == pClient)
	{
		LOGError("nullptr == pClient");
		return ;
	}

	ICountry *pCountry = pClient->GetCountry();
	if (nullptr == pCountry)
	{
		LOGError("nullptr == pCountry");
		return ;
	}

	pCountry->LeaveGround();
}

void CGameDispatcher::OnGetGroundInfo(int nProtoId, const char *pMessage, int nLength, CClient *pClient)
{
	// 离开场景
	if (nullptr == pClient)
	{
		LOGError("nullptr == pClient");
		return ;
	}

	ICountry *pCountry = pClient->GetCountry();
	if (nullptr == pCountry)
	{
		LOGError("nullptr == pCountry");
		return ;
	}

	IBattleGround *pBattleGround = pCountry->GetBattleGround();
	if (nullptr == pBattleGround)
	{
		LOGError("nullptr == pBattleGround");
		return ;
	}

	pBattleGround->GetGroundInfo(pCountry);
}

void CGameDispatcher::OnPrepareFinish(int nProtoId, const char *pMessage, int nLength, CClient *pClient)
{
	// 准备完毕
	if (nullptr == pClient)
	{
		LOGError("nullptr == pClient");
		return ;
	}

	ICountry *pCountry = pClient->GetCountry();
	if (nullptr == pCountry)
	{
		LOGError("nullptr == pCountry");
		return ;
	}

	IBattleGround *pBattleGround = pCountry->GetBattleGround();
	if (nullptr == pBattleGround)
	{
		LOGError("nullptr == pBattleGround");
		return ;
	}	

	pBattleGround->PrepareFinish(pCountry);
}

void CGameDispatcher::OnBattleFightEnd(int nProtoId, const char *pMessage, int nLength, CClient *pClient)
{
	// 准备完毕
	if (nullptr == pClient)
	{
		LOGError("nullptr == pClient");
		return ;
	}

	ICountry *pCountry = pClient->GetCountry();
	if (nullptr == pCountry)
	{
		LOGError("nullptr == pCountry");
		return ;
	}

	IBattleGround *pBattleGround = pCountry->GetBattleGround();
	if (nullptr == pBattleGround)
	{
		LOGError("nullptr == pBattleGround");
		return ;
	}

	pBattleGround->BattleBoutFinish(pCountry);
}

void CGameDispatcher::OnMoveFight(int nProtoId, const char *pMessage, int nLength, CClient *pClient)
{
	// 准备完毕
	if (nullptr == pClient)
	{
		LOGError("nullptr == pClient");
		return ;
	}

	DecodeProtoBuf(gproto::MSG_C2G_MoveFight, msg);

	ICountry *pCountry = pClient->GetCountry();
	if (nullptr == pCountry)
	{
		LOGError("nullptr == pCountry");
		return ;
	}

	IBattleGround *pBattleGround = pCountry->GetBattleGround();
	if (nullptr == pBattleGround)
	{
		LOGError("nullptr == pBattleGround");
		return ;
	}

	int nGObjectIndexId = msg.gobjectindexid();
	VtCoor2 vtCoor2;
	for (int i=0; i<msg.path_size(); ++i)
	{
		COOR2 coor2(msg.path(i).x(), msg.path(i).y());

		vtCoor2.push_back(coor2);
	}

	IGObject *pMoveGObject = pBattleGround->FindGObject(nGObjectIndexId);
	if (nullptr == pMoveGObject)
	{
		LOGError("nullptr == pMoveGObject");
		return ;
	}

	if (GObjectType_Walkable != pMoveGObject->GetType())
	{
		return ;
	}

	CWalkableObject *pWalkableObject = pMoveGObject->GetWalkableObject();
	if (nullptr == pWalkableObject)
	{
		LOGError("nullptr == pWalkableObject");
		return ;
	}

	if (!pWalkableObject->Move(vtCoor2))
	{
		// 行走失败
		pCountry->SendMoveFight(false, nGObjectIndexId);
	}

	// 攻击
	int nTargetGObjectIndexId = msg.targetgobjectindexid();
	if (0 == nTargetGObjectIndexId)
	{
		// 没有攻击目标
		return ;
	}

	// 攻击处理
	
}
