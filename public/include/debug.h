//-------------------------------------------
// author : alterhz
// date : 6/18/2015
// description : 

#ifndef _DEBUG_H_
#define _DEBUG_H_

#ifndef NS_IO

#define NS_IO		dgs
#define NS_IO_Header	namespace dgs {
#define NS_IO_Footer	}

#endif	//NS_IO

#include <iostream>
#include <assert.h>
#include "autostr.h"

NS_IO_Header

typedef AutoStr<1024> AutoStr1024;

// 控制台打印
#define LOGPrint(log) { \
	AutoStr1024 at;	\
	at + log;	\
	std::cout << at.GetStr() << std::endl; \
}

// debug
#define LOGDebug(log) { \
	AutoStr1024 at;	\
	at + log + "[DEBUG][" + __FILE__ + " - " + __LINE__ + "]";	\
	std::cout << at.GetStr() << std::endl; \
}

// 警告
#define LOGWarn(log) { \
	AutoStr1024 at;	\
	at + log + "[WARN][" + __FILE__ + " - " + __LINE__ + "]";	\
	std::cout << at.GetStr() << std::endl; \
}

// 错误
#define LOGError(log) { \
	AutoStr1024 at;	\
	at + log + "[ERROR][" + __FILE__ + " - " + __LINE__ + "]";	\
	std::cout << at.GetStr() << std::endl; \
	assert(false);	\
}

// 记录
#define LOGInfo(log) { \
	AutoStr1024 at;	\
	at + log + "[INFO][" + __FILE__ + " - " + __LINE__ + "]";	\
	std::cout << at.GetStr() << std::endl; \
}

NS_IO_Footer

#endif