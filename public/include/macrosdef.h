//-------------------------------------------
// author : alterhz
// date : 9/9/2015
// description : 

#ifndef _MACROSDEF_H_
#define _MACROSDEF_H_

#include <map>
#include <vector>
#include <list>
#include "mymath.h"

#define SAFE_DELETE(p) { delete p; p = nullptr; }

typedef std::vector<int> VtInt;
typedef std::map<int, int> MapInt;
typedef std::list<int> ListInt;

enum EGroundType
{
	EGroundType_None = 0,
	EGroundType_Land = 1,	//土地
	EGroundType_Sand = 2,	//沙地
	EGroundType_River = 3,	//河流
	EGroundType_Lawn = 4,	//草地
	EGroundType_Snow = 5,	//雪地
};

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
	GObjectType_Still,
	GObjectType_Walkable,
};

struct COOR2
{
	COOR2()
		: x(0)
		, y(0) { }
	COOR2(int _x, int _y)
		: x(_x)
		, y(_y) { }

	static int Length(COOR2 a, COOR2 b)
	{
		int nLength = NS_IO::ABS(a.x - b.x) + NS_IO::ABS(a.y - b.y);

		return nLength;
	}

	const bool operator == (const COOR2& v) const
	{
		if (v.x == this->x && v.y == this->y)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	int x;
	int y;
};

typedef std::vector<COOR2> VtCoor2;

// 战斗对象
class IGObject;
typedef std::vector<IGObject *> VtGObject;
typedef std::map<int, IGObject *> MapGObject;

class IGrid;
typedef std::vector<IGrid *> VtGrid;
typedef std::map<int, IGrid *> MapGrid;

class ICountry;
typedef std::vector<ICountry *> VtCountry;
typedef std::map<int, ICountry *> MapCountry;



#endif