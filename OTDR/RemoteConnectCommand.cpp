#include "StdAfx.h"
#include "Command.h"
#include "RemoteConnectCommand.h"
#include "CommandManager.h"
#include "SorFileInfo.h"
#include "MainFrm.h"

extern SorFileArray g_sorFileArray;

CRemoteConnectCommand::CRemoteConnectCommand(CCommandManager* pCommandManager) : Command
	(pCommandManager)
{
	m_CommandID = Commands::REMOTE_CONNECT;
}

CRemoteConnectCommand::~CRemoteConnectCommand(void)
{
}

void CRemoteConnectCommand::Execute()
{
	if (MAIN->m_pRemoteConnDlg)
	{
		MAIN->m_pRemoteConnDlg->ShowWindow(SW_SHOW);
		MAIN->m_pRemoteConnDlg->CenterWindow();
	}
}
