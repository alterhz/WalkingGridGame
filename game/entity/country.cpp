#include "country.h"

#include "debug.h"

#include "app.h"
#include "battleground.h"
#include "client.h"
#include "ground.h"
#include "gobject.h"


ICountry::ICountry()
	: m_nIndexId(0)
	, m_pClient(nullptr)
	, m_tDisconnect(0)
	, m_nPrepareBattleGroundIndexId(0)
	, m_pBattleGround(nullptr)
{
	static int g_nIndexId = 0;
	m_nIndexId = (++g_nIndexId);
}

bool ICountry::SendMessage(unsigned short wProtocolId, google::protobuf::Message *pMessage)
{
	if (nullptr == m_pClient)
	{
		LOGError("nullptr == m_pClient");
		return false;
	}

	return m_pClient->SendMessage(wProtocolId, pMessage);
}

//bool ICountry::SendPrepare(bool bOk)
//{
//	gproto::MSG_G2C_Prepare msg;
//	if (bOk)
//	{
//		msg.set_ret(gproto::MSG_G2C_Prepare_EResult_OK);
//	}
//	else
//	{
//		msg.set_ret(gproto::MSG_G2C_Prepare_EResult_ERR);
//	}
//
//	return SendMessage(gproto::CSID_G2C_Prepare, &msg);
//}

bool ICountry::SendGetGroundInfo(int nWGCount, int nHGCount, const MapGrid &mapGrid, const MapGObject &mapGObject)
{
	gproto::MSG_G2C_GetGroundInfo msg;
	msg.set_wgcount(nWGCount);
	msg.set_hgcount(nHGCount);

	auto itGrid = mapGrid.begin();
	for (; itGrid != mapGrid.end(); ++itGrid)
	{
		gproto::Info_Grid *pInfoGrid = msg.add_grids();
		if (pInfoGrid)
		{
			pInfoGrid->set_sn(itGrid->second->GetSN());
			pInfoGrid->set_x(itGrid->second->GetX());
			pInfoGrid->set_y(itGrid->second->GetY());
		}
	}

	auto itGObject = mapGObject.begin();
	for ( ; itGObject != mapGObject.end(); ++itGObject)
	{
		gproto::Info_GObject *pInfoGObject = msg.add_gobjects();
		if (pInfoGObject)
		{
			if (GObjectType_Still == itGObject->second->GetType())
			{
				pInfoGObject->set_type(gproto::Info_GObject_EType_Still);
			}
			else if (GObjectType_Walkable == itGObject->second->GetType())
			{
				pInfoGObject->set_type(gproto::Info_GObject_EType_Walkable);
			}
			else
			{
				pInfoGObject->set_type(gproto::Info_GObject_EType_None);

				LOGError("场景对象类型错误：" + static_cast<int>(itGObject->second->GetType()));
			}

			pInfoGObject->set_indexid(itGObject->second->GetIndexId());
			pInfoGObject->set_sn(itGObject->second->GetSN());
			pInfoGObject->set_x(itGObject->second->GetX());
			pInfoGObject->set_y(itGObject->second->GetY());
			pInfoGObject->set_hp(itGObject->second->GetHP());
			pInfoGObject->set_maxhp(itGObject->second->GetMaxHP());
			pInfoGObject->set_sp(itGObject->second->GetSP());
			pInfoGObject->set_level(itGObject->second->GetLevel());
			pInfoGObject->set_campid(itGObject->second->GetCampId());
		}
	}

	return SendMessage(gproto::CSID_G2C_GetGroundInfo, &msg);
}

bool ICountry::SendPrepareGround()
{
	gproto::MSG_G2C_PrepareGround msg;
	return SendMessage(gproto::CSID_G2C_PrepareGround, &msg);
}

bool ICountry::SendEnterGround(bool bOk)
{
	gproto::MSG_G2C_EnterGround msg;

	if (bOk)
	{
		msg.set_ret(gproto::MSG_G2C_EnterGround_EResult_OK);
	}
	else
	{
		msg.set_ret(gproto::MSG_G2C_EnterGround_EResult_ERR);
	}

	return SendMessage(gproto::CSID_G2C_EnterGround, &msg);
}

bool ICountry::SendLeaveGround(bool bOk)
{
	gproto::MSG_G2C_LeaveGround msg;

	if (bOk)
	{
		msg.set_ret(gproto::MSG_G2C_LeaveGround_EResult_OK);
	}
	else
	{
		msg.set_ret(gproto::MSG_G2C_LeaveGround_EResult_ERR);
	}

	return SendMessage(gproto::CSID_G2C_LeaveGround, &msg);
}


void ICountry::OnConnected(CClient *pClient)
{
	m_pClient = pClient;
}

void ICountry::OnDisconnect()
{
	m_pClient = nullptr;

}

bool ICountry::AddFightGObject(IGObject *pFightGObject)
{
	if (nullptr == pFightGObject)
	{
		LOGError("nullptr == pFightGObject");
		return false;
	}

	m_vtFightGObject.push_back(pFightGObject);

	return true;
}

bool ICountry::AddFightGObject(const VtGObject &vtFightGObject)
{
	for (IGObject *pFightGObject : vtFightGObject)
	{
		m_vtFightGObject.push_back(pFightGObject);
	}

	return true;
}

void ICountry::PrepareGround(int nBattleGroundIndexId)
{
	m_nPrepareBattleGroundIndexId = nBattleGroundIndexId;

	SendPrepareGround();
}

