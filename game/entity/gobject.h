// 场景物件

#ifndef _GROUNDOBJECT_H_
#define _GROUNDOBJECT_H_

#include "macrosdef.h"

class IGrid;
class IGround;
class IFightGObject;
class IWalkableGObject;


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

public:
	// 初始化函数
	bool Init();
	// 进入场景
	bool EnterGround(int x, int y, IGround *pGround);

protected:
	virtual bool OnInit() { return true; }

public:
	// 获取对象类型
	virtual GObjectType GetType() const = 0;
	// 获取战斗对象
	virtual IFightGObject * GetFightGObject() { return nullptr; }
	// 获取可行走对象
	virtual IWalkableGObject * GetWalkableGObject() { return nullptr; }

	// 对象不可以通过(一个格子不能存在两个角色单位)
	virtual bool IsWalkable(EToWard eToWard) const = 0;

private:
	int m_nIndexId;

protected:
	// 位置数据
	int m_nX;
	int m_nY;
	// 场景对象
	IGround *m_pGround;

	// 格子
	IGrid *m_pGrid;
};

// 桥梁
class CGBridge : public IGObject
{
public:
	CGBridge(EToWard eToWard) 
		: m_eToWard(eToWard)
	{}
	~CGBridge() {}

public:
	virtual GObjectType GetType() const { return GObjectType_Bridge; }
	virtual bool IsWalkable(EToWard eToWard) const;

private:
	// 可以通过的朝向
	EToWard m_eToWard;
};

// 战斗对象
class IFightGObject : public IGObject
{
public:
	IFightGObject();
	virtual ~IFightGObject();

public:
	// 对象不可以通过(一个格子不能存在两个角色单位)
	virtual bool IsWalkable(EToWard eToWard) const { return false; }
	// 获取战斗对象
	virtual IFightGObject * GetFightGObject() { return this; }

protected:
	virtual bool OnInit();

public:
	virtual int GetHP() const { return m_nHP; }
	virtual int GetAtt() const { return m_nAtt; }

protected:
	int m_nHP;
	int m_nAtt;
};

// 可行走对象
class IWalkableGObject : public IFightGObject
{
public:
	IWalkableGObject();
	virtual ~IWalkableGObject();

public:
	// 获取可行走对象
	virtual IWalkableGObject * GetWalkableGObject() { return this; }

public:
	// 行走
	bool Move(int x, int y);

};

// 小兵
class CDogFace : public IWalkableGObject 
{
public:
	CDogFace();
	~CDogFace();

public:
	virtual GObjectType GetType() const { return GObjectType_DogFace; }
};

// 英雄
class CHero : public IWalkableGObject 
{
public:
	CHero() {}
	~CHero() {}

public:
	virtual GObjectType GetType() const { return GObjectType_Hero; }

};

// 将领（司令官）
class CSirdar : public IFightGObject
{
public:
	CSirdar() {}
	~CSirdar() {}

public:
	virtual GObjectType GetType() const { return GObjectType_Sirdar; }

};


#endif