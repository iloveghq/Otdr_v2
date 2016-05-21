// Prop.cpp : implementation file
//

#include "stdafx.h"
#include "PropStep1.h"
#include "MainFrm.h"

extern SorFileArray g_sorFileArray;

/////////////////////////////////////////////////////////////////////////////
// CPropStep1 property page

IMPLEMENT_DYNCREATE(CPropStep1, CPropertyPage)

CPropStep1::CPropStep1() : CPropertyPage(CPropStep1::IDD)
{
}

CPropStep1::~CPropStep1()
{
}

void CPropStep1::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);

}


BEGIN_MESSAGE_MAP(CPropStep1, CPropertyPage)

	ON_BN_CLICKED(IDC_RDO_STEP1_RPT_WAVE, &CPropStep1::OnBnClickedRdoStep1RptWave)
	ON_BN_CLICKED(IDC_RDO_STEP1_RPT_RPT, &CPropStep1::OnBnClickedRdoStep1RptRpt)
	ON_BN_CLICKED(IDC_RDO_STEP1_1WAVE, &CPropStep1::OnBnClickedRdoStep11wave)
	ON_BN_CLICKED(IDC_RDO_STEP1_2WAVE, &CPropStep1::OnBnClickedRdoStep12wave)
	ON_BN_CLICKED(IDC_RDO_STEP1_4WAVE, &CPropStep1::OnBnClickedRdoStep14wave)
	ON_BN_CLICKED(IDC_RDO_STEP1_6WAVE, &CPropStep1::OnBnClickedRdoStep16wave)
	ON_BN_CLICKED(IDC_RDO_STEP1_8WAVE, &CPropStep1::OnBnClickedRdoStep18wave)
	ON_BN_CLICKED(IDC_RDO_STEP1_PRINT_HOR, &CPropStep1::OnBnClickedRdoStep1PrintHor)
	ON_BN_CLICKED(IDC_RDO_STEP1_PRINT_VER, &CPropStep1::OnBnClickedRdoStep1PrintVer)
	ON_BN_CLICKED(IDC_RDO_STEP1_A1, &CPropStep1::OnBnClickedRdoStep1A1)
	ON_BN_CLICKED(IDC_RDO_STEP1_A2, &CPropStep1::OnBnClickedRdoStep1A2)
	ON_BN_CLICKED(IDC_RDO_STEP1_A3, &CPropStep1::OnBnClickedRdoStep1A3)
	ON_BN_CLICKED(IDC_RDO_STEP1_B1, &CPropStep1::OnBnClickedRdoStep1B1)
	ON_BN_CLICKED(IDC_BUTTON_RPT_SETTING, &CPropStep1::OnButtonRptSetting)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropStep1 message handlers

BOOL CPropStep1::OnSetActive() 
{
	// TODO: Add your specialized code here and/or call the base class
	((CPropSheet*)GetParent())->SetReportHeadBtn(1);
	//重绘图形区域
	CMainFrame* pMainFrame = MAIN;
	if (pMainFrame && pMainFrame->GetSafeHwnd() &&
		pMainFrame->m_pReportFormView &&
		pMainFrame->m_pReportFormView->m_ReportViewDlg &&
		pMainFrame->m_pReportFormView->m_ReportViewDlg.m_DrawButton)
		pMainFrame->m_pReportFormView->m_ReportViewDlg.m_DrawButton.ReDrawRect(1);
	//
	((CPropSheet*)GetParent())->EnableNextBtn(TRUE);
	((CPropSheet*)GetParent())->EnablePrevBtn(FALSE);
	return CPropertyPage::OnSetActive();
}

LRESULT CPropStep1::OnWizardNext() 
{
	// TODO: Add your specialized code here and/or call the base class
	UpdateData();
	((CPropSheet*)GetParent())->EnableNextBtn(TRUE);
	((CPropSheet*)GetParent())->EnablePrevBtn(TRUE);
	//return CPropertyPage::OnWizardNext();
	return 0;
}

