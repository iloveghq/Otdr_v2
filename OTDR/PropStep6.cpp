// PropStep6.cpp : 实现文件
//

#include "stdafx.h"
#include "OTDR.h"
#include "Util.h"
#include "PrintSheet.h"
#include "PropStep6.h"
#include "CSpreadSheet.h"
#include "XmlNode.h"
#include "XmlNodes.h"

extern SorFileArray g_sorFileArray;
extern LANG g_currentLang;
//模板文件
CString str_cannot_find_file=_T("");
CString str_check_file=_T("");
// CPropStep6 对话框

IMPLEMENT_DYNAMIC(CPropStep6, CPropertyPage)

CPropStep6::CPropStep6()
	: CPropertyPage(CPropStep6::IDD)
{
	m_pThread = NULL;
}

CPropStep6::~CPropStep6()
{
}

void CPropStep6::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPropStep6, CPropertyPage)
	ON_BN_CLICKED(IDC_BTN_STEP6_PREVIEW, &CPropStep6::OnBnClickedBtnStep6Preview)
	ON_BN_CLICKED(IDC_BTN_STEP6_SETTING, &CPropStep6::OnBnClickedBtnStep6Setting)
	ON_BN_CLICKED(IDC_BTN_STEP6_PRINT, &CPropStep6::OnBnClickedBtnStep6Print)
	ON_BN_CLICKED(IDC_BTN_STEP6_EXCEL, &CPropStep6::OnBnClickedBtnStep6Excel)
END_MESSAGE_MAP()


// CPropStep6 消息处理程序
BOOL CPropStep6::OnSetActive() 
{
	// TODO: Add your specialized code here and/or call the base class
	//((CPropertySheet*)GetParent())->SetWizardButtons(PSWIZB_FINISH);
	((CPropSheet*)GetParent())->SetReportHeadBtn(6);
	//重绘图形区域
	MAIN->m_pReportFormView->m_ReportViewDlg.m_DrawButton.ReDrawRect(6);
	((CPropSheet*)GetParent())->EnableNextBtn(FALSE);
	((CPropSheet*)GetParent())->EnablePrevBtn(TRUE);
	return CPropertyPage::OnSetActive();
}

BOOL CPropStep6::OnWizardFinish() 
{
	// TODO: Add your specialized code here and/or call the base class
	//UpdateData();

	return CPropertyPage::OnWizardFinish();
}
void CPropStep6::OnBnClickedBtnStep6Preview()
{
	MAIN->printType=1;//printType=1时，打印报表向导中所有曲线文件
	//填充文件列表
	MAIN->m_pMainSetting->m_pageMainSetting.PopulateFileList();//再次填充所有曲线文件，否则只有4条曲线可打印
	MAIN->OnPrintPreview();
}

void CPropStep6::OnBnClickedBtnStep6Setting()
{
	CPrintSheet printSheet(_T("页面设置"));
	printSheet.DoModal();
}

void CPropStep6::OnBnClickedBtnStep6Print()
{
	MAIN->m_pMainSetting->m_pageMainSetting.PopulateFileList();//再次填充所有曲线文件，否则只有4条曲线可打印//原来要先预览wcq2016.03.03
	MAIN->OnPrint();
}

void CPropStep6::OnBnClickedBtnStep6Excel()
{
	//m_pThread = AfxBeginThread(CPropStep6::ExportXlsThread,this);//pThread2	
	//显示进度条
	m_progress.Create(IDD_DIALOG_PROGRRESS);
	m_progress.ShowWindow(SW_HIDE);
	m_progress.CenterWindow();
	ExportToExcel();
	//m_progress.SetPos(g_sorFileArray.totalCurveNumber);
	m_progress.DestroyWindow();
}

BOOL CPropStep6::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	SetLocalString();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pag	es should return FALSE
}

void CPropStep6::SetLocalString()
{
	/*                      输出设置                            */
	CString strOutputSetting = IniUtil::ReadResString(_T("IDS_REPORT_STEP6_OUTPUT_SETTING"));
	GetDlgItem(IDC_STATIC_STEP6_SETTING)->SetWindowText(strOutputSetting);
	//预览
	CString strPreview = IniUtil::ReadResString(_T("IDS_REPORT_STEP6_PREVIEW"));
	GetDlgItem(IDC_BTN_STEP6_PREVIEW)->SetWindowText(strPreview);
	//页面设置
	CString strPageSetup = IniUtil::ReadResString(_T("IDS_REPORT_STEP6_PAGE_SETUP"));
	GetDlgItem(IDC_BTN_STEP6_SETTING)->SetWindowText(strPageSetup);
	/*                      打印方向                            */
	CString strPrintDirect = IniUtil::ReadResString(_T("IDS_REPORT_STEP6_OUTPUT_SETTING"));
	GetDlgItem(IDC_STATIC_STEP6_SELECT)->SetWindowText(strPrintDirect);
	//打印
	CString strPrint = IniUtil::ReadResString(_T("IDS_REPORT_STEP6_PRINT"));
	GetDlgItem(IDC_BTN_STEP6_PRINT)->SetWindowText(strPrint);
	//模板文件
	str_cannot_find_file=IniUtil::ReadResString(_T("IDS_CANNOT_FIND_FILE"));
	str_check_file=IniUtil::ReadResString(_T("IDS_CHECK_FILE"));
}

LRESULT CPropStep6::OnWizardBack()
{
	MAIN->ShowMainSetting(TRUE);
	return 0;
}

UINT CPropStep6::ExportXlsThread(LPVOID lparam)
{
	CPropStep6* dlg=(CPropStep6*)lparam;
	dlg->ExportToExcel();
	dlg->m_progress.SetPos(g_sorFileArray.totalCurveNumber);
	dlg->m_progress.DestroyWindow();
	return 0;
}

void CPropStep6::ExportToExcel()
{
	//填充文件列表
	MAIN->m_pMainSetting->m_pageMainSetting.PopulateFileList();
	//若是波形需要产生位图文件
	int nPrint = _wtoi(g_sorFileArray.reportConfig.Print);
	if (nPrint)
	{
		MAIN->m_pReportFormView->m_ReportViewDlg.m_DrawButton.OutPutBmp();//产生位图
	}//eif

	//导出Excel
	ExportXlsByRpt();
	//m_progress.SetPos(15);
	if (nPrint)//删除位图文件
		MAIN->m_pReportFormView->m_ReportViewDlg.m_DrawButton.DeleteBmpFiles();
}

//导出列表框中单条数据,通过报表
BOOL CPropStep6::ExportXlsByRpt()
{
	//获取保存excel路径
	CString strPath = IniUtil::ReadSingleConfigInfo(_T("ExcelOut"), _T("Path"));
	//CFileDialog dlg(FALSE,_T("*.xls"),NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,_T("xls (Excel格式)|*.xls||"));
	CFileDialog dlg(FALSE,_T("*.xls"),NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,_T("*.xls;*.xlsx (Excel)|*.xls;*.xlsx||"));//zll 2013.7.9 同时兼容.xlsx后缀的文件
	dlg.m_ofn.lpstrInitialDir=strPath;
	if(dlg.DoModal() != IDOK)
		return FALSE;

	CString strFileName = dlg.GetPathName();//路径加文件名

	/////////////////zll 2013.7.8
	CString temp=_T("");
	temp=strFileName.Right(1);
	int a=temp.CompareNoCase(_T("x"));
	if (temp.CompareNoCase(_T("x"))==0)
	{
		strFileName=strFileName.Left(strFileName.GetLength()-1);
	}///////////////

	//显示进度条
	m_progress.ShowWindow(SW_SHOW);
	m_progress.SetPos(1);
	//根据设置不同加载不同的Excel模板
	CString strExcelTemp;
	if (g_sorFileArray.reportConfig.Print.CompareNoCase(_T("1")) == 0)
	{
		strExcelTemp.Format(_T("%s\\Excel\\Temp1.xls"), CUtil::getExePath());//波形Excel模板
		if(g_currentLang == LANG::CHS)
		{
			m_strXmlFile.Format(_T("%s\\Excel\\Wave.xml"), CUtil::getExePath());//中文波形数据模板
		}
		else if(g_currentLang == LANG::ENG)
		{
			m_strXmlFile.Format(_T("%s\\Excel\\WaveEn.xml"), CUtil::getExePath());//英文波形数据模板 zll 2013.6.9
		}
		else if(g_currentLang == LANG::PORT)  //葡萄牙语波形数据模板 lzy 2014.7.17
		{
			m_strXmlFile.Format(_T("%s\\Excel\\WavePt.xml"), CUtil::getExePath());
		}
	}
	else
	{
		strExcelTemp.Format(_T("%s\\Excel\\Temp2.xls"), CUtil::getExePath());//总表Excel模板
		m_strXmlFile.Format(_T("%s\\Excel\\Rpt.xml"), CUtil::getExePath());//总表数据模板
	}

	//检查模板文件xxx.xls是否存在.
	if(!CUtil::isFileExist(strExcelTemp))
	{
		CString strMsg=str_cannot_find_file+strExcelTemp+str_check_file;
		MessageBox(strMsg,_T("OTDR Viewer"),MB_OK|MB_ICONEXCLAMATION);
		return FALSE;
	}
	
	CopyFile(strExcelTemp, strFileName, false);
	
	int nSheetIndex = GetSheetIndex();
	int nCurveNumber = MAIN->m_pReportFormView->m_ReportViewDlg.m_DrawButton.m_nCurveNumber;
	if (nCurveNumber == 0)
		nCurveNumber= 1;  //1 curve per page
	else if (nCurveNumber == 1)
		nCurveNumber= 2;  //2 curve per page
	else if (nCurveNumber == 2)
		nCurveNumber= 4;  //4 curve per page
	else if (nCurveNumber == 3)
		nCurveNumber= 6;   //6 curve per page
	else if (nCurveNumber == 4)
		nCurveNumber= 8;   //8 curve per page
	int page_sheetcount=0;
	page_sheetcount=g_sorFileArray.totalCurveNumber/nCurveNumber;
	if (0!=g_sorFileArray.totalCurveNumber%nCurveNumber)
	{
		page_sheetcount+=1;
	}
	WriteToExcel(strFileName, nSheetIndex,page_sheetcount);//zll 2012.9.25 SheetCount
	return TRUE;
}

int CPropStep6::GetSheetIndex()
{
	int nPrint = _wtoi(g_sorFileArray.reportConfig.Print);
	if (nPrint)
	{
		//波形
		int nCurveNumber = MAIN->m_pReportFormView->m_ReportViewDlg.m_DrawButton.m_nCurveNumber;
		if (nCurveNumber == 0)
			return 1;  //1 curve per page
		else if (nCurveNumber == 1)
			return 2;  //2 curve per page
		else if (nCurveNumber == 2)
			return 3;  //4 curve per page
		else if (nCurveNumber == 3)
			return 4;   //6 curve per page
		else if (nCurveNumber == 4)
			return 5;   //8 curve per page
	}
	else
	{
		//总表
	}

	return 1;
}

