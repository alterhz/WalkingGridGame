#include "eventmanager.h"
#include "debug.h"

NS_IO_Header

CEventManager::CEventManager(boost::asio::io_service &iosMain)
	: m_iosMain(iosMain)
	, m_pWorkThread(nullptr)
	, m_pSemaphore(nullptr)
{
}

CEventManager::~CEventManager()
{
	Stop();
}

bool CEventManager::Start(int nThreadNumber /*= 1*/)
{
	if (nullptr != m_pWorkThread)
	{
		return false;
	}

	if (m_pSemaphore)
	{
		delete m_pSemaphore;
		m_pSemaphore = nullptr;
	}

	// 初始化信号量数量
	m_pSemaphore = new boost::interprocess::interprocess_semaphore(nThreadNumber);

	m_pWorkThread = new boost::asio::io_service::work(m_iosThread);

	for (int i=0; i<nThreadNumber; ++i)
	{
		// 启动线程
		boost::thread t1(boost::bind(&CEventManager::OnThreadRun, this));
		t1.detach();
	}

	return true;
}

void CEventManager::Stop()
{
	delete m_pWorkThread;
	m_pWorkThread = nullptr;

	// 等待所有执行线程退出
	if (m_pSemaphore)
	{
		m_pSemaphore->wait();

		delete m_pSemaphore;
		m_pSemaphore = nullptr;
	}
}

bool CEventManager::PostAsyncEvent(IAsyncEvent *pAsyncEvent)
{
	m_iosThread.post(boost::bind(&CEventManager::OnEventExcute, this, pAsyncEvent));

	return true;
}
void CEventManager::OnEventExcute(IAsyncEvent *pAsyncEvent)
{
	if (pAsyncEvent)
	{
		pAsyncEvent->OnExcute();

		m_iosMain.post(boost::bind(&CEventManager::OnEventResult, this, pAsyncEvent));
	}
}
void CEventManager::OnEventResult(IAsyncEvent *pAsyncEvent)
{
	if (pAsyncEvent)
	{
		pAsyncEvent->OnResult();
	}
}

void CEventManager::OnThreadRun()
{
_run:
	try
	{
		size_t n = m_iosThread.run();

		LOGPrint("数据库线程结束..." + n);

		if (m_pSemaphore)
		{
			m_pSemaphore->post();
		}
	}
	catch (boost::system::system_error &e)
	{
		LOGPrint("boost.trycatch:" + e.what());
		goto _run;
	}
}

int CEventManager::SetTimer(ITimerEvent *pTimerEvent, int nInterval)
{
	CDeadlineTimer *pInfoDeadlineTimer = new CDeadlineTimer(m_iosMain, nInterval, pTimerEvent);
	if (nullptr == pInfoDeadlineTimer)
	{
		return INVALID_TIMER_ID;
	}

	int nTimerId = pInfoDeadlineTimer->GetTimerId();

	m_mapDeadlineTimer.insert(std::make_pair(nTimerId, pInfoDeadlineTimer));

	pInfoDeadlineTimer->AsyncWait();

	return nTimerId;
}

void CEventManager::KillTimer(int nTimerId)
{
	MapDeadlineTimer::iterator it = m_mapDeadlineTimer.find(nTimerId);
	if (it != m_mapDeadlineTimer.end())
	{
		CDeadlineTimer *pDeadlineTimer = it->second;
		m_mapDeadlineTimer.erase(it);

		if (pDeadlineTimer)
		{
			delete pDeadlineTimer;
			pDeadlineTimer = nullptr;
		}
	}
}

NS_IO_Footer

