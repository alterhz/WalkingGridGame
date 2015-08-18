#include "role.h"

CRole::CRole()
{

}

CRole::~CRole()
{

}

bool CRole::Init()
{
	int nConfigHP = 300;

	m_attr.SetAttr(EAttrType_MaxHP, 300);

	return true;
}