void CPropStep6::WriteToExcel(CSpreadSheet* excel)
{
	BOOL bSucceed = m_xml.Open(m_strXmlFile);
	if (!bSucceed)
		return;

	excel->BeginTransaction();
	// 加入标题
	CXmlNode nodeTitle;
	nodeTitle = m_xml.GetRoot()->GetChild(_T("P1"))->GetChild(_T("Title"));
	if (!nodeTitle.IsNull())
	{
		CString strTitle = nodeTitle.GetAttribute(_T("text"));
		CString strRow = nodeTitle.GetAttribute(_T("row"));
		int nRow = _wtoi(strRow);
		CString strCol = nodeTitle.GetAttribute(_T("col"));
		int nCol = _wtoi(strCol);
		excel->AddCell(strTitle,nCol,nRow);
	}
	//加入项目1-12
	for (int i = 1; i <= 12; i++)
	{
		CString strItem;
		strItem.Format(_T("Item%d"), i);
		CXmlNode nodeItem;
		nodeItem = m_xml.GetRoot()->GetChild(_T("P1"))->GetChild(strItem);
		if (!nodeItem.IsNull())
		{
			CString strText = nodeItem.GetAttribute(_T("text"));
			CString strRow = nodeItem.GetAttribute(_T("row"));
			int nRow = _wtoi(strRow);
			CString strCol = nodeItem.GetAttribute(_T("col"));
			int nCol = _wtoi(strCol);
			excel->AddCell(strText,nCol,nRow);
		}
	}
	//excel->AddHeaders(sampleArray,TRUE);
	excel->Commit();
}

void CPropStep6::WriteToExcel(const CString& fileName, int nSheetIndex,int SheetCount)
{
	BOOL bSucceed = m_xml.Open(m_strXmlFile);
	if (!bSucceed)
		return;
	//初始化Excel
	
	bSucceed = m_ExcelUtil.InitExcel(fileName, nSheetIndex,SheetCount);

	if (!bSucceed)
	{
		m_ExcelUtil.ExitExcel();
		return;
	}	

	int nPrint = _wtoi(g_sorFileArray.reportConfig.Print);
	if (!nPrint) //总表
	{
		ExportReport();
	}
	else //迹线波形
	{
		//波形
		int nCurveNumber = MAIN->m_pReportFormView->m_ReportViewDlg.m_DrawButton.m_nCurveNumber;
		if (nCurveNumber == 0)
			Export1Curve();  //1 curve per page
		else if (nCurveNumber == 1)
			Export2Curve();  //2 curve per page
		else if (nCurveNumber == 2)
			Export4Curve();  //4 curve per page
		else if (nCurveNumber == 3)
			Export6Curve();   //6 curve per page
		else if (nCurveNumber == 4)
			Export8Curve();   //8 curve per page
	}
    if (_T("9.0")==m_ExcelUtil.str_Office_Edition||_T("10.0")==m_ExcelUtil.str_Office_Edition||_T("11.0")==m_ExcelUtil.str_Office_Edition)
    {
		m_ExcelUtil.Save();
	}
	if (_T("12.0")==m_ExcelUtil.str_Office_Edition||_T("14.0")==m_ExcelUtil.str_Office_Edition)
	{
		m_ExcelUtil.Save(fileName+_T("x"));//wcq2012.10.31 zll 2013.7.8 office2007版后缀保存为.xlsx格式文件
		DeleteFile(fileName);//zll 2013.7.8 会多生成对应的fileName.xls后缀文件，在此需删除，只显示fileName.xlsx文件
	}
	else//以前没加 wcq2016.03.03
	{
		INT nVersion = _ttoi(m_ExcelUtil.str_Office_Edition);
		if (nVersion < 9)//低版本
		{
			m_ExcelUtil.Save();
		}
		else//高版本
		{
			m_ExcelUtil.Save(fileName+_T("x"));//wcq2012.10.31 zll 2013.7.8 office2007版后缀保存为.xlsx格式文件
			DeleteFile(fileName);//zll 2013.7.8 会多生成对应的fileName.xls后缀文件，在此需删除，只显示fileName.xlsx文件
		}//eif
	}//eif
	//TRACE(_T("zll2 %d"),2);

	m_ExcelUtil.ExitExcel();
}

void CPropStep6::ExportReport() //总表
{
	// 加入标题
	CXmlNode nodeTitle;
	nodeTitle = m_xml.GetRoot()->GetChild(_T("Title"));
	if (!nodeTitle.IsNull())
	{
		CString strTitle = nodeTitle.GetAttribute(_T("text"));
		CString strRow = nodeTitle.GetAttribute(_T("row"));
		int nRow = _wtoi(strRow);
		CString strCol = nodeTitle.GetAttribute(_T("col"));
		int nCol = _wtoi(strCol);
		m_ExcelUtil.InsertItem(nRow, nCol, strTitle);
	}
	
	//加入项目1-12
	for (int i = 1; i <= 12; i++)
	{
		CString strItem;
		strItem.Format(_T("Item%d"), i);
		CXmlNode nodeItem;
		nodeItem = m_xml.GetRoot()->GetChild(strItem);
		if (!nodeItem.IsNull())
		{
			CString strText = nodeItem.GetAttribute(_T("text"));
			CString strRow = nodeItem.GetAttribute(_T("row"));
			int nRow = _wtoi(strRow);
			CString strCol = nodeItem.GetAttribute(_T("col"));
			int nCol = _wtoi(strCol);
			m_ExcelUtil.InsertItem(nRow, nCol, strText);
		}
	}
	////加入迹线
	//for (int i = 1; i <= nWaveNumber; i++)
	//{
	//	//判断已经加载曲线的数量
	//	if (i > g_sorFileArray.totalCurveNumber)
	//		return;

	//	CString strTrace;
	//	strTrace.Format(_T("Trace%d"), i);
	//	CXmlNode nodeTrace;
	//	nodeTrace = m_xml.GetRoot()->GetChild(strPageName)->GetChild(_T("Traces"))->GetChild(strTrace);
	//	if (!nodeTrace.IsNull())
	//	{
	//		CString strBmpFile;
	//		strBmpFile.Format(_T("%s\\%s"), CUtil::getExePath(), nodeTrace.GetAttribute(_T("file")));
	//		CString strPos = nodeTrace.GetAttribute(_T("fromcell"));
	//		if (CUtil::isFileExist(strBmpFile)) //位图文件存在
	//			m_ExcelUtil.InsertPicture(strBmpFile, strPos);
	//	}

	//	//添加迹线标题项目1-3
	//	for (int i = 1; i <= 3; i++)
	//	{
	//		CString strItem;
	//		strItem.Format(_T("Item%d"), i);
	//		CXmlNode nodeItem;
	//		nodeItem = nodeTrace.GetChild(strItem);
	//		if (!nodeItem.IsNull())
	//		{
	//			CString strText = nodeItem.GetAttribute(_T("text"));
	//			CString strRow = nodeItem.GetAttribute(_T("row"));
	//			int nRow = _wtoi(strRow);
	//			CString strCol = nodeItem.GetAttribute(_T("col"));
	//			int nCol = _wtoi(strCol);
	//			m_ExcelUtil.InsertItem(nRow, nCol, strText);
	//		}
	//	}
	//}

	////写入事件列表
	//if (nWaveNumber == 1)
	//{

	//}
}

void CPropStep6::Export1Curve() //每页1个波形
{
	WriteDataToExcel(1, TRUE);  //新报表
}

void CPropStep6::Export2Curve() //每页2个波形
{
	//WriteDataToExcel(2);
	WriteDataToExcel(2, TRUE);  //新报表
}

void CPropStep6::Export4Curve() //每页4个波形
{
	WriteDataToExcel(4, TRUE);
}

void CPropStep6::Export6Curve() //每页6个波形
{
	WriteDataToExcel(6, TRUE);
}

void CPropStep6::Export8Curve() //每页8个波形
{
	WriteDataToExcel(8, TRUE);
}

void CPropStep6::WriteDataToExcel(int nWaveNumber) //每页nWaveNumber个波形
{
	//加入迹线
	//for (int i = 1; i <= nWaveNumber; i++) //zll 2012.9.24注释
	int SheetCount=1;
	for (int i = 1; i <= g_sorFileArray.totalCurveNumber; i++)//zll 2012.9.24 可导出大于8条的数据
	{
		int wave_Count=i;
		//判断已经加载曲线的数量
		/*if (i > g_sorFileArray.totalCurveNumber)
			return;*/
		if (i-nWaveNumber>0 && i-nWaveNumber*SheetCount>0)
		{
			SheetCount++;
		}
		CString strPageName;
		strPageName.Format(_T("P%d"), nWaveNumber);

		CString strTrace;
		strTrace.Format(_T("Trace%d"), i-(SheetCount-1)*nWaveNumber);//改为下面的 cwcq2012.11.05
		//strTrace.Format(_T("trace%d"), i-(SheetCount-1)*nWaveNumber);//wcq2012.11.05 trace
		CXmlNode nodeTrace;
		nodeTrace = m_xml.GetRoot()->GetChild(strPageName)->GetChild(_T("Traces"))->GetChild(strTrace);
		if (!nodeTrace.IsNull())
		{
			//strTrace.Format(_T("Trace%d.bmp"), i);//改为下面的 cwcq2012.11.05
			strTrace.Format(_T("trace%d.bmp"), i);//wcq2012.11.05
			CString strBmpFile;
			strBmpFile.Format(_T("%s\\%s"), CUtil::getExePath(), strTrace);
			//strBmpFile.Format(_T("%s\\%s"), CUtil::getExePath(), nodeTrace.GetAttribute(_T("file")));//zll注释 2012.9.26
			CString strPos = nodeTrace.GetAttribute(_T("fromcell"));
			//AfxMessageBox(strBmpFile);//wcq2012.11.05
			
			if (CUtil::isFileExist(strBmpFile)) //位图文件存在
			{
				m_ExcelUtil.InsertPicture(strBmpFile, strPos,SheetCount,0,0,1);//zll 2013.5.28 参数iBorder赋值，使得输出excel时曲线有边框，左边不空
				//m_ExcelUtil.InsertPicture(strBmpFile, _T("A38"),SheetCount);//临时测试一下 wcq2012.11.05???????????????????
			}
		}

		// 加入标题
		CXmlNode nodeTitle;
		nodeTitle = m_xml.GetRoot()->GetChild(strPageName)->GetChild(_T("Title"));
		if (!nodeTitle.IsNull())
		{
			//CString strTitle = nodeTitle.GetAttribute(_T("text"));
			CString strTitle = g_sorFileArray.reportConfig.Title;
			CString strRow = nodeTitle.GetAttribute(_T("row"));
			int nRow = _wtoi(strRow);
			CString strCol = nodeTitle.GetAttribute(_T("col"));
			int nCol = _wtoi(strCol);
			m_ExcelUtil.InsertItem(nRow, nCol, strTitle);
		}

		if (nWaveNumber != 8)//每页8个波形时不写入此数据
		{
			//加入项目1-12
			for (int i = 1; i <= 12; i++)
			{
				CString strItem;
				strItem.Format(_T("Item%d"), i);
				CXmlNode nodeItem;
				nodeItem = m_xml.GetRoot()->GetChild(strPageName)->GetChild(strItem);
				if (!nodeItem.IsNull())
				{
					CString strText = nodeItem.GetAttribute(_T("text"));
					CString strTextVal;
					strTextVal.Format(_T("%s:  %s"),strText,GetItemValById(i));
					CString strRow = nodeItem.GetAttribute(_T("row"));
					int nRow = _wtoi(strRow);
					CString strCol = nodeItem.GetAttribute(_T("col"));
					int nCol = _wtoi(strCol);
					m_ExcelUtil.InsertItem(nRow, nCol, strTextVal);
				}
			}


			//写入事件列表 zll 2013.5.24
			WriteEventList(nWaveNumber,i);
		}

		int filePathNameLength=g_sorFileArray.sorFileList[i-1].filePathName.GetLength();
		int fileNameLength=g_sorFileArray.sorFileList[i-1].fileName.GetLength();
        CString sorName=g_sorFileArray.sorFileList[i-1].fileName.Mid(filePathNameLength+1,fileNameLength-(filePathNameLength+1));
        SetSorName(nWaveNumber,i,sorName);//设置.sor曲线文件名
		SetVerUnitText(nWaveNumber,i);//设置纵坐标刻度文本
		SetHorUnitText(nWaveNumber,i);//设置横坐标刻度文本

		//添加迹线标题项目1-3
		//for (int i = 1; i <= 3; i++)
		for (int i = 2; i < 3; i++)
		{
			CString strItem;
			strItem.Format(_T("Item%d"), i);
			CXmlNode nodeItem;
			nodeItem = nodeTrace.GetChild(strItem);
			if (!nodeItem.IsNull())
			{
				CString strText = nodeItem.GetAttribute(_T("text"));
				CString str_wave_Count=_T("");
				str_wave_Count.Format(_T("%d"),wave_Count);
				strText+=_T(":")+str_wave_Count;
				CString strRow = nodeItem.GetAttribute(_T("row"));
				int nRow = _wtoi(strRow);
				CString strCol = nodeItem.GetAttribute(_T("col"));
				int nCol = _wtoi(strCol);
				m_ExcelUtil.InsertItem(nRow, nCol, strText);
			}
		}
		m_progress.SetPos(i);
		TRACE(_T("zll zll %d\n"),i);
	}
}

