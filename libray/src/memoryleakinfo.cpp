#include "memoryleakinfo.h"
#include "memorystatistics.h"

VtMemoryLeak GetMemoryLeak()
{
	return CMemoryStatistics::getMe().GetMemoryLeakInfo();
}