//-------------------------------------------
// author : alterhz
// date : 8/15/2015
// description : 

#ifndef _ROLE_H_
#define _ROLE_H_

#include "sprite.h"

class CRole : public ISprite
{
public:
	CRole();
	~CRole();

	// 初始化
	bool Init();

public:
	// 获取精灵对象类型
	virtual ESpriteType GetSpriteType() const { return ESpriteType_Role; }

};


#endif