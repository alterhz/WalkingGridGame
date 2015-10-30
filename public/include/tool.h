//-------------------------------------------
// author : alterhz
// date : 8/19/2015
// description : 

#ifndef _TOOL_H_
#define _TOOL_H_

#include <string>
#include <list>
#include <vector>

#ifndef NS_IO

#define NS_IO		dgs
#define NS_IO_Header	namespace dgs {
#define NS_IO_Footer	}

#endif	//NS_IO

NS_IO_Header

	// ²ð·Ö×Ö·û´®
	std::vector<std::string> Split(std::string str, std::string c);

NS_IO_Footer

#endif