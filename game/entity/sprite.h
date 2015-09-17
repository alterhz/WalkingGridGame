//-------------------------------------------
// author : alterhz
// date : 8/15/2015
// description : 

#ifndef _SPRITE_H_
#define _SPRITE_H_

#include "memoryleak.h"
#include <string>
#include "action.h"
#include "attribute.h"

class IScene;

enum ESpriteType
{
	ESpriteType_Null = 0,
	ESpriteType_Role,
	ESpriteType_Monster,
};

// 所有精灵（场景中的活动物体）的基类
class ISprite
{
public:
	ISprite();
	virtual ~ISprite();

public:	//base
	int GetId() const { return m_nId; }
	std::string GetName() const { return m_strName; }
	void SetName(std::string strName) { m_strName = strName; }

	// 设置当前血量
	void SetHP(int nHP);
	// 设置最大血量
	void SetMaxHP(int nMaxHP);
	
public:	//base
	// 获取精灵对象类型
	virtual ESpriteType GetSpriteType() const = 0;

public:
	// 进入场景
	virtual bool DoEnterScene();
	virtual bool DoLeaveScene();

public:
	// 逻辑循环
	virtual bool DoTick(MSTIME msNow);
	// 获取角色当前坐标位置
	virtual Vector2 GetCurrentPosition(MSTIME msNow) const;
	// 移动到某个位置(瞬移站立)，此函数很少使用
	virtual bool MoveTo(Vector2 &v2);
	// 跑
	virtual bool RunTo(VtPath &vtPath, MSTIME msNow);

public:
	virtual bool OnEnterScene(IScene *pScene);
	virtual bool OnLeaveScene(IScene *pScene);


protected:
	// 逻辑循环事件
	virtual bool OnTick(MSTIME msNow);
	// 状态自动切换
	virtual void OnActionAutoSwitch(MSTIME msNow);

protected:
	IAction * GetCurrentAction() { return m_pCurrentAction; }
	// 改变当前状态
	bool ChangeAction(IAction *pAction);

private:
	// 创建Action和销毁Action
	void DestroyAction(IAction *pAction);

protected:
	CAttribute m_attr;

private:
	int m_nId;
	std::string m_strName;

	IAction *m_pCurrentAction;

	int m_nWaitEnterSceneId;

	// 当前所在场景
	IScene *m_pCurrentScene;
};

#endif