//新报表所用的Excel输出， lzy 2014.8.22
void CPropStep6::WriteDataToExcel(int nWaveNumber, BOOL bNewRpt) //每页nWaveNumber个波形, bNewRpt=1表示新报表
{
	if(!bNewRpt)//旧报表直接调用老的函数
	{
		WriteDataToExcel(nWaveNumber);
		return;
	}

	int nLineOffset = 60;  //Excel表中相邻报表的行数偏差值
	
	//MAIN->m_pPrintView->GetPrintInfo(g_sorFileArray.curSelection);  //获取打印的一些信息
	
	//加入迹线
	//for (int i = 1; i <= nWaveNumber; i++) //zll 2012.9.24注释
	//int nCurveCount = g_sorFileArray.totalCurveNumber;    //lzy 2014.9.4
	int SheetCount=1;
	int j = 0;   //本页上第几个图
	for (int i = 1; i <= g_sorFileArray.totalCurveNumber; i++)//zll 2012.9.24 可导出大于8条的数据
	{
		if(1==nWaveNumber && i!=g_sorFileArray.curSelection+1) //单报表的话，只打印当前的选择的文件。lzy2014.9.3
			continue;
		
		if(1==nWaveNumber)
			MAIN->m_pPrintView->GetPrintInfo(g_sorFileArray.curSelection);  //获取打印的一些信息
		else
			MAIN->m_pPrintView->GetPrintInfo( i - 1 );  //获取打印的一些信息

		int wave_Count=i;
		//判断已经加载曲线的数量
		/*if (i > g_sorFileArray.totalCurveNumber)
			return;*/
		if (i-nWaveNumber>0 && i-nWaveNumber*SheetCount>0)
		{
			SheetCount++;
		}
		CString strPageName;
		strPageName.Format(_T("P%d"), nWaveNumber);

		CString strTrace;
		//strTrace.Format(_T("Trace%d"), i-(SheetCount-1)*nWaveNumber);//改为下面的 cwcq2012.11.05
		strTrace.Format(_T("Trace1"));  //, i-(SheetCount-1)*nWaveNumber);  //固定为Trace1, 因为每个子报表内容都相同。lzy2014.9.1

		j = (i-1) % nWaveNumber;//两个图之间的偏移量
		
		CXmlNode nodeTrace;
		nodeTrace = m_xml.GetRoot()->GetChild(strPageName)->GetChild(_T("Traces"))->GetChild(strTrace);
		if (!nodeTrace.IsNull())
		{
			//strTrace.Format(_T("Trace%d.bmp"), i);//改为下面的 cwcq2012.11.05
			strTrace.Format(_T("trace%d.bmp"), i);//wcq2012.11.05
			CString strBmpFile;
			strBmpFile.Format(_T("%s\\%s"), CUtil::getExePath(), strTrace);
			//strBmpFile.Format(_T("%s\\%s"), CUtil::getExePath(), nodeTrace.GetAttribute(_T("file")));//zll注释 2012.9.26
			CString strPos = nodeTrace.GetAttribute(_T("fromcell"));
			//AfxMessageBox(strBmpFile);//wcq2012.11.05
			if(i > 1)  //lzy2014.9.3
			{
				CString str = strPos.Right(2);
				int nPos = _wtoi(str) +  nLineOffset * j ; //*(i - 1) 
				strPos.Format(_T("A%d"), nPos);
			}

			if (CUtil::isFileExist(strBmpFile)) //位图文件存在
			{
				m_ExcelUtil.InsertPicture(strBmpFile, strPos,SheetCount,0,0,1);//zll 2013.5.28 参数iBorder赋值，使得输出excel时曲线有边框，左边不空
				//m_ExcelUtil.InsertPicture(strBmpFile, _T("A38"),SheetCount);//临时测试一下 wcq2012.11.05???????????????????
			}
		}

		// 加入标题
		CXmlNode nodeTitle, nodeItem, nodeSub;
		CString strItem, strHead, strTitle, strRow, strCol; 
		nodeTitle = m_xml.GetRoot()->GetChild(strPageName)->GetChild(_T("Title"));
		
		if (!nodeTitle.IsNull())
		{
			//CString strTitle = nodeTitle.GetAttribute(_T("text"));
			strTitle = g_sorFileArray.reportConfig.Title; 
			strRow = nodeTitle.GetAttribute(_T("row"));
			int nRow = _wtoi(strRow) + nLineOffset * j; //( i - 1 ) ;
			strCol = nodeTitle.GetAttribute(_T("col"));
			int nCol = _wtoi(strCol);
			m_ExcelUtil.InsertItem(nRow, nCol, strTitle);

			nodeItem = nodeTitle.GetChild(_T("Logo"));
			strItem = nodeItem.GetAttribute(_T("text"));
			strRow = nodeItem.GetAttribute(_T("row"));
			nRow = _wtoi(strRow) + nLineOffset * j; //( i - 1 );
			strCol = nodeItem.GetAttribute(_T("col"));
			nCol = _wtoi(strCol);
			m_ExcelUtil.InsertItem(nRow, nCol, strItem);

			nodeItem = nodeTitle.GetChild(_T("PrintDate"));
			strHead = nodeItem.GetAttribute(_T("text"));
			CTime tm = CTime::GetCurrentTime();
			CString strTime = tm.Format(_T("%Y-%m-%d %H:%M:%S"));
			strItem = strHead + _T(": ") + strTime;
			strRow = nodeItem.GetAttribute(_T("row"));
			nRow = _wtoi(strRow) + nLineOffset * j; //( i - 1 );
			strCol = nodeItem.GetAttribute(_T("col"));
			nCol = _wtoi(strCol);
			m_ExcelUtil.InsertItem(nRow, nCol, strItem);
		}

		//加入Job info
		nodeTitle = m_xml.GetRoot()->GetChild(strPageName)->GetChild(_T("Job"));
		if (!nodeTitle.IsNull())
		{
			strTitle = _T("<") + nodeTitle.GetAttribute(_T("text")) + _T(">");   //插入JOB标题 
			strRow = nodeTitle.GetAttribute(_T("row"));
			int nRow = _wtoi(strRow) + nLineOffset * j; //( i - 1 );
			strCol = nodeTitle.GetAttribute(_T("col"));
			int nCol = _wtoi(strCol);
			m_ExcelUtil.InsertItem(nRow, nCol, strTitle);

			nodeItem = nodeTitle.GetChild(_T("JobID"));  //插入任务编号
			strItem = nodeItem.GetAttribute(_T("text")) + _T(":") + g_sorFileArray.reportConfig.Job.JobID;
			strRow = nodeItem.GetAttribute(_T("row"));
			nRow = _wtoi(strRow) + nLineOffset * j;//( i - 1 );
			strCol = nodeItem.GetAttribute(_T("col"));
			nCol = _wtoi(strCol);
			m_ExcelUtil.InsertItem(nRow, nCol, strItem);

			nodeItem = nodeTitle.GetChild(_T("File"));  //插入sor文件名
			strItem = nodeItem.GetAttribute(_T("text")) + _T(":") + g_sorFileArray.reportConfig.Job.SorFileName;
			strRow = nodeItem.GetAttribute(_T("row"));
			nRow = _wtoi(strRow) + nLineOffset * j;//( i - 1 );
			strCol = nodeItem.GetAttribute(_T("col"));
			nCol = _wtoi(strCol);
			m_ExcelUtil.InsertItem(nRow, nCol, strItem);

			nodeItem = nodeTitle.GetChild(_T("Date"));  //插入测量时间
			strItem = nodeItem.GetAttribute(_T("text")) + _T(":") + g_sorFileArray.reportConfig.Job.TestDate;
			strRow = nodeItem.GetAttribute(_T("row"));
			nRow = _wtoi(strRow) + nLineOffset * j;//( i - 1 );
			strCol = nodeItem.GetAttribute(_T("col"));
			nCol = _wtoi(strCol);
			m_ExcelUtil.InsertItem(nRow, nCol, strItem);

			nodeItem = nodeTitle.GetChild(_T("Contractor"));  //插入承包商
			strItem = nodeItem.GetAttribute(_T("text")) + _T(":") + g_sorFileArray.reportConfig.Job.Contractor;
			strRow = nodeItem.GetAttribute(_T("row"));
			nRow = _wtoi(strRow) + nLineOffset * j;//( i - 1 );
			strCol = nodeItem.GetAttribute(_T("col"));
			nCol = _wtoi(strCol);
			m_ExcelUtil.InsertItem(nRow, nCol, strItem);

			nodeItem = nodeTitle.GetChild(_T("Customer"));  //插入用户
			strItem = nodeItem.GetAttribute(_T("text")) + _T(":") + g_sorFileArray.reportConfig.Job.Customer;
			strRow = nodeItem.GetAttribute(_T("row"));
			nRow = _wtoi(strRow) + nLineOffset * j;//( i - 1 );
			strCol = nodeItem.GetAttribute(_T("col"));
			nCol = _wtoi(strCol);
			m_ExcelUtil.InsertItem(nRow, nCol, strItem);

			nodeItem = nodeTitle.GetChild(_T("Operator"));  //插入操作者
			strItem = nodeItem.GetAttribute(_T("text")) + _T(":") + g_sorFileArray.reportConfig.Job.Operator;
			strRow = nodeItem.GetAttribute(_T("row"));
			nRow = _wtoi(strRow) + nLineOffset * j;//( i - 1 );
			strCol = nodeItem.GetAttribute(_T("col"));
			nCol = _wtoi(strCol);
			m_ExcelUtil.InsertItem(nRow, nCol, strItem);			
		}

		//加入Configuration info
		nodeTitle = m_xml.GetRoot()->GetChild(strPageName)->GetChild(_T("Configuration"));
		if (!nodeTitle.IsNull())
		{
			strTitle = _T("<") + nodeTitle.GetAttribute(_T("text")) + _T(">");   //插入Configuration标题 
			strRow = nodeTitle.GetAttribute(_T("row"));
			int nRow = _wtoi(strRow) + nLineOffset * j;//( i - 1 );
			strCol = nodeTitle.GetAttribute(_T("col"));
			int nCol = _wtoi(strCol);
			m_ExcelUtil.InsertItem(nRow, nCol, strTitle);

			nodeItem = nodeTitle.GetChild(_T("Wavelength"));  //插入波长
			strItem = nodeItem.GetAttribute(_T("text")) + _T(":") + g_sorFileArray.reportConfig.Configuration.Wavelength;
			strRow = nodeItem.GetAttribute(_T("row"));
			nRow = _wtoi(strRow) + nLineOffset * j;//( i - 1 );
			strCol = nodeItem.GetAttribute(_T("col"));
			nCol = _wtoi(strCol);
			m_ExcelUtil.InsertItem(nRow, nCol, strItem);

			nodeItem = nodeTitle.GetChild(_T("Range"));  //插入距离
			strItem = nodeItem.GetAttribute(_T("text")) + _T(":") + g_sorFileArray.reportConfig.Configuration.Range;
			strRow = nodeItem.GetAttribute(_T("row"));
			nRow = _wtoi(strRow) + nLineOffset * j;//( i - 1 );
			strCol = nodeItem.GetAttribute(_T("col"));
			nCol = _wtoi(strCol);
			m_ExcelUtil.InsertItem(nRow, nCol, strItem);

			nodeItem = nodeTitle.GetChild(_T("Backscatter"));  //背向散射系数
			strItem = nodeItem.GetAttribute(_T("text")) + _T(":") + g_sorFileArray.reportConfig.Configuration.BackScatter;
			strRow = nodeItem.GetAttribute(_T("row"));
			nRow = _wtoi(strRow) + nLineOffset * j;//( i - 1 );
			strCol = nodeItem.GetAttribute(_T("col"));
			nCol = _wtoi(strCol);
			m_ExcelUtil.InsertItem(nRow, nCol, strItem);

			nodeItem = nodeTitle.GetChild(_T("Pulse"));  //脉宽
			strItem = nodeItem.GetAttribute(_T("text")) + _T(":") + g_sorFileArray.reportConfig.Configuration.Pulse;
			strRow = nodeItem.GetAttribute(_T("row"));
			nRow = _wtoi(strRow) + nLineOffset * j;//( i - 1 );
			strCol = nodeItem.GetAttribute(_T("col"));
			nCol = _wtoi(strCol);
			m_ExcelUtil.InsertItem(nRow, nCol, strItem);

			nodeItem = nodeTitle.GetChild(_T("Average"));  //平均
			strItem = nodeItem.GetAttribute(_T("text")) + _T(":") + g_sorFileArray.reportConfig.Configuration.Average;
			strRow = nodeItem.GetAttribute(_T("row"));
			nRow = _wtoi(strRow) + nLineOffset * j;//( i - 1 );
			strCol = nodeItem.GetAttribute(_T("col"));
			nCol = _wtoi(strCol);
			m_ExcelUtil.InsertItem(nRow, nCol, strItem);

			nodeItem = nodeTitle.GetChild(_T("LossThreshold"));  //损耗门限
			strItem = nodeItem.GetAttribute(_T("text")) + _T(":") + g_sorFileArray.reportConfig.Configuration.LossThreshold;
			strRow = nodeItem.GetAttribute(_T("row"));
			nRow = _wtoi(strRow) + nLineOffset * j;//( i - 1 );
			strCol = nodeItem.GetAttribute(_T("col"));
			nCol = _wtoi(strCol);
			m_ExcelUtil.InsertItem(nRow, nCol, strItem);

			nodeItem =  nodeTitle.GetChild(_T("Origin")) ;  //起点
			strItem = _T("---") + nodeItem.GetAttribute(_T("text")) + _T("---") ;
			strRow = nodeItem.GetAttribute(_T("row"));
			nRow = _wtoi(strRow) + nLineOffset * j;//( i - 1 );
			strCol = nodeItem.GetAttribute(_T("col"));
			nCol = _wtoi(strCol);
			m_ExcelUtil.InsertItem(nRow, nCol, strItem);

			nodeItem =  nodeTitle.GetChild(_T("End")) ;  //终点
			strItem = _T("---") + nodeItem.GetAttribute(_T("text")) + _T("---");
			strRow = nodeItem.GetAttribute(_T("row"));
			nRow = _wtoi(strRow) + nLineOffset * j;//( i - 1 );
			strCol = nodeItem.GetAttribute(_T("col"));
			nCol = _wtoi(strCol);
			m_ExcelUtil.InsertItem(nRow, nCol, strItem);

			nodeItem = nodeTitle.GetChild(_T("ReflectThreshold"));  //反射门限
			strItem = nodeItem.GetAttribute(_T("text")) + _T(":") + g_sorFileArray.reportConfig.Configuration.RefThreshold;
			strRow = nodeItem.GetAttribute(_T("row"));
			nRow = _wtoi(strRow) + nLineOffset * j;//( i - 1 );
			strCol = nodeItem.GetAttribute(_T("col"));
			nCol = _wtoi(strCol);
			m_ExcelUtil.InsertItem(nRow, nCol, strItem);

			nodeItem = nodeTitle.GetChild(_T("OriginLocation"));  //起点位置
			strItem = nodeItem.GetAttribute(_T("text")) + _T(":") + g_sorFileArray.reportConfig.Configuration.OriLocation;
			strRow = nodeItem.GetAttribute(_T("row"));
			nRow = _wtoi(strRow) + nLineOffset * j;//( i - 1 );
			strCol = nodeItem.GetAttribute(_T("col"));
			nCol = _wtoi(strCol);
			if(nWaveNumber <= 1)
				m_ExcelUtil.InsertItem(nRow, nCol, strItem);

			nodeItem = nodeTitle.GetChild(_T("EndLocation"));  //终点位置
			strItem = nodeItem.GetAttribute(_T("text")) + _T(":") + g_sorFileArray.reportConfig.Configuration.EndLocation;
			strRow = nodeItem.GetAttribute(_T("row"));
			nRow = _wtoi(strRow) + nLineOffset * j;//( i - 1 );
			strCol = nodeItem.GetAttribute(_T("col"));
			nCol = _wtoi(strCol);
			if(nWaveNumber <= 1)
				m_ExcelUtil.InsertItem(nRow, nCol, strItem);

			nodeItem = nodeTitle.GetChild(_T("EndThreshold"));  //光纤末端门限
			strItem = nodeItem.GetAttribute(_T("text")) + _T(":") + g_sorFileArray.reportConfig.Configuration.EndThreshold;
			strRow = nodeItem.GetAttribute(_T("row"));
			nRow = _wtoi(strRow) + nLineOffset * j;//( i - 1 );
			strCol = nodeItem.GetAttribute(_T("col"));
			nCol = _wtoi(strCol);
			m_ExcelUtil.InsertItem(nRow, nCol, strItem);

			nodeItem = nodeTitle.GetChild(_T("OriginCable"));  //起点光缆编号
			strItem = nodeItem.GetAttribute(_T("text")) + _T(":") + g_sorFileArray.reportConfig.Configuration.OriCable;
			strRow = nodeItem.GetAttribute(_T("row"));
			nRow = _wtoi(strRow) + nLineOffset * j;//( i - 1 );
			strCol = nodeItem.GetAttribute(_T("col"));
			nCol = _wtoi(strCol);
			if(nWaveNumber <= 1)
				m_ExcelUtil.InsertItem(nRow, nCol, strItem);

			nodeItem = nodeTitle.GetChild(_T("EndCable"));  //终点光缆编号
			strItem = nodeItem.GetAttribute(_T("text")) + _T(":") + g_sorFileArray.reportConfig.Configuration.EndCable;
			strRow = nodeItem.GetAttribute(_T("row"));
			nRow = _wtoi(strRow) + nLineOffset * j;//( i - 1 );
			strCol = nodeItem.GetAttribute(_T("col"));
			nCol = _wtoi(strCol);
			if(nWaveNumber <= 1)
				m_ExcelUtil.InsertItem(nRow, nCol, strItem);

			nodeItem = nodeTitle.GetChild(_T("Index"));  //折射率
			strItem = nodeItem.GetAttribute(_T("text")) + _T(":") + g_sorFileArray.reportConfig.Configuration.Index;
			strRow = nodeItem.GetAttribute(_T("row"));
			nRow = _wtoi(strRow) + nLineOffset * j;//( i - 1 );
			strCol = nodeItem.GetAttribute(_T("col"));
			nCol = _wtoi(strCol);
			m_ExcelUtil.InsertItem(nRow, nCol, strItem);

			nodeItem = nodeTitle.GetChild(_T("OriginFiber"));  //起点光纤编号
			strItem = nodeItem.GetAttribute(_T("text")) + _T(":") + g_sorFileArray.reportConfig.Configuration.OriFiber;
			strRow = nodeItem.GetAttribute(_T("row"));
			nRow = _wtoi(strRow) + nLineOffset * j;//( i - 1 );
			strCol = nodeItem.GetAttribute(_T("col"));
			nCol = _wtoi(strCol);
			if(nWaveNumber <= 1)
				m_ExcelUtil.InsertItem(nRow, nCol, strItem);

			nodeItem = nodeTitle.GetChild(_T("EndFiber"));  //终点光纤编号
			strItem = nodeItem.GetAttribute(_T("text")) + _T(":") + g_sorFileArray.reportConfig.Configuration.EndFiber;
			strRow = nodeItem.GetAttribute(_T("row"));
			nRow = _wtoi(strRow) + nLineOffset * j;//( i - 1 );
			strCol = nodeItem.GetAttribute(_T("col"));
			nCol = _wtoi(strCol);
			if(nWaveNumber <= 1)
				m_ExcelUtil.InsertItem(nRow, nCol, strItem);

			nodeItem = nodeTitle.GetChild(_T("OriginColor"));  //起点光纤颜色
			strItem = nodeItem.GetAttribute(_T("text")) + _T(":") + g_sorFileArray.reportConfig.Configuration.OriColor;
			strRow = nodeItem.GetAttribute(_T("row"));
			nRow = _wtoi(strRow) + nLineOffset * j;//( i - 1 );
			strCol = nodeItem.GetAttribute(_T("col"));
			nCol = _wtoi(strCol);
			if(nWaveNumber <= 1)
				m_ExcelUtil.InsertItem(nRow, nCol, strItem);

			nodeItem = nodeTitle.GetChild(_T("EndColor"));  //终点光纤颜色
			strItem = nodeItem.GetAttribute(_T("text")) + _T(":") + g_sorFileArray.reportConfig.Configuration.EndColor;
			strRow = nodeItem.GetAttribute(_T("row"));
			nRow = _wtoi(strRow) + nLineOffset * j;//( i - 1 );
			strCol = nodeItem.GetAttribute(_T("col"));
			nCol = _wtoi(strCol);
			if(nWaveNumber <= 1)
				m_ExcelUtil.InsertItem(nRow, nCol, strItem);

		}

		//添加Comment
		nodeTitle = m_xml.GetRoot()->GetChild(strPageName)->GetChild(_T("Comment"));
		if (!nodeTitle.IsNull())
		{
			strTitle = _T("<") + nodeTitle.GetAttribute(_T("text")) + _T(">");   //插入Comment标题 
			strRow = nodeTitle.GetAttribute(_T("row"));
			int nRow = _wtoi(strRow) + nLineOffset * j;//( i - 1 );
			strCol = nodeTitle.GetAttribute(_T("col"));
			int nCol = _wtoi(strCol);
			m_ExcelUtil.InsertItem(nRow, nCol, strTitle);

			nodeItem = nodeTitle.GetChild(_T("CommentContent"));  //插入注释内容
			strItem =  g_sorFileArray.reportConfig.Comment;
			strRow = nodeItem.GetAttribute(_T("row"));
			nRow = _wtoi(strRow) + nLineOffset * j;//( i - 1 );
			strCol = nodeItem.GetAttribute(_T("col"));
			nCol = _wtoi(strCol);
			if(nWaveNumber <= 1)
				m_ExcelUtil.InsertItem(nRow, nCol, strItem);
		}

		//添加Trace图
		nodeTitle = m_xml.GetRoot()->GetChild(strPageName)->GetChild(_T("Traces"));
		if (!nodeTitle.IsNull())
		{
			strTitle = _T("<") + nodeTitle.GetAttribute(_T("text")) + _T(">");   //插入Trace标题 
			strRow = nodeTitle.GetAttribute(_T("row"));
			int nRow = _wtoi(strRow) + nLineOffset * j;//( i - 1 );
			strCol = nodeTitle.GetAttribute(_T("col"));
			int nCol = _wtoi(strCol);
			m_ExcelUtil.InsertItem(nRow, nCol, strTitle);

			nodeItem = nodeTitle.GetChild(_T("ItemA"));  //光标A
			strItem = nodeItem.GetAttribute(_T("text"));
			strRow = nodeItem.GetAttribute(_T("row"));
			nRow = _wtoi(strRow) + nLineOffset * j;//( i - 1 );
			strCol = nodeItem.GetAttribute(_T("col"));
			nCol = _wtoi(strCol);
			m_ExcelUtil.InsertItem(nRow, nCol, strItem);

			nodeSub = nodeItem.GetChild(_T("Location")); //光标A的位置
			strItem = nodeSub.GetAttribute(_T("text")) + _T(":") + g_sorFileArray.reportConfig.Trace.LocationA;// + nodeSub.GetAttribute(_T("unit"));
			strRow = nodeSub.GetAttribute(_T("row"));
			nRow = _wtoi(strRow) + nLineOffset * j;//( i - 1 );
			strCol = nodeSub.GetAttribute(_T("col"));
			nCol = _wtoi(strCol);
			if(nWaveNumber <= 1)
				m_ExcelUtil.InsertItem(nRow, nCol, strItem);

			nodeSub = nodeItem.GetChild(_T("Loss")); //光标A的损耗值
			strItem = nodeSub.GetAttribute(_T("text")) + _T(":") + g_sorFileArray.reportConfig.Trace.LossA ; //+ nodeSub.GetAttribute(_T("unit"));
			strRow = nodeSub.GetAttribute(_T("row"));
			nRow = _wtoi(strRow) + nLineOffset * j;//( i - 1 );
			strCol = nodeSub.GetAttribute(_T("col"));
			nCol = _wtoi(strCol);
			if(nWaveNumber <= 1)
				m_ExcelUtil.InsertItem(nRow, nCol, strItem);

			nodeItem = nodeTitle.GetChild(_T("ItemB"));  //光标B
			strItem = nodeItem.GetAttribute(_T("text"));
			strRow = nodeItem.GetAttribute(_T("row"));
			nRow = _wtoi(strRow) + nLineOffset * j;//( i - 1 );
			strCol = nodeItem.GetAttribute(_T("col"));
			nCol = _wtoi(strCol);
			m_ExcelUtil.InsertItem(nRow, nCol, strItem);

			nodeSub = nodeItem.GetChild(_T("Location")); //光标B的位置
			strItem = nodeSub.GetAttribute(_T("text")) + _T(":") + g_sorFileArray.reportConfig.Trace.LocationB ; //+ nodeSub.GetAttribute(_T("unit"));
			strRow = nodeSub.GetAttribute(_T("row"));
			nRow = _wtoi(strRow) + nLineOffset * j;//( i - 1 );
			strCol = nodeSub.GetAttribute(_T("col"));
			nCol = _wtoi(strCol);
			if(nWaveNumber <= 1)
				m_ExcelUtil.InsertItem(nRow, nCol, strItem);

			nodeSub = nodeItem.GetChild(_T("Loss")); //光标B的损耗值
			strItem = nodeSub.GetAttribute(_T("text")) + _T(":") + g_sorFileArray.reportConfig.Trace.LossB ; //+ nodeSub.GetAttribute(_T("unit"));
			strRow = nodeSub.GetAttribute(_T("row"));
			nRow = _wtoi(strRow) + nLineOffset * j;//( i - 1 );
			strCol = nodeSub.GetAttribute(_T("col"));
			nCol = _wtoi(strCol);
			if(nWaveNumber <= 1)
				m_ExcelUtil.InsertItem(nRow, nCol, strItem);

			nodeItem = nodeTitle.GetChild(_T("ItemAB"));  //光标AB
			strItem = nodeItem.GetAttribute(_T("text"));
			strRow = nodeItem.GetAttribute(_T("row"));
			nRow = _wtoi(strRow) + nLineOffset * j;//( i - 1 );
			strCol = nodeItem.GetAttribute(_T("col"));
			nCol = _wtoi(strCol);
			m_ExcelUtil.InsertItem(nRow, nCol, strItem);

			nodeSub = nodeItem.GetChild(_T("DistanceAB")); //光标AB的位置
			strItem = nodeSub.GetAttribute(_T("text")) + _T(":") + g_sorFileArray.reportConfig.Trace.DistanceAB + nodeSub.GetAttribute(_T("unit"));
			strRow = nodeSub.GetAttribute(_T("row"));
			nRow = _wtoi(strRow) + nLineOffset * j;//( i - 1 );
			strCol = nodeSub.GetAttribute(_T("col"));
			nCol = _wtoi(strCol);
			if(nWaveNumber <= 1)
				m_ExcelUtil.InsertItem(nRow, nCol, strItem);

			nodeSub = nodeItem.GetChild(_T("LossAB")); //光标AB的损耗值差值
			strItem = nodeSub.GetAttribute(_T("text")) + _T(":") + g_sorFileArray.reportConfig.Trace.LossAB + nodeSub.GetAttribute(_T("unit"));
			strRow = nodeSub.GetAttribute(_T("row"));
			nRow = _wtoi(strRow) + nLineOffset * j;//( i - 1 );
			strCol = nodeSub.GetAttribute(_T("col"));
			nCol = _wtoi(strCol);
			if(nWaveNumber <= 1)
				m_ExcelUtil.InsertItem(nRow, nCol, strItem);

			nodeSub = nodeItem.GetChild(_T("SlopeAB")); //光标AB间的衰减斜率
			strItem = nodeSub.GetAttribute(_T("text")) + _T(":") + g_sorFileArray.reportConfig.Trace.SlopeAB + nodeSub.GetAttribute(_T("unit"));
			strRow = nodeSub.GetAttribute(_T("row"));
			nRow = _wtoi(strRow) + nLineOffset * j;//( i - 1 );
			strCol = nodeSub.GetAttribute(_T("col"));
			nCol = _wtoi(strCol);
			if(nWaveNumber <= 1)
				m_ExcelUtil.InsertItem(nRow, nCol, strItem);

		}

		
		//写入事件列表 zll 2013.5.24
		nodeTitle = m_xml.GetRoot()->GetChild(strPageName)->GetChild(_T("EventList"));
		if (!nodeTitle.IsNull())
		{
			nodeItem = nodeTitle.GetChild(_T("Title"));
			strTitle = _T("<") + nodeItem.GetAttribute(_T("text")) + _T(">");   //插入事件列表标题 
			strRow = nodeItem.GetAttribute(_T("row"));
			int nRow = _wtoi(strRow) + nLineOffset * j;//( i - 1 );
			strCol = nodeItem.GetAttribute(_T("col"));
			int nCol = _wtoi(strCol);
			m_ExcelUtil.InsertItem(nRow, nCol, strTitle);
			//WriteEventList(nWaveNumber,i, 41 + nLineOffset * ( i - 1 ));
			WriteEventList(1,i, 41 + nLineOffset * j);//( i - 1 ));   //第一个参数固定为1，偏移量由第三个参数决定。lzy2014.9.3
		}	
	
		m_progress.SetPos(i);
	}
}

