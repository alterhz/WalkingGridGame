//-------------------------------------------
// author : alterhz
// date : 6/26/2015
// description : 

#ifndef _THREAD_H_
#define _THREAD_H_

#ifndef NS_IO

#define NS_IO		dgs
#define NS_IO_Header	namespace dgs {
#define NS_IO_Footer	}

#endif	//NS_IO

namespace boost
{
	class shared_mutex;
	class mutex;

	namespace interprocess
	{
		class interprocess_semaphore;
	}
}

NS_IO_Header

// 线程睡眠
void Sleep(unsigned long nInterval);
// 获取当前线程ID
int GetCurThreadId();

// 锁
class CMutex
{
public:
	CMutex();
	~CMutex();

	void try_lock();
	void lock();
	void unlock();

private:
	boost::mutex *m_pMtx;
};

// 线程
class IThread
{
public:
	virtual ~IThread() {}

public:
	virtual bool OnThreadRun() = 0;
};

// 启动线程
bool CreateThread(IThread &thread);

NS_IO_Footer

#endif