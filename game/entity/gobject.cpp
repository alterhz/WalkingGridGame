#include "gobject.h"
#include "debug.h"
#include "ground.h"

IGObject::IGObject()
	: m_nIndexId(0)
	, m_nX(0)
	, m_nY(0)
	, m_pGround(nullptr)
{
	static int G_nIndexId = 0;
	m_nIndexId = (++G_nIndexId);
}

bool IGObject::EnterGround(int x, int y, IGround *pGround)
{
	if (nullptr == pGround)
	{
		LOGError("nullptr == pGround");
		return false;
	}

	m_nX = x;
	m_nY = y;
	m_pGround = pGround;

	IGrid *pGrid = m_pGround->GetGrid(m_nX, m_nY);
	if (nullptr == pGrid)
	{
		return false;
	}

	pGrid->AddGObject(this);

	return true;
}

bool CGBridge::IsWalkable(EToWard eToWard) const
{
	if (EToWard_None == m_eToWard)
	{
		return false;
	}

	if (EToWard_Both == m_eToWard)
	{
		return true;
	}

	if (m_eToWard == eToWard)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool IWalkableGObject::Move(int x, int y)
{
	if (nullptr == m_pGround)
	{
		LOGError("nullptr == m_pGround");
		return false;
	}

	IGrid *pGrid = m_pGround->GetGrid(x, y);
	if (nullptr == pGrid)
	{
		return false;
	}

	// 步长
	int nStepLength = 1;

	// 判断是否可以行走过去


	// 更新位置
	m_nX = x;
	m_nY = y;

	// 解除绑定
	if (m_pGrid)
	{
		m_pGrid->DelGObject(this);
	}

	// 新绑定
	pGrid->AddGObject(this);

	return true;
}
