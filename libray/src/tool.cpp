#include "tool.h"
#include "boostinc.h"

NS_IO_Header

int string2int(std::string strI)
{
	int nVal = boost::lexical_cast<int>(strI.c_str());
	return nVal;
}

std::string int2string(int n)
{
	std::string strText = boost::lexical_cast<std::string>(n);
	return strText;
}

double string2double(std::string strF)
{
	double dVal = boost::lexical_cast<double>(strF.c_str());
	return dVal;
}

std::string double2string(double f)
{
	std::string strText = boost::lexical_cast<std::string>(f);
	return strText;
}

NS_IO_Footer