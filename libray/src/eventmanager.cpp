#include "eventmanager.h"
#include "debug.h"

NS_IO_Header

CTimerManager::CTimerManager(boost::asio::io_service &iosMain)
	: m_iosMain(iosMain)
{
}

CTimerManager::~CTimerManager()
{
	for (auto itDeadlineTimer : m_mapDeadlineTimer)
	{
		CDeadlineTimer *pDelDeadlineTimer = itDeadlineTimer.second;
		delete pDelDeadlineTimer;
		pDelDeadlineTimer = nullptr;
	}

	m_mapDeadlineTimer.clear();
}

int CTimerManager::SetTimer(ITimerEvent *pTimerEvent, int nInterval)
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

void CTimerManager::KillTimer(int nTimerId)
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

//////////////////////////////////////////////////////////////////////////
CEventManager::CEventManager(boost::asio::io_service &iosMain)
	: m_iosMain(iosMain)
	, m_pWorkThread(nullptr)
	, m_pSemaphore(nullptr)
	, m_nThreadNumber(0)
{
}

CEventManager::~CEventManager()
{
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
	m_pSemaphore = new boost::interprocess::interprocess_semaphore(0);

	m_pWorkThread = new boost::asio::io_service::work(m_iosThread);

	m_nThreadNumber = nThreadNumber;

	for (int i=0; i<m_nThreadNumber; ++i)
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
		for (int i=0; i<m_nThreadNumber; ++i)
		{
			m_pSemaphore->wait();
		}

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
	while (true)
	{
		boost::system::error_code ec;
		size_t n = m_iosThread.run(ec);
		if (ec)
		{
			LOGPrint("run error[" + ec.value() + "]:" + ec.message());
		}
		else
		{
			// 正常退出
			break;
		}
	}

	if (m_pSemaphore)
	{
		m_pSemaphore->post();
	}

	LOGPrint("event thread normal finished.");
}


void CTimerManager::CDeadlineTimer::OnTimerEvent(const boost::system::error_code &ec)
{
	if (ec)
	{
		if (ec.value() != boost::asio::error::operation_aborted)
		{
			LOGPrint("TimerManager error[" + ec.value() + "]:" + ec.message().c_str());
		}

		return ;
	}

	m_deadlineTimer.expires_at(m_deadlineTimer.expires_at() 
		+ boost::posix_time::millisec(m_nInterval));

	m_deadlineTimer.async_wait(boost::bind(&CDeadlineTimer::OnTimerEvent, this, 
		boost::asio::placeholders::error) );

	if (m_pTimerEvent)
	{
		m_pTimerEvent->OnTimerEvent(m_nTimerId);
	}
}

NS_IO_Footer

