#include "operatornewdelete.h"

#include "memorystatistics.h"

void * operator new(unsigned int size, const char *file, int line)
{
	// 下面两种方法可以达到同样的效果，但下面一种比较好  
	// 因为用下面一种可以保持原有的申请方式一样  
	//void * p = malloc(size);
	void *p = operator new(size);

	std::cout << "[new addr:" << p << " - new size:" << size;
	std::cout << " - file:" << file << " - " << line << "]" << std::endl;

	CMemoryStatistics::getMe().New(p, size, file, line);

	return p;
}

void operator delete(void *p)
{
	std::cout << "[delete addr:" << p  << "]" << std::endl;
	free(p);

	CMemoryStatistics::getMe().Delete(p);
}

void operator delete [](void *p)
{
	std::cout << "[delete [] addr:" << p << "]" << std::endl;
	free(p);

	CMemoryStatistics::getMe().Delete(p);
}

void operator delete(void *p, const char *file, int line)
{
	std::cout << "[delete addr:" << p << " - file:" << file << " - line:" << line << "]" << std::endl;
	free(p);

	CMemoryStatistics::getMe().Delete(p);
}

void operator delete [](void *p, const char *file, int line)
{
	std::cout << "[delete [] addr:" << p << " - file:" << file << " - line:" << line << "]" << std::endl;
	free(p);

	CMemoryStatistics::getMe().Delete(p);
}
