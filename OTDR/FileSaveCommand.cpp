#include "StdAfx.h"
#include "Command.h"
#include "FileSaveCommand.h"
#include "CommandManager.h"
#include "SorFileInfo.h"
#include "MainFrm.h"

extern SorFileArray g_sorFileArray;

CFileSaveCommand::CFileSaveCommand(CCommandManager* pCommandManager) : Command
	(pCommandManager)
{
	m_CommandID = Commands::FILE_SAVE;
}

CFileSaveCommand::~CFileSaveCommand(void)
{
}

void CFileSaveCommand::Execute()
{
	CString strMsg;

	CFileDialog dlg(FALSE,_T("*.SOR"),NULL,OFN_HIDEREADONLY,_T("SOR (*.SOR)|*.SOR||"));
	if(dlg.DoModal() != IDOK)
		return;

	CString strFilePath=dlg.GetPathName();//得到文件名
	CString strFileName=dlg.GetFileName();//路径加文件名
	
	TestResultInfo* pFileInfo = g_sorFileArray.sorFileList[g_sorFileArray.curSelection].pTestResultInfo;

	//保存文件
	CSorFile sorFile;
	if (1 == sorFile.saveSorFile(strFileName, *pFileInfo))
	{
	}
	else//出错，进行错误提示????????????????????????
	{

	}
}
