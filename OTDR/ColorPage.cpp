// ColorPage.cpp : 实现文件
//

#include "stdafx.h"
#include "ColorPage.h"
#include "ConfigSheet.h"
#include <WinSock2.h>
#include "IniUtil.h"
#include "SorFileInfo.h"

extern SorFileArray g_sorFileArray;
extern COLORREF  g_COLORREF[12];
// CColorPage 对话框

IMPLEMENT_DYNAMIC(CColorPage, CPropertyPage)

CColorPage::CColorPage()
	: CPropertyPage(CColorPage::IDD)
{

}

CColorPage::~CColorPage()
{
}

void CColorPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_WAVE1_COLOR, m_colorStatic1);
	DDX_Control(pDX, IDC_STATIC_WAVE2_COLOR, m_colorStatic2);
	DDX_Control(pDX, IDC_STATIC_WAVE3_COLOR, m_colorStatic3);
	DDX_Control(pDX, IDC_STATIC_WAVE4_COLOR, m_colorStatic4);
	DDX_Control(pDX, IDC_STATIC_WAVE5_COLOR, m_colorStatic5);
	DDX_Control(pDX, IDC_STATIC_WAVE6_COLOR, m_colorStatic6);
	DDX_Control(pDX, IDC_STATIC_WAVE7_COLOR, m_colorStatic7);
	DDX_Control(pDX, IDC_STATIC_WAVE8_COLOR, m_colorStatic8);
	DDX_Control(pDX, IDC_STATIC_BG_COLOR, m_colorStaticBack);
	DDX_Control(pDX, IDC_STATIC_BG_GRID_COLOR, m_colorStaticGrid);
	DDX_Control(pDX, IDC_STATIC_OTHER_EVTNTMARK_COLOR, m_colorStaticEventMarker);
	DDX_Control(pDX, IDC_STATIC_OTHER_CURSOR_COLOR, m_colorStaticCursor);
	DDX_Control(pDX, IDC_STATIC_COLOR_DRAW, m_drawStatic);
}

