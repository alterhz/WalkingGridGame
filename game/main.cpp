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
	CApp::getMe().Run();

	LOGPrint("Have run.");

	system("pause");

	// 停止
	CApp::getMe().Stop();

	LOGPrint("Have stop.");
	
	system("pause");

	return 0;
}