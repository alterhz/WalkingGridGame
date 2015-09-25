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


void CXmlData_Config::OnRead(const tinyxml2::XMLElement *pEleRecord)
{
	ReadInt(pEleRecord, "InitAtt", nInitAtt);
	ReadInt(pEleRecord, "InitHP", nInitHP);
}
