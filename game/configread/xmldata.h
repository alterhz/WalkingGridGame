//-------------------------------------------
// author : alterhz
// date : 8/20/2015
// description : 

#ifndef _XMLDATA_H_
#define _XMLDATA_H_

#include <string>
#include "macrosdef.h"

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
	virtual int GetSN() const { return m_nSN; }
	virtual void SetSN(int nSN) { m_nSN = nSN; }
	// 单条记录载入
	virtual void OnRead(int nSN, const tinyxml2::XMLElement *pEleRecord) = 0;

protected:
	bool ReadInt(const tinyxml2::XMLElement *pEleRecord, std::string strColumnName, int &n) const;
	bool ReadFloat(const tinyxml2::XMLElement *pEleRecord, std::string strColumnName, double &dValue) const;
	bool ReadString(const tinyxml2::XMLElement *pEleRecord, std::string strColumnName, std::string &str) const;
	bool ReadBool(const tinyxml2::XMLElement *pEleRecord, std::string strColumnName, bool &b) const;

private:
	int m_nSN;
};

// 全局配置
class CXmlData_Config : public IXmlData
{
public:
	CXmlData_Config() 
		: nInitAtt(0)
		, nInitHP(0)
	{}
	~CXmlData_Config() {}

	// 单条记录载入
	virtual void OnRead(int nSN, const tinyxml2::XMLElement *pEleRecord);

	int nInitAtt;	//初始化攻击力
	int nInitHP;	//初始化血量
};

// 地形
class CXmlData_Ground : public IXmlData
{
public:
	CXmlData_Ground()
		: eGroundType(EGroundType_None)
		, eToWard(EToWard_None)
	{
	}
	~CXmlData_Ground() {}

	// 单条记录载入
	virtual void OnRead(int nSN, const tinyxml2::XMLElement *pEleRecord);

	std::string strName;
	std::string strSrc;
	EGroundType eGroundType;
	std::string strEffects;
	EToWard eToWard;
};

// 场景物体
class CXmlData_Still : public IXmlData
{
public:
	CXmlData_Still()
	{
	}
	~CXmlData_Still() {}

	// 单条记录载入
	virtual void OnRead(int nSN, const tinyxml2::XMLElement *pEleRecord);

	std::string strName;
	std::string strResource;
	std::string strDestroyDesource;
	int nTakeUpGridX;
	int nTakeUpGridY;
	EToWard eToWard;
	bool bCanDestroy;
	int nHP;
	bool bFlag;
};

// 场景角色
class CXmlData_Walkable : public IXmlData
{
public:
	CXmlData_Walkable()
	{
	}
	~CXmlData_Walkable() {}

	// 单条记录载入
	virtual void OnRead(int nSN, const tinyxml2::XMLElement *pEleRecord);

	std::string strName;
	std::string strResource;
	std::string strDeadDesource;
	int nTakeUpGridX;
	int nTakeUpGridY;
	EToWard eToWard;
	int nWalkLength;
	VtInt vtSuperGrounds;
};


#endif