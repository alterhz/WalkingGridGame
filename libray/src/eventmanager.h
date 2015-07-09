//-------------------------------------------
// author : alterhz
// date : 6/25/2015
// description : 

#ifndef _EVENTMANAGER_H_
#define _EVENTMANAGER_H_

#include "event.h"
#include "boostinc.h"
#include "debug.h"

NS_IO_Header

class CEventManager : public IEventManager
{
	class CDeadlineTimer
	{
	public:
		CDeadlineTimer(boost::asio::io_service &ios, int nInterval, ITimerEvent *pTimerEvent)
			: m_nTimerId(0)
			, m_nInterval(nInterval)
			, m_deadlineTimer(ios, boost::posix_time::millisec(m_nInterval))
			, m_pTimerEvent(pTimerEvent)
		{
			static int g_TimerId = 0;
			m_nTimerId = (++g_TimerId);
		}
		~CDeadlineTimer()
		{
			m_deadlineTimer.cancel();

			if (m_pTimerEvent)
			{
				m_pTimerEvent->OnKilled(m_nTimerId);
			}
		}

		void AsyncWait()
		{
			m_deadlineTimer.async_wait(boost::bind(&CDeadlineTimer::OnTimerEvent, this, 
				boost::asio::placeholders::error) );
		}

		int GetTimerId() const { return m_nTimerId; }

	private:
		void OnTimerEvent(const boost::system::error_code &ec)
		{
			if (ec)
			{
				LOGPrint("err[" + ec.value() + "]:" + ec.message().c_str());

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

		int m_nTimerId;
		int m_nInterval;
		boost::asio::deadline_timer m_deadlineTimer;
		ITimerEvent *m_pTimerEvent;
	};

	typedef std::map<int, CDeadlineTimer *> MapDeadlineTimer;

public:
	CEventManager(boost::asio::io_service &iosMain);
	virtual ~CEventManager();

public:
	bool Start(int nThreadNumber = 1);
	void Stop();
	// 投递异步事件
	virtual bool PostAsyncEvent(IAsyncEvent *pAsyncEvent);
	// 设置Timer事件,返回TimerId，设置失败，返回INVALID_TIMER_ID
	virtual int SetTimer(ITimerEvent *pTimerEvent, int nInterval);
	// 删除Timer事件
	virtual void KillTimer(int nTimerId);

private:
	void OnEventExcute(IAsyncEvent *pAsyncEvent);
	void OnEventResult(IAsyncEvent *pAsyncEvent);

	void OnThreadRun();

private:
	boost::asio::io_service &m_iosMain;
	boost::asio::io_service::work *m_pWorkThread;
	boost::asio::io_service m_iosThread;
	boost::interprocess::interprocess_semaphore *m_pSemaphore;

	MapDeadlineTimer m_mapDeadlineTimer;
};

NS_IO_Footer

#endif