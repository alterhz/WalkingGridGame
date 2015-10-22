#include "ground.h"
#include "debug.h"


//格子
IGrid::IGrid()
	: m_nX(0)
	, m_nY(0)
{
}


// 普通格子
CBaseGrid::CBaseGrid(EGroundType eGroundType)
	: m_eGroundType(eGroundType)
{
}

bool CBaseGrid::IsWalkable(EToWard eToWard) const
{
	switch (m_eGroundType)
	{
	case CBaseGrid::EGroundType_Land:
	case CBaseGrid::EGroundType_Sand:
	case CBaseGrid::EGroundType_Lawn:
	case CBaseGrid::EGroundType_Snow:
		return true;	//可以行走
		break;
	case CBaseGrid::EGroundType_River:
		return false;	//不能通过
		break;
	default:
		{
			LOGError("格子类型[" + static_cast<int>(m_eGroundType) + "]错误。");
			return false;
		}
		break;
	}

	if (EGroundType_River == m_eGroundType)
	{
		// 河流不可以行走
		return false;
	}
}


// 桥梁
CBridgeGrid::CBridgeGrid(EGroundType eGroundType, EToWard eToWard)
	: CBaseGrid(eGroundType)
	, m_eToWard(eToWard)
{
}

bool CBridgeGrid::IsWalkable(EToWard eToWard) const
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



IGround::IGround()
	: m_nWGCount(0)
	, m_nHGCount(0)
{
}

IGround::~IGround()
{
	// 删除格子
	for (auto itDel : m_mapGrid)
	{
		delete itDel.second;
	}
	m_mapGrid.clear();
}

bool IGround::Init()
{
	InitGroundSize(G_nDemoWidthCount, G_nDemoHeigthCount);

	return true;
}

bool IGround::InitGroundSize( int nWGCount, int nHGCount )
{
	if (nWGCount <=0
		|| nHGCount <= 0)
	{
		return false;
	}

	m_nWGCount = nWGCount;
	m_nHGCount = nHGCount;

	m_mapGrid.clear();

	for (int i=0; i<m_nHGCount; ++i)
	{
		for (int j=0; j<m_nWGCount; ++j)
		{
			int n = XY2N(i, j);

			m_mapGrid.insert(std::make_pair(n, nullptr));
		}
	}

	return true;
}

int IGround::XY2N( int nH, int nW ) const
{
	int n = m_nWGCount * nH + nW;

	return n;
}

int IGround::N2X( int n ) const
{
	int nH = n / m_nWGCount;

	return nH;
}

int IGround::N2Y( int n ) const
{
	int nW = n % m_nWGCount;

	return nW;
}

bool IGround::SetGrid( int nX, int nY, IGrid *pGrid )
{
	int n = XY2N(nX, nY);

	MapGrid::iterator it = m_mapGrid.find(n);
	if (it != m_mapGrid.end())
	{
		IGrid *pOldGrid = it->second;
		if (pOldGrid)
		{
			delete pOldGrid;
			pOldGrid = nullptr;
		}

		it->second = pGrid;

		return true;
	}

	return false;
}

// demo地形场景
CDemoGround::CDemoGround()
{
}

CDemoGround::~CDemoGround()
{

}

bool CDemoGround::Init( int nWGCount, int nHGCount )
{
	// 初始化场景地图
	InitGroundSize(G_nDemoWidthCount, G_nDemoHeigthCount);

	// 修改地形数据
	for (int nY=0; nY<nHGCount; ++nY)
	{
		for (int nX=0; nX<nWGCount; ++nX)
		{
			IGrid *pNewGrid = new CBaseGrid(CBaseGrid::EGroundType_Land);
			if (nullptr == pNewGrid)
			{
				LOGError("nullptr == pNewGrid");
				continue;
			}

			SetGrid(nX, nY, pNewGrid);
		}
	}

	return true;
}
