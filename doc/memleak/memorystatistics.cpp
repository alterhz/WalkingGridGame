#include "memorystatistics.h"

CMemoryStatistics * CMemoryStatistics::m_pMemoryStatistics = nullptr;

CMemoryStatistics::CMemoryStatistics()
	: m_nAllocOnlyIndex(0)
{
}

CMemoryStatistics::~CMemoryStatistics()
{}

bool CMemoryStatistics::NewObject(void *pObject, int nMemorySize, int nOnlyIndex)
{
	intptr_t addr = reinterpret_cast<intptr_t>(pObject);
	const InfoObjectMemory *pInfoObjectMemory = Find(addr);
	if (pInfoObjectMemory)
	{
		std::cout << "new addr[" << pObject << "] have exist." << std::endl;
		return false;
	}

	InfoObjectMemory newInfoObjectMemory(pObject, nMemorySize, nOnlyIndex);
	m_mapObject.insert(std::make_pair(addr, newInfoObjectMemory));

	AddMemory(nOnlyIndex, nMemorySize);

	return true;
}

bool CMemoryStatistics::DeleteObject(void *pObject)
{
	intptr_t addr = reinterpret_cast<intptr_t>(pObject);
	const InfoObjectMemory *pInfoObjectMemory = Find(addr);
	if (nullptr == pInfoObjectMemory)
	{
		return false;
	}

	DelMemory(pInfoObjectMemory->nOnlyIndex, pInfoObjectMemory->nMemorySize);

	m_mapObject.erase(addr);

	return true;
}

const CMemoryStatistics::InfoObjectMemory * CMemoryStatistics::Find(intptr_t addr) const
{
	MapObject::const_iterator it = m_mapObject.find(addr);
	if (it != m_mapObject.end())
	{
		return &(it->second);
	}

	return nullptr;
}

void CMemoryStatistics::AddMemory(int nOnlyIndex, int nMemorySize)
{
	MapIntPair::iterator it = m_mapMemory.find(nOnlyIndex);
	if (it != m_mapMemory.end())
	{
		it->second += nMemorySize;
	}
	else
	{
		m_mapMemory.insert(std::make_pair(nOnlyIndex, nMemorySize));
	}
}

void CMemoryStatistics::DelMemory(int nOnlyIndex, int nMemorySize)
{
	MapIntPair::iterator it = m_mapMemory.find(nOnlyIndex);
	if (it != m_mapMemory.end())
	{
		it->second -= nMemorySize;
	}
}

void CMemoryStatistics::New(void *pMemory, int nMemorySize, const char *pFileName, int nLine)
{
	// 获取唯一索引，没有就创建
	int nOnlyIndex = FindOnlyIndex(pFileName, nLine);
	if (0 == nOnlyIndex)
	{
		return ;
	}

	NewObject(pMemory, nMemorySize, nOnlyIndex);
}

void CMemoryStatistics::Delete(void *pMemory)
{
	DeleteObject(pMemory);
}

void CMemoryStatistics::Debug()
{
	std::cout << "##########################################################" << std::endl;
	std::cout << "[leak object count:" << m_mapObject.size() << "]" << std::endl;

	MapObject::const_iterator itObject = m_mapObject.begin();
	for (; itObject!=m_mapObject.end(); ++itObject)
	{
		void *p = reinterpret_cast<void *>(itObject->first);
		std::cout << "addr[" << p << "]:" << itObject->first << " - OnlyIndex" << itObject->second.nOnlyIndex << " - size:" << itObject->second.nMemorySize << std::endl;
	}

	std::cout << "##########################################################" << std::endl;
	std::cout << "[memory leak:" << m_mapObject.size() << "]" << std::endl;

	MapIntPair::const_iterator itPair = m_mapMemory.begin();
	for (; itPair!=m_mapMemory.end(); ++itPair)
	{
		std::cout << "OnlyIndex[" << itPair->first << "] - memory:" << itPair->second << std::endl;
	}

	std::cout << "##########################################################" << std::endl;
	std::cout << "[OnlyIndex file line:" << m_mapObject.size() << "]" << std::endl;

	MapStringInt::const_iterator itString = m_mapStringInt.begin();
	for (; itString!=m_mapStringInt.end(); ++itString)
	{
		const MapInt &mapInt = itString->second;
		MapInt::const_iterator itLine = mapInt.begin();
		for (; itLine!=mapInt.end(); ++itLine)
		{
			std::cout << "file:" << itString->first.c_str() << " - line:" << itLine->first 
				<< " - OnlyIndex:" << itLine->second << std::endl;
		}
	}

	std::cout << "##########################################################" << std::endl;
}

CMemoryStatistics * CMemoryStatistics::getInstance()
{ 
	if (nullptr == m_pMemoryStatistics)
	{ 
		//std::map<int, int> mapTest;
		//mapTest.insert(std::make_pair(1, 1));
		//std::nothrow_t tr;
		//size_t sz = sizeof(CMemoryStatistics);
		//m_pMemoryStatistics = (CMemoryStatistics*)operator new(sz, tr) _THROW1(_STD bad_alloc);
		//m_pMemoryStatistics = (CMemoryStatistics*)malloc(sizeof(CMemoryStatistics));
		//m_pMemoryStatistics = (CMemoryStatistics *)::operator new(sizeof(CMemoryStatistics));
		m_pMemoryStatistics = new CMemoryStatistics();
		return m_pMemoryStatistics;
	}
	else
	{
		return m_pMemoryStatistics;
	} 
}

int CMemoryStatistics::FindOnlyIndex(const char *pFileName, int nLine) 
{
	if (nullptr == pFileName)
	{
		return 0;
	}

	MapStringInt::iterator itString = m_mapStringInt.find(pFileName);
	if (itString == m_mapStringInt.end())
	{
		// 没有找到创建
		int nNewOnlyIndex = AllocOnlyIndex();

		MapInt mapInt;
		mapInt.insert(std::make_pair(nLine, nNewOnlyIndex));

		m_mapStringInt.insert(std::make_pair(pFileName, mapInt));

		return nNewOnlyIndex;
	}

	MapInt &mapLine = itString->second;
	MapInt::iterator itLine = mapLine.find(nLine);
	if (itLine == mapLine.end())
	{
		int nNewOnlyIndex = AllocOnlyIndex();
		
		mapLine.insert(std::make_pair(nLine, nNewOnlyIndex));
		
		return nNewOnlyIndex;
	}

	return itLine->second;
}


