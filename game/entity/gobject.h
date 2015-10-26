// 场景物件

#ifndef _GROUNDOBJECT_H_
#define _GROUNDOBJECT_H_

#include "macrosdef.h"
#include "xmldata.h"

class IGrid;
class IGround;

class CStillObject;
class CWalkableObject;


// 场景对象
class IGObject
{
public:
	IGObject();
	virtual ~IGObject() {}

	int GetIndexId() const { return m_nIndexId; }

	int GetX() const { return m_nX; }
	int GetY() const { return m_nY; }

	IGrid * GetGrid() const { return m_pGrid; }
	void BindGrid(IGrid *pGrid) { m_pGrid = pGrid; }

	// 阵营
	void SetCampId(int nCampId) { m_nCampId = nCampId; }
	int GetCampId() const { return m_nCampId; }


public:
	// 初始化函数
	bool Init(int nSN);
	// 进入场景
	bool EnterGround(int x, int y, IGround *pGround);

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
	// 是否可以攻击
	virtual bool IsCanAttack() const { return true; }
	virtual int GetHP() const { return 0; }
	virtual int GetSP() const { return 0; }
	virtual int GetAtt() const { return 0; }


private:
	int m_nIndexId;

protected:
	int m_nSN;
	// 位置数据
	int m_nX;
	int m_nY;
	// 场景对象
	IGround *m_pGround;

	// 格子
	IGrid *m_pGrid;
	// 阵营Id
	int m_nCampId;
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
	virtual bool IsCanAttack() const;

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
	virtual bool IsCanAttack() const { return true; }
	

public:
	// 行走
	bool Move(int x, int y);
	// 获取移动步长
	int GetWalkLength() const;

protected:
	const CXmlData_Walkable *m_pXmlData_Walkable;
};


#endif