BOOL CColorPage::OnInitDialog()
{
	CDialog::OnInitDialog();

	/*               设置颜色选择组                                                  */
	//m_ComboColSel.InsertString(-1, _T("black"));/* black */
	//m_ComboColSel.InsertString(-1, _T("white"));/* white */
	//m_ComboColSel.InsertString(-1, _T("gray"));/* gray */
	//m_ComboColSel.InsertString(-1, _T("user"));/* user */
	//CString strListNo = IniUtil::ReadSingleConfigInfo(_T("Color"), _T("ListNo"));
	//int nListNo = _wtoi(strListNo);
	//m_ComboColSel.SetCurSel(nListNo);

	////颜色选择组
	//CString strColorSelect = IniUtil::ReadResString(_T("IDS_SCREEN_COLOR_SELECT_GROUP"));
	//GetDlgItem(IDC_STATIC_SELECT_COLORGROUP)->SetWindowText(strColorSelect);

	/*               设置波形颜色                                                    */
	CString strWave = IniUtil::ReadResString(_T("IDS_SCREEN_COLOR_WAVE"));
	GetDlgItem(IDC_STATIC_WAVE)->SetWindowText(strWave);
	
	CString strTrace1 = IniUtil::ReadSingleConfigInfo(_T("Color"), _T("Trace1"));
	COLORREF col1 = _wtol(strTrace1);
	m_colorStatic1.SetBkColor(col1);
	g_COLORREF[0]=col1;
	strTrace1 = IniUtil::ReadResString(_T("IDS_SCREEN_COLOR_WAVE1"));
	GetDlgItem(IDC_STATIC_WAVE1)->SetWindowText(strTrace1);

	CString strTrace2 = IniUtil::ReadSingleConfigInfo(_T("Color"), _T("Trace2"));
	COLORREF col2 = _wtol(strTrace2);
	m_colorStatic2.SetBkColor(col2);
	g_COLORREF[1]=col2;
	strTrace2 = IniUtil::ReadResString(_T("IDS_SCREEN_COLOR_WAVE2"));
	GetDlgItem(IDC_STATIC_WAVE2)->SetWindowText(strTrace2);

	CString strTrace3 = IniUtil::ReadSingleConfigInfo(_T("Color"), _T("Trace3"));
	COLORREF col3 = _wtol(strTrace3);
	m_colorStatic3.SetBkColor(col3);
	g_COLORREF[2]=col3;
	strTrace3 = IniUtil::ReadResString(_T("IDS_SCREEN_COLOR_WAVE3"));
	GetDlgItem(IDC_STATIC_WAVE3)->SetWindowText(strTrace3);

	CString strTrace4 = IniUtil::ReadSingleConfigInfo(_T("Color"), _T("Trace4"));
	COLORREF col4 = _wtol(strTrace4);
	m_colorStatic4.SetBkColor(col4);
	g_COLORREF[3]=col4;
	strTrace4 = IniUtil::ReadResString(_T("IDS_SCREEN_COLOR_WAVE4"));
	GetDlgItem(IDC_STATIC_WAVE4)->SetWindowText(strTrace4);

	CString strTrace5 = IniUtil::ReadSingleConfigInfo(_T("Color"), _T("Trace5"));
	COLORREF col5 = _wtol(strTrace5);
	m_colorStatic5.SetBkColor(col5);
	g_COLORREF[4]=col5;
	strTrace5 = IniUtil::ReadResString(_T("IDS_SCREEN_COLOR_WAVE5"));
	GetDlgItem(IDC_STATIC_WAVE5)->SetWindowText(strTrace5);

	CString strTrace6 = IniUtil::ReadSingleConfigInfo(_T("Color"), _T("Trace6"));
	COLORREF col6 = _wtol(strTrace6);
	m_colorStatic6.SetBkColor(col6);
	g_COLORREF[5]=col6;
	strTrace6 = IniUtil::ReadResString(_T("IDS_SCREEN_COLOR_WAVE6"));
	GetDlgItem(IDC_STATIC_WAVE6)->SetWindowText(strTrace6);

	CString strTrace7 = IniUtil::ReadSingleConfigInfo(_T("Color"), _T("Trace7"));
	COLORREF col7 = _wtol(strTrace7);
	m_colorStatic7.SetBkColor(col7);
	g_COLORREF[6]=col7;
	strTrace7 = IniUtil::ReadResString(_T("IDS_SCREEN_COLOR_WAVE7"));
	GetDlgItem(IDC_STATIC_WAVE7)->SetWindowText(strTrace7);

	CString strTrace8 = IniUtil::ReadSingleConfigInfo(_T("Color"), _T("Trace8"));
	COLORREF col8 = _wtol(strTrace8);
	m_colorStatic8.SetBkColor(col8);
	g_COLORREF[7]=col8;
	strTrace8 = IniUtil::ReadResString(_T("IDS_SCREEN_COLOR_WAVE8"));
	GetDlgItem(IDC_STATIC_WAVE8)->SetWindowText(strTrace8);

	/*                   设置背景颜色                                         */
	//背景和栅格
	CString strBackAndGrid = IniUtil::ReadResString(_T("IDS_SCREEN_COLOR_BACKANDGRID"));
	GetDlgItem(IDC_STATIC_COLOR_BG)->SetWindowText(strBackAndGrid);
	//背景
	CString strBack = IniUtil::ReadSingleConfigInfo(_T("Color"), _T("Back"));
	COLORREF colBack = _wtol(strBack);
	m_colorStaticBack.SetBkColor(colBack);
	g_COLORREF[8]=colBack;
	strBack = IniUtil::ReadResString(_T("IDS_SCREEN_COLOR_BACKGROUND"));
	GetDlgItem(IDC_STATIC_BG_BG)->SetWindowText(strBack);
	//栅格
	CString strGrid = IniUtil::ReadSingleConfigInfo(_T("Color"), _T("Grid"));
	COLORREF colGrid = _wtol(strGrid);
	m_colorStaticGrid.SetBkColor(colGrid);
	g_COLORREF[9]=colGrid;
	strGrid = IniUtil::ReadResString(_T("IDS_SCREEN_COLOR_GRID"));
	GetDlgItem(IDC_STATIC_BG_GRID)->SetWindowText(strGrid);

	/*                    设置其他                                           */
	CString strOther = IniUtil::ReadResString(_T("IDS_SCREEN_COLOR_OTHER"));
	GetDlgItem(IDC_STATIC_OTHER)->SetWindowText(strOther);
	CString strEventMarker = IniUtil::ReadSingleConfigInfo(_T("Color"), _T("EventMarker"));
	COLORREF colEventMarker = _wtol(strEventMarker);
	m_colorStaticEventMarker.SetBkColor(colEventMarker);
	g_COLORREF[10]=colEventMarker;
	strEventMarker = IniUtil::ReadResString(_T("IDS_SCREEN_COLOR_EVENT_MARK"));
	GetDlgItem(IDC_STATIC_OTHER_EVENTMARKER_MOD)->SetWindowText(strEventMarker);

	CString strCursor = IniUtil::ReadSingleConfigInfo(_T("Color"), _T("Cursor"));
	COLORREF colCursor = _wtol(strCursor);
	m_colorStaticCursor.SetBkColor(colCursor);
	g_COLORREF[11]=colCursor;
	strCursor = IniUtil::ReadResString(_T("IDS_SCREEN_COLOR_CURSOR"));
	GetDlgItem(IDC_STATIC_OTHER_CURSOR)->SetWindowText(strCursor);

	//修改按钮文本
	CString strBtnText = IniUtil::ReadResString(_T("IDS_SCREEN_COLOR_BTN_MODE"));
	GetDlgItem(IDC_BTN_MOD1)->SetWindowText(strBtnText);
	GetDlgItem(IDC_BTN_MOD2)->SetWindowText(strBtnText);
	GetDlgItem(IDC_BTN_MOD3)->SetWindowText(strBtnText);
	GetDlgItem(IDC_BTN_MOD4)->SetWindowText(strBtnText);
	GetDlgItem(IDC_BTN_MOD5)->SetWindowText(strBtnText);
	GetDlgItem(IDC_BTN_MOD6)->SetWindowText(strBtnText);
	GetDlgItem(IDC_BTN_MOD7)->SetWindowText(strBtnText);
	GetDlgItem(IDC_BTN_MOD8)->SetWindowText(strBtnText);
	GetDlgItem(IDC_BTN_BG_MOD)->SetWindowText(strBtnText);
	GetDlgItem(IDC_BTN_BG_GIRD_MOD)->SetWindowText(strBtnText);
	GetDlgItem(IDC_BTN_OTHER_EVENTMARKER_MOD)->SetWindowText(strBtnText);
	GetDlgItem(IDC_BTN_OTHER_CURSOR_MOD)->SetWindowText(strBtnText);

	//预览
	CString strPreview = IniUtil::ReadResString(_T("IDS_SCREEN_COLOR_PREVIEW"));
	GetDlgItem(IDC_STATIC_PREVIEW)->SetWindowText(strPreview);


	return TRUE;// 除非将焦点设置到控件，否则返回 TRUE
}

BEGIN_MESSAGE_MAP(CColorPage, CPropertyPage)
	ON_BN_CLICKED(IDC_BTN_MOD1, &CColorPage::OnBnClickedBtnMod1)
	ON_BN_CLICKED(IDC_BTN_MOD2, &CColorPage::OnBnClickedBtnMod2)
	ON_BN_CLICKED(IDC_BTN_MOD3, &CColorPage::OnBnClickedBtnMod3)
	ON_BN_CLICKED(IDC_BTN_MOD4, &CColorPage::OnBnClickedBtnMod4)
	ON_BN_CLICKED(IDC_BTN_MOD5, &CColorPage::OnBnClickedBtnMod5)
	ON_BN_CLICKED(IDC_BTN_MOD6, &CColorPage::OnBnClickedBtnMod6)
	ON_BN_CLICKED(IDC_BTN_MOD7, &CColorPage::OnBnClickedBtnMod7)
	ON_BN_CLICKED(IDC_BTN_MOD8, &CColorPage::OnBnClickedBtnMod8)
	ON_BN_CLICKED(IDC_BTN_BG_MOD, &CColorPage::OnBnClickedBtnBgMod)
	ON_BN_CLICKED(IDC_BTN_BG_GIRD_MOD, &CColorPage::OnBnClickedBtnBgGirdMod)
	ON_BN_CLICKED(IDC_BTN_OTHER_EVENTMARKER_MOD, &CColorPage::OnBnClickedBtnOtherEventMarkerMod)
	ON_BN_CLICKED(IDC_BTN_OTHER_CURSOR_MOD, &CColorPage::OnBnClickedBtnOtherCursorMod)
END_MESSAGE_MAP()

// CColorPage 消息处理程序
BOOL CColorPage::OnApply()
{
	/*                设置颜色选择组                                       */
	//CString strListNo;
	//strListNo.Format(_T("%d"), m_ComboColSel.GetCurSel());
	//IniUtil::WriteSingleConfigInfo(_T("Color"), _T("ListNo"), strListNo);

	/*                 设置波形颜色选择组                                */
	CString strTrace1;
	strTrace1.Format(_T("%u"), m_colorStatic1.GetBkColor());
	IniUtil::WriteSingleConfigInfo(_T("Color"), _T("Trace1"), strTrace1);
	g_sorFileArray.sorFileList[0].pTestResultInfo->curveColor = m_colorStatic1.GetBkColor();

	CString strTrace2;
	strTrace2.Format(_T("%u"), m_colorStatic2.GetBkColor());
	IniUtil::WriteSingleConfigInfo(_T("Color"), _T("Trace2"), strTrace2);
	g_sorFileArray.sorFileList[1].pTestResultInfo->curveColor = m_colorStatic2.GetBkColor();

	CString strTrace3;
	strTrace3.Format(_T("%u"), m_colorStatic3.GetBkColor());
	IniUtil::WriteSingleConfigInfo(_T("Color"), _T("Trace3"), strTrace3);
	g_sorFileArray.sorFileList[2].pTestResultInfo->curveColor = m_colorStatic3.GetBkColor();

	CString strTrace4;
	strTrace4.Format(_T("%u"), m_colorStatic4.GetBkColor());//zll 2013.6.6 
	IniUtil::WriteSingleConfigInfo(_T("Color"), _T("Trace4"), strTrace4);
	g_sorFileArray.sorFileList[3].pTestResultInfo->curveColor = m_colorStatic4.GetBkColor();

	CString strTrace5;
	strTrace5.Format(_T("%u"), m_colorStatic5.GetBkColor());
	IniUtil::WriteSingleConfigInfo(_T("Color"), _T("Trace5"), strTrace5);
	g_sorFileArray.sorFileList[4].pTestResultInfo->curveColor = m_colorStatic5.GetBkColor();

	CString strTrace6;
	strTrace6.Format(_T("%u"), m_colorStatic6.GetBkColor());
	IniUtil::WriteSingleConfigInfo(_T("Color"), _T("Trace6"), strTrace6);
	g_sorFileArray.sorFileList[5].pTestResultInfo->curveColor = m_colorStatic6.GetBkColor();

	CString strTrace7;
	strTrace7.Format(_T("%u"), m_colorStatic7.GetBkColor());
	IniUtil::WriteSingleConfigInfo(_T("Color"), _T("Trace7"), strTrace7);
	g_sorFileArray.sorFileList[6].pTestResultInfo->curveColor = m_colorStatic7.GetBkColor();

	CString strTrace8;
	strTrace8.Format(_T("%u"), m_colorStatic8.GetBkColor());
	IniUtil::WriteSingleConfigInfo(_T("Color"), _T("Trace8"), strTrace8);
	g_sorFileArray.sorFileList[7].pTestResultInfo->curveColor = m_colorStatic8.GetBkColor();

	/*                 设置背景                                */
	CString strBack;
	strBack.Format(_T("%u"), m_colorStaticBack.GetBkColor());
	IniUtil::WriteSingleConfigInfo(_T("Color"), _T("Back"), strBack);

	CString strGrid;
	strGrid.Format(_T("%u"), m_colorStaticGrid.GetBkColor());
	IniUtil::WriteSingleConfigInfo(_T("Color"), _T("Grid"), strGrid);

	/*                 设置其他                                */
	CString strEventMarker;
	strEventMarker.Format(_T("%u"), m_colorStaticEventMarker.GetBkColor());
	IniUtil::WriteSingleConfigInfo(_T("Color"), _T("EventMarker"), strEventMarker);

	CString strCursor;
	strCursor.Format(_T("%u"), m_colorStaticCursor.GetBkColor());
	IniUtil::WriteSingleConfigInfo(_T("Color"), _T("Cursor"), strCursor);

	//重绘曲线
	CMainFrame * pMainFrame = MAIN;
	ASSERT(pMainFrame);
	//pMainFrame->m_pWaveFormView->m_sorDrawStatic.Invalidate(); //曲线 cwcq2013.03.29
	pMainFrame->m_pThumbFormView->Invalidate();//缩略图
	//重新设置曲线位图
	pMainFrame->m_pWaveFormView->m_leftDlg.ResetImageList();

	return TRUE;
}

void CColorPage::ModifyWaveColor(CColorStatic& control, int nIndex)
{
	CColorDialog dlg;
	if(dlg.DoModal()==IDOK) 
	{ 
		COLORREF curColor=dlg.GetColor();
		control.SetBkColor(curColor);

		switch (nIndex)
		{
		case 1:
			g_sorFileArray.waveConfig.ColorCurve1 = curColor;
			g_COLORREF[0]=curColor;
			break;
		case 2:
			g_sorFileArray.waveConfig.ColorCurve2 = curColor;
			g_COLORREF[1]=curColor;
			break;
		case 3:
			g_sorFileArray.waveConfig.ColorCurve3 = curColor;
			g_COLORREF[2]=curColor;
			break;
		case 4:
			g_sorFileArray.waveConfig.ColorCurve4 = curColor;
			g_COLORREF[3]=curColor;
			break;
		case 5:
			g_sorFileArray.waveConfig.ColorCurve5 = curColor;
			g_COLORREF[4]=curColor;
			break;
		case 6:
			g_sorFileArray.waveConfig.ColorCurve6 = curColor;
			g_COLORREF[5]=curColor;
			break;
		case 7:
			g_sorFileArray.waveConfig.ColorCurve7 = curColor;
			g_COLORREF[6]=curColor;
			break;
		case 8:
			g_sorFileArray.waveConfig.ColorCurve8 = curColor;
			g_COLORREF[7]=curColor;
			break;
		case 9:
			g_sorFileArray.waveConfig.ColorBack = curColor;
			g_COLORREF[8]=curColor;
			break;
		case 10:
			g_sorFileArray.waveConfig.ColorGrid = curColor;
			g_COLORREF[9]=curColor;
			break;
		case 11:
			g_sorFileArray.waveConfig.ColorEvent = curColor;
			g_COLORREF[10]=curColor;
			break;
		case 12:
			g_sorFileArray.waveConfig.ColorCursor = curColor;
			g_COLORREF[11]=curColor;
			break;
	
		}

		m_drawStatic.Invalidate();
	} 
}

void CColorPage::OnBnClickedBtnMod1()
{
	ModifyWaveColor(m_colorStatic1 ,1);
}

void CColorPage::OnBnClickedBtnMod2()
{
	ModifyWaveColor(m_colorStatic2, 2);
}

void CColorPage::OnBnClickedBtnMod3()
{
	ModifyWaveColor(m_colorStatic3, 3);
}

void CColorPage::OnBnClickedBtnMod4()
{
	ModifyWaveColor(m_colorStatic4, 4);
}

void CColorPage::OnBnClickedBtnMod5()
{
	ModifyWaveColor(m_colorStatic5, 5);
}

void CColorPage::OnBnClickedBtnMod6()
{
	ModifyWaveColor(m_colorStatic6, 6);
}

void CColorPage::OnBnClickedBtnMod7()
{
	ModifyWaveColor(m_colorStatic7, 7);
}

void CColorPage::OnBnClickedBtnMod8()
{
	ModifyWaveColor(m_colorStatic8, 8);
}

void CColorPage::OnBnClickedBtnBgMod()
{
	ModifyWaveColor(m_colorStaticBack, 9);
	MAIN->m_pWaveFormView->m_sorDrawStatic.DrawCurves(&MAIN->m_pWaveFormView->m_sorDrawStatic.m_dcCurve,-1);
	MAIN->m_pWaveFormView->m_sorDrawStatic.Invalidate();
}

void CColorPage::OnBnClickedBtnBgGirdMod()
{
	ModifyWaveColor(m_colorStaticGrid , 10);
	MAIN->m_pWaveFormView->m_sorDrawStatic.DrawCurves(&MAIN->m_pWaveFormView->m_sorDrawStatic.m_dcCurve,-1);
	MAIN->m_pWaveFormView->m_sorDrawStatic.Invalidate();
}

void CColorPage::OnBnClickedBtnOtherEventMarkerMod()
{
	ModifyWaveColor(m_colorStaticEventMarker , 11);
}

void CColorPage::OnBnClickedBtnOtherCursorMod()
{
	ModifyWaveColor(m_colorStaticCursor , 12);
}