void ICountry::EnterGround()
{
	if (0 == m_nPrepareBattleGroundIndexId)
	{
		SendEnterGround(false);
		return ;
	}

	if (nullptr != m_pBattleGround)
	{
		SendEnterGround(false);

		LOGDebug("Country[CountryIndexId:" + GetIndexId() + "]在场景中[battleGroundIndexId:" + m_pBattleGround->GetIndexId() + "]。");
		return ;
	}

	IBattleGround *pBattleGround = CBattleGroundManager::getMe().FindBattleGround(m_nPrepareBattleGroundIndexId);
	if (nullptr == pBattleGround)
	{
		SendEnterGround(false);

		LOGError("BattleGround[" + m_nPrepareBattleGroundIndexId + "]不存在。");
		return ;
	}

	if (pBattleGround->Enter(this))
	{
		m_nPrepareBattleGroundIndexId = 0;

		SendEnterGround(true);
	}
	else
	{
		SendEnterGround(false);
	}
}

void ICountry::LeaveGround()
{
	if (m_pBattleGround)
	{
		if (m_pBattleGround->Leave(this))
		{
			SendLeaveGround(true);
		}
		else
		{
			SendLeaveGround(false);
		}
	}
	else
	{
		SendLeaveGround(false);
	}
}




// CCountry
CCountry::CCountry()
{
}

CCountry::~CCountry()
{

}

// CCountryMananger
CCountryManager::CCountryManager()
	: m_nTimerId(0)
{
	m_nTimerId = CApp::getMe().SetTimer(this, 1000);
}

CCountryManager::~CCountryManager()
{
	CApp::getMe().KillTimer(m_nTimerId);
	m_nTimerId = 0;
}

ICountry * CCountryManager::CreateCountry()
{
	ICountry *pNetCountry = new CCountry();
	if (nullptr == pNetCountry)
	{
		LOGError("nullptr == pNetCountry");
		return false;
	}

	m_mapCountry.insert(std::make_pair(pNetCountry->GetIndexId(), pNetCountry));

	return pNetCountry;
}

bool CCountryManager::PushPrepareList(ICountry *pCountry)
{
	if (nullptr == pCountry)
	{
		LOGError("nullptr == pCountry");
		return false;
	}

	for (ICountry *pTmpCountry : m_listCountry)
	{
		if (pTmpCountry == pCountry)
		{
			// 已经存在
			return false;
		}
	}

	m_listCountry.push_back(pCountry);

	return true;
}

bool CCountryManager::OnTimerEvent(int nTimerId)
{
	while (m_listCountry.size() >= 2)
	{
		ICountry *pCountryA = m_listCountry.front();
		m_listCountry.pop_front();
		ICountry *pCountryB = m_listCountry.front();
		m_listCountry.pop_front();

		OnMatched(pCountryA, pCountryB);
	}

	return true;
}

void CCountryManager::OnMatched(ICountry *pCountryA, ICountry *pCountryB)
{
	LOGDebug("两个国家(部队)匹配成功。");

	if (nullptr == pCountryA || nullptr == pCountryB)
	{
		LOGError("nullptr == pCountryA || nullptr == pCountryB");
		return ;
	}

	// 创建场景
	CFrontBattleGround *pNewFrontBattleGround = CBattleGroundManager::getMe().CreateFrontBattleGround();
	if (nullptr == pNewFrontBattleGround)
	{
		LOGError("nullptr == pNewBattleGround");
		return ;
	}

	// 测试，给双方添加部队
	{
		VtGObject vtGObject;
		// 添加将领
		CStillObject*pSirdar = new CStillObject();
		if (pSirdar)
		{
			pSirdar->Init(4);

			vtGObject.push_back(pSirdar);
		}

		// 添加英雄
		CWalkableObject *pHeroA = new CWalkableObject();
		if (pHeroA)
		{
			pHeroA->Init(2);

			vtGObject.push_back(pHeroA);
		}

		CWalkableObject *pHeroB = new CWalkableObject();
		if (pHeroB)
		{
			pHeroB->Init(3);

			vtGObject.push_back(pHeroB);
		}

		// 添加小兵
		CWalkableObject *pDogFaceA = new CWalkableObject();
		if (pDogFaceA)
		{
			pDogFaceA->Init(1);

			vtGObject.push_back(pDogFaceA);
		}

		CWalkableObject *pDogFaceB = new CWalkableObject();
		if (pDogFaceB)
		{
			pDogFaceB->Init(1);

			vtGObject.push_back(pDogFaceB);
		}

		pCountryA->ClearFightGObject();
		pCountryA->AddFightGObject(vtGObject);
	}

	{
		VtGObject vtGObject;
		// 添加将领
		CStillObject*pSirdar = new CStillObject();
		if (pSirdar)
		{
			pSirdar->Init(4);

			vtGObject.push_back(pSirdar);
		}

		// 添加英雄
		CWalkableObject *pHeroA = new CWalkableObject();
		if (pHeroA)
		{
			pHeroA->Init(2);

			vtGObject.push_back(pHeroA);
		}

		CWalkableObject *pHeroB = new CWalkableObject();
		if (pHeroB)
		{
			pHeroB->Init(3);

			vtGObject.push_back(pHeroB);
		}

		// 添加小兵
		CWalkableObject *pDogFaceA = new CWalkableObject();
		if (pDogFaceA)
		{
			pDogFaceA->Init(1);

			vtGObject.push_back(pDogFaceA);
		}

		CWalkableObject *pDogFaceB = new CWalkableObject();
		if (pDogFaceB)
		{
			pDogFaceB->Init(1);

			vtGObject.push_back(pDogFaceB);
		}

		pCountryB->ClearFightGObject();
		pCountryB->AddFightGObject(vtGObject);
	}

	// 准备场景
	pCountryA->PrepareGround(pNewFrontBattleGround->GetIndexId());
	pCountryB->PrepareGround(pNewFrontBattleGround->GetIndexId());
}


