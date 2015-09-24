//--------------------------------------------------------------------
// auther: hanzhe
// datetime: 2015-9-24
// description:

#ifndef _MATH_H_
#define _MATH_H_

#include <string>

#ifndef NS_IO

#define NS_IO		dgs
#define NS_IO_Header	namespace dgs {
#define NS_IO_Footer	}

#endif	//NS_IO

NS_IO_Header

int String2Int(std::string value);
std::string Int2String(int n);

double String2Double(std::string value);
std::string Double2String(double d);

unsigned int Random();

NS_IO_Footer

#endif