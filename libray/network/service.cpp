#include "service.h"
#include "acceptor.h"
#include "connector.h"
#include "eventmanager.h"
#include "debug.h"

NS_IO_Header

CNetService::CNetService()
{
}

CNetService::~CNetService()
{
}

bool CNetService::DoTick(int nExcuteCount/* = 1*/)
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

bool CNetService::Run()
{
_run:
	try
	{
		size_t n = m_ioService.run();

		LOGPrint("网络线程退出，run " + n + " 次.");
	}
	catch (boost::system::system_error &e)
	{
		LOGPrint("boost.trycatch:" + e.what());
		goto _run;
	}

	return true;
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