#include "ground.h"
#include "debug.h"
#include "gobject.h"


//格子
IGrid::IGrid(EGroundType eGroundType, int x, int y)
	: m_nX(x)
	, m_nY(y)
	, m_eGroundType(eGroundType)
{
}

bool IGrid::IsWalkable(EToWard eToWard) const
{
	switch (m_eGroundType)
	{
	case CGrid::EGroundType_Land:
	case CGrid::EGroundType_Sand:
	case CGrid::EGroundType_Lawn:
	case CGrid::EGroundType_Snow:
		{
			auto itGObject = m_mapGObject.begin();
			for (; itGObject!=m_mapGObject.end(); ++itGObject)
			{
				if (!itGObject->second->IsWalkable(eToWard))
				{
					// 场景中存在阻挡不能过去
					return false;
				}
			}

			return true;	//可以通过
		}
		break;
	case CGrid::EGroundType_River:
		{
			if (m_mapGObject.size() > 0)
			{
				auto itGObject = m_mapGObject.begin();
				for (; itGObject!=m_mapGObject.end(); ++itGObject)
				{
					if (!itGObject->second->IsWalkable(eToWard))
					{
						// 场景中存在阻挡不能过去
						return false;
					}
				}

				return true;	//可以通过
			}
			else
			{
				return false;	//不能通过
			}
		}
		break;
	default:
		{
			LOGError("格子类型[" + static_cast<int>(m_eGroundType) + "]错误。");
			return false;
		}
		break;
	}
}

bool IGrid::AddGObject(IGObject *pGObject)
{
	if (nullptr == pGObject)
	{
		LOGError("nullptr == pGObject");
		return false;
	}

	int nIndexId = pGObject->GetIndexId();

	if (FindGObject(nIndexId))
	{
		LOGError("GObject对象[" + pGObject->GetIndexId() + "]已经绑定。");
		return false;
	}

	m_mapGObject.insert(std::make_pair(pGObject->GetIndexId(), pGObject));

	// 绑定
	pGObject->BindGrid(this);

	OnAddGObject(pGObject);

	return true;
}

bool IGrid::DelGObject(IGObject *pGObject)
{
	if (nullptr == pGObject)
	{
		LOGError("nullptr == pGObject");
		return false;
	}

	OnDelGObject(pGObject);

	int nIndexId = pGObject->GetIndexId();

	if (FindGObject(nIndexId))
	{
		m_mapGObject.erase(pGObject->GetIndexId());
	}

	pGObject->BindGrid(nullptr);

	return true;
}

IGObject * IGrid::FindGObject(int nIndexId)
{
	MapGObject::iterator itGObject = m_mapGObject.find(nIndexId);
	if (itGObject != m_mapGObject.end())
	{
		return itGObject->second;
	}

	return nullptr;
}

IGObject * IGrid::FindGObject(int nIndexId) const
{
	MapGObject::const_iterator itGObject = m_mapGObject.find(nIndexId);
	if (itGObject != m_mapGObject.end())
	{
		return itGObject->second;
	}

	return nullptr;
}

void IGrid::OnAddGObject(IGObject *pGObject)
{

}

void IGrid::OnDelGObject(IGObject *pGObject)
{

}


// 普通格子
CGrid::CGrid(EGroundType eGroundType, int x, int y)
	: IGrid(eGroundType, x, y)
{
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

int IGround::XY2N( int x, int y ) const
{
	int n = m_nWGCount * y + x;
	return n;
}

int IGround::N2X( int n ) const
{
	int x = n % m_nWGCount;

	return x;
}

int IGround::N2Y( int n ) const
{
	int y = n / m_nWGCount;

	return y;
}

bool IGround::SetGrid( int x, int y, IGrid *pGrid )
{
	int n = XY2N(x, y);

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

IGrid * IGround::GetGrid(int x, int y)
{
	int n = XY2N(x, y);

	return Find(n);
}

IGrid * IGround::GetGrid(int x, int y) const
{
	int n = XY2N(x, y);

	return Find(n);
}

IGrid * IGround::Find(int n)
{
	MapGrid::iterator itGrid = m_mapGrid.find(n);
	if (itGrid != m_mapGrid.end())
	{
		return itGrid->second;
	}

	return nullptr;
}

IGrid * IGround::Find(int n) const
{
	MapGrid::const_iterator itGrid = m_mapGrid.find(n);
	if (itGrid != m_mapGrid.end())
	{
		return itGrid->second;
	}

	return nullptr;
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

	// 初始化地形数据
	for (int nY=0; nY<nHGCount; ++nY)
	{
		for (int nX=0; nX<nWGCount; ++nX)
		{
			IGrid *pNewGrid = new CGrid(CGrid::EGroundType_Land, nX, nY);
			if (nullptr == pNewGrid)
			{
				LOGError("nullptr == pNewGrid");
				continue;
			}

			SetGrid(nX, nY, pNewGrid);
		}
	}

	// 加载场景（固定）单位，如桥梁，树木，城墙，大炮

	// 地图中心放入3个桥梁
	{
		CGBridge *pNewGBridge = new CGBridge(EToWard_Y);
		if (nullptr == pNewGBridge)
		{
			LOGError("nullptr == pNewGBridge");
			return false;
		}

		pNewGBridge->EnterGround(10, 15, this);
	}

	{
		CGBridge *pNewGBridge = new CGBridge(EToWard_Y);
		if (nullptr == pNewGBridge)
		{
			LOGError("nullptr == pNewGBridge");
			return false;
		}

		pNewGBridge->EnterGround(8, 15, this);
	}

	{
		CGBridge *pNewGBridge = new CGBridge(EToWard_Y);
		if (nullptr == pNewGBridge)
		{
			LOGError("nullptr == pNewGBridge");
			return false;
		}

		pNewGBridge->EnterGround(12, 15, this);
	}
	

	// 加载场景（移动）中立单位
	{
		CDogFace *pNewDogFace = new CDogFace();
		if (nullptr == pNewDogFace)
		{
			LOGError("nullptr == pNewDogFace");
			return false;
		}

		pNewDogFace->Init();
		pNewDogFace->EnterGround(1, 15, this);
	}

	{
		CDogFace *pNewDogFace = new CDogFace();
		if (nullptr == pNewDogFace)
		{
			LOGError("nullptr == pNewDogFace");
			return false;
		}

		pNewDogFace->Init();
		pNewDogFace->EnterGround(18, 15, this);
	}

	return true;
}
