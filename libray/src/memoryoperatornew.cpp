#include "memoryoperatornew.h"
#include "memorystatistics.h"

void * operator new(unsigned int size, const char *file, int line)
{
	// 下面两种方法可以达到同样的效果，但下面一种比较好  
	// 因为用下面一种可以保持原有的申请方式一样  
	//void * p = malloc(size);
	void *p = operator new(size);

	if (CMemoryStatistics::m_bOpen)
	{
		CMemoryStatistics::getMe().New(p, size, file, line);
	}

	return p;
}

void operator delete(void *p)
{
	free(p);

	if (CMemoryStatistics::m_bOpen)
	{
		CMemoryStatistics::getMe().Delete(p);
	}
}

void operator delete [](void *p)
{
	free(p);

	if (CMemoryStatistics::m_bOpen)
	{
		CMemoryStatistics::getMe().Delete(p);
	}
}

void operator delete(void *p, const char *file, int line)
{
	free(p);

	if (CMemoryStatistics::m_bOpen)
	{
		CMemoryStatistics::getMe().Delete(p);
	}
}

void operator delete [](void *p, const char *file, int line)
{
	free(p);

	if (CMemoryStatistics::m_bOpen)
	{
		CMemoryStatistics::getMe().Delete(p);
	}
}

