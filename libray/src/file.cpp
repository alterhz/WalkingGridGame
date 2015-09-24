#include "file.h"
#include "boostinc.h"

NS_IO_Header

std::string GetExeRootPath()
{
	std::string strRootPath = boost::filesystem::initial_path<boost::filesystem::path>().string();

	return strRootPath;
}

NS_IO_Footer

