//-------------------------------------------
// author : alterhz
// date : 8/15/2015
// description : 

#ifndef _LANDSCENE_H_
#define _LANDSCENE_H_

#include "scene.h"

class CLandScene : public IScene
{
public:
	CLandScene();
	~CLandScene();


public:
	virtual bool OnTick();
};

#endif