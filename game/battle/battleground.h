// 基础战场

#ifndef _BATTLEGROUND_H_
#define _BATTLEGROUND_H_

#include <map>

class IBattleGround
{
public:
	IBattleGround() {}
	virtual ~IBattleGround() {}

public:
	virtual bool Init() { return true; }

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
	virtual bool Init();


private:
	int m_nIndexId;
	// 国家（部队）列表
	MapCountry m_mapCountry;
	// 地形
	IGround * m_pGround;
};


#endif