//输出事件列表，lzy 2014.8.25 增加首行位置参数
void CPropStep6::WriteEventList(int m_nWaveNumberPerPage,int m_nCurCurve, int nRow)
{
	float preDistance=0;
	WriteEventHead(m_nWaveNumberPerPage,m_nCurCurve, nRow);//事件列表表头
	int nFirst = nRow + 1;

	TestResultInfo* pData = g_sorFileArray.sorFileList[m_nCurCurve-1].pTestResultInfo;
	if (NULL == pData)
		return;

	CString strText=_T("");
	CRect textRect;
	std::vector<EventInfo> ve = pData->EventList.vEventInfo;//速度??
	int nSize=ve.size();
	for (int i=0;i<nSize;i++)//
	{
		// 序号
		{
			//类型
			strText.Format(_T("%d"),i+1);

			if (g_currentLang == LANG::CHS)
			{
				if (0 == ve[i].nEventType)
				{
					strText += _T(" 非反射");//0代表非反射事件符号
				}
				else if (1 == ve[i].nEventType)
				{
					strText += _T(" 反射");;//1代表反射事件符号
				}
			}
			else if (g_currentLang == LANG::ENG)
			{
				if (0 == ve[i].nEventType)
				{
					strText += _T(" NoReflect");//0代表非反射事件符号
				}
				else if (1 == ve[i].nEventType)
				{
					strText += _T(" Reflect");;//1代表反射事件符号
				}
			}
			

			if (_T('S')==ve[i].cEventType)
			{
				strText+=_T("(S)");
			}
			else if (_T('E')==ve[i].cEventType)
			{
				strText+=_T("(E)");
			}
			else if (_T('A')==ve[i].cEventType)
			{
				strText+=_T("(A)");
			}
			else if (_T('F')==ve[i].cEventType)
			{
				strText+=_T("(F)");
			}
			else if (_T('O')==ve[i].cEventType)
			{
				strText+=_T("(O)");
			}

			if (1==m_nWaveNumberPerPage)
			{
				m_ExcelUtil.InsertItem(nFirst+i, 1, strText);
			}
			else if (2==m_nWaveNumberPerPage)
			{
				if (1==m_nCurCurve%m_nWaveNumberPerPage)
				{
					m_ExcelUtil.InsertItem(34+i, 1, strText);
				}
				else if (0==m_nCurCurve%m_nWaveNumberPerPage)
				{
					m_ExcelUtil.InsertItem(66+i, 1, strText);
				}
			}
			else if (4==m_nWaveNumberPerPage)
			{
				if (1==m_nCurCurve%m_nWaveNumberPerPage)
				{
					m_ExcelUtil.InsertItem(30+i, 1, strText);
				}
				else if (2==m_nCurCurve%m_nWaveNumberPerPage)
				{
					m_ExcelUtil.InsertItem(30+i, 8, strText);
				}
				else if (3==m_nCurCurve%m_nWaveNumberPerPage)
				{
					m_ExcelUtil.InsertItem(63+i, 1, strText);
				}
				else if (0==m_nCurCurve%m_nWaveNumberPerPage)
				{
					m_ExcelUtil.InsertItem(63+i, 8, strText);
				}
			}
		}

		// 距离
		{
			float m_float_zll=0.0f;
			if (0==i)
			{
				strText.Format(_T("%.3f"),0.0f);//距离
			}
			else
			{
				if(_T("m")==g_sorFileArray.waveConfig.DistUnit)//横坐标单位为km
					strText.Format(_T("%.3f"),ve[i].fLocation/1000);//距离
				else if(_T("mi")==g_sorFileArray.waveConfig.DistUnit)//横坐标单位为miles
				{
					m_float_zll=KmeterToOther(ve[i].fLocation/1000,UNIT_SHOW_MILE);
					strText.Format(_T("%.3f"),m_float_zll);//距离
				}
				else if(_T("ft")==g_sorFileArray.waveConfig.DistUnit)//横坐标单位为kft
				{
					m_float_zll=KmeterToOther(ve[i].fLocation/1000,UNIT_SHOW_KFEET);
					strText.Format(_T("%.3f"),m_float_zll);//距离
				}
			}
			if (1==m_nWaveNumberPerPage)
			{
				m_ExcelUtil.InsertItem(nFirst+i, 3, strText);
			}
			else if (2==m_nWaveNumberPerPage)
			{
				if (1==m_nCurCurve%m_nWaveNumberPerPage)
				{
					m_ExcelUtil.InsertItem(34+i, 3, strText);
				}
				else if (0==m_nCurCurve%m_nWaveNumberPerPage)
				{
					m_ExcelUtil.InsertItem(66+i, 3, strText);
				}
			}
			else if (4==m_nWaveNumberPerPage)
			{
				if (1==m_nCurCurve%m_nWaveNumberPerPage)
				{
					m_ExcelUtil.InsertItem(30+i, 2, strText);
				}
				else if (2==m_nCurCurve%m_nWaveNumberPerPage)
				{
					m_ExcelUtil.InsertItem(30+i, 9, strText);
				}
				else if (3==m_nCurCurve%m_nWaveNumberPerPage)
				{
					m_ExcelUtil.InsertItem(63+i, 2, strText);
				}
				else if (0==m_nCurCurve%m_nWaveNumberPerPage)
				{
					m_ExcelUtil.InsertItem(63+i, 9, strText);
				}
			}
			
		}
		// 段
		{
			float a,b=0.0f;
			if (0==i)
			{
				strText.Format(_T("%.3f"),0.0f);//段
			}
			else
			{
				if(_T("m")==g_sorFileArray.waveConfig.DistUnit)//横坐标单位为km
					strText.Format(_T("%.3f"),(ve[i].fLocation-preDistance)/1000);//段,两事件点间距离
				else if(_T("mi")==g_sorFileArray.waveConfig.DistUnit)//横坐标单位为miles
				{
					b=(ve[i].fLocation-preDistance)/1000;
					a=KmeterToOther(b,UNIT_SHOW_MILE);
					strText.Format(_T("%.3f"),a);//段,两事件点间距离
				}
				else if(_T("ft")==g_sorFileArray.waveConfig.DistUnit)//横坐标单位为kft
				{
					b=(ve[i].fLocation-preDistance)/1000;
					a=KmeterToOther(b,UNIT_SHOW_KFEET);
					strText.Format(_T("%.3f"),a);//段,两事件点间距离
				}
			}
			if (0==i)
			{
				preDistance = 0.0f;
			}
			else
			{
				preDistance = ve[i].fLocation;
			}

			if (1==m_nWaveNumberPerPage)
			{
				m_ExcelUtil.InsertItem(nFirst + i, 4, strText);
			}
			else if (2==m_nWaveNumberPerPage)
			{
				if (1==m_nCurCurve%m_nWaveNumberPerPage)
				{
					m_ExcelUtil.InsertItem(34+i, 4, strText);
				}
				else if (0==m_nCurCurve%m_nWaveNumberPerPage)
				{
					m_ExcelUtil.InsertItem(66+i, 4, strText);
				}
			}
			else if (4==m_nWaveNumberPerPage)
			{
				if (1==m_nCurCurve%m_nWaveNumberPerPage)
				{
					m_ExcelUtil.InsertItem(30+i, 3, strText);
				}
				else if (2==m_nCurCurve%m_nWaveNumberPerPage)
				{
					m_ExcelUtil.InsertItem(30+i, 10, strText);
				}
				else if (3==m_nCurCurve%m_nWaveNumberPerPage)
				{
					m_ExcelUtil.InsertItem(63+i, 3, strText);
				}
				else if (0==m_nCurCurve%m_nWaveNumberPerPage)
				{
					m_ExcelUtil.InsertItem(63+i, 10, strText);
				}
			}
			
		}
		// 损耗
		{
			if (0 == ve[i].fEventloss)//为0时显示"-"wcq2012.07.09
			{
				strText.Format(_T("--"));//总损耗
			}
			else
			{
				strText.Format(_T("%.3f"), ve[i].fEventloss);//损耗
			}

			if (1==m_nWaveNumberPerPage)
			{
				m_ExcelUtil.InsertItem(nFirst+i, 5, strText);
			}
			else if (2==m_nWaveNumberPerPage)
			{
				if (1==m_nCurCurve%m_nWaveNumberPerPage)
				{
					m_ExcelUtil.InsertItem(34+i, 5, strText);
				}
				else if (0==m_nCurCurve%m_nWaveNumberPerPage)
				{
					m_ExcelUtil.InsertItem(66+i, 5, strText);
				}
			}
			else if (4==m_nWaveNumberPerPage)
			{
				if (1==m_nCurCurve%m_nWaveNumberPerPage)
				{
					m_ExcelUtil.InsertItem(30+i, 4, strText);
				}
				else if (2==m_nCurCurve%m_nWaveNumberPerPage)
				{
					m_ExcelUtil.InsertItem(30+i, 11, strText);
				}
				else if (3==m_nCurCurve%m_nWaveNumberPerPage)
				{
					m_ExcelUtil.InsertItem(63+i, 4, strText);
				}
				else if (0==m_nCurCurve%m_nWaveNumberPerPage)
				{
					m_ExcelUtil.InsertItem(63+i, 11, strText);
				}
			}
			
		}
		// 总损耗
		{
			if (0 == ve[i].fTotalLoss)//为0时显示"-"wcq2012.07.09
			{
				strText.Format(_T("--"));//总损耗
			}
			else
			{
				strText.Format(_T("%.3f"),ve[i].fTotalLoss);//总损耗
			}

			if (1==m_nWaveNumberPerPage)
			{
				m_ExcelUtil.InsertItem(nFirst + i, 6, strText);
			}
			else if (2==m_nWaveNumberPerPage)
			{
				if (1==m_nCurCurve%m_nWaveNumberPerPage)
				{
					m_ExcelUtil.InsertItem(34+i, 6, strText);
				}
				else if (0==m_nCurCurve%m_nWaveNumberPerPage)
				{
					m_ExcelUtil.InsertItem(66+i, 6, strText);
				}
			}
			else if (4==m_nWaveNumberPerPage)
			{
				if (1==m_nCurCurve%m_nWaveNumberPerPage)
				{
					m_ExcelUtil.InsertItem(30+i, 5, strText);
				}
				else if (2==m_nCurCurve%m_nWaveNumberPerPage)
				{
					m_ExcelUtil.InsertItem(30+i, 12, strText);
				}
				else if (3==m_nCurCurve%m_nWaveNumberPerPage)
				{
					m_ExcelUtil.InsertItem(63+i, 5, strText);
				}
				else if (0==m_nCurCurve%m_nWaveNumberPerPage)
				{
					m_ExcelUtil.InsertItem(63+i, 12, strText);
				}
			}
		}
		// 斜率
		{
			//斜率
			if (fabs(ve[i].fLineK)>0.5)//zll 2012.10.31
			{
				strText.Format(_T("--"));
			}
			else if (0 == ve[i].fLineK)//为0时显示"-" zll 2012.10.31
			{
				strText.Format(_T("--"));
			}
			else
			{
				strText.Format(_T("%.3f"),ve[i].fLineK);//斜率
			}

			if (1==m_nWaveNumberPerPage)
			{
				m_ExcelUtil.InsertItem(nFirst + i, 7, strText);
			}
			else if (2==m_nWaveNumberPerPage)
			{
				if (1==m_nCurCurve%m_nWaveNumberPerPage)
				{
					m_ExcelUtil.InsertItem(34+i, 7, strText);
				}
				else if (0==m_nCurCurve%m_nWaveNumberPerPage)
				{
					m_ExcelUtil.InsertItem(66+i, 7, strText);
				}
			}
			else if (4==m_nWaveNumberPerPage)
			{
				if (1==m_nCurCurve%m_nWaveNumberPerPage)
				{
					m_ExcelUtil.InsertItem(30+i, 6, strText);
				}
				else if (2==m_nCurCurve%m_nWaveNumberPerPage)
				{
					m_ExcelUtil.InsertItem(30+i, 13, strText);
				}
				else if (3==m_nCurCurve%m_nWaveNumberPerPage)
				{
					m_ExcelUtil.InsertItem(63+i, 6, strText);
				}
				else if (0==m_nCurCurve%m_nWaveNumberPerPage)
				{
					m_ExcelUtil.InsertItem(63+i, 13, strText);
				}
			}
		}
		// 反射
		{
			if (0 == ve[i].nEventType)//非反射
			{
				strText=_T("--");
			}
			else if (0 == ve[i].fEventHigh)//为0时显示"-"wcq2012.07.09
			{
				strText=_T("--");
			}
			else
			{
				strText.Format(_T("%.3f"),ve[i].fEventHigh);//反射
			}
			if (1==m_nWaveNumberPerPage)
			{
				m_ExcelUtil.InsertItem(nFirst + i, 8, strText);
			}
			else if (2==m_nWaveNumberPerPage)
			{
				if (1==m_nCurCurve%m_nWaveNumberPerPage)
				{
					m_ExcelUtil.InsertItem(34+i, 8, strText);
				}
				else if (0==m_nCurCurve%m_nWaveNumberPerPage)
				{
					m_ExcelUtil.InsertItem(66+i, 8, strText);
				}
			}
			else if (4==m_nWaveNumberPerPage)
			{
				if (1==m_nCurCurve%m_nWaveNumberPerPage)
				{
					m_ExcelUtil.InsertItem(30+i, 7, strText);
				}
				else if (2==m_nCurCurve%m_nWaveNumberPerPage)
				{
					m_ExcelUtil.InsertItem(30+i, 14, strText);
				}
				else if (3==m_nCurCurve%m_nWaveNumberPerPage)
				{
					m_ExcelUtil.InsertItem(63+i, 7, strText);
				}
				else if (0==m_nCurCurve%m_nWaveNumberPerPage)
				{
					m_ExcelUtil.InsertItem(63+i, 14, strText);
				}
			}
		}
	}
}

