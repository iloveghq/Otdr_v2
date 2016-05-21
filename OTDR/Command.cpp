#include "StdAfx.h"
#include "Command.h"
#include "CommandManager.h"

Command::Command(void) : 
	m_CommandID(0),m_pCommandManager(NULL)
{
}

Command::Command(CCommandManager* pCommandManager) :
	m_pCommandManager(pCommandManager)
{
	
}

Command::~Command(void)
{
}

void Command::Execute()
{
	
}
