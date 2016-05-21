// UnitPage.cpp : 实现文件
//

#include "stdafx.h"
#include "UnitPage.h"
#include "ConfigSheet.h"
#include "FolderDialog.h"
#include "Eventlist.h"

extern SorFileArray g_sorFileArray;

// CUnitPage 对话框

IMPLEMENT_DYNAMIC(CUnitPage, CPropertyPage)

CUnitPage::CUnitPage()
	: CPropertyPage(CUnitPage::IDD)
{

}

CUnitPage::~CUnitPage()
{
}

void CUnitPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}

BOOL CUnitPage::OnInitDialog()
{
	CDialog::OnInitDialog();
	/*                 dB刻度                                          */
	//dB刻度
	CString strDbFig = IniUtil::ReadResString(_T("IDS_SCREEN_UNIT_DB_FIG"));
	GetDlgItem(IDC_STATIC_DB)->SetWindowText(strDbFig);
	CButton* pBtnFig3dot = (CButton*)GetDlgItem(IDC_RADIO_DB_3DOT);
	CString strFig3dot = IniUtil::ReadResString(_T("IDS_SCREEN_UNIT_DB_3DOT"));
	pBtnFig3dot->SetWindowText(strFig3dot);

	CButton* pBtnFig2dot = (CButton*)GetDlgItem(IDC_RADIO_DB_2DOT);
	CString strFig2dot = IniUtil::ReadResString(_T("IDS_SCREEN_UNIT_DB_2DOT"));
	pBtnFig2dot->SetWindowText(strFig2dot);

	CButton* pBtnFig1dot = (CButton*)GetDlgItem(IDC_RADIO_DB_1DOT);
	CString strFig1dot = IniUtil::ReadResString(_T("IDS_SCREEN_UNIT_DB_1DOT"));
	pBtnFig1dot->SetWindowText(strFig1dot);

	switch (g_sorFileArray.waveConfig.UnitDbFig)
	{
	case 3:
		pBtnFig3dot->SetCheck(TRUE);
		break;
	case 2:
		pBtnFig2dot->SetCheck(TRUE);
		break;
	case 1:
		pBtnFig1dot->SetCheck(TRUE);
		break;
	}

	/*                 距离刻度                                          */
	//距离刻度
	CString strDistFig = IniUtil::ReadResString(_T("IDS_SCREEN_UNIT_DIST_FIG"));
	GetDlgItem(IDC_STATIC_DIST)->SetWindowText(strDistFig);
	CButton* pBtnDistFig5dot = (CButton*)GetDlgItem(IDC_RADIO_DIST_5DOT);
	CString strDistFig5dot = IniUtil::ReadResString(_T("IDS_SCREEN_UNIT_DIST_FIG_5DOT"));
	pBtnDistFig5dot->SetWindowText(strDistFig5dot);

	CButton* pBtnDistFig4dot = (CButton*)GetDlgItem(IDC_RADIO_DIST_4DOT);
	CString strDistFig4dot = IniUtil::ReadResString(_T("IDS_SCREEN_UNIT_DIST_FIG_4DOT"));
	pBtnDistFig4dot->SetWindowText(strDistFig4dot);

	CButton* pBtnDistFig3dot = (CButton*)GetDlgItem(IDC_RADIO_DIST_3DOT);
	CString strDistFig3dot = IniUtil::ReadResString(_T("IDS_SCREEN_UNIT_DIST_FIG_3DOT"));
	pBtnDistFig3dot->SetWindowText(strDistFig3dot);

	CString strDistFigure = IniUtil::ReadSingleConfigInfo(_T("Unit"), _T("DFigure"));
	switch (g_sorFileArray.waveConfig.UnitDistFig)
	{
	case 5:
		pBtnDistFig5dot->SetCheck(TRUE);
		break;
	case 4:
		pBtnDistFig4dot->SetCheck(TRUE);
		break;
	case 3:
		pBtnDistFig3dot->SetCheck(TRUE);
		break;
	}
	
	/*                 距离单位                                          */
	//距离单位
	CString strDistUnit = IniUtil::ReadResString(_T("IDS_SCREEN_UNIT_DIST_UNIT"));
	GetDlgItem(IDC_STATIC_UNIT)->SetWindowText(strDistUnit);

	CButton* pBtnUnitKm = (CButton*)GetDlgItem(IDC_RADIO_UNIT_KM);
	CButton* pBtnUnitMile = (CButton*)GetDlgItem(IDC_RADIO_UNIT_MILE);
	CButton* pBtnUnitKf = (CButton*)GetDlgItem(IDC_RADIO_UNIT_KF);

	CString strUnit = IniUtil::ReadSingleConfigInfo(_T("Unit"), _T("Distance"));
	if (UNIT_SHOW_KFEET == strUnit)//1 英尺 = 0.3048 米
	{
		pBtnUnitKf->SetCheck(TRUE);
	}
	else if (UNIT_SHOW_MILE == strUnit)//1 英里 = 1609.344 米
	{
		pBtnUnitMile->SetCheck(TRUE);
	}
	else//km
	{
		pBtnUnitKm->SetCheck(TRUE);
	}//

	m_pEventFormView=((CMainFrame*)AfxGetMainWnd())->m_pEventFormView;//zll 2012.11.20 m_pEventFormView指向CMainFrame
	m_pWaveFormView=((CMainFrame*)AfxGetMainWnd())->m_pWaveFormView;//zll 2012.11.21
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

BEGIN_MESSAGE_MAP(CUnitPage, CPropertyPage)

	ON_BN_CLICKED(IDC_RADIO_UNIT_KM, &CUnitPage::OnBnClickedRadioUnitKm)
	ON_BN_CLICKED(IDC_RADIO_UNIT_MILE, &CUnitPage::OnBnClickedRadioUnitMile)
	ON_BN_CLICKED(IDC_RADIO_UNIT_KF, &CUnitPage::OnBnClickedRadioUnitKf)
END_MESSAGE_MAP()


// CUnitPage 消息处理程序

BOOL CUnitPage::OnApply()
{
	/*                 dB刻度                                          */
	int nFig3dot = ((CButton*)GetDlgItem(IDC_RADIO_DB_3DOT))->GetCheck();
	int nFig2dot = ((CButton*)GetDlgItem(IDC_RADIO_DB_2DOT))->GetCheck();
	int nFig1dot = ((CButton*)GetDlgItem(IDC_RADIO_DB_1DOT))->GetCheck();
	if (nFig3dot == 1)
		g_sorFileArray.waveConfig.UnitDbFig = 3;
	else if (nFig3dot == 0 && nFig2dot == 1)
		g_sorFileArray.waveConfig.UnitDbFig = 2;
	else if (nFig3dot == 0 && nFig2dot == 0 && nFig1dot == 1)
		g_sorFileArray.waveConfig.UnitDbFig = 1;

	CString strFigure;
	strFigure.Format(_T("%d"), g_sorFileArray.waveConfig.UnitDbFig);
	IniUtil::WriteSingleConfigInfo(_T("Unit"), _T("Figure"), strFigure);

	/*                 距离刻度                                          */
	int nDistFig5dot = ((CButton*)GetDlgItem(IDC_RADIO_DIST_5DOT))->GetCheck();
	int nDistFig4dot = ((CButton*)GetDlgItem(IDC_RADIO_DIST_4DOT))->GetCheck();
	int nDistFig3dot = ((CButton*)GetDlgItem(IDC_RADIO_DIST_3DOT))->GetCheck();

	if (nDistFig5dot == 1)
		g_sorFileArray.waveConfig.UnitDistFig = 5;
	else if (nDistFig5dot == 0 && nDistFig4dot == 1)
		g_sorFileArray.waveConfig.UnitDistFig = 4;
	else if (nDistFig5dot == 0 && nDistFig4dot == 0 && nDistFig3dot == 1)
		g_sorFileArray.waveConfig.UnitDistFig = 3;


	CString strDistFig;
	strDistFig.Format(_T("%d"), g_sorFileArray.waveConfig.UnitDistFig);
	IniUtil::WriteSingleConfigInfo(_T("Unit"), _T("DFigure"), strDistFig);

	/*                 距离单位                                          */
	int nUnitKm = ((CButton*)GetDlgItem(IDC_RADIO_UNIT_KM))->GetCheck();
	int nUnitMile = ((CButton*)GetDlgItem(IDC_RADIO_UNIT_MILE))->GetCheck();
	int nUnitKf = ((CButton*)GetDlgItem(IDC_RADIO_UNIT_KF))->GetCheck();

	if (nUnitKm == 1)
		g_sorFileArray.waveConfig.DistUnit = UNIT_SHOW_KM;
	else if (nUnitKm == 0 && nUnitMile == 1)
		g_sorFileArray.waveConfig.DistUnit = UNIT_SHOW_MILE;
	else if (nUnitKm == 0 && nUnitMile == 0 && nUnitKf == 1)
		g_sorFileArray.waveConfig.DistUnit = UNIT_SHOW_KFEET;

	IniUtil::WriteSingleConfigInfo(_T("Unit"), _T("Distance"), g_sorFileArray.waveConfig.DistUnit);

	//刷新事件列表 //zll 2012.11.21
	m_pEventFormView->FreshEventList();
	//刷新测量条件 //zll 2012.11.21
	m_pWaveFormView->m_leftDlg.FillParams();
	//刷新横坐标单位文本 //zll 2012.11.21
	m_pWaveFormView->SetHorUnitText();
	//刷新纵坐标单位文本 //zll 2012.11.21
	m_pWaveFormView->SetVerUnitText();
	return TRUE;
}
void CUnitPage::OnBnClickedRadioUnitKm()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CUnitPage::OnBnClickedRadioUnitMile()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CUnitPage::OnBnClickedRadioUnitKf()
{
	// TODO: 在此添加控件通知处理程序代码
}