void CPropStep6::WriteEventHead(int m_nWaveNumberPerPage,int m_nCurCurve, int nRow)
{
	//绘制标题栏
	LV_COLUMN lvColumn;
	TCHAR uin[50];
	lvColumn.pszText=uin;
	lvColumn.cchTextMax=50;
	lvColumn.mask=LVCF_TEXT;
	CString header=_T("");
	int nLen = MAIN->m_pEventFormView->m_Eventlist.GetHeaderCtrl()->GetItemCount();//得到列数
	//表头内容
	if (nLen > 8) //最多9列
	{
		nLen = 8;
	}
	CRect textRect;
	for(int i=0;i<nLen;i++)
	{
		if(MAIN->m_pEventFormView->m_Eventlist.GetColumn(i,&lvColumn))
		{
			header=lvColumn.pszText;
			if (1==m_nWaveNumberPerPage)
			{
				if (0==i)
				{
					m_ExcelUtil.InsertItem(nRow, i+1, header);
				}
				else if (0!=i)
				{
					m_ExcelUtil.InsertItem(nRow, i+2, header);
				}
			}
			else if (2==m_nWaveNumberPerPage)
			{
				if (1==m_nCurCurve%m_nWaveNumberPerPage)
				{
					if (0==i)
					{
						m_ExcelUtil.InsertItem(33, i+1, header);
					}
					else if (0!=i)
					{
						m_ExcelUtil.InsertItem(33, i+2, header);
					}
				}
				else if (0==m_nCurCurve%m_nWaveNumberPerPage)
				{
					if (0==i)
					{
						m_ExcelUtil.InsertItem(65, i+1, header);
					}
					else if (0!=i)
					{
						m_ExcelUtil.InsertItem(65, i+2, header);
					}
				}
			}
			else if (4==m_nWaveNumberPerPage)
			{
				if (1==m_nCurCurve%m_nWaveNumberPerPage)
				{
					m_ExcelUtil.InsertItem(29, i+1, header);
				}
				else if (2==m_nCurCurve%m_nWaveNumberPerPage)
				{
					m_ExcelUtil.InsertItem(62, i+1, header);
				}
				else if (3==m_nCurCurve%m_nWaveNumberPerPage)
				{
					m_ExcelUtil.InsertItem(29, i+8, header);
				}
				else if (0==m_nCurCurve%m_nWaveNumberPerPage)
				{
					m_ExcelUtil.InsertItem(62, i+8, header);
				}
			}
		}
	}
}

void CPropStep6::SetHorUnitText(int m_nWaveNumberPerPage,int m_nCurCurve)
{
	if (g_sorFileArray.totalCurveNumber == 0) //没有曲线加载
		return;

	//当期选择曲线
	TestResultInfo* pData = g_sorFileArray.sorFileList[m_nCurCurve-1].pTestResultInfo;
	if(_T("m")==g_sorFileArray.waveConfig.DistUnit)//横坐标单位为km
	{
		//设置横坐标文本 开始距离
		float fStartVal = MAIN->m_pWaveFormView->m_sorDrawStatic.m_fRangeX[OTDR_TRACE_MIN]/1000;//zll 2013.5.10
		if(3==g_sorFileArray.waveConfig.UnitDistFig)
			m_strStartDis.Format(_T("%.3fkm"), fStartVal);
		else if(4==g_sorFileArray.waveConfig.UnitDistFig)
			m_strStartDis.Format(_T("%.4fkm"), fStartVal);
		else if(5==g_sorFileArray.waveConfig.UnitDistFig)
			m_strStartDis.Format(_T("%.5fkm"), fStartVal);

		//设置横坐标文本 结束距离
		float fEndVal = pData->Length/1000;//zll 2013.5.10
		if(3==g_sorFileArray.waveConfig.UnitDistFig)
			m_strEndDis.Format(_T("%.3fkm"), fEndVal);
		else if(4==g_sorFileArray.waveConfig.UnitDistFig)
			m_strEndDis.Format(_T("%.4fkm"), fEndVal);
		else if(5==g_sorFileArray.waveConfig.UnitDistFig)
			m_strEndDis.Format(_T("%.5fkm"), fEndVal);

		//设置横坐标文本 单位长度
		if(3==g_sorFileArray.waveConfig.UnitDistFig)
			m_strUnitDis.Format(_T("%.3fkm/div"), (fEndVal-fStartVal)/10);
		else if(4==g_sorFileArray.waveConfig.UnitDistFig)
			m_strUnitDis.Format(_T("%.4fkm/div"), (fEndVal-fStartVal)/10);
		else if(5==g_sorFileArray.waveConfig.UnitDistFig)
			m_strUnitDis.Format(_T("%.5fkm/div"), (fEndVal-fStartVal)/10);

		GetHorLocation(m_nWaveNumberPerPage,m_nCurCurve,m_strStartDis,m_strUnitDis,m_strEndDis);
	}
	else if(_T("mi")==g_sorFileArray.waveConfig.DistUnit)//横坐标单位为miles
	{
		//设置横坐标文本 开始距离
		float fStartVal = MAIN->m_pWaveFormView->m_sorDrawStatic.m_fRangeX[OTDR_TRACE_MIN]/1000;//zll 2013.5.10
		fStartVal=KmeterToOther(fStartVal,UNIT_SHOW_MILE);
		if(3==g_sorFileArray.waveConfig.UnitDistFig)
			m_strStartDis.Format(_T("%.3fmiles"), fStartVal);
		else if(4==g_sorFileArray.waveConfig.UnitDistFig)
			m_strStartDis.Format(_T("%.4fmiles"), fStartVal);
		else if(5==g_sorFileArray.waveConfig.UnitDistFig)
			m_strStartDis.Format(_T("%.5fmiles"), fStartVal);

		//设置横坐标文本 结束距离
		float fEndVal = pData->Length/1000;//zll 2013.5.10
		fEndVal=KmeterToOther(fEndVal,UNIT_SHOW_MILE);
		if(3==g_sorFileArray.waveConfig.UnitDistFig)
			m_strEndDis.Format(_T("%.3fmiles"), fEndVal);
		else if(4==g_sorFileArray.waveConfig.UnitDistFig)
			m_strEndDis.Format(_T("%.4fmiles"), fEndVal);
		else if(5==g_sorFileArray.waveConfig.UnitDistFig)
			m_strEndDis.Format(_T("%.5fmiles"), fEndVal);

		//设置横坐标文本 单位长度
		if(3==g_sorFileArray.waveConfig.UnitDistFig)
			m_strUnitDis.Format(_T("%.3fmiles/div"), (fEndVal-fStartVal)/10);
		else if(4==g_sorFileArray.waveConfig.UnitDistFig)
			m_strUnitDis.Format(_T("%.4fmiles/div"), (fEndVal-fStartVal)/10);
		else if(5==g_sorFileArray.waveConfig.UnitDistFig)
			m_strUnitDis.Format(_T("%.5fmiles/div"), (fEndVal-fStartVal)/10);

		GetHorLocation(m_nWaveNumberPerPage,m_nCurCurve,m_strStartDis,m_strUnitDis,m_strEndDis);
	}
	else if(_T("ft")==g_sorFileArray.waveConfig.DistUnit)//横坐标单位为kft
	{
		//设置横坐标文本 开始距离
		float fStartVal = MAIN->m_pWaveFormView->m_sorDrawStatic.m_fRangeX[OTDR_TRACE_MIN]/1000;//zll 2013.5.10
		fStartVal=KmeterToOther(fStartVal,UNIT_SHOW_KFEET);
		if(3==g_sorFileArray.waveConfig.UnitDistFig)
			m_strStartDis.Format(_T("%.3fkft"), fStartVal);
		else if(4==g_sorFileArray.waveConfig.UnitDistFig)
			m_strStartDis.Format(_T("%.4fkft"), fStartVal);
		else if(5==g_sorFileArray.waveConfig.UnitDistFig)
			m_strStartDis.Format(_T("%.5fkft"), fStartVal);

		//设置横坐标文本 结束距离
		float fEndVal = pData->Length/1000;//zll 2013.5.10
		fEndVal=KmeterToOther(fEndVal,UNIT_SHOW_KFEET);
		if(3==g_sorFileArray.waveConfig.UnitDistFig)
			m_strEndDis.Format(_T("%.3fkft"), fEndVal);
		else if(4==g_sorFileArray.waveConfig.UnitDistFig)
			m_strEndDis.Format(_T("%.4fkft"), fEndVal);
		else if(5==g_sorFileArray.waveConfig.UnitDistFig)
			m_strEndDis.Format(_T("%.5fkft"), fEndVal);

		//设置横坐标文本 单位长度
		if(3==g_sorFileArray.waveConfig.UnitDistFig)
			m_strUnitDis.Format(_T("%.3fkft/div"), (fEndVal-fStartVal)/10);
		else if(4==g_sorFileArray.waveConfig.UnitDistFig)
			m_strUnitDis.Format(_T("%.4fkft/div"), (fEndVal-fStartVal)/10);
		else if(5==g_sorFileArray.waveConfig.UnitDistFig)
			m_strUnitDis.Format(_T("%.5fkft/div"), (fEndVal-fStartVal)/10);

		GetHorLocation(m_nWaveNumberPerPage,m_nCurCurve,m_strStartDis,m_strUnitDis,m_strEndDis);
	}
}
void CPropStep6::SetVerUnitText(int m_nWaveNumberPerPage,int m_nCurCurve)
{
	if (g_sorFileArray.totalCurveNumber == 0) //没有曲线加载
		return;

	//设置纵坐标文本 单位长度
	float fUnitDiv = (MAIN->m_pWaveFormView->m_sorDrawStatic.m_fRangeY[OTDR_TRACE_MAX] - MAIN->m_pWaveFormView->m_sorDrawStatic.m_fRangeY[OTDR_TRACE_MIN]) / COUNT_GRIDLINE_VERTICAL;
	if(1==g_sorFileArray.waveConfig.UnitDbFig)
		m_strUnitDiv.Format(_T("%.1f"), fUnitDiv);
	else if(2==g_sorFileArray.waveConfig.UnitDbFig)
		m_strUnitDiv.Format(_T("%.2f"), fUnitDiv);
	else if(3==g_sorFileArray.waveConfig.UnitDbFig)
		m_strUnitDiv.Format(_T("%.3f"), fUnitDiv);

	if (1==m_nWaveNumberPerPage)
	{
		m_ExcelUtil.InsertItem(13, 1, m_strUnitDiv);
		m_ExcelUtil.InsertItem(14, 1, _T("dB/Div"));
	}
	else if (2==m_nWaveNumberPerPage)
	{
		if (1==m_nCurCurve%m_nWaveNumberPerPage)
		{
			m_ExcelUtil.InsertItem(13, 1, m_strUnitDiv);
			m_ExcelUtil.InsertItem(14, 1, _T("dB/Div"));
		}
		else if (0==m_nCurCurve%m_nWaveNumberPerPage)
		{
			m_ExcelUtil.InsertItem(45, 1, m_strUnitDiv);
			m_ExcelUtil.InsertItem(46, 1, _T("dB/Div"));
		}
	}
	else if (4==m_nWaveNumberPerPage)
	{
		if (1==m_nCurCurve%m_nWaveNumberPerPage)
		{
			m_ExcelUtil.InsertItem(11, 1, m_strUnitDiv+_T("dB/Div"));
		}
		else if (2==m_nCurCurve%m_nWaveNumberPerPage)
		{
			m_ExcelUtil.InsertItem(11, 8, m_strUnitDiv+_T("dB/Div"));
		}
		else if (3==m_nCurCurve%m_nWaveNumberPerPage)
		{
			m_ExcelUtil.InsertItem(44, 1, m_strUnitDiv+_T("dB/Div"));
		}
		else if (0==m_nCurCurve%m_nWaveNumberPerPage)
		{
			m_ExcelUtil.InsertItem(44, 8, m_strUnitDiv+_T("dB/Div"));
		}
	}
	else if (6==m_nWaveNumberPerPage)
	{
		if (1==m_nCurCurve%m_nWaveNumberPerPage)
		{
			m_ExcelUtil.InsertItem(11, 1, m_strUnitDiv+_T("dB/Div"));
		}
		else if (2==m_nCurCurve%m_nWaveNumberPerPage)
		{
			m_ExcelUtil.InsertItem(11, 8, m_strUnitDiv+_T("dB/Div"));
		}
		else if (3==m_nCurCurve%m_nWaveNumberPerPage)
		{
			m_ExcelUtil.InsertItem(33, 1, m_strUnitDiv+_T("dB/Div"));
		}
		else if (4==m_nCurCurve%m_nWaveNumberPerPage)
		{
			m_ExcelUtil.InsertItem(33, 8, m_strUnitDiv+_T("dB/Div"));
		}
		else if (5==m_nCurCurve%m_nWaveNumberPerPage)
		{
			m_ExcelUtil.InsertItem(55, 1, m_strUnitDiv+_T("dB/Div"));
		}
		else if (0==m_nCurCurve%m_nWaveNumberPerPage)
		{
			m_ExcelUtil.InsertItem(55, 8, m_strUnitDiv+_T("dB/Div"));
		}
	}
	else if (8==m_nWaveNumberPerPage)
	{
		if (1==m_nCurCurve%m_nWaveNumberPerPage)
		{
			m_ExcelUtil.InsertItem(4, 1, m_strUnitDiv+_T("dB/Div"));
		}
		else if (2==m_nCurCurve%m_nWaveNumberPerPage)
		{
			m_ExcelUtil.InsertItem(4, 8, m_strUnitDiv+_T("dB/Div"));
		}
		else if (3==m_nCurCurve%m_nWaveNumberPerPage)
		{
			m_ExcelUtil.InsertItem(25, 1, m_strUnitDiv+_T("dB/Div"));
		}
		else if (4==m_nCurCurve%m_nWaveNumberPerPage)
		{
			m_ExcelUtil.InsertItem(25, 8, m_strUnitDiv+_T("dB/Div"));
		}
		else if (5==m_nCurCurve%m_nWaveNumberPerPage)
		{
			m_ExcelUtil.InsertItem(46, 1, m_strUnitDiv+_T("dB/Div"));
		}
		else if (6==m_nCurCurve%m_nWaveNumberPerPage)
		{
			m_ExcelUtil.InsertItem(46, 8, m_strUnitDiv+_T("dB/Div"));
		}
		else if (7==m_nCurCurve%m_nWaveNumberPerPage)
		{
			m_ExcelUtil.InsertItem(67, 1, m_strUnitDiv+_T("dB/Div"));
		}
		else if (0==m_nCurCurve%m_nWaveNumberPerPage)
		{
			m_ExcelUtil.InsertItem(67, 8, m_strUnitDiv+_T("dB/Div"));
		}
	}
}

void CPropStep6::GetHorLocation(int m_nWaveNumberPerPage,int m_nCurCurve,CString m_strStartDis,CString m_strUnitDis,CString m_strEndDis)
{
	if (1==m_nWaveNumberPerPage)
	{
		m_ExcelUtil.InsertItem(29, 2, m_strStartDis);
		m_ExcelUtil.InsertItem(29, 5, m_strUnitDis);
		m_ExcelUtil.InsertItem(29, 8, m_strEndDis);
	}
	else if (2==m_nWaveNumberPerPage)
	{
		if (1==m_nCurCurve%m_nWaveNumberPerPage)
		{
			m_ExcelUtil.InsertItem(31, 2, m_strStartDis);
			m_ExcelUtil.InsertItem(31, 5, m_strUnitDis);
			m_ExcelUtil.InsertItem(31, 8, m_strEndDis);
		}
		else if (0==m_nCurCurve%m_nWaveNumberPerPage)
		{
			m_ExcelUtil.InsertItem(63, 2, m_strStartDis);
			m_ExcelUtil.InsertItem(63, 5, m_strUnitDis);
			m_ExcelUtil.InsertItem(63, 8, m_strEndDis);
		}
	}
	else if (4==m_nWaveNumberPerPage)
	{
		if (1==m_nCurCurve%m_nWaveNumberPerPage)
		{
			m_ExcelUtil.InsertItem(27, 1, m_strStartDis);
			m_ExcelUtil.InsertItem(27, 3, m_strUnitDis);
			m_ExcelUtil.InsertItem(27, 6, m_strEndDis);
		}
		else if (2==m_nCurCurve%m_nWaveNumberPerPage)
		{
			m_ExcelUtil.InsertItem(27, 8, m_strStartDis);
			m_ExcelUtil.InsertItem(27, 10, m_strUnitDis);
			m_ExcelUtil.InsertItem(27, 13, m_strEndDis);
		}
		else if (3==m_nCurCurve%m_nWaveNumberPerPage)
		{
			m_ExcelUtil.InsertItem(60, 1, m_strStartDis);
			m_ExcelUtil.InsertItem(60, 3, m_strUnitDis);
			m_ExcelUtil.InsertItem(60, 6, m_strEndDis);
		}
		else if (0==m_nCurCurve%m_nWaveNumberPerPage)
		{
			m_ExcelUtil.InsertItem(60, 8, m_strStartDis);
			m_ExcelUtil.InsertItem(60, 10, m_strUnitDis);
			m_ExcelUtil.InsertItem(60, 13, m_strEndDis);
		}
	}
	else if (6==m_nWaveNumberPerPage)
	{
		if (1==m_nCurCurve%m_nWaveNumberPerPage)
		{
			m_ExcelUtil.InsertItem(28, 1, m_strStartDis);
			m_ExcelUtil.InsertItem(28, 3, m_strUnitDis);
			m_ExcelUtil.InsertItem(28, 6, m_strEndDis);
		}
		else if (2==m_nCurCurve%m_nWaveNumberPerPage)
		{
			m_ExcelUtil.InsertItem(28, 8, m_strStartDis);
			m_ExcelUtil.InsertItem(28, 10, m_strUnitDis);
			m_ExcelUtil.InsertItem(28, 13, m_strEndDis);
		}
		else if (3==m_nCurCurve%m_nWaveNumberPerPage)
		{
			m_ExcelUtil.InsertItem(50, 1, m_strStartDis);
			m_ExcelUtil.InsertItem(50, 3, m_strUnitDis);
			m_ExcelUtil.InsertItem(50, 6, m_strEndDis);
		}
		else if (4==m_nCurCurve%m_nWaveNumberPerPage)
		{
			m_ExcelUtil.InsertItem(50, 8, m_strStartDis);
			m_ExcelUtil.InsertItem(50, 10, m_strUnitDis);
			m_ExcelUtil.InsertItem(50, 13, m_strEndDis);
		}
		else if (5==m_nCurCurve%m_nWaveNumberPerPage)
		{
			m_ExcelUtil.InsertItem(72, 1, m_strStartDis);
			m_ExcelUtil.InsertItem(72, 3, m_strUnitDis);
			m_ExcelUtil.InsertItem(72, 6, m_strEndDis);
		}
		else if (0==m_nCurCurve%m_nWaveNumberPerPage)
		{
			m_ExcelUtil.InsertItem(72, 8, m_strStartDis);
			m_ExcelUtil.InsertItem(72, 10, m_strUnitDis);
			m_ExcelUtil.InsertItem(72, 13, m_strEndDis);
		}
	}
	else if (8==m_nWaveNumberPerPage)
	{
		if (1==m_nCurCurve%m_nWaveNumberPerPage)
		{
			m_ExcelUtil.InsertItem(20, 1, m_strStartDis);
			m_ExcelUtil.InsertItem(20, 3, m_strUnitDis);
			m_ExcelUtil.InsertItem(20, 6, m_strEndDis);
		}
		else if (2==m_nCurCurve%m_nWaveNumberPerPage)
		{
			m_ExcelUtil.InsertItem(20, 8, m_strStartDis);
			m_ExcelUtil.InsertItem(20, 10, m_strUnitDis);
			m_ExcelUtil.InsertItem(20, 13, m_strEndDis);
		}
		else if (3==m_nCurCurve%m_nWaveNumberPerPage)
		{
			m_ExcelUtil.InsertItem(41, 1, m_strStartDis);
			m_ExcelUtil.InsertItem(41, 3, m_strUnitDis);
			m_ExcelUtil.InsertItem(41, 6, m_strEndDis);
		}
		else if (4==m_nCurCurve%m_nWaveNumberPerPage)
		{
			m_ExcelUtil.InsertItem(41, 8, m_strStartDis);
			m_ExcelUtil.InsertItem(41, 10, m_strUnitDis);
			m_ExcelUtil.InsertItem(41, 13, m_strEndDis);
		}
		else if (5==m_nCurCurve%m_nWaveNumberPerPage)
		{
			m_ExcelUtil.InsertItem(62, 1, m_strStartDis);
			m_ExcelUtil.InsertItem(62, 3, m_strUnitDis);
			m_ExcelUtil.InsertItem(62, 6, m_strEndDis);
		}
		else if (6==m_nCurCurve%m_nWaveNumberPerPage)
		{
			m_ExcelUtil.InsertItem(62, 8, m_strStartDis);
			m_ExcelUtil.InsertItem(62, 10, m_strUnitDis);
			m_ExcelUtil.InsertItem(62, 13, m_strEndDis);
		}
		else if (7==m_nCurCurve%m_nWaveNumberPerPage)
		{
			m_ExcelUtil.InsertItem(83, 1, m_strStartDis);
			m_ExcelUtil.InsertItem(83, 3, m_strUnitDis);
			m_ExcelUtil.InsertItem(83, 6, m_strEndDis);
		}
		else if (0==m_nCurCurve%m_nWaveNumberPerPage)
		{
			m_ExcelUtil.InsertItem(83, 8, m_strStartDis);
			m_ExcelUtil.InsertItem(83, 10, m_strUnitDis);
			m_ExcelUtil.InsertItem(83, 13, m_strEndDis);
		}
	}
}

void CPropStep6::SetSorName(int m_nWaveNumberPerPage,int m_nCurCurve,CString m_sorName)
{
	if (1==m_nWaveNumberPerPage)
	{
		m_ExcelUtil.InsertItem(9, 1, m_sorName);
	}
	if (2==m_nWaveNumberPerPage)
	{
		if (1==m_nCurCurve%m_nWaveNumberPerPage)
		{
			m_ExcelUtil.InsertItem(9, 1, m_sorName);
		}
		else if (0==m_nCurCurve%m_nWaveNumberPerPage)
		{
			m_ExcelUtil.InsertItem(41, 1, m_sorName);
		}
	}
	if (4==m_nWaveNumberPerPage)
	{
		if (1==m_nCurCurve%m_nWaveNumberPerPage)
		{
			m_ExcelUtil.InsertItem(9, 1, m_sorName);
		}
		else if (2==m_nCurCurve%m_nWaveNumberPerPage)
		{
			m_ExcelUtil.InsertItem(9, 8, m_sorName);
		}
		else if (3==m_nCurCurve%m_nWaveNumberPerPage)
		{
			m_ExcelUtil.InsertItem(42, 1, m_sorName);
		}
		else if (0==m_nCurCurve%m_nWaveNumberPerPage)
		{
			m_ExcelUtil.InsertItem(42, 8, m_sorName);
		}
	}
	if (6==m_nWaveNumberPerPage)
	{
		if (1==m_nCurCurve%m_nWaveNumberPerPage)
		{
			m_ExcelUtil.InsertItem(9, 1, m_sorName);
		}
		else if (2==m_nCurCurve%m_nWaveNumberPerPage)
		{
			m_ExcelUtil.InsertItem(9, 8, m_sorName);
		}
		else if (3==m_nCurCurve%m_nWaveNumberPerPage)
		{
			m_ExcelUtil.InsertItem(31, 1, m_sorName);
		}
		else if (4==m_nCurCurve%m_nWaveNumberPerPage)
		{
			m_ExcelUtil.InsertItem(31, 8, m_sorName);
		}
		else if (5==m_nCurCurve%m_nWaveNumberPerPage)
		{
			m_ExcelUtil.InsertItem(53, 1, m_sorName);
		}
		else if (0==m_nCurCurve%m_nWaveNumberPerPage)
		{
			m_ExcelUtil.InsertItem(53, 8, m_sorName);
		}
	}
	if (8==m_nWaveNumberPerPage)
	{
		if (1==m_nCurCurve%m_nWaveNumberPerPage)
		{
			m_ExcelUtil.InsertItem(2, 1, m_sorName);
		}
		else if (2==m_nCurCurve%m_nWaveNumberPerPage)
		{
			m_ExcelUtil.InsertItem(2, 8, m_sorName);
		}
		else if (3==m_nCurCurve%m_nWaveNumberPerPage)
		{
			m_ExcelUtil.InsertItem(23, 1, m_sorName);
		}
		else if (4==m_nCurCurve%m_nWaveNumberPerPage)
		{
			m_ExcelUtil.InsertItem(23, 8, m_sorName);
		}
		else if (5==m_nCurCurve%m_nWaveNumberPerPage)
		{
			m_ExcelUtil.InsertItem(44, 1, m_sorName);
		}
		else if (6==m_nCurCurve%m_nWaveNumberPerPage)
		{
			m_ExcelUtil.InsertItem(44, 8, m_sorName);
		}
		else if (7==m_nCurCurve%m_nWaveNumberPerPage)
		{
			m_ExcelUtil.InsertItem(65, 1, m_sorName);
		}
		else if (0==m_nCurCurve%m_nWaveNumberPerPage)
		{
			m_ExcelUtil.InsertItem(65, 8, m_sorName);
		}
	}
}