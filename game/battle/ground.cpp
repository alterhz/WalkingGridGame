#include "ground.h"
#include "debug.h"

IGround::IGround()
	: m_nWGCount(0)
	, m_nHGCount(0)
{
}

IGround::~IGround()
{
}

bool IGround::Init( int nWGCount, int nHGCount )
{
	if (nWGCount <= 0
		|| nHGCount <= 0)
	{
		return false;
	}

	m_nWGCount = nWGCount;
	m_nHGCount = nHGCount;

	for (int i=0; i<m_nHGCount; ++i)
	{
		for (int j=0; j<m_nWGCount; ++j)
		{
			int n = HW2N(i, j);

			m_mapGrid.insert(std::make_pair(n, nullptr));
		}
	}

	return true;
}

int IGround::HW2N( int nH, int nW ) const
{
	int n = m_nWGCount * nH + nW;

	return n;
}

int IGround::N2H( int n ) const
{
	int nH = n / m_nWGCount;

	return nH;
}

int IGround::N2W( int n ) const
{
	int nW = n % m_nWGCount;

	return nW;
}
