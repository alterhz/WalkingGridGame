#include "battleground.h"

#include "debug.h"

#include "app.h"
#include "country.h"
#include "gobject.h"
#include "ground.h"


IBattleGround::IBattleGround()
	: m_nIndexId(0)
	, m_eStatus(EStatus_Waiting)
{
	static int g_nIndexId = 0;
	m_nIndexId = (++g_nIndexId);
}

IBattleGround::~IBattleGround()
{
}

bool IBattleGround::Init()
{
	return OnInit();
}

bool IBattleGround::DoTick()
{
	return OnTick();
}

bool IBattleGround::ChangeStatus(EStatus eStatus)
{
	switch (m_eStatus)
	{
	case IBattleGround::EStatus_None:
		break;
	case IBattleGround::EStatus_Waiting:
		{
			if (EStatus_Run == eStatus)
			{
				m_eStatus = EStatus_Run;
				return true;
			}
		}
		break;
	case IBattleGround::EStatus_Run:
		{
			if (EStatus_Finish == eStatus)
			{
				m_eStatus = EStatus_Finish;
				return true;
			}
		}
		break;
	case IBattleGround::EStatus_Pause:
		break;
	case IBattleGround::EStatus_Finish:
		break;
	default:
		break;
	}

	return false;
}

void IBattleGround::GetGroundInfo(ICountry *pCountry)
{
	if (nullptr == pCountry)
	{
		LOGError("nullptr == pCountry");
		return ;
	}

	pCountry->SendGetGroundInfo(m_nWGCount, m_nHGCount, m_mapGrid, m_mapGObject);
}

ICountry * IBattleGround::FindCountry(int nIndexId)
{
	MapCountry::iterator itCountry = m_mapCountry.find(nIndexId);
	if (itCountry != m_mapCountry.end())
	{
		return itCountry->second;
	}

	return nullptr;
}

ICountry * IBattleGround::FindCountry(int nIndexId) const
{
	MapCountry::const_iterator itCountry = m_mapCountry.find(nIndexId);
	if (itCountry != m_mapCountry.end())
	{
		return itCountry->second;
	}

	return nullptr;
}

bool IBattleGround::Enter(ICountry *pCountry)
{
	if (nullptr == pCountry)
	{
		LOGError("nullptr == pCountry");
		return false;
	}

	if (FindCountry(pCountry->GetIndexId()))
	{
		LOGError("Country[" + pCountry->GetIndexId() + "]重复进入战场。");
		return false;
	}

	// 进入战场完毕
	m_mapCountry.insert(std::make_pair(pCountry->GetIndexId(), pCountry));
	pCountry->SetBattleGround(this);

	return true;
}

bool IBattleGround::Leave(ICountry *pCountry)
{
	if (nullptr == pCountry)
	{
		LOGError("nullptr == pCountry");
		return false;
	}

	int nCountryIndexId = pCountry->GetIndexId();

	if (FindCountry(nCountryIndexId))
	{
		pCountry->SetBattleGround(nullptr);

		m_mapCountry.erase(nCountryIndexId);
	}

	return true;
}

bool IBattleGround::GObjectEnter(IGObject *pGObject)
{
	if (nullptr == pGObject)
	{
		LOGError("nullptr == pGObject");
		return false;
	}

	int nGObjectIndexId = pGObject->GetIndexId();

	if (FindGObject(nGObjectIndexId))
	{
		LOGError("场景中已经存在当前GObject[" + nGObjectIndexId + "]。");
		return false;
	}

	m_mapGObject.insert(std::make_pair(pGObject->GetIndexId(), pGObject));

	pGObject->SetBattleGround(this);

	OnGObjectEnter(pGObject);

	int nX = pGObject->GetX();
	int nY = pGObject->GetY();

	IGrid *pGrid = GetGrid(nX, nY);
	if (nullptr == pGrid)
	{
		return false;
	}

	pGrid->AddGObject(pGObject);

	return true;
}

bool IBattleGround::GObjectLeave(IGObject *pGObject)
{
	if (nullptr == pGObject)
	{
		LOGError("nullptr == pGObject");
		return false;
	}

	int nGObjectIndexId = pGObject->GetIndexId();

	if (!FindGObject(nGObjectIndexId))
	{
		LOGError("场景中不存在当前GObject[" + nGObjectIndexId + "]。");
		return false;
	}

	OnGObjectLeave(pGObject);

	m_mapGObject.erase(nGObjectIndexId);

	pGObject->SetBattleGround(nullptr);

	int nX = pGObject->GetX();
	int nY = pGObject->GetY();

	IGrid *pGrid = GetGrid(nX, nY);
	if (nullptr == pGrid)
	{
		return false;
	}

	pGrid->DelGObject(pGObject);

	return true;
}

IGObject * IBattleGround::FindGObject(int nIndexId)
{
	MapGObject::iterator itGObject = m_mapGObject.find(nIndexId);
	if (itGObject != m_mapGObject.end())
	{
		return itGObject->second;
	}

	return nullptr;
}

IGObject * IBattleGround::FindGObject(int nIndexId) const
{
	MapGObject::const_iterator itGObject = m_mapGObject.find(nIndexId);
	if (itGObject != m_mapGObject.end())
	{
		return itGObject->second;
	}

	return nullptr;
}

const int G_nDemoWidthCount = 20;
const int G_nDemoHeigthCount = 30;

CFrontBattleGround::CFrontBattleGround()
{
}

CFrontBattleGround::~CFrontBattleGround()
{
}

bool CFrontBattleGround::OnInit()
{
	int nHGCount = G_nDemoHeigthCount;
	int nWGCount = G_nDemoWidthCount;

	// 初始化场景地图
	InitGroundSize(nWGCount, nHGCount);

	// 初始化地形数据
	for (int nY=0; nY<nHGCount; ++nY)
	{
		for (int nX=0; nX<nWGCount; ++nX)
		{
			IGrid *pNewGrid = new CGrid(nX, nY);
			if (nullptr == pNewGrid)
			{
				LOGError("nullptr == pNewGrid");
				continue;
			}

			int nSN = 1;

			if (!pNewGrid->Init(nSN))
			{
				LOGError("初始化地图格子[SN:" + nSN + "]失败。");
				continue;
			}

			SetGrid(nX, nY, pNewGrid);
		}
	}

	// 加载场景（固定）单位，如桥梁，树木，城墙，大炮

	// 地图中心放入3个桥梁
	{
		CStillObject *pNewGBridge = new CStillObject();
		if (nullptr == pNewGBridge)
		{
			LOGError("nullptr == pNewGBridge");
			return false;
		}

		pNewGBridge->Init(1);
		pNewGBridge->SetCampId(0);
		pNewGBridge->SetLevel(1);

		pNewGBridge->EnterGround(10, 15, this);
	}

	{
		CStillObject *pNewGBridge = new CStillObject();
		if (nullptr == pNewGBridge)
		{
			LOGError("nullptr == pNewGBridge");
			return false;
		}

		pNewGBridge->Init(2);
		pNewGBridge->SetCampId(0);
		pNewGBridge->SetLevel(1);

		pNewGBridge->EnterGround(8, 15, this);
	}

	{
		CStillObject *pNewGBridge = new CStillObject();
		if (nullptr == pNewGBridge)
		{
			LOGError("nullptr == pNewGBridge");
			return false;
		}

		pNewGBridge->Init(2);
		pNewGBridge->SetCampId(0);
		pNewGBridge->SetLevel(1);

		pNewGBridge->EnterGround(12, 15, this);
	}


	// 加载场景（移动）中立单位
	{
		CWalkableObject *pNewDogFace = new CWalkableObject();
		if (nullptr == pNewDogFace)
		{
			LOGError("nullptr == pNewDogFace");
			return false;
		}

		pNewDogFace->Init(1);
		pNewDogFace->SetCampId(0);
		pNewDogFace->SetLevel(1);

		pNewDogFace->EnterGround(1, 15, this);
	}

	{
		CWalkableObject *pNewDogFace = new CWalkableObject();
		if (nullptr == pNewDogFace)
		{
			LOGError("nullptr == pNewDogFace");
			return false;
		}

		pNewDogFace->Init(1);
		pNewDogFace->SetCampId(0);
		pNewDogFace->SetLevel(1);

		pNewDogFace->EnterGround(18, 15, this);
	}

	return true;
}

bool CFrontBattleGround::OnTick()
{
	//LOGDebug("阵地战DoTick");
	return true;
}


CBattleGroundManager::CBattleGroundManager()
	: m_nDoTickTimerId(0)
{
	m_nDoTickTimerId = CApp::getMe().SetTimer(this, 100);
}

CBattleGroundManager::~CBattleGroundManager()
{
	CApp::getMe().KillTimer(m_nDoTickTimerId);
	m_nDoTickTimerId = 0;
}

bool CBattleGroundManager::Init()
{
	return true;
}

CFrontBattleGround* CBattleGroundManager::CreateFrontBattleGround()
{
	CFrontBattleGround *pNewFrontBattleGround = new CFrontBattleGround();
	if (nullptr == pNewFrontBattleGround)
	{
		LOGError("nullptr == pNewFrontBattleGround");
		return nullptr;
	}

	pNewFrontBattleGround->Init();

	m_mapBattleGround.insert(std::make_pair(pNewFrontBattleGround->GetIndexId(), pNewFrontBattleGround));

	return pNewFrontBattleGround;
}

bool CBattleGroundManager::OnTimerEvent(int nTimerId)
{
	if (m_nDoTickTimerId == nTimerId)
	{
		auto itBattleGround = m_mapBattleGround.begin();
		for (; itBattleGround!=m_mapBattleGround.end(); ++itBattleGround)
		{
			itBattleGround->second->DoTick();
		}
	}

	return true;
}

IBattleGround * CBattleGroundManager::FindBattleGround(int nBattleGroundIndexId)
{
	MapBattleGround::iterator itBattleGround = m_mapBattleGround.find(nBattleGroundIndexId);
	if (itBattleGround != m_mapBattleGround.end())
	{
		return itBattleGround->second;
	}

	return nullptr;
}

IBattleGround * CBattleGroundManager::FindBattleGround(int nBattleGroundIndexId) const
{
	MapBattleGround::const_iterator itBattleGround = m_mapBattleGround.find(nBattleGroundIndexId);
	if (itBattleGround != m_mapBattleGround.end())
	{
		return itBattleGround->second;
	}

	return nullptr;
}
