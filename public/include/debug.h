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
#include "autostr.h"

NS_IO_Header

typedef AutoStr<1024> AutoStr1024;

#define LOGPrint(log) { \
	AutoStr1024 at;	\
	at + log + "[" + __FILE__ + " - " + __LINE__ + "]";	\
	std::cout << at.GetStr() << std::endl; \
}

NS_IO_Footer

#endif