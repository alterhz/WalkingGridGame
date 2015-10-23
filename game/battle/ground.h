// 地形

#ifndef _GROUND_H_
#define _GROUND_H_

#include <map>
#include "macrosdef.h"

class IGObject;

// 格子
class IGrid
{
	typedef std::map<int, IGObject *> MapGObject;

public:
	enum EGroundType
	{
		EGroundType_Null = 0,
		EGroundType_Land,	//土地
		EGroundType_Sand,	//沙地
		EGroundType_River,	//河流
		EGroundType_Lawn,	//草地
		EGroundType_Snow,	//雪地
	};

public:
	IGrid(EGroundType eGroundType, int x, int y);
	virtual ~IGrid() {}


public:
	int GetX() const { return m_nX; }
	int GetY() const { return m_nY; }

	EGroundType GetGroundType() const { return m_eGroundType; }

	// 添加场景绑定单位
	bool AddGObject(IGObject *pGObject);
	bool DelGObject(IGObject *pGObject);

public:
	// 是否可以行走
	virtual bool IsWalkable(EToWard eToWard) const;

protected:
	virtual void OnAddGObject(IGObject *pGObject);
	virtual void OnDelGObject(IGObject *pGObject);


private:
	IGObject * FindGObject(int nIndexId);
	IGObject * FindGObject(int nIndexId) const;


private:
	int m_nX;
	int m_nY;
	// 场景类型
	EGroundType m_eGroundType;
	// 格子上绑定的场景对象，最多2个，如桥梁和战斗单位；如果是城墙，当城墙被摧毁后，可以和战斗单位一起共存。
	MapGObject m_mapGObject;
};

// 普通格子
class CGrid : public IGrid
{
public:
	CGrid(EGroundType eGroundType, int x, int y);
	~CGrid() {}
};

const int G_nDemoWidthCount = 20;
const int G_nDemoHeigthCount = 30;

// 地形
class IGround
{
public:
	typedef std::map<int, IGrid *> MapGrid;

public:
	IGround();
	virtual ~IGround();

public:
	virtual bool Init();

	IGrid * GetGrid(int x, int y);
	IGrid * GetGrid(int x, int y) const;

protected:
	bool InitGroundSize(int nWGCount, int nHGCount);

protected:
	int XY2N(int x, int y) const;
	int N2X(int n) const;
	int N2Y(int n) const;

	// 修改目标格子地形数据
	bool SetGrid(int x, int y, IGrid *pGrid);

private:
	IGrid * Find(int n);
	IGrid * Find(int n) const;

protected:
	MapGrid m_mapGrid;
	int m_nWGCount;	//Width方向Grid数量
	int m_nHGCount;	//Heigth方向Grid数量
};

class CDemoGround : public IGround
{
public:
	CDemoGround();
	~CDemoGround();

public:
	virtual bool Init(int nWGCount, int nHGCount);

	// 加载
};


#endif