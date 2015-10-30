#include "tool.h"
#include "boostinc.h"

NS_IO_Header

std::vector<std::string> Split(std::string str, std::string c)
{
	std::vector<std::string> vtSplit;
	boost::split(vtSplit, str, boost::is_any_of(c.c_str()), boost::algorithm::token_compress_on);

	return vtSplit;
}

NS_IO_Footer