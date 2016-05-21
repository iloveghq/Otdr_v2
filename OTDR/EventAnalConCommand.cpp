#include "StdAfx.h"
#include "Command.h"
#include "EventAnalConCommand.h"
#include "CommandManager.h"
#include "SorFileInfo.h"
#include "MainFrm.h"
#include "EventSearchPageSheet.h"

CEventAnalConCommand::CEventAnalConCommand(CCommandManager* pCommandManager) : Command
	(pCommandManager)
{
	m_CommandID = Commands::EVENT_ANALYSE_CON;
}

CEventAnalConCommand::~CEventAnalConCommand(void)
{
}

void CEventAnalConCommand::Execute()
{
	CEventSearchPageSheet configSheet(_T("…Ë÷√"));
	configSheet.DoModal();
}
