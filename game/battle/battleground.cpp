#include "battleground.h"

#include "debug.h"
#include "ground.h"

CFrontBattleGround::CFrontBattleGround()
	: m_nIndexId(0)
	, m_pGround(nullptr)
{
	static int g_nIndexId = 0;
	m_nIndexId = (++g_nIndexId);
}

CFrontBattleGround::~CFrontBattleGround()
{
	delete m_pGround;
	m_pGround = nullptr;
}

bool CFrontBattleGround::Init()
{
	IGround *pGround = new IGround();
	if (nullptr == pGround)
	{
		LOGError("nullptr == pGround");
		return false;
	}

	if (!pGround->Init(20, 30))
	{
		LOGError("初始化场景格子失败！");
		return false;
	}



	return true;
}

