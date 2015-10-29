#include "ground.h"
#include "debug.h"

#include "gobject.h"



//格子
IGrid::IGrid(int x, int y)
	: m_nSN(0)
	, m_pXmlData_Ground(nullptr)
	, m_nX(x)
	, m_nY(y)
{
}

bool IGrid::Init(int nSN)
{
	m_nSN = nSN;

	const CXmlData_Ground *pXmlData_Ground = CConfigReadManager::getMe().xdGround.GetRecord(m_nSN);
	if (nullptr == pXmlData_Ground)
	{
		LOGError("查找Ground的SN[" + m_nSN + "]没有找到。");
		return false;
	}

	m_pXmlData_Ground = pXmlData_Ground;

	return true;
}

bool IGrid::IsWalkable(EToWard eToWard) const
{
	if (nullptr == m_pXmlData_Ground)
	{
		LOGError("nullptr == m_pXmlData_Ground");
		return false;
	}

	if (EToWard_None == m_pXmlData_Ground->eToWard)
	{
		// 阻挡，不可以通过
		return false;
	}
	
	if (EToWard_Both == m_pXmlData_Ground->eToWard)
	{
		// 判断是否有角色影响阻挡
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
	
	if (m_pXmlData_Ground->eToWard == eToWard)
	{
		// 判断是否有角色影响阻挡
		auto itGObject = m_mapGObject.begin();
		for (; itGObject!=m_mapGObject.end(); ++itGObject)
		{
			if (!itGObject->second->IsWalkable(eToWard))
			{
				// 场景中存在阻挡不能过去
				return false;
			}
		}

		return true;
	}
	else
	{
		// 不能通过
		return false;
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
CGrid::CGrid(int x, int y)
	: IGrid(x, y)
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

	for (int y=1; y<=m_nHGCount; ++y)
	{
		for (int x=1; x<=m_nWGCount; ++x)
		{
			int n = XY2N(x, y);

			m_mapGrid.insert(std::make_pair(n, nullptr));
		}
	}

	return true;
}

int IGround::XY2N( int x, int y ) const
{
	int n = m_nWGCount * (y - 1) + x;
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

