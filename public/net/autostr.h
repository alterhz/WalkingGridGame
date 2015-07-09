//-------------------------------------------
// author : alterhz
// date : 6/18/2015
// description : 

#ifndef _AUTOSTR_H_
#define _AUTOSTR_H_

#ifndef NS_IO

#define NS_IO		dgs
#define NS_IO_Header	namespace dgs {
#define NS_IO_Footer	}

#endif	//NS_IO

#include <string>

NS_IO_Header

#ifdef WIN32
	#define swPrintf _snprintf_s
#else
	#include <memory.h>
	#define swPrintf snprintf
#endif // WIN32

template<unsigned int length>
class AutoStr
{
public:
	AutoStr() 
	{
		memset(m_szStr, 0, sizeof(m_szStr));
	}
	~AutoStr() 
	{
	}

	AutoStr & operator + (short n)
	{
		char szFormat[32] = {0};
		swPrintf(szFormat, sizeof(szFormat)-1, "%d", n);
		strcat(m_szStr, szFormat);
		return *this;
	}
	AutoStr & operator + (unsigned short n)
	{
		char szFormat[32] = {0};
		swPrintf(szFormat, sizeof(szFormat)-1, "%d", n);
		strcat(m_szStr, szFormat);
		return *this;
	}
	AutoStr & operator + (int n)
	{
		char szFormat[32] = {0};
		swPrintf(szFormat, sizeof(szFormat)-1, "%d", n);
		strcat(m_szStr, szFormat);
		return *this;
	}
	AutoStr & operator + (unsigned int n)
	{
		char szFormat[32] = {0};
		swPrintf(szFormat, sizeof(szFormat)-1, "%d", n);
		strcat(m_szStr, szFormat);
		return *this;
	}
	AutoStr & operator + (long n)
	{
		char szFormat[32] = {0};
		swPrintf(szFormat, sizeof(szFormat)-1, "%ld", n);
		strcat(m_szStr, szFormat);
		return *this;
	}
	AutoStr & operator + (unsigned long n)
	{
		char szFormat[32] = {0};
		swPrintf(szFormat, sizeof(szFormat)-1, "%ld", n);
		strcat(m_szStr, szFormat);
		return *this;
	}
	AutoStr & operator + (float n)
	{
		char szFormat[32] = {0};
		swPrintf(szFormat, sizeof(szFormat)-1, "%f", n);
		strcat(m_szStr, szFormat);
		return *this;
	}
	AutoStr & operator + (double n)
	{
		char szFormat[32] = {0};
		swPrintf(szFormat, sizeof(szFormat)-1, "%lf", n);
		strcat(m_szStr, szFormat);
		return *this;
	}
	AutoStr & operator + (const char *p)
	{
		strcat(m_szStr, p);
		return *this;
	}
	AutoStr & operator + (std::string s)
	{
		strcat(m_szStr, s.data());
		return *this;
	}
	AutoStr & operator + (bool b)
	{
		if (b)
		{
			strcat(m_szStr, "true");
		}
		else
		{
			strcat(m_szStr, "false");
		}

		return *this;
	}

	const char * GetStr() const { return m_szStr; }

private:
	char m_szStr[length];
};

NS_IO_Footer

#endif	//_AUTOSTR_H_