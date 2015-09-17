#include "memorystatistics.h"

bool CMemoryStatistics::m_bOpen = true;

CMemoryStatistics::CMemoryStatistics()
	: m_nAllocOnlyIndex(0)
	, m_pWork(nullptr)
	, m_sema(0)
{
	Start();
}

CMemoryStatistics::~CMemoryStatistics()
{
	Stop();
	m_bOpen = false;
}

bool CMemoryStatistics::NewObject(intptr_t addr, int nMemorySize, int nOnlyIndex)
{
	m_mtx.lock();

	const InfoObjectMemory *pInfoObjectMemory = Find(addr);
	if (pInfoObjectMemory)
	{
		m_mtx.unlock();
		return false;
	}

	InfoObjectMemory newInfoObjectMemory(addr, nMemorySize, nOnlyIndex);
	m_mapObject.insert(std::make_pair(addr, newInfoObjectMemory));

	AddMemory(nOnlyIndex, nMemorySize);

	m_mtx.unlock();

	return true;
}

bool CMemoryStatistics::DeleteObject(intptr_t addr)
{
	m_mtx.lock();

	const InfoObjectMemory *pInfoObjectMemory = Find(addr);
	if (nullptr == pInfoObjectMemory)
	{
		m_mtx.unlock();
		return false;
	}

	DelMemory(pInfoObjectMemory->nOnlyIndex, pInfoObjectMemory->nMemorySize);

	m_mapObject.erase(addr);

	m_mtx.unlock();

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

int CMemoryStatistics::GetMemory(int nOnlyIndex) const
{
	MapIntPair::const_iterator it = m_mapMemory.find(nOnlyIndex);
	if (it != m_mapMemory.end())
	{
		return it->second;
	}

	return 0;
}

void CMemoryStatistics::New(void *pMemory, int nMemorySize, const char *pFileName, int nLine)
{
	// 获取唯一索引，没有就创建
	int nOnlyIndex = FindOnlyIndex(pFileName, nLine);
	if (0 == nOnlyIndex)
	{
		return ;
	}

	intptr_t addr = reinterpret_cast<intptr_t>(pMemory);

	m_iosIn.dispatch(boost::bind(&CMemoryStatistics::NewObject, this, addr, nMemorySize, nOnlyIndex));
}

void CMemoryStatistics::Delete(void *pMemory)
{
	intptr_t addr = reinterpret_cast<intptr_t>(pMemory);

	m_iosIn.dispatch(boost::bind(&CMemoryStatistics::DeleteObject, this, addr));
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

void CMemoryStatistics::OnMemoryStatistics()
{
_run:
	try
	{
		size_t n = m_iosIn.run();

		m_sema.post();
	}
	catch (boost::system::system_error &e)
	{
		goto _run;
	}
}

void CMemoryStatistics::Debug()
{
	std::cout << std::endl << "### memory leak status ###" << std::endl;
	std::cout << "##########################################################" << std::endl;
	std::cout << "[leak object count:" << m_mapObject.size() << "]" << std::endl;

	MapObject::const_iterator itObject = m_mapObject.begin();
	for (; itObject!=m_mapObject.end(); ++itObject)
	{
		void *p = reinterpret_cast<void *>(itObject->first);
		std::cout << "addr[" << p << "]:" << itObject->first << " - OnlyIndex" << itObject->second.nOnlyIndex << " - size:" << itObject->second.nMemorySize << std::endl;
	}

	std::cout << "##########################################################" << std::endl;
	std::cout << "[memory leak:]" << std::endl;

	MapIntPair::const_iterator itPair = m_mapMemory.begin();
	for (; itPair!=m_mapMemory.end(); ++itPair)
	{
		std::cout << "OnlyIndex[" << itPair->first << "] - memory:" << itPair->second << std::endl;
	}

	std::cout << "##########################################################" << std::endl;
	std::cout << "[OnlyIndex file line:]" << std::endl;

	MapStringInt::const_iterator itString = m_mapStringInt.begin();
	for (; itString!=m_mapStringInt.end(); ++itString)
	{
		const MapInt &mapInt = itString->second;
		MapInt::const_iterator itLine = mapInt.begin();
		for (; itLine!=mapInt.end(); ++itLine)
		{
			std::cout << "OnlyIndex:" << itLine->second << " - file:" << itString->first.c_str() 
				<< " - line:" << itLine->first << std::endl;
		}
	}

	std::cout << "##########################################################" << std::endl;
}

void CMemoryStatistics::PrintStatus()
{
	std::cout << std::endl;
	std::cout << "##########################################################" << std::endl;
	std::cout << "[memory leak status]" << std::endl;

	MapStringInt::const_iterator itString = m_mapStringInt.begin();
	for (; itString!=m_mapStringInt.end(); ++itString)
	{
		const MapInt &mapInt = itString->second;
		MapInt::const_iterator itLine = mapInt.begin();
		for (; itLine!=mapInt.end(); ++itLine)
		{
			int nOnlyIndex = itLine->second;
			int nMemorySize = GetMemory(nOnlyIndex);
			std::cout << "OnlyIndex[" << nOnlyIndex << "] leak memory:" << nMemorySize << "\n - [file:" << itString->first.c_str() 
				<< " - line:" << itLine->first << "]" << std::endl;	
		}
	}

	std::cout << "##########################################################" << std::endl;
}

VtMemoryLeak CMemoryStatistics::GetMemoryLeakInfo()
{
	VtMemoryLeak vtMemoryLeak;

	m_mtx.lock();
	MapStringInt::const_iterator itString = m_mapStringInt.begin();
	for (; itString!=m_mapStringInt.end(); ++itString)
	{
		const MapInt &mapInt = itString->second;
		MapInt::const_iterator itLine = mapInt.begin();
		for (; itLine!=mapInt.end(); ++itLine)
		{
			int nOnlyIndex = itLine->second;
			int nMemorySize = GetMemory(nOnlyIndex);


			InfoMemoryLeak infoMemoryLeak;
			infoMemoryLeak.nOnlyIndex = nOnlyIndex;
			infoMemoryLeak.nLeakMemory = nMemorySize;
			infoMemoryLeak.strFileName = itString->first;
			infoMemoryLeak.nFileLine = itLine->first;

			vtMemoryLeak.push_back(infoMemoryLeak);
		}
	}
	m_mtx.unlock();

	return vtMemoryLeak;
}

bool CMemoryStatistics::Start()
{
	if (m_pWork)
	{
		return false;
	}

	m_pWork = new boost::asio::io_service::work(m_iosIn);

	// 启动线程
	boost::thread t1(boost::bind(&CMemoryStatistics::OnMemoryStatistics, this));
	t1.detach();

	return true;
}

bool CMemoryStatistics::Stop()
{
	delete m_pWork;
	m_pWork = nullptr;

	m_sema.wait();

	std::cout << "内存泄露检测线程已经停止。" << std::endl;

	return true;
}
