//-------------------------------------------
// author : alterhz
// date : 8/15/2015
// description : 

#ifndef _SPRITE_H_
#define _SPRITE_H_

#include <string>
using namespace std;

// 所有精灵（场景中的活动物体）的基类
class ISprite
{
public:
	ISprite() {}
	virtual ~ISprite() {}

	int GetId() const { return m_nId; }
	string GetName() const { return m_strName; }


private:
	int m_nId;
	string m_strName;

};

#endif