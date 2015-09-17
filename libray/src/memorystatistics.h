//-------------------------------------------
// author : alterhz
// date : 2015-9-17
// description : 

#ifndef _MEMORYSTATISTICS_H_
#define _MEMORYSTATISTICS_H_

#include <iostream>
#include <malloc.h>
#include <string>
#include <map>

#include "boostinc.h"
#include "memoryleakinfo.h"

class CMemoryStatistics
{
	struct InfoObjectMemory
	{
		InfoObjectMemory()
			: addr(0)
			, nMemorySize(0)
			, nOnlyIndex(0) 
		{}
		InfoObjectMemory(intptr_t _addr, int _nMemorySize, int _nOnlyIndex)
			: addr(_addr)
			, nMemorySize(_nMemorySize)
			, nOnlyIndex(_nOnlyIndex)
		{}
		~InfoObjectMemory() {}

		intptr_t addr;
		int nMemorySize;
		int nOnlyIndex;
	};

	typedef std::map<int, int> MapInt;

	typedef std::map<intptr_t, InfoObjectMemory> MapObject;
	typedef std::map<int, int> MapIntPair;
	typedef std::map<std::string, MapInt> MapStringInt;

public:
	CMemoryStatistics();
	~CMemoryStatistics();
	static CMemoryStatistics & getMe()
	{
		static CMemoryStatistics m_MemoryStatistics;
		return m_MemoryStatistics;
	}
	static bool m_bOpen;

public:
	void New(void *pMemory, int nMemorySize, const char *pFileName, int nLine);
	void Delete(void *pMemory);

	void Debug();
	void PrintStatus();

	// 获取内存泄露信息
	VtMemoryLeak GetMemoryLeakInfo();

	bool Start();
	bool Stop();

private:
	const CMemoryStatistics::InfoObjectMemory * Find(intptr_t addr) const;

	// 内存统计
	void AddMemory(int nOnlyIndex, int nMemorySize);
	void DelMemory(int nOnlyIndex, int nMemorySize);
	// 查找内存占用量
	int GetMemory(int nOnlyIndex) const;

	int FindOnlyIndex(const char *pFileName, int nLine);
	int AllocOnlyIndex() { return ++m_nAllocOnlyIndex; }

	void OnMemoryStatistics();
	bool NewObject(intptr_t addr, int nMemorySize, int nOnlyIndex);
	bool DeleteObject(intptr_t addr);

private:
	int m_nAllocOnlyIndex;
	// new运算符调用的文件名称、代码位置行号与唯一标识OnlyIndex的对照表
	MapStringInt m_mapStringInt;	//<FileName, <Line, nOnlyIndex>>

	// 已经new出来的内存对象列表
	MapObject m_mapObject;
	// 相同位置new出来的内存占用量统计
	MapIntPair m_mapMemory;	//<nOnlyIndex, MemorySize>
	boost::mutex m_mtx;

	boost::asio::io_service::work *m_pWork;
	boost::asio::io_service m_iosIn;
	boost::interprocess::interprocess_semaphore m_sema;
};

#endif

