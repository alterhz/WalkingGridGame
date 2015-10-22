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

