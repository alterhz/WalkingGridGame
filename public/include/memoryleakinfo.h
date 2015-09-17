//-------------------------------------------
// author : alterhz
// date : 2015-9-17
// description : 

#ifndef _MEMORYLEAKINFO_H_
#define _MEMORYLEAKINFO_H_

#include <string>
#include <vector>

struct InfoMemoryLeak
{
	InfoMemoryLeak()
		: nOnlyIndex(0)
		, nFileLine(0)
		, nLeakMemory(0)
	{}

	int nOnlyIndex;
	std::string strFileName;
	int nFileLine;
	int nLeakMemory;
};

typedef std::vector<InfoMemoryLeak> VtMemoryLeak;

VtMemoryLeak GetMemoryLeak();

#endif

