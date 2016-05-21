// WavePage.cpp : 实现文件
//

#include "stdafx.h"
#include "WavePage.h"
#include "ConfigSheet.h"

extern SorFileArray g_sorFileArray;

IMPLEMENT_DYNAMIC(CWavePage, CPropertyPage)

CWavePage::CWavePage()
: CPropertyPage(CWavePage::IDD)
,m_bAvgProc(FALSE)
{
}

CWavePage::~CWavePage()
{
}

void CWavePage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_PREVIEW_DRAW, m_drawStatic);
}

BOOL CWavePage::OnInitDialog()
{
	CDialog::OnInitDialog();

	/*               平均化处理中                                                  */
	//CString strAvgProc = IniUtil::ReadResString(_T("IDS_SCREEN_WAVE_AVG_PROC"));
	//GetDlgItem(IDC_STATIC_AVGPROC)->SetWindowText(strAvgProc);
	//GetDlgItem(IDC_STATIC_AVG_PROCING)->SetWindowText(strAvgProc);
	CString strShow = IniUtil::ReadResString(_T("IDS_SCREEN_WAVE_SHOW"));
	//GetDlgItem(IDC_RADIO_AVGPROC_SHOW)->SetWindowText(strShow);
	CString strHide = IniUtil::ReadResString(_T("IDS_SCREEN_WAVE_HIDE"));
	//GetDlgItem(IDC_RADIO_AVGPROC_HIDE)->SetWindowText(strHide);
	//GetDlgItem(IDC_STATIC_AVGPROC)->SetWindowText(strAvgProc);
	//CButton* pBtnShow = (CButton*)GetDlgItem(IDC_RADIO_AVGPROC_SHOW);
	//CButton* pBtnHide = (CButton*)GetDlgItem(IDC_RADIO_AVGPROC_HIDE);
	//CString strShowOrNot = IniUtil::ReadSingleConfigInfo(_T("Condition"), _T("AvRate"));
	//int nShow = _wtoi(strShowOrNot);
	//if (nShow == 0)
	//{
	//	pBtnShow->SetCheck(TRUE);
	//	m_bAvgProc = TRUE;
	//}
	//else
	//{
	//	pBtnHide->SetCheck(TRUE);
	//	m_bAvgProc = FALSE;
	//	GetDlgItem(IDC_STATIC_AVG_PROCING)->EnableWindow(FALSE);
	//}

	///*               光标                                                  */
	//CString strCursor = IniUtil::ReadResString(_T("IDS_SCREEN_WAVE_CURSOR"));
	//GetDlgItem(IDC_STATIC_CURSOR)->SetWindowText(strCursor);
	//CString strCrossLine = IniUtil::ReadResString(_T("IDS_SCREEN_WAVE_CURSOR_CROSSLINE"));
	//GetDlgItem(IDC_RADIO_CURSOR_CROSSLINE)->SetWindowText(strCrossLine);
	//CString strLine = IniUtil::ReadResString(_T("IDS_SREEN_WAVE_CURSOR_LINE"));
	//GetDlgItem(IDC_RADIO_CURSOR_LINE)->SetWindowText(strLine);
	//pBtnShow = (CButton*)GetDlgItem(IDC_RADIO_CURSOR_CROSSLINE);
	//pBtnHide = (CButton*)GetDlgItem(IDC_RADIO_CURSOR_LINE);
	//strShowOrNot = IniUtil::ReadSingleConfigInfo(_T("Condition"), _T("Cursor"));
	//nShow = _wtoi(strShowOrNot);
	//if (nShow == 0)
	//{
	//	pBtnShow->SetCheck(TRUE);
	//	g_sorFileArray.waveConfig.ShowCursor = TRUE;
	//}
	//else
	//{
	//	pBtnHide->SetCheck(TRUE);
	//	g_sorFileArray.waveConfig.ShowCursor = FALSE;
	//}

	/*               栅格                                                  */
	CString strGrid = IniUtil::ReadResString(_T("IDS_SCREEN_WAVE_GRID"));
	GetDlgItem(IDC_STATIC_GRID)->SetWindowText(strGrid);
	GetDlgItem(IDC_RADIO_GIRD_SHOW)->SetWindowText(strShow);
	GetDlgItem(IDC_RADIO_GRID_HIDE)->SetWindowText(strHide);
	CButton* pBtnShow = (CButton*)GetDlgItem(IDC_RADIO_GIRD_SHOW);
	CButton* pBtnHide = (CButton*)GetDlgItem(IDC_RADIO_GRID_HIDE);
	CString strShowOrNot = IniUtil::ReadSingleConfigInfo(_T("Condition"), _T("Grid"));
	int nShow = _wtoi(strShowOrNot);
	if (nShow == 0)
	{
		pBtnShow->SetCheck(TRUE);
		g_sorFileArray.waveConfig.ShowGrid = TRUE;
	}
	else
	{
		pBtnHide->SetCheck(TRUE);
		g_sorFileArray.waveConfig.ShowGrid = FALSE;
	}

	/*               双光标                                                  */
	/*CString str2Cursor = IniUtil::ReadResString(_T("IDS_SCRREN_WAVE_2CURSOR"));
	GetDlgItem(IDC_STATIC_DOUBLE_CURSOR)->SetWindowText(str2Cursor);
	GetDlgItem(IDC_RADIO_DOUBLE_CURSOR_SHOW)->SetWindowText(strShow);
	GetDlgItem(IDC_RADIO_DOUBLE_CURSOR_HIDE)->SetWindowText(strHide);
	pBtnShow = (CButton*)GetDlgItem(IDC_RADIO_DOUBLE_CURSOR_SHOW);
	pBtnHide = (CButton*)GetDlgItem(IDC_RADIO_DOUBLE_CURSOR_HIDE);
	strShowOrNot = IniUtil::ReadSingleConfigInfo(_T("Condition"), _T("Ghost"));
	nShow = _wtoi(strShowOrNot);
	if (nShow == 0)
	{
		pBtnShow->SetCheck(TRUE);
		g_sorFileArray.waveConfig.ShowDoubleCursor = TRUE;
	}
	else
	{
		pBtnHide->SetCheck(TRUE);
		g_sorFileArray.waveConfig.ShowDoubleCursor = FALSE;
	}*/

	/*               预览                                                  */
	CString strPreview = IniUtil::ReadResString(_T("IDS_SCREEN_WAVE_PREVIEW"));
	GetDlgItem(IDC_STATIC_PREVIEW)->SetWindowText(strPreview);

	m_drawStatic.Invalidate();

	return TRUE;// 除非将焦点设置到控件，否则返回 TRUE
}

BEGIN_MESSAGE_MAP(CWavePage, CDialog)
	ON_BN_CLICKED(IDC_RADIO_AVGPROC_SHOW, &CWavePage::OnBnClickedRadioAvgprocShow)
	ON_BN_CLICKED(IDC_RADIO_AVGPROC_HIDE, &CWavePage::OnBnClickedRadioAvgprocHide)
	ON_BN_CLICKED(IDC_RADIO_CURSOR_CROSSLINE, &CWavePage::OnBnClickedRadioCursorCrossline)
	ON_BN_CLICKED(IDC_RADIO_CURSOR_LINE, &CWavePage::OnBnClickedRadioCursorLine)
	ON_BN_CLICKED(IDC_RADIO_GIRD_SHOW, &CWavePage::OnBnClickedRadioGirdShow)
	ON_BN_CLICKED(IDC_RADIO_GRID_HIDE, &CWavePage::OnBnClickedRadioGridHide)
	ON_BN_CLICKED(IDC_RADIO_DOUBLE_CURSOR_SHOW, &CWavePage::OnBnClickedRadioDoubleCursorShow)
	ON_BN_CLICKED(IDC_RADIO_DOUBLE_CURSOR_HIDE, &CWavePage::OnBnClickedRadioDoubleCursorHide)
END_MESSAGE_MAP()


// CWavePage 消息处理程序
BOOL CWavePage::OnApply()
{
	/*               平均化处理中                                               */
	//int nShow = ((CButton*)GetDlgItem(IDC_RADIO_AVGPROC_SHOW))->GetCheck();
	//int nHide = ((CButton*)GetDlgItem(IDC_RADIO_AVGPROC_HIDE))->GetCheck();
	//int nRes = 0;
	//if (nShow == 1)
	//{
	//	nRes = 0;
	//	m_bAvgProc =TRUE;
	//}
	//else if (nShow == 0 && nHide == 1)
	//{
	//	nRes = 1;
	//	m_bAvgProc =FALSE;
	//}
	CString strRes;
	//strRes.Format(_T("%d"), nRes);
	//IniUtil::WriteSingleConfigInfo(_T("Condition"), _T("AvRate"), strRes);

	///*               光标                                                */
	//nShow = ((CButton*)GetDlgItem(IDC_RADIO_CURSOR_CROSSLINE))->GetCheck();
	//nHide = ((CButton*)GetDlgItem(IDC_RADIO_CURSOR_LINE))->GetCheck();
	//nRes = 0;
	//if (nShow == 1)
	//{
	//	nRes = 0;
	//	g_sorFileArray.waveConfig.ShowCursor = TRUE;
	//}
	//else if (nShow == 0 && nHide == 1)
	//{
	//	nRes = 1;
	//	g_sorFileArray.waveConfig.ShowCursor =FALSE;
	//}
	//strRes.Format(_T("%d"), nRes);
	//IniUtil::WriteSingleConfigInfo(_T("Condition"), _T("Cursor"), strRes);

	/*               栅格                                                  */
	int nShow = ((CButton*)GetDlgItem(IDC_RADIO_GIRD_SHOW))->GetCheck();
	int nHide = ((CButton*)GetDlgItem(IDC_RADIO_GRID_HIDE))->GetCheck();
	int nRes = 0;
	if (nShow == 1)
	{
		nRes = 0;
		g_sorFileArray.waveConfig.ShowGrid = TRUE;
	}
	else if (nShow == 0 && nHide == 1)
	{
		nRes = 1;
		g_sorFileArray.waveConfig.ShowGrid  = FALSE;
	}
	strRes.Format(_T("%d"), nRes);
	IniUtil::WriteSingleConfigInfo(_T("Condition"), _T("Grid"), strRes);

	/*               双光标                                                  */
	/*nShow = ((CButton*)GetDlgItem(IDC_RADIO_DOUBLE_CURSOR_SHOW))->GetCheck();
	nHide = ((CButton*)GetDlgItem(IDC_RADIO_DOUBLE_CURSOR_HIDE))->GetCheck();
	nRes = 0;
	if (nShow == 1)
	{
		nRes = 0;
		g_sorFileArray.waveConfig.ShowDoubleCursor = TRUE;
	}
	else if (nShow == 0 && nHide == 1)
	{
		nRes = 1;
		g_sorFileArray.waveConfig.ShowDoubleCursor = FALSE;
	}
	strRes.Format(_T("%d"), nRes);
	IniUtil::WriteSingleConfigInfo(_T("Condition"), _T("Ghost"), strRes);*/

	return TRUE;
}

void CWavePage::OnBnClickedRadioAvgprocShow()
{
	m_bAvgProc = TRUE;
	GetDlgItem(IDC_STATIC_AVG_PROCING)->EnableWindow(TRUE);
}

void CWavePage::OnBnClickedRadioAvgprocHide()
{
	m_bAvgProc = FALSE;
	GetDlgItem(IDC_STATIC_AVG_PROCING)->EnableWindow(FALSE);
}

void CWavePage::OnBnClickedRadioCursorCrossline()
{
	g_sorFileArray.waveConfig.ShowCursor = TRUE;
	m_drawStatic.Invalidate();
}

void CWavePage::OnBnClickedRadioCursorLine()
{
	g_sorFileArray.waveConfig.ShowCursor = FALSE;
	m_drawStatic.Invalidate();
}

void CWavePage::OnBnClickedRadioGirdShow()
{
	g_sorFileArray.waveConfig.ShowGrid = TRUE;
	m_drawStatic.Invalidate();
}

void CWavePage::OnBnClickedRadioGridHide()
{
	g_sorFileArray.waveConfig.ShowGrid = FALSE;
	m_drawStatic.Invalidate();
}

void CWavePage::OnBnClickedRadioDoubleCursorShow()
{
	g_sorFileArray.waveConfig.ShowDoubleCursor = TRUE;
	m_drawStatic.Invalidate();
}

void CWavePage::OnBnClickedRadioDoubleCursorHide()
{
	g_sorFileArray.waveConfig.ShowDoubleCursor = FALSE;
	m_drawStatic.Invalidate();
}
