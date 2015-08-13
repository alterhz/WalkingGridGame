#include "logindispatcher.h"

#include <iostream>

void CLoginDispatcher::OnInit()
{
	RegMessage(1001, &CLoginDispatcher::OnRecvTest, this);
}

void CLoginDispatcher::OnRecvTest(const char *pMessage, int nLength)
{
	std::cout << pMessage << nLength << std::endl;
}

