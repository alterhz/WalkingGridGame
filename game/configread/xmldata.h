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
		: nTakeUpGridX(0)
		, nTakeUpGridY(0)
		, bCanDestroy(false)
		, nHP(0)
		, bFlag(false)
	{}
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
		: nWalkLength(0)
		, nHP(0)
		, nAtt(0)
		, nCommonSkillSN(0)
		, nSkillSN1(0)
		, nSkillSN2(0)
	{}
	~CXmlData_Walkable() {}

	// 单条记录载入
	virtual void OnRead(int nSN, const tinyxml2::XMLElement *pEleRecord);

	std::string strName;
	std::string strResource;
	std::string strDeadDesource;
	EToWard eToWard;
	int nWalkLength;
	VtInt vtSuperGrounds;
	int nHP;
	int nAtt;
	int nCommonSkillSN;
	int nSkillSN1;
	int nSkillSN2;
};

// Skill
class CXmlData_Skill : public IXmlData
{
public:
	enum EType
	{
		EType_Sirdar = 1,	//将领
		EType_Hero = 2,		//英雄
	};

	enum ERange
	{
		ERange_Single	= 1,	//单体
		ERange_Multi	= 2,	//范围
		ERange_All		= 3,	//全屏
	};

	enum ETarget
	{
		ETarget_Self	= 1,	//自身
		ETarget_Friend	= 2,	//友方
		ETarget_Enemy	= 3,	//敌方
	};

public:
	CXmlData_Skill()
		: nAddDamage(0)
		, nMinAttackLength(0)
		, nMaxAttackLength(0)
	{}
	~CXmlData_Skill() {}

	// 单条记录载入
	virtual void OnRead(int nSN, const tinyxml2::XMLElement *pEleRecord);

	std::string strName;
	std::string strDescription;
	std::string strResource;
	std::string strDeadDesource;
	EType eType;
	ERange eRange;
	ETarget eTarget;
	VtCoor2 vtCoor2;
	int nAddDamage;	//技能附加伤害
	int nMinAttackLength;	//最小攻击距离
	int nMaxAttackLength;	//最大攻击距离
};


#endif