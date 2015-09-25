//-------------------------------------------
// author : alterhz
// date : 8/20/2015
// description : 

#ifndef _XMLDATA_H_
#define _XMLDATA_H_

#include <string>

namespace tinyxml2
{
	class XMLElement;
}

class IXmlData
{
	enum EFieldType
	{
		EFieldType_Int = 0,
		EFieldType_Float,
		EFieldType_Bool,
		EFieldType_String,
		EFieldType_Enum,
	};

#define NanInt		(-99999);
#define NanFloat	(-99999.9999);
	
public:
	virtual ~IXmlData() {}

public:
	virtual int GetId() const { return m_nId; }
	virtual void SetId(int nId) { m_nId = nId; }
	// 单条记录载入
	virtual void OnRead(const tinyxml2::XMLElement *pEleRecord) = 0;

protected:
	bool ReadInt(const tinyxml2::XMLElement *pEleRecord, std::string strColumnName, int &n) const;
	bool ReadFloat(const tinyxml2::XMLElement *pEleRecord, std::string strColumnName, double &dValue) const;
	bool ReadString(const tinyxml2::XMLElement *pEleRecord, std::string strColumnName, std::string &str) const;
	bool ReadBool(const tinyxml2::XMLElement *pEleRecord, std::string strColumnName, bool &b) const;

private:
	int m_nId;
};

class CXmlData_Config : public IXmlData
{
public:
	CXmlData_Config() 
		: nInitAtt(0)
		, nInitHP(0)
	{}
	~CXmlData_Config() {}

	// 单条记录载入
	virtual void OnRead(const tinyxml2::XMLElement *pEleRecord);

	int nInitAtt;	//初始化攻击力
	int nInitHP;	//初始化血量
};

#endif