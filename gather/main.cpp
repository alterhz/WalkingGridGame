//-------------------------------------------
// author : alterhz
// date : 6/17/2015
// description : 

#include <iostream>
#include "app.h"
#include "debug.h"
#include "thread.h"

int main(int argc, char* argv[])
{
	CApp::getMe().InitDb();
	CApp::getMe().InitNet();

	CApp::getMe().DoTick();
	
	return 0;
}