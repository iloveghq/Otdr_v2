#include "StdAfx.h"
#include "Command.h"
#include "ScreenSettingCommand.h"
#include "CommandManager.h"
#include "SorFileInfo.h"
#include "MainFrm.h"
#include "ConfigSheet.h"

CScrenSettingCommand::CScrenSettingCommand(CCommandManager* pCommandManager) : Command
	(pCommandManager)
{
	m_CommandID = Commands::SCREEN_SETTING;
}

CScrenSettingCommand::~CScrenSettingCommand(void)
{
}

void CScrenSettingCommand::Execute()
{
	CConfigSheet configSheet(_T("…Ë÷√"));
	configSheet.DoModal();
}
