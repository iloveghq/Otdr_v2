#include "StdAfx.h"
#include "Command.h"
#include "FileOpenCommand.h"
#include "CommandManager.h"
#include "SorFileInfo.h"
#include "MainFrm.h"

extern SorFileArray g_sorFileArray;

CFileOpenCommand::CFileOpenCommand(CCommandManager* pCommandManager) : Command
	(pCommandManager)
{
	m_CommandID = Commands::FILE_OPEN;
}

CFileOpenCommand::~CFileOpenCommand(void)
{
}

void CFileOpenCommand::Execute()
{
	CString strMsg;
	int index = GetNextCurveNum(g_sorFileArray);
	if (LB_ERR == index)//全部都加载过了
	{
		strMsg.Format(_T("最多加载%d条曲线.      "), MAX_CURVE_NUM);
		MessageBox(NULL, strMsg, _T("提示"), MB_OK|MB_ICONEXCLAMATION);
		return;
	}

	CFileDialog dlg(TRUE,_T("*.SOR"),NULL,OFN_HIDEREADONLY,_T("SOR (*.SOR)|*.SOR||"));
	if(dlg.DoModal() != IDOK)
		return;

	CString strFilePath=dlg.GetPathName();//得到文件名
	CString strFileName=dlg.GetFileName();//路径加文件名
	//判断文件是否已经加载
	if (IsFileAlreadyOpen(strFilePath))
	{	
		return;
	}
	//读取文件
	CSorFile sorFile;
	if (1 == sorFile.readSorFile(strFileName, *(g_sorFileArray.sorFileList[index].pTestResultInfo)))
	{
		g_sorFileArray.sorFileList[index].bValid = TRUE;
		g_sorFileArray.sorFileList[index].fileName = strFileName;
		g_sorFileArray.sorFileList[index].filePathName = strFilePath;
		g_sorFileArray.totalCurveNumber++;
		//设置曲线颜色
		g_sorFileArray.sorFileList[index].pTestResultInfo->curveColor = GetCurveColor(g_sorFileArray.totalCurveNumber);
		g_sorFileArray.curSelection = index;//设为当前选中行号
	}
	else//出错，进行错误提示????????????????????????
	{
		CString strMsg = _T("打开文件出错");
		MessageBox(NULL, strMsg, _T("提示"), MB_OK|MB_ICONEXCLAMATION);
		return; //
	}
	if (MAIN)
	{
		MAIN->m_pWaveFormView->PopulateData();
		//填充曲线数据
		MAIN->m_pWaveFormView->m_sorDrawStatic.FillCurveDataFromFile(g_sorFileArray.sorFileList[index].pTestResultInfo);
		MAIN->m_pWaveFormView->m_sorDrawStatic.Invalidate();//redraw
		MAIN->m_pThumbFormView->Invalidate(); //redraw thumbnail

		//设置横坐标单位文本
		MAIN->m_pWaveFormView->SetHorUnitText();
		//设置纵坐标单位文本
		MAIN->m_pWaveFormView->SetVerUnitText();

		//激活保存和打印菜单
		MAIN->m_bFileMenuEnable = TRUE;
	}
}
