//--------------------------------------------------------------------
// auther: hanzhe
// datetime: 2015-9-24
// description:

#ifndef _FILE_H_
#define _FILE_H_

#include <string>

#ifndef NS_IO

#define NS_IO		dgs
#define NS_IO_Header	namespace dgs {
#define NS_IO_Footer	}

#endif	//NS_IO

NS_IO_Header

std::string GetExeRootPath();

NS_IO_Footer


#endif