BOOL CPropStep1::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	//SetLocalString();//ZLL 2012.8.24注释，否则会本地化2次，已经在PropSheet.cpp中本地化过

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPropStep1::SetLocalString()
{
	/*                 报告                                          */
	CString strReport = IniUtil::ReadResString(_T("IDS_REPORT_STEP1_REPORT"));
	GetDlgItem(IDC_STATIC_STEP1_RPT)->SetWindowText(strReport);
	CButton* pBtnWave = (CButton*)GetDlgItem(IDC_RDO_STEP1_RPT_WAVE);
	CString strWave = IniUtil::ReadResString(_T("IDS_REPORT_STEP1_WAVE"));
	pBtnWave->SetWindowText(strWave);

	CButton* pBtnRpt = (CButton*)GetDlgItem(IDC_RDO_STEP1_RPT_RPT);
	CString strRpt = IniUtil::ReadResString(_T("IDS_REPORT_STEP1_WAVE_SUMMARY_REPORT"));
	pBtnRpt->SetWindowText(strRpt);
	if (g_sorFileArray.reportConfig.Print.CompareNoCase(_T("1")) == 0)
	{
		if (!pBtnRpt->GetCheck())
		{
			pBtnWave->SetCheck(TRUE);
			OnBnClickedRdoStep1RptWave();
		}
		else
		{
			OnBnClickedRdoStep1RptRpt();
		}
		//隐藏A1\A2\A3\B1
		GetDlgItem(IDC_RDO_STEP1_A1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RDO_STEP1_A2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RDO_STEP1_A3)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RDO_STEP1_B1)->ShowWindow(SW_HIDE);
		//显示1wave/2wave/4wave/6wave/8wave
		GetDlgItem(IDC_RDO_STEP1_1WAVE)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_RDO_STEP1_2WAVE)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_RDO_STEP1_4WAVE)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_RDO_STEP1_6WAVE)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_RDO_STEP1_8WAVE)->ShowWindow(SW_SHOW);
		int nWaveList = _wtoi(g_sorFileArray.reportConfig.WaveList);
		//隐藏打印方向
		if (nWaveList == 0 ||
			nWaveList == 1)
		{
			GetDlgItem(IDC_STATIC_STEP1_PRINT)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_RDO_STEP1_PRINT_HOR)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_RDO_STEP1_PRINT_VER)->ShowWindow(SW_HIDE);
		}/* zll注释,使打印方向 横向 纵向不可选 2012.9.24 
		else 
		{
			GetDlgItem(IDC_STATIC_STEP1_PRINT)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_RDO_STEP1_PRINT_HOR)->ShowWindow(SW_SHOW);
			GetDlgItem(IDC_RDO_STEP1_PRINT_VER)->ShowWindow(SW_SHOW);
		}*/
	}
	else
	{
		if (!pBtnWave->GetCheck())
		{
			pBtnRpt->SetCheck(TRUE);
			OnBnClickedRdoStep1RptRpt();
		}
		else
		{

			OnBnClickedRdoStep1RptWave();
		}
		//显示A1\A2\A3\B1
		GetDlgItem(IDC_RDO_STEP1_A1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_RDO_STEP1_A2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_RDO_STEP1_A3)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_RDO_STEP1_B1)->ShowWindow(SW_SHOW);
		//隐藏1wave/2wave/4wave/6wave/8wave
		GetDlgItem(IDC_RDO_STEP1_1WAVE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RDO_STEP1_2WAVE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RDO_STEP1_4WAVE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RDO_STEP1_6WAVE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RDO_STEP1_8WAVE)->ShowWindow(SW_HIDE);
		//隐藏布局
		GetDlgItem(IDC_STATIC_STEP1_PRINT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RDO_STEP1_PRINT_HOR)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RDO_STEP1_PRINT_VER)->ShowWindow(SW_HIDE);
	}

	/*                 设置                                          */
	CString strSetting = IniUtil::ReadResString(_T("IDS_REPORT_STEP1_SETTING"));
	GetDlgItem(IDC_STATIC_STEP1_SETTING)->SetWindowText(strSetting);
	//button setting
	GetDlgItem(IDC_BUTTON_RPT_SETTING)->SetWindowText(strSetting);  //lzy2014.9.5
	// 1 wave/page
	CButton* pBtn1Wave = (CButton*)GetDlgItem(IDC_RDO_STEP1_1WAVE);
	CString str1Wave = IniUtil::ReadResString(_T("IDS_REPORT_STEP1_1WAVE"));
	pBtn1Wave->SetWindowText(str1Wave);
	//2 wave/page
	CButton* pBtn2Wave = (CButton*)GetDlgItem(IDC_RDO_STEP1_2WAVE);
	CString str2Wave = IniUtil::ReadResString(_T("IDS_REPORT_STEP1_2WAVE"));
	pBtn2Wave->SetWindowText(str2Wave);
	//4 wave/page
	CButton* pBtn4Wave = (CButton*)GetDlgItem(IDC_RDO_STEP1_4WAVE);
	CString str4Wave = IniUtil::ReadResString(_T("IDS_REPORT_STEP1_4WAVE"));
	pBtn4Wave->SetWindowText(str4Wave);
	//6 wave/page
	CButton* pBtn6Wave = (CButton*)GetDlgItem(IDC_RDO_STEP1_6WAVE);
	CString str6Wave = IniUtil::ReadResString(_T("IDS_REPORT_STEP1_6WAVE"));
	pBtn6Wave->SetWindowText(str6Wave);
	//8 wave/page
	CButton* pBtn8Wave = (CButton*)GetDlgItem(IDC_RDO_STEP1_8WAVE);
	CString str8Wave = IniUtil::ReadResString(_T("IDS_REPORT_STEP1_8WAVE"));
	pBtn8Wave->SetWindowText(str8Wave);

	int nWaveList = _wtoi(g_sorFileArray.reportConfig.WaveList);
	switch (nWaveList)
	{
	case 0:
		pBtn1Wave->SetCheck(TRUE);
		break;
	case 1:
		pBtn2Wave->SetCheck(TRUE);
		break;
	case 2:
		pBtn4Wave->SetCheck(TRUE);
		break;
	case 3:
		pBtn6Wave->SetCheck(TRUE);
		break;
	case 4:
		pBtn8Wave->SetCheck(TRUE);
		break;
	}
	// A1
	CButton* pBtnA1 = (CButton*)GetDlgItem(IDC_RDO_STEP1_A1);
	CString strA1 = IniUtil::ReadResString(_T("IDS_REPORT_STEP1_A1"));
	pBtnA1->SetWindowText(strA1);
	//A2
	CButton* pBtnA2 = (CButton*)GetDlgItem(IDC_RDO_STEP1_A2);
	CString strA2 = IniUtil::ReadResString(_T("IDS_REPORT_STEP1_A2"));
	pBtnA2->SetWindowText(strA2);
	//A3
	CButton* pBtnA3 = (CButton*)GetDlgItem(IDC_RDO_STEP1_A3);
	CString strA3 = IniUtil::ReadResString(_T("IDS_REPORT_STEP1_A3"));
	pBtnA3->SetWindowText(strA3);
	//B1
	CButton* pBtnB1 = (CButton*)GetDlgItem(IDC_RDO_STEP1_B1);
	CString strB1 = IniUtil::ReadResString(_T("IDS_REPORT_STEP1_B1"));
	pBtnB1->SetWindowText(strB1);
	int nListFormat = _wtoi(g_sorFileArray.reportConfig.ListFormat);
	switch (nListFormat)
	{
	case 0:
		pBtnA1->SetCheck(TRUE);
		break;
	case 1:
		pBtnA2->SetCheck(TRUE);
		break;
	case 2:
		pBtnA3->SetCheck(TRUE);
		break;
	case 3:
		pBtnB1->SetCheck(TRUE);
		break;
	}
	// zll注释,使打印方向 横向 纵向不可选 2012.9.24
	/*                 打印方向                                          */
	/*CString strDirection = IniUtil::ReadResString(_T("IDS_REPORT_STEP1_PRINT_DIRECTION"));
	GetDlgItem(IDC_STATIC_STEP1_PRINT)->SetWindowText(strDirection);
	CButton* pBtnHor = (CButton*)GetDlgItem(IDC_RDO_STEP1_PRINT_HOR);
	CString strHor = IniUtil::ReadResString(_T("IDS_REPORT_STEP1_PRINT_HOR"));
	pBtnHor->SetWindowText(strHor);

	CButton* pBtnVer = (CButton*)GetDlgItem(IDC_RDO_STEP1_PRINT_VER);
	CString strVer = IniUtil::ReadResString(_T("IDS_REPORT_STEP1_PRINT_VER"));
	pBtnVer->SetWindowText(strVer);
	if (g_sorFileArray.reportConfig.Direction.CompareNoCase(_T("1")) == 0)
		pBtnVer->SetCheck(TRUE);
	else
		pBtnHor->SetCheck(TRUE);*/
}

void CPropStep1::OnBnClickedRdoStep1RptWave()
{
	//设置报表模式
	g_sorFileArray.reportConfig.Print = _T("1");
	MAIN->m_pReportFormView->m_ReportViewDlg.m_DrawButton.SetDrawMode(FALSE);
	//隐藏A1\A2\A3\B1
	GetDlgItem(IDC_RDO_STEP1_A1)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_RDO_STEP1_A2)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_RDO_STEP1_A3)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_RDO_STEP1_B1)->ShowWindow(SW_HIDE);
	//显示1wave/2wave/4wave/6wave/8wave
	GetDlgItem(IDC_RDO_STEP1_1WAVE)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_RDO_STEP1_2WAVE)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_RDO_STEP1_4WAVE)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_RDO_STEP1_6WAVE)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_RDO_STEP1_8WAVE)->ShowWindow(SW_SHOW);
	int nWaveList = _wtoi(g_sorFileArray.reportConfig.WaveList);
	MAIN->m_pReportFormView->m_ReportViewDlg.m_DrawButton.Draw(1, nWaveList);
	//隐藏打印方向
	if (nWaveList == 0 ||
		nWaveList == 1)
	{
		GetDlgItem(IDC_STATIC_STEP1_PRINT)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RDO_STEP1_PRINT_HOR)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RDO_STEP1_PRINT_VER)->ShowWindow(SW_HIDE);
	}/*  zll注释,使打印方向 横向 纵向不可选 2012.9.24
	else 
	{
		GetDlgItem(IDC_STATIC_STEP1_PRINT)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_RDO_STEP1_PRINT_HOR)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_RDO_STEP1_PRINT_VER)->ShowWindow(SW_SHOW);
	}*/
}

void CPropStep1::OnBnClickedRdoStep1RptRpt()
{
	g_sorFileArray.reportConfig.Print = _T("0");
	MAIN->m_pReportFormView->m_ReportViewDlg.m_DrawButton.SetDrawMode(TRUE);
	//设置报表模式
	//显示A1\A2\A3\B1
	GetDlgItem(IDC_RDO_STEP1_A1)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_RDO_STEP1_A2)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_RDO_STEP1_A3)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_RDO_STEP1_B1)->ShowWindow(SW_SHOW);
	//隐藏1wave/2wave/4wave/6wave/8wave
	GetDlgItem(IDC_RDO_STEP1_1WAVE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_RDO_STEP1_2WAVE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_RDO_STEP1_4WAVE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_RDO_STEP1_6WAVE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_RDO_STEP1_8WAVE)->ShowWindow(SW_HIDE);
	//隐藏打印方向
	GetDlgItem(IDC_STATIC_STEP1_PRINT)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_RDO_STEP1_PRINT_HOR)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_RDO_STEP1_PRINT_VER)->ShowWindow(SW_HIDE);
}

void CPropStep1::OnBnClickedRdoStep11wave()
{
	g_sorFileArray.reportConfig.WaveList = _T("0");
	IniUtil::WriteSingleConfigInfo(_T("RepLayWaveLay"), _T("WaveList"),g_sorFileArray.reportConfig.WaveList);//zll 2013.6.13
	MAIN->m_pReportFormView->m_ReportViewDlg.m_DrawButton.Draw(1, 0);
	//隐藏打印方向
	GetDlgItem(IDC_STATIC_STEP1_PRINT)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_RDO_STEP1_PRINT_HOR)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_RDO_STEP1_PRINT_VER)->ShowWindow(SW_HIDE);
}

void CPropStep1::OnBnClickedRdoStep12wave()
{
	g_sorFileArray.reportConfig.WaveList = _T("1");
	IniUtil::WriteSingleConfigInfo(_T("RepLayWaveLay"), _T("WaveList"),g_sorFileArray.reportConfig.WaveList);//zll 2013.6.13
	MAIN->m_pReportFormView->m_ReportViewDlg.m_DrawButton.Draw(1, 1);
	//隐藏打印方向
	GetDlgItem(IDC_STATIC_STEP1_PRINT)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_RDO_STEP1_PRINT_HOR)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_RDO_STEP1_PRINT_VER)->ShowWindow(SW_HIDE);
}

void CPropStep1::OnBnClickedRdoStep14wave()
{
	g_sorFileArray.reportConfig.WaveList = _T("2");
	IniUtil::WriteSingleConfigInfo(_T("RepLayWaveLay"), _T("WaveList"),g_sorFileArray.reportConfig.WaveList);//zll 2013.6.13
	MAIN->m_pReportFormView->m_ReportViewDlg.m_DrawButton.Draw(1, 2);
	//显示打印方向  zll注释,使打印方向 横向 纵向不可选 2012.9.24
	/*GetDlgItem(IDC_STATIC_STEP1_PRINT)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_RDO_STEP1_PRINT_HOR)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_RDO_STEP1_PRINT_VER)->ShowWindow(SW_SHOW);*/
}

void CPropStep1::OnBnClickedRdoStep16wave()
{
	g_sorFileArray.reportConfig.WaveList = _T("3");
	IniUtil::WriteSingleConfigInfo(_T("RepLayWaveLay"), _T("WaveList"),g_sorFileArray.reportConfig.WaveList);//zll 2013.6.13
	MAIN->m_pReportFormView->m_ReportViewDlg.m_DrawButton.Draw(1, 3);
	//显示打印方向  zll注释,使打印方向 横向 纵向不可选 2012.9.24
	/*GetDlgItem(IDC_STATIC_STEP1_PRINT)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_RDO_STEP1_PRINT_HOR)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_RDO_STEP1_PRINT_VER)->ShowWindow(SW_SHOW);*/
}

void CPropStep1::OnBnClickedRdoStep18wave()
{
	g_sorFileArray.reportConfig.WaveList = _T("4");
	IniUtil::WriteSingleConfigInfo(_T("RepLayWaveLay"), _T("WaveList"),g_sorFileArray.reportConfig.WaveList);//zll 2013.6.13
	MAIN->m_pReportFormView->m_ReportViewDlg.m_DrawButton.Draw(1, 4);
	//显示打印方向  zll注释,使打印方向 横向 纵向不可选 2012.9.24
	/*GetDlgItem(IDC_STATIC_STEP1_PRINT)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_RDO_STEP1_PRINT_HOR)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_RDO_STEP1_PRINT_VER)->ShowWindow(SW_SHOW);*/
}

void CPropStep1::OnBnClickedRdoStep1PrintHor()
{
	g_sorFileArray.reportConfig.Direction = _T("0");
	MAIN->m_pReportFormView->m_ReportViewDlg.m_DrawButton.Setlayout(TRUE);
}

void CPropStep1::OnBnClickedRdoStep1PrintVer()
{
	g_sorFileArray.reportConfig.Direction = _T("1");
	MAIN->m_pReportFormView->m_ReportViewDlg.m_DrawButton.Setlayout(FALSE);
}

void CPropStep1::OnBnClickedRdoStep1A1()
{
	g_sorFileArray.reportConfig.ListFormat = _T("0");
	MAIN->m_pReportFormView->m_ReportViewDlg.m_DrawButton.SetListFormat(0);
}

void CPropStep1::OnBnClickedRdoStep1A2()
{
	g_sorFileArray.reportConfig.ListFormat = _T("1");
	MAIN->m_pReportFormView->m_ReportViewDlg.m_DrawButton.SetListFormat(1);
}

void CPropStep1::OnBnClickedRdoStep1A3()
{
	g_sorFileArray.reportConfig.ListFormat = _T("2");
	MAIN->m_pReportFormView->m_ReportViewDlg.m_DrawButton.SetListFormat(2);
}

void CPropStep1::OnBnClickedRdoStep1B1()
{
	g_sorFileArray.reportConfig.ListFormat = _T("3");
	MAIN->m_pReportFormView->m_ReportViewDlg.m_DrawButton.SetListFormat(3);
}

//参数设置，lzy2014.9.5
void CPropStep1::OnButtonRptSetting()
{
	MAIN->OnSetting();
}
