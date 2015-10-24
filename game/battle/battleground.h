// 基础战场

#ifndef _BATTLEGROUND_H_
#define _BATTLEGROUND_H_

#include <map>

#include "utilityinc.h"

class IBattleGround
{
public:
	IBattleGround();
	virtual ~IBattleGround();

	int GetIndexId() const { return m_nIndexId; }

public:
	virtual bool Init();

protected:
	virtual bool OnInit() { return true; }

private:
	int m_nIndexId;
};

class ICountry;
class IGround;

// 战场(阵地战)
class CFrontBattleGround : public IBattleGround
{
	typedef std::map<int, ICountry *> MapCountry;

public:
	CFrontBattleGround();
	~CFrontBattleGround();

public:
	// 初始化
	virtual bool OnInit();

	// 添加对战双方
	bool InitTwoCountry(ICountry *pCountryA, ICountry *pCountryB);


private:
	// 地形
	IGround * m_pGround;
	// 国家（部队）列表
	MapCountry m_mapCountry;
};

class CBattleGroundManager : public Singleton<CBattleGroundManager>
{
	typedef std::map<int, IBattleGround *> MapBattleGround;
public:
	CBattleGroundManager();
	~CBattleGroundManager();

public:
	bool Init();

	// 分配一个阵地战场景
	CFrontBattleGround* CreateFrontBattleGround();


private:
	MapBattleGround m_mapBattleGround;
};

#endif