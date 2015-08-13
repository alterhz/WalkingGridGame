//-------------------------------------------
// author : alterhz
// date : 8/10/2015
// description : 

#ifndef _MESSAGECOMMAND_H_
#define _MESSAGECOMMAND_H_

#include "msginc.h"

class CClient;

class IMessageCommand
{
public:
	IMessageCommand() {}
	~IMessageCommand() {}

public:
	virtual bool OnExcute(unsigned short wProtocolID, const char *pMessageBuffer, int nMessageLength, CClient *pClient) = 0;
};

class CMessageCommandManager
{
public:
	CMessageCommandManager() {}
	~CMessageCommandManager() {}

public:
	bool Init();
	bool DispatchMessage(unsigned short wProtocolId, const char *pMessageBuffer, int nMessageLength, CClient *pClient);

protected:
	virtual bool OnRegMessageCommand();
	bool BindMessageCommand(unsigned short wProtocolId, IMessageCommand *pMessageCommand);
	

private:
	typedef std::map<unsigned short, IMessageCommand *> MapMessageCommand;
	MapMessageCommand m_mapMessageCommand;
};



#endif