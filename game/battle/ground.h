// 地形

#ifndef _GROUND_H_
#define _GROUND_H_

#include <map>

// 格子
class IGrid
{
public:
	enum EToWard
	{
		EToWard_None = 0,	//不通
		EToWard_X ,	//X方向通畅
		EToWard_Y,	//Y方向通畅
		EToWard_Both,	//双向通畅
	};

public:
	IGrid();
	virtual ~IGrid() {}


public:
	int GetX() const { return m_nX; }
	int GetY() const { return m_nY; }

	void Set(int nX, int nY) { m_nX = nX; m_nY = nY; }

public:
	// 是否可以行走
	virtual bool IsWalkable(EToWard eToWard) const { return true; }

private:
	int m_nX;
	int m_nY;
};

// 普通格子
class CBaseGrid : public IGrid
{
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
	CBaseGrid(EGroundType eGroundType);
	~CBaseGrid() {}

public:
	void SetGroundType(EGroundType eGroundType) { m_eGroundType = eGroundType; }
	EGroundType GetGroundType() const { return m_eGroundType; }

public:
	virtual bool IsWalkable(EToWard eToWard) const;

private:
	EGroundType m_eGroundType;
};

// 桥梁
class CBridgeGrid : public CBaseGrid
{
public:
	CBridgeGrid(EGroundType eGroundType, EToWard eToWard);
	~CBridgeGrid() {}

public:
	virtual bool IsWalkable(EToWard eToWard) const;

private:
	// 可以通过的朝向
	EToWard m_eToWard;
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

protected:
	bool InitGroundSize(int nWGCount, int nHGCount);

protected:
	int XY2N(int nH, int nW) const;
	int N2X(int n) const;
	int N2Y(int n) const;

	// 修改目标格子地形数据
	bool SetGrid(int nX, int nY, IGrid *pGrid);

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

};

#endif