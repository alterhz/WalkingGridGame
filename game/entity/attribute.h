//-------------------------------------------
// author : alterhz
// date : 8/17/2015
// description : 

#ifndef _ATTRIBUTE_H_
#define _ATTRIBUTE_H_

#include <map>

enum EAttrType
{
	EAttrType_Null = 0,
	EAttrType_HP,		//当前血量
	EAttrType_MaxHP,	//最大血量
	EAttrType_Att,		//攻击力
	EAttrType_Speed,	//移动速度
};

class CAttribute
{
	typedef std::map<EAttrType, float> MapAttr;
public:
	CAttribute() {}
	~CAttribute() {}

public:
	void SetAttr(EAttrType eAttrType, float f);
	float GetAttr(EAttrType eAttrType);

private:
	MapAttr m_mapAttr;
};

#endif