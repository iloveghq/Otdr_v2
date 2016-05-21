#include "StdAfx.h"
#include "Command.h"
#include "CommandManager.h"

#include "FileOpenCommand.h"
#include "FileSaveCommand.h"
#include "FilePrintCommand.h"

#include "RemoteConnectCommand.h"
#include "RemoteSettingCommand.h"
#include "LangSwitchCommand.h"

#include "ScreenSettingCommand.h"

#include "EventAnalConCommand.h"

//¶¯Ì¬´´½¨Command
#define COMMAND_DYNCREATE(class_name) { Command* pCommand = new class_name(this); \
	m_mapCommands.insert(std::map<const int, Command*>::value_type(pCommand->m_CommandID, pCommand));}\

CCommandManager::CCommandManager()
{
}

CCommandManager::~CCommandManager(void)
{
	DeleteCommands();
}

void CCommandManager::CreateCommands()
{
	COMMAND_DYNCREATE(CFileOpenCommand)  //File Open Command
	COMMAND_DYNCREATE(CFileSaveCommand)  //File Save Command
	COMMAND_DYNCREATE(CFilePrintCommand) //File Print Command

	COMMAND_DYNCREATE(CRemoteConnectCommand) //Remote Connect Setting Command
	COMMAND_DYNCREATE(CRemoteSettingCommand) //Tool Setting Command
	COMMAND_DYNCREATE(CLangSwitchCommand) //Lang Switch Command

	COMMAND_DYNCREATE(CScrenSettingCommand) //Screen Setting Command

	COMMAND_DYNCREATE(CEventAnalConCommand) //Event Analyse Command
	
}

Command* CCommandManager::GetCommandByID(int nCommandID)
{
	return m_mapCommands[nCommandID];
}

void CCommandManager::DeleteCommands()
{
	//delete the commands
	std::map<const int, Command*>::iterator it;
	for(it = m_mapCommands.begin(); it != m_mapCommands.end(); ++it)
	{
		Command* pCommand = it->second;
		if (pCommand)
		{
			delete pCommand;
			pCommand = NULL;
		}
	}
}
