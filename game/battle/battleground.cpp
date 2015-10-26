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
	LOGDebug("Country[" + pCountry->GetIndexId() + "]获取场景数据。");
}


CFrontBattleGround::CFrontBattleGround()
	: m_pGround(nullptr)
{
}

CFrontBattleGround::~CFrontBattleGround()
{
	delete m_pGround;
	m_pGround = nullptr;
}

bool CFrontBattleGround::OnInit()
{
	// 如果已经存在格子数据，删除
	if (m_pGround)
	{
		delete m_pGround;
		m_pGround = nullptr;
	}

	IGround *pGround = new CDemoGround();
	if (nullptr == pGround)
	{
		LOGError("nullptr == pGround");
		return false;
	}

	if (!pGround->Init())
	{
		LOGError("初始化场景格子失败！");
		return false;
	}

	m_pGround = pGround;

	return true;
}

bool CFrontBattleGround::InitTwoCountry(ICountry *pCountryA, ICountry *pCountryB)
{
	if (nullptr == pCountryA || nullptr == pCountryB)
	{
		LOGError("nullptr == pCountryA || nullptr == pCountryB");
		return false;
	}

	// 测试，给双方添加部队
	{
		VtFightGObject vtFightGObject;
		// 添加将领
		CSirdar *pSirdar = new CSirdar();
		if (pSirdar)
		{
			pSirdar->Init();

			vtFightGObject.push_back(pSirdar);
		}

		// 添加英雄
		CHero *pHeroA = new CHero();
		if (pHeroA)
		{
			pHeroA->Init();

			vtFightGObject.push_back(pHeroA);
		}

		CHero *pHeroB = new CHero();
		if (pHeroB)
		{
			pHeroB->Init();

			vtFightGObject.push_back(pHeroB);
		}

		// 添加小兵
		CDogFace *pDogFaceA = new CDogFace();
		if (pDogFaceA)
		{
			pDogFaceA->Init();

			vtFightGObject.push_back(pDogFaceA);
		}

		CDogFace *pDogFaceB = new CDogFace();
		if (pDogFaceB)
		{
			pDogFaceB->Init();

			vtFightGObject.push_back(pDogFaceB);
		}

		pCountryA->ClearFightGObject();
		pCountryA->AddFightGObject(vtFightGObject);
	}

	{
		VtFightGObject vtFightGObject;
		// 添加将领
		CSirdar *pSirdar = new CSirdar();
		if (pSirdar)
		{
			pSirdar->Init();

			vtFightGObject.push_back(pSirdar);
		}

		// 添加英雄
		CHero *pHeroA = new CHero();
		if (pHeroA)
		{
			pHeroA->Init();

			vtFightGObject.push_back(pHeroA);
		}

		CHero *pHeroB = new CHero();
		if (pHeroB)
		{
			pHeroB->Init();

			vtFightGObject.push_back(pHeroB);
		}

		// 添加小兵
		CDogFace *pDogFaceA = new CDogFace();
		if (pDogFaceA)
		{
			pDogFaceA->Init();

			vtFightGObject.push_back(pDogFaceA);
		}

		CDogFace *pDogFaceB = new CDogFace();
		if (pDogFaceB)
		{
			pDogFaceB->Init();

			vtFightGObject.push_back(pDogFaceB);
		}

		pCountryB->ClearFightGObject();
		pCountryB->AddFightGObject(vtFightGObject);
	}

	m_mapCountry.insert(std::make_pair(pCountryA->GetIndexId(), pCountryA));
	m_mapCountry.insert(std::make_pair(pCountryB->GetIndexId(), pCountryB));

	return true;
}

bool CFrontBattleGround::OnTick()
{

	//LOGDebug("阵地战DoTick");
	return true;
}

void CFrontBattleGround::GetGroundInfo(ICountry *pCountry)
{

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
