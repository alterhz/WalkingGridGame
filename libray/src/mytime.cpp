#include "mytime.h"
#include <ctime>

#ifdef WIN32
	#include <windows.h>
#endif // WIN32


NS_IO_Header

	// 获取应用程序启动时间戳
	long dgs::GetClock()
	{
		std::clock_t tNow = std::clock();
		return tNow;
	}

	unsigned int GetMSTime()
	{
	#ifdef WIN32
		return GetTickCount();
	#else
		struct timeval cur;
		gettimeofday(&cur, nullptr);
		return (swtime)(cur.tv_sec * 1000 + cur.tv_usec / 1000);
	#endif
	}

	time_t GetTime()
	{
		return std::time(nullptr);
	}

NS_IO_Footer