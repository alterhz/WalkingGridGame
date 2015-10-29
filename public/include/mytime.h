//--------------------------------------------------------------------
// auther: hanzhe
// datetime: 2015-10-28
// description:

#ifndef _MYTIME_H_
#define _MYTIME_H_

#include <time.h>

#ifndef NS_IO

#define NS_IO		dgs
#define NS_IO_Header	namespace dgs {
#define NS_IO_Footer	}

#endif	//NS_IO

NS_IO_Header

	// 获取应用程序启动时间戳
	long GetClock();

	// 获取系统时间(ms)
	unsigned int GetMSTime();

	// 获取标准0时区时间戳
	time_t GetTime();

NS_IO_Footer

#endif