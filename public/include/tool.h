//-------------------------------------------
// author : alterhz
// date : 8/19/2015
// description : 

#ifndef _TOOL_H_
#define _TOOL_H_

#ifndef NS_IO

#define NS_IO		dgs
#define NS_IO_Header	namespace dgs {
#define NS_IO_Footer	}

#endif	//NS_IO

#include <string>

NS_IO_Header

	int string2int(std::string strI);
	std::string int2string(int n);

	double string2double(std::string strF);
	std::string double2string(double f);

NS_IO_Footer

#endif