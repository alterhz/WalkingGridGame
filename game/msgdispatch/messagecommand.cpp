#include "messagecommand.h"
#include "debug.h"

using namespace NS_IO;

bool CMessageCommandManager::OnRegMessageCommand()
{

	return true;
}

bool CMessageCommandManager::Init()
{
	return OnRegMessageCommand();
}

bool CMessageCommandManager::BindMessageCommand(unsigned short wProtocolId, IMessageCommand *pMessageCommand)
{
	MapMessageCommand::iterator it = m_mapMessageCommand.find(wProtocolId);
	if (it != m_mapMessageCommand.end())
	{
		LOGPrint("Ð­Òé[" + wProtocolId + "]ÖØ¸´×¢²á¡£");
		return false;
	}

	m_mapMessageCommand.insert(std::make_pair(wProtocolId, pMessageCommand));


	return true;
}


