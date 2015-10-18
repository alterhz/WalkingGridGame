#include "country.h"


CCountry::CCountry()
	: m_nIndexId(0)
{
	static int g_nIndexId = 0;
	m_nIndexId = (++g_nIndexId);
}

CCountry::~CCountry()
{

}

