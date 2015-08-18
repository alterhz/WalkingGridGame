#include "attribute.h"

void CAttribute::SetAttr(EAttrType eAttrType, float f)
{
	m_mapAttr[eAttrType] = f;
}

float CAttribute::GetAttr(EAttrType eAttrType)
{
	MapAttr::iterator it = m_mapAttr.find(eAttrType);
	if (it != m_mapAttr.end())
	{
		return it->second;
	}

	return 0.f;
}

