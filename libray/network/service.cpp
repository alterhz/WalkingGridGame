#include "service.h"
#include "acceptor.h"
#include "connector.h"
#include "eventmanager.h"
#include "debug.h"

NS_IO_Header

CNetService::CNetService()
	: m_pWork(nullptr)
	, m_pTimerStop(nullptr)
{
}

CNetService::~CNetService()
{
}

bool CNetService::Run()
{
	// 防止io_service中没有事件退出
	m_pWork = new boost::asio::io_service::work(m_ioService);

	while (true)
	{
		boost::system::error_code ec;
		size_t n = m_ioService.run(ec);
		if (ec)
		{
			LOGPrint("service run error[" + ec.value() + "]:" + ec.message());
		}
		else
		{
			// 正常退出
			break;
		}
	}

	LOGPrint("io_service normal finished.");

	return true;
}

void CNetService::Stop(bool bForceStop)
{
	if (nullptr == m_pWork)
	{
		// 没有io_service运行
		return ;
	}

	delete m_pWork;
	m_pWork = nullptr;

	if (bForceStop)
	{
		// 异步强制退出事件1s后触发
		m_pTimerStop = new boost::asio::deadline_timer(m_ioService, boost::posix_time::millisec(1000));
		if (nullptr == m_pTimerStop)
		{
			return ;
		}

		m_pTimerStop->async_wait(boost::bind(&CNetService::OnStop, this, 
			boost::asio::placeholders::error) );
	}
}

void CNetService::OnStop(const boost::system::error_code &ec)
{
	delete m_pTimerStop;
	m_pTimerStop = nullptr;

	m_ioService.stop();

	LOGPrint("强制停止io_service.");
}

bool CNetService::Poll(int nExcuteCount /*= 0*/)
{
	if (0 == nExcuteCount)
	{
		m_ioService.poll();

		return true;
	}
	else
	{
		for (int i=0; i<nExcuteCount; ++i)
		{
			if (0 == m_ioService.poll_one())
			{
				// 队列中的事件处理完毕
				return false;
			}
		}

		return true;
	}
}

INetAcceptor * CNetService::CreateListener()
{
	CNetAcceptor *pNewNetAcceptor = new CNetAcceptor(m_ioService);

	return pNewNetAcceptor;
}

INetConnector * CNetService::CreateConnector()
{
	CNetConnector *pNetConnector = new CNetConnector(m_ioService);

	return pNetConnector;
}

ITimerManager * CNetService::CreateTimerManager()
{
	CTimerManager *pTimerManager = new CTimerManager(m_ioService);

	return pTimerManager;
}

IEventManager * CNetService::CreateEventManager(int nThreadNumber /*= 1*/)
{
	CEventManager *pEventManager = new CEventManager(m_ioService);
	if (nullptr == pEventManager)
	{
		return nullptr;
	}

	pEventManager->Start(nThreadNumber);

	return pEventManager;
}

NS_IO_Footer