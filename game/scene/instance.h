//-------------------------------------------
// author : alterhz
// date : 8/15/2015
// description : 

#ifndef _INSTANCE_H_
#define _INSTANCE_H_

#include "scene.h"

class CInstance : public IScene
{
public:
	CInstance();
	~CInstance();

public:


protected:
	virtual bool OnTick();

private:

};

#endif