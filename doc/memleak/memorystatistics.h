//-------------------------------------------
// author : alterhz
// date : 9/7/2015
// description : 

#ifndef _MEMORYSTATISTICS_H_
#define _MEMORYSTATISTICS_H_

#include <iostream>
#include <malloc.h>
#include <string>
#include <map>

class CMemoryStatistics
{
	struct InfoObjectMemory
	{
		InfoObjectMemory()
			: pObject(nullptr)
			, nMemorySize(0)
			, nOnlyIndex(0) 
		{}
		InfoObjectMemory(void *_pObject, int _nMemorySize, int _nOnlyIndex)
			: pObject(_pObject)
			, nMemorySize(_nMemorySize)
			, nOnlyIndex(_nOnlyIndex)
		{}
		~InfoObjectMemory() {}

		void *pObject;
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
	static CMemoryStatistics * getInstance();
	static CMemoryStatistics & getMe()
	{
		return *getInstance();
	}
	static void Release()
	{
		delete m_pMemoryStatistics;
		m_pMemoryStatistics = nullptr;
	}
	static CMemoryStatistics *m_pMemoryStatistics;

public:
	void New(void *pMemory, int nMemorySize, const char *pFileName, int nLine);
	void Delete(void *pMemory);

	void Debug();

private:
	bool NewObject(void *pObject, int nMemorySize, int nOnlyIndex);
	bool DeleteObject(void *pObject);

	const CMemoryStatistics::InfoObjectMemory * Find(intptr_t addr) const;

	// ÄÚ´æÍ³¼Æ
	void AddMemory(int nOnlyIndex, int nMemorySize);
	void DelMemory(int nOnlyIndex, int nMemorySize);

	int FindOnlyIndex(const char *pFileName, int nLine);
	int AllocOnlyIndex() { return ++m_nAllocOnlyIndex; }

private:
	int m_nAllocOnlyIndex;
	MapObject m_mapObject;
	MapIntPair m_mapMemory;	//<nOnlyIndex, MemorySize>
	MapStringInt m_mapStringInt;	//<FileName, <Line, nOnlyIndex>>
};

#endif