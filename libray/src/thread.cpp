#include "thread.h"
#include "boostinc.h"

NS_IO_Header

void Sleep(unsigned long nInterval)
{
	boost::this_thread::sleep(boost::posix_time::millisec(nInterval));
}

int GetCurThreadId()
{
#ifdef WIN32
	return GetCurrentThreadId();
#else
	return pthread_self();
#endif
}

// Ëø
CMutex::CMutex() : m_pMtx(new boost::mutex())
{
}
CMutex::~CMutex()
{
	delete m_pMtx;
	m_pMtx = nullptr;
}
void CMutex::try_lock()
{
	if (m_pMtx)
	{
		m_pMtx->try_lock();
	}
}
void CMutex::lock()
{
	if (m_pMtx)
	{
		m_pMtx->lock();
	}
}
void CMutex::unlock()
{
	if (m_pMtx)
	{
		m_pMtx->unlock();
	}
}

NS_IO_Footer
