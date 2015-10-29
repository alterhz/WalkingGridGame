#include "mymath.h"
#include "boostinc.h"
#include <boost/random.hpp>

NS_IO_Header

int String2Int(std::string value)
{
	int nVal = boost::lexical_cast<int>(value.c_str());
	return nVal;
}
std::string Int2String(int n)
{
	std::string strText = boost::lexical_cast<std::string>(n);
	return strText;
}

double String2Double(std::string value)
{
	double d = boost::lexical_cast<double>(value.c_str());
	return d;
}
std::string Double2String(double d)
{
	std::string value = boost::lexical_cast<std::string>(d);
	return value;
}

//////////////////////////////////////////////////////////////////////////
// Ëæ»úÊý
template<typename T>
class rng_wraper
{
private:
	T rng;
public:
	rng_wraper() : rng((typename T::result_type)time(0)) {}
	typename T::result_type operator()() //override ()
	{
		return rng();
	}
};

class rng_t
{
public:
	static rng_t &getMe()
	{
		static rng_t m_rng_t;
		return m_rng_t;
	}
public:
	unsigned int rng()
	{
		return _rng();
	}

private:
	rng_wraper<boost::mt19937> _rng;
};
//////////////////////////////////////////////////////////////////////////

unsigned int Random()
{
	return rng_t::getMe().rng();
}

int ABS(int n)
{
	return abs(n);
}

NS_IO_Footer
