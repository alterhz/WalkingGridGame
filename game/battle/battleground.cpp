#include "battleground.h"

#include "debug.h"

#include "country.h"
#include "ground.h"

IBattleGround::IBattleGround()
	: m_nIndexId(0)
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

	m_mapCountry.insert(std::make_pair(pCountryA->GetIndexId(), pCountryA));
	m_mapCountry.insert(std::make_pair(pCountryB->GetIndexId(), pCountryB));

	return true;
}


CBattleGroundManager::CBattleGroundManager()
{
}

CBattleGroundManager::~CBattleGroundManager()
{
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
