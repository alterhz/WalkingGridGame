#include "hero.h"


IFightObj::IFightObj()
	: m_nIndexId(0)
{
	static int G_nIndexId = 0;
	m_nIndexId = (++G_nIndexId);
}

IFightObj::~IFightObj()
{

}
