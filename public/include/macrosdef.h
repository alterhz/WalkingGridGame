//-------------------------------------------
// author : alterhz
// date : 9/9/2015
// description : 

#ifndef _MACROSDEF_H_
#define _MACROSDEF_H_

#include <map>
#include <vector>
#include <list>

#define SAFE_DELETE(p) { delete p; p = nullptr; }

typedef std::vector<int> VtInt;
typedef std::map<int, int> MapInt;
typedef std::list<int> ListInt;

enum EToWard
{
	EToWard_None = 0,	//不通
	EToWard_X ,	//X方向通畅
	EToWard_Y,	//Y方向通畅
	EToWard_Both,	//双向通畅
};

enum GObjectType
{
	GObjectType_Base  = 0,	//基础对象
	GObjectType_Bridge,		//桥梁
	GObjectType_DogFace,	//小兵
	GObjectType_Hero,	//英雄
	GObjectType_Sirdar,	//将领
};

#endif