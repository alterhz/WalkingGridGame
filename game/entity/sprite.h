//-------------------------------------------
// author : alterhz
// date : 8/15/2015
// description : 

#ifndef _SPRITE_H_
#define _SPRITE_H_

#include <string>
#include "action.h"


// 所有精灵（场景中的活动物体）的基类
class ISprite
{
public:
	ISprite();
	virtual ~ISprite();

	int GetId() const { return m_nId; }
	std::string GetName() const { return m_strName; }
	void SetName(std::string strName) { m_strName = strName; }
	

public:
	// 获取角色当前坐标位置
	virtual Vector2 GetCurrentPosition(MSTIME msNow) const;
	// 移动到某个位置(瞬移站立)，此函数很少使用
	virtual bool MoveTo(Vector2 &v2);
	// 跑
	virtual bool RunTo(VtPath &vtPath, MSTIME msNow);


protected:
	IAction * GetCurrentAction() { return m_pCurrentAction; }
	// 改变当前状态
	bool ChangeAction(IAction *pAction);

	// 创建Action和销毁Action
	void DestroyAction(IAction *pAction);

private:
	int m_nId;
	std::string m_strName;
	float m_fSpeed;

	IAction *m_pCurrentAction;
};

#endif