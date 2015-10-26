#include "xmldata.h"
#include "tinyxml2.h"
#include "tool.h"
#include "debug.h"

bool IXmlData::ReadInt(const tinyxml2::XMLElement *pEleRecord, std::string strColumnName, int &n) const
{
	if (nullptr == pEleRecord)
	{
		return false;
	}

	const tinyxml2::XMLElement *pEleColumn = pEleRecord->FirstChildElement(strColumnName.c_str());
	if (nullptr == pEleColumn)
	{
		return false;
	}

	const char *pText = pEleColumn->GetText();
	if (nullptr == pText)
	{
		return false;
	}

	n = NS_IO::string2int(pText);

	return true;
}

bool IXmlData::ReadFloat(const tinyxml2::XMLElement *pEleRecord, std::string strColumnName, double &dValue) const
{
	if (nullptr == pEleRecord)
	{
		return false;
	}

	const tinyxml2::XMLElement *pEleColumn = pEleRecord->FirstChildElement(strColumnName.c_str());
	if (nullptr == pEleColumn)
	{
		return false;
	}

	const char *pText = pEleColumn->GetText();
	if (nullptr == pText)
	{
		return false;
	}

	dValue = NS_IO::string2double(pText);

	return true;
}

bool IXmlData::ReadString(const tinyxml2::XMLElement *pEleRecord, std::string strColumnName, std::string &str) const
{
	if (nullptr == pEleRecord)
	{
		return false;
	}

	const tinyxml2::XMLElement *pEleColumn = pEleRecord->FirstChildElement(strColumnName.c_str());
	if (nullptr == pEleColumn)
	{
		return false;
	}

	const char *pText = pEleColumn->GetText();
	if (nullptr == pText)
	{
		return false;
	}

	str = pText;

	return true;
}

bool IXmlData::ReadBool(const tinyxml2::XMLElement *pEleRecord, std::string strColumnName, bool &b) const
{
	if (nullptr == pEleRecord)
	{
		return false;
	}

	const tinyxml2::XMLElement *pEleColumn = pEleRecord->FirstChildElement(strColumnName.c_str());
	if (nullptr == pEleColumn)
	{
		return false;
	}

	const char *pText = pEleColumn->GetText();
	if (nullptr == pText)
	{
		return false;
	}

	int n = NS_IO::string2int(pText);

	if (0 == n)
	{
		b = false;
	}
	else
	{
		b = true;
	}

	return true;
}


#define READINT(NAME, VAR) ReadInt(pEleRecord, NAME, VAR)
//////////////////////////////////////////////////////////////////////////
// 配置文件字段匹配读取

void CXmlData_Config::OnRead(int nSN, const tinyxml2::XMLElement *pEleRecord)
{
	ReadInt(pEleRecord, "InitAtt", nInitAtt);
	ReadInt(pEleRecord, "InitHP", nInitHP);
}

void CXmlData_Ground::OnRead(int nSN, const tinyxml2::XMLElement *pEleRecord)
{
	ReadString(pEleRecord, "Name", strName);
	ReadString(pEleRecord, "Src", strSrc);

	int nGroundType = 0;
	ReadInt(pEleRecord, "Type", nGroundType);
	eGroundType = static_cast<EGroundType>(nGroundType);

	ReadString(pEleRecord, "Effects", strEffects);

	int nToWard = 0;
	ReadInt(pEleRecord, "ToWard", nToWard);
	eToWard = static_cast<EToWard>(nToWard);
}

void CXmlData_Still::OnRead(int nSN, const tinyxml2::XMLElement *pEleRecord)
{
	ReadString(pEleRecord, "Name", strName);
	ReadString(pEleRecord, "Resource", strResource);
	ReadString(pEleRecord, "DestroyResource", strDestroyDesource);

	ReadInt(pEleRecord, "TakeUpGridX", nTakeUpGridX);
	ReadInt(pEleRecord, "TakeUpGridY", nTakeUpGridY);

	int nToWard = 0;
	ReadInt(pEleRecord, "ToWard", nToWard);
	eToWard = static_cast<EToWard>(nToWard);

	ReadBool(pEleRecord, "CanDestroy", bCanDestroy);

	ReadInt(pEleRecord, "HP", nHP);

	ReadBool(pEleRecord, "Flag", bFlag);
}

void CXmlData_Walkable::OnRead(int nSN, const tinyxml2::XMLElement *pEleRecord)
{
	ReadString(pEleRecord, "Name", strName);
	ReadString(pEleRecord, "Resource", strResource);
	ReadString(pEleRecord, "DestroyResource", strDeadDesource);

	ReadInt(pEleRecord, "TakeUpGridX", nTakeUpGridX);
	ReadInt(pEleRecord, "TakeUpGridY", nTakeUpGridY);

	int nToWard = 0;
	ReadInt(pEleRecord, "ToWard", nToWard);
	eToWard = static_cast<EToWard>(nToWard);

	ReadInt(pEleRecord, "WalkLength", nWalkLength);

	// 优势地形
	std::string strSuperGround;
	ReadString(pEleRecord, "SuperGround", strSuperGround);

}
