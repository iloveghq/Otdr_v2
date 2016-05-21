#include "StdAfx.h"
#include "Command.h"
#include "RemoteSettingCommand.h"
#include "CommandManager.h"
#include "SorFileInfo.h"
#include "MainFrm.h"
#include "RemotePageSheet.h"

CRemoteSettingCommand::CRemoteSettingCommand(CCommandManager* pCommandManager) : Command
	(pCommandManager)
{
	m_CommandID = Commands::REMOTE_SETTING;
}

CRemoteSettingCommand::~CRemoteSettingCommand(void)
{
}

void CRemoteSettingCommand::Execute()
{
	CRemotePageSheet remotePageSheet(_T("…Ë÷√"));
	remotePageSheet.DoModal();
}
