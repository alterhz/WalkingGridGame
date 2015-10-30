// 场景物件

#ifndef _GROUNDOBJECT_H_
#define _GROUNDOBJECT_H_

#include "macrosdef.h"
#include "xmldata.h"

class IGrid;
class IBattleGround;

class CStillObject;
class CWalkableObject;

#define CAMP_NEUTRAL	0	//中立阵营
#define CAMP_EAST		1	//东方
#define CAMP_WEST		2	//西方

// 场景对象
class IGObject
{
public:
	IGObject();
	virtual ~IGObject() {}

	int GetIndexId() const { return m_nIndexId; }

	int GetSN() const { return m_nSN; }
	int GetX() const { return m_nX; }
	int GetY() const { return m_nY; }
	void SetXY(int x, int y) { m_nX = x, m_nY = y; }

	COOR2 GetCoor2() const { return COOR2(m_nX, m_nY); }

	IGrid * GetGrid() const { return m_pGrid; }
	void BindGrid(IGrid *pGrid) { m_pGrid = pGrid; }

	// 阵营
	void SetCampId(int nCampId) { m_nCampId = nCampId; }
	int GetCampId() const { return m_nCampId; }

	void SetBattleGround(IBattleGround *pBattleGround) { m_pBattleGround = pBattleGround; }
	IBattleGround * GetBattleGround() const { return m_pBattleGround; }

public:
	// 初始化函数
	bool Init(int nSN);
	// 进入场景
	bool EnterGround(int x, int y, IBattleGround *pBattleGround);

protected:
	virtual bool OnInit() { return true; }

public:
	// 获取对象类型
	virtual GObjectType GetType() const = 0;
	// 获取静止对象
	virtual CStillObject * GetStillObject() { return nullptr; }
	// 获取移动对象
	virtual CWalkableObject* GetWalkableObject() { return nullptr; }
	// 对象不可以通过(一个格子不能存在两个角色单位)
	virtual bool IsWalkable(EToWard eToWard) const = 0;
	
	// 战斗相关
	// 是否参与战斗
	virtual bool IsCanFight() const { return true; }
	// 是否可以攻击
	virtual bool IsCanAttack() const { return true; }

	// 属性相关
	virtual int GetAtt() const { return 1; }
	int GetHP() const { return m_nHP; }
	virtual int GetMaxHP() const = 0;
	// 掉血
	int LostHP(int nHP);
	virtual int OnLostHP(int nHP) { return nHP; }
	virtual void OnDead() { }

	int GetSP() const { return m_nSP; }
	void AddSP(int nSP);
	virtual void OnAddSP(int nSP) {}

	int GetLevel() const { return m_nLevel; }
	void SetLevel(int nLevel) { m_nLevel = nLevel; }


private:
	int m_nIndexId;

protected:
	int m_nSN;
	// 位置数据
	int m_nX;
	int m_nY;
	// 场景对象
	IBattleGround *m_pBattleGround;

	// 格子
	IGrid *m_pGrid;
	// 阵营Id
	int m_nCampId;

	int m_nHP;
	int m_nSP;
	int m_nLevel;
};

// 静止（不可以移动）的场景对象
class CStillObject : public IGObject
{
public:
	CStillObject();
	virtual ~CStillObject();

protected:
	// 对象类型
	virtual GObjectType GetType() const { return GObjectType_Still; }
	// 获取静止对象
	virtual CStillObject * GetStillObject() { return this; }
	// 初始化
	virtual bool OnInit();
	// 是否可以通过
	virtual bool IsWalkable(EToWard eToWard) const;
	//战斗相关
	// 是否参与战斗
	virtual bool IsCanFight() const;
	// 是否可以攻击
	virtual bool IsCanAttack() const { return false; }
	// 获取最大血量
	virtual int GetMaxHP() const;
	// 死亡事件
	virtual void OnDead();

public:
	// 是否为旗帜类型（将领）
	bool IsFlag() const;

protected:
	const CXmlData_Still *m_pXmlData_Still;
};

// 角色对象
class CWalkableObject : public IGObject
{
public:
	CWalkableObject();
	virtual ~CWalkableObject();

public:
	// 释放技能
	virtual bool UseSkill(int nTargetGObjectIndexId);

protected:
	// 对象类型
	virtual GObjectType GetType() const { return GObjectType_Walkable; }
	// 获取移动对象
	virtual CWalkableObject * GetWalkableObject() { return this; }
	// 初始化
	virtual bool OnInit();
	// 对象不可以通过(一个格子不能存在两个角色单位)
	virtual bool IsWalkable(EToWard eToWard) const;
	// 战斗相关
	virtual int GetAtt() const;
	// 获取最大血量
	virtual int GetMaxHP() const;

private:
	int GetCommonSkillSN() const;
	int GetSkillSN1() const;
	int GetSkillSN2() const;

public:
	// 行走
	bool Move(int x, int y);
	bool Move(const VtCoor2 &vtCoor2);
	// 获取移动步长
	int GetWalkLength() const;

protected:
	const CXmlData_Walkable *m_pXmlData_Walkable;
};


#endif