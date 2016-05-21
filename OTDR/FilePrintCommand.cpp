#include "StdAfx.h"
#include "Command.h"
#include "FilePrintCommand.h"
#include "CommandManager.h"
#include "SorFileInfo.h"
#include "MainFrm.h"

CFilePrintCommand::CFilePrintCommand(CCommandManager* pCommandManager) : Command
	(pCommandManager)
{
	m_CommandID = Commands::FILE_PRINT;
}

CFilePrintCommand::~CFilePrintCommand(void)
{
}

void CFilePrintCommand::Execute()
{
	CMainFrame* pMainWnd = (CMainFrame*)AfxGetMainWnd();
	if (pMainWnd)
	{
		//pMainWnd->m_pWaveFormView->Print();
	}
}
