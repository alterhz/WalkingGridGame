#include "role.h"
#include "configread.h"
#include "debug.h"
using namespace NS_IO;

CRole::CRole()
{

}

CRole::~CRole()
{

}

bool CRole::Init()
{
	int nGlobalConfigId = CConfigReadManager::getMe().GetGlobalConfigId();
	const CXmlData_Config *pXD_Config = CConfigReadManager::getMe().xdConfig.GetRecord(nGlobalConfigId);
	if (nullptr == pXD_Config)
	{
		LOGError("²éÕÒÅäÖÃÊ§°Ü¡£");
		return false;
	}

	int nConfigHP = pXD_Config->nInitHP;

	m_attr.SetAttr(EAttrType_MaxHP, nConfigHP);

	return true;
}
