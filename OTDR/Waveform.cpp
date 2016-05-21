// Waveform.cpp : implementation file
//

#include "stdafx.h"
#include "OTDR.h"
#include "Waveform.h"
#include "SorFileInfo.h"


#define PARAM_DIST 8 //横坐标参数距离底部距离
#define SCROOL_DIST 24 //左右滚动条距离底部距离
#define SCROOL_WIDTH 21 //滚动条宽度
#define DRAW_DIST  24 //绘图区域距底部距离
#define LEFT_DLG_WIDTH 254 //左面对话框宽度

extern SorFileArray g_sorFileArray;
extern LANG g_currentLang;

// CWaveform

IMPLEMENT_DYNCREATE(CWaveform, CFormView)

CWaveform::CWaveform()
	: CFormView(CWaveform::IDD)
	, m_bCheckSelectAB(FALSE)
{
	////zll 2013.4.28
	m_nSelectCursor = CURSOR_SELECT::CURSOR_A; //默认选中A
	m_nMaxIndex = 0;
}

CWaveform::~CWaveform()
{
}

void CWaveform::SetLocalString()
{
	//设置Frame标题
	CString strTitle = IniUtil::ReadResString(_T("IDS_FRM_WAVE_TITLE"));
	GetParentFrame()->SetWindowText(strTitle);

	//设置按钮注释
	strH_Zoom_in=IniUtil::ReadResString(_T("IDS_BUTTON_HORIZON_ZOOM_IN"));
	m_hAmplify.SetTooltipText(strH_Zoom_in);
	strH_Zoom_out=IniUtil::ReadResString(_T("IDS_BUTTON_HORIZON_ZOOM_OUT"));
	m_hDeflate.SetTooltipText(strH_Zoom_out);
	strV_Zoom_in=IniUtil::ReadResString(_T("IDS_BUTTON_VERTICAL_ZOOM_IN"));
	m_vAmplify.SetTooltipText(strV_Zoom_in);
	strV_Zoom_out=IniUtil::ReadResString(_T("IDS_BUTTON_VERTICAL_ZOOM_OUT"));
	m_vDeflate.SetTooltipText(strV_Zoom_out);
	strScaleInit=IniUtil::ReadResString(_T("IDS_BUTTON_SCALE_INITIALIZE"));
	m_Return.SetTooltipText(strScaleInit);
	strCursorSwitch=IniUtil::ReadResString(_T("IDS_BUTTON_CURSOR"));
	m_btnCursor.SetTooltipText(strCursorSwitch);
	strPreEvent=IniUtil::ReadResString(_T("IDS_BUTTON_PRE_EVENT"));
	m_EventPre.SetTooltipText(strPreEvent);
	strNextEvent=IniUtil::ReadResString(_T("IDS_BUTTON_NEXT_EVENT"));
	m_EventNext.SetTooltipText(strNextEvent);
	strAddEvent=IniUtil::ReadResString(_T("IDS_BUTTON_ADD_EVENT"));
	m_EventAdd.SetTooltipText(strAddEvent);
	strDeleteEvent=IniUtil::ReadResString(_T("IDS_BUTTON_DELETE_EVENT"));
	m_EventDelete.SetTooltipText(strDeleteEvent);
	strMK1=IniUtil::ReadResString(_T("IDS_BUTTON_MK1"));
	m_Mk1.SetTooltipText(strMK1);
	strMK2=IniUtil::ReadResString(_T("IDS_BUTTON_MK2"));
	m_Mk2.SetTooltipText(strMK2);
	strClear=IniUtil::ReadResString(_T("IDS_BUTTON_CLEAR"));
	m_Clear.SetTooltipText(strClear);
	//设置对话框本地化
	m_leftDlg.SetLocalString();
}

void CWaveform::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_HOR_AMPLIFY, m_hAmplify);
	DDX_Control(pDX, IDC_HOR_DEFLATE, m_hDeflate);
	DDX_Control(pDX, IDC_RETURN, m_Return);
	DDX_Control(pDX, IDC_AUTO, m_Auto);
	DDX_Control(pDX, IDC_EVENT_ADD, m_EventAdd);
	DDX_Control(pDX, IDC_EVENT_SUBTRACT, m_EventDelete);
	DDX_Control(pDX, IDC_MK1, m_Mk1);
	DDX_Control(pDX, IDC_MK2, m_Mk2);
	DDX_Control(pDX, IDC_CLEAR, m_Clear);
	DDX_Control(pDX, IDC_V_AMPLIFY, m_vAmplify);
	DDX_Control(pDX, IDC_V_DEFLATE, m_vDeflate);
	DDX_Control(pDX, IDC_EVENT_NEXT, m_EventNext);
	DDX_Control(pDX, IDC_EVENT_PRE, m_EventPre);
	DDX_Control(pDX, IDC_SOR_DRAWAREA, m_sorDrawStatic);
	DDX_Control(pDX, IDC_UNIT_DIV_V, m_unitDivV);
	DDX_Control(pDX, IDC_SPIN_OFFSET, m_spinOffset);
	DDX_Control(pDX, IDC_EDIT_OFFSET, m_editOffset);
	DDX_Control(pDX, IDC_CURSOR_SELECT, m_btnCursor);
	DDX_Control(pDX, IDC_DISTANCE_LEFT, m_distance_left);
	DDX_Control(pDX, IDC_UNIT_DIV, m_unit_div);
	DDX_Control(pDX, IDC_DISTANCE_RIGHT, m_distance_right);
	DDX_Control(pDX, IDC_RADIO1, m_AorB);
	DDX_Check(pDX, IDC_CHECK_AB_SELECT, m_bCheckSelectAB);
	DDX_Control(pDX, IDC_CHECK_AB_SELECT, m_ABCheck);
}

BEGIN_MESSAGE_MAP(CWaveform, CFormView)
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_EVENT_PRE, &CWaveform::OnBnClickedEventPre)
	ON_BN_CLICKED(IDC_EVENT_NEXT, &CWaveform::OnBnClickedEventNext)
	ON_BN_CLICKED(IDC_EVENT_ADD, &CWaveform::OnBnClickedEventAdd)
	ON_BN_CLICKED(IDC_EVENT_DELETE, &CWaveform::OnBnClickedEventDelete)
	ON_BN_CLICKED(IDC_HOR_AMPLIFY, &CWaveform::OnBnClickedHorAmplify)
	ON_BN_CLICKED(IDC_HOR_DEFLATE, &CWaveform::OnBnClickedHorDeflate)
	ON_BN_CLICKED(IDC_RETURN, &CWaveform::OnBnClickedReturn)
	ON_BN_CLICKED(IDC_V_DEFLATE, &CWaveform::OnBnClickedVDeflate)
	ON_BN_CLICKED(IDC_V_AMPLIFY, &CWaveform::OnBnClickedVAmplify)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_OFFSET, &CWaveform::OnDeltaposSpinOffset)
	ON_BN_CLICKED(IDC_CURSOR_SELECT, &CWaveform::OnBnClickedCursorSelect)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_RADIO1, &CWaveform::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CWaveform::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_CHECK_AB_SELECT, &CWaveform::OnBnClickedCheckAbSelect)
END_MESSAGE_MAP()


// CWaveform diagnostics

#ifdef _DEBUG
void CWaveform::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CWaveform::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CWaveform message handlers

void CWaveform::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	CRect rcWaveForm;
	GetWindowRect(rcWaveForm);
	m_sorDrawStatic.GetWindowRect(m_rectSorDrawStatic);
	ScreenToClient(m_rectSorDrawStatic);
	// TODO: Add your specialized code here and/or call the base class
	GetParentFrame()->SetWindowText(_T("Waveform"));//设置子窗口标题

	//获取 Unit Div V窗口大小
	m_unitDivV.GetWindowRect(m_rectUnitDivV);
	ScreenToClient(&m_rectUnitDivV);
	// Div Offset
	m_editOffset.GetWindowRect(m_rectOffset);
	ScreenToClient(&m_rectOffset);
	m_spinOffset.GetWindowRect(&m_rectSpinOffset);
	ScreenToClient(&m_rectSpinOffset);

	//获取m_distance_left窗口大小
	m_distance_left.GetWindowRect(m_rectdistance_left);
	ScreenToClient(&m_rectdistance_left);
	//获取m_unit_div窗口大小
	m_unit_div.GetWindowRect(m_rectunit_div);
	ScreenToClient(&m_rectunit_div);
	//获取m_distance_right窗口大小
	m_distance_right.GetWindowRect(m_rectdistance_right);
	ScreenToClient(&m_rectdistance_right);

	//******设置各按钮状态
	m_hAmplify.SetIcon(IDI_AMPLIFY,32,32,IDI_L_AMPLIFY,32,32);
	m_hAmplify.SetColor(CButtonST::BTNST_COLOR_FG_IN,RGB(0,128,0));
    m_hAmplify.SetTooltipText(_T("水平放大"));
	m_hAmplify.GetWindowRect(m_recthAmplify);
	ScreenToClient(&m_recthAmplify);

	m_hDeflate.SetIcon(IDI_DEFLATE,32,32,IDI_L_DEFLATE,32,32);
	m_hDeflate.SetColor(CButtonST::BTNST_COLOR_FG_IN,RGB(0,128,0));
	m_hDeflate.SetTooltipText(_T("水平缩小"));
	m_hDeflate.GetWindowRect(m_recthDeflate);
	ScreenToClient(&m_recthDeflate);

	m_vAmplify.SetIcon(IDI_AMPLIFY,32,32,IDI_L_AMPLIFY,32,32);
	m_vAmplify.SetColor(CButtonST::BTNST_COLOR_FG_IN,RGB(0,128,0));
	m_vAmplify.SetTooltipText(_T("竖直放大"));
	m_vAmplify.GetWindowRect(m_rectvAmplify);
	ScreenToClient(&m_rectvAmplify);

	m_vDeflate.SetIcon(IDI_DEFLATE,32,32,IDI_L_DEFLATE,32,32);
	m_vDeflate.SetColor(CButtonST::BTNST_COLOR_FG_IN,RGB(0,128,0));
	m_vDeflate.SetTooltipText(_T("竖直缩小"));
	m_vDeflate.GetWindowRect(m_rectvDeflate);
	ScreenToClient(&m_rectvDeflate);

	m_Return.SetIcon(IDI_RETURN,32,32,IDI_L_RETURN,32,32);
	m_Return.SetColor(CButtonST::BTNST_COLOR_FG_IN,RGB(0,128,0));
	m_Return.SetTooltipText(_T("原始大小"));
	m_Return.GetWindowRect(m_rectReturn);
	ScreenToClient(&m_rectReturn);

	//m_btnCursor.SetIcon(IDI_CURSOR_A,32,32,IDI_CURSOR_A,32,32);
	m_btnCursor.SetColor(CButtonST::BTNST_COLOR_FG_IN,RGB(0,128,0));
	//m_btnCursor.SetWindowText(_T("A"));
	m_btnCursor.SetIcon(IDI_L_A,32,32);
	m_btnCursor.SetTooltipText(_T("光标A\\B\\AB切换"));
	m_btnCursor.GetWindowRect(m_rectCursor);
	ScreenToClient(&m_rectCursor);

	/*m_Auto.SetIcon(IDI_L_AUTO,32,32,IDI_AUTO,32,32);
	m_Auto.SetColor(CButtonST::BTNST_COLOR_FG_IN,RGB(0,128,0));
	m_Auto.SetTooltipText(_T("自动"));
	m_Auto.DrawTransparent();*/

	m_EventPre.SetIcon(IDI_PRE,32,32,IDI_L_PRE,32,32);
	m_EventPre.SetColor(CButtonST::BTNST_COLOR_FG_IN,RGB(0,128,0));
	m_EventPre.SetTooltipText(_T("上一事件"));
	m_EventPre.GetWindowRect(m_rectEventPre);
	ScreenToClient(&m_rectEventPre);

	m_EventNext.SetIcon(IDI_NEXT,32,32,IDI_L_NEXT,32,32);
	m_EventNext.SetColor(CButtonST::BTNST_COLOR_FG_IN,RGB(0,128,0));
	m_EventNext.SetTooltipText(_T("下一事件"));
	m_EventNext.GetWindowRect(m_rectEventNext);
	ScreenToClient(&m_rectEventNext);

	m_EventAdd.SetIcon(IDI_ADD,32,32,IDI_L_ADD,32,32);
	m_EventAdd.SetColor(CButtonST::BTNST_COLOR_FG_IN,RGB(0,128,0));
	m_EventAdd.SetTooltipText(_T("添加事件"));
	m_EventAdd.GetWindowRect(m_rectEventAdd);
	ScreenToClient(&m_rectEventAdd);

	m_EventDelete.SetIcon(IDI_DELETE,32,32,IDI_L_DELETE,32,32);
	m_EventDelete.SetColor(CButtonST::BTNST_COLOR_FG_IN,RGB(0,128,0));
	m_EventDelete.SetTooltipText(_T("删除事件"));
	m_EventDelete.GetWindowRect(m_rectEventDelete);
	ScreenToClient(&m_rectEventDelete);

	//m_Mk1.SetIcon(IDI_AMPLIFY,32,32);
	m_Mk1.SetColor(CButtonST::BTNST_COLOR_FG_IN,RGB(0,128,0));
	m_Mk1.SetTooltipText(_T("MK1"));
	m_Mk1.GetWindowRect(m_rectMk1);
	ScreenToClient(&m_rectMk1);

	//m_Mk2.SetIcon(IDI_AMPLIFY,32,32);
	m_Mk2.SetColor(CButtonST::BTNST_COLOR_FG_IN,RGB(0,128,0));
	m_Mk2.SetTooltipText(_T("MK2"));
	m_Mk2.GetWindowRect(m_rectMk2);
	ScreenToClient(&m_rectMk2);

	m_Clear.SetIcon(IDI_L_CLEAR,32,32,IDI_CLEAR,32,32);
	m_Clear.SetColor(CButtonST::BTNST_COLOR_FG_IN,RGB(0,128,0));
	m_Clear.SetTooltipText(_T("消除"));
	m_Clear.GetWindowRect(m_rectClear);

	//初始化旋钮
	m_spinOffset.SetSpin(1,0,300,1);

	//cwcq2013.03.29
	/*m_sorDrawStatic.SetVerScoll();
	m_sorDrawStatic.SetHorScroll();*/
	m_sorDrawStatic.InitScrollInfo();
	InitCurveColor();

	CButton* radio=(CButton*)GetDlgItem(IDC_RADIO1);
	radio->SetCheck(1);

	m_AorB.GetWindowRect(m_rectAorB);
	ScreenToClient(&m_rectAorB);

	GetDlgItem(IDC_RADIO2)->GetWindowRect(m_rectAandB);
	ScreenToClient(&m_rectAandB);

	m_ABCheck.GetWindowRect(m_rectABCheck);
	ScreenToClient(&m_rectABCheck);

	if(!m_bCheckSelectAB)
	{
		GetDlgItem(IDC_RADIO1)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO2)->EnableWindow(FALSE);
	}
}

void CWaveform::InitCurveColor()
{
	CString strTrace1 = IniUtil::ReadSingleConfigInfo(_T("Color"), _T("Trace1"));
	COLORREF col1 = _wtol(strTrace1);
	g_COLORREF[0]=col1;

	CString strTrace2 = IniUtil::ReadSingleConfigInfo(_T("Color"), _T("Trace2"));
	COLORREF col2 = _wtol(strTrace2);
	g_COLORREF[1]=col2;

	CString strTrace3 = IniUtil::ReadSingleConfigInfo(_T("Color"), _T("Trace3"));
	COLORREF col3 = _wtol(strTrace3);
	g_COLORREF[2]=col3;

	CString strTrace4 = IniUtil::ReadSingleConfigInfo(_T("Color"), _T("Trace4"));
	COLORREF col4 = _wtol(strTrace4);
	g_COLORREF[3]=col4;

	CString strTrace5 = IniUtil::ReadSingleConfigInfo(_T("Color"), _T("Trace5"));
	COLORREF col5 = _wtol(strTrace5);
	g_COLORREF[4]=col5;

	CString strTrace6 = IniUtil::ReadSingleConfigInfo(_T("Color"), _T("Trace6"));
	COLORREF col6 = _wtol(strTrace6);
	g_COLORREF[5]=col6;

	CString strTrace7 = IniUtil::ReadSingleConfigInfo(_T("Color"), _T("Trace7"));
	COLORREF col7 = _wtol(strTrace7);
	g_COLORREF[6]=col7;

	CString strTrace8 = IniUtil::ReadSingleConfigInfo(_T("Color"), _T("Trace8"));
	COLORREF col8 = _wtol(strTrace8);
	g_COLORREF[7]=col8;

	CString strTraceBack = IniUtil::ReadSingleConfigInfo(_T("Color"), _T("Back"));
	COLORREF colBack = _wtol(strTraceBack);
	g_COLORREF[8]=colBack;

	CString strTraceGrid = IniUtil::ReadSingleConfigInfo(_T("Color"), _T("Grid"));
	COLORREF colGrid = _wtol(strTraceGrid);
	g_COLORREF[9]=colGrid;

	CString strTraceEventMarker = IniUtil::ReadSingleConfigInfo(_T("Color"), _T("EventMarker"));
	COLORREF colEventMarker = _wtol(strTraceEventMarker);
	g_COLORREF[10]=colEventMarker;

	CString strTraceCursor = IniUtil::ReadSingleConfigInfo(_T("Color"), _T("Cursor"));
	COLORREF colCursor = _wtol(strTraceCursor);
	g_COLORREF[11]=colCursor;
}


BOOL CWaveform::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
    CRect rcClient;
	GetClientRect(&rcClient);
	pDC->FillRect(&rcClient,&CBrush(RGB(128,128,255)));
    
	//return TRUE;
	return CFormView::OnEraseBkgnd(pDC);
}

void CWaveform::OnPostEraseBkgnd(CDC* pDC)
{
	m_hAmplify.SetBk(pDC);
	m_hDeflate.SetBk(pDC);
}

void CWaveform::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy); //避免出现滚动条,由CFormView改为CView

	if(GetDlgItem(IDC_SOR_DRAWAREA)->GetSafeHwnd() == NULL)
		return;

	//整个窗口大小
	CRect rect;
	GetWindowRect(&rect);
	//绘图区域大小
	CRect rectDraw;
	GetDlgItem(IDC_SOR_DRAWAREA)->GetWindowRect(&rectDraw);
	rectDraw.bottom = rect.bottom - DRAW_DIST-32;//zll,2012.9.3
	rectDraw.right = rect.right;
	ScreenToClient(&rectDraw);
	GetDlgItem(IDC_SOR_DRAWAREA)->MoveWindow(&rectDraw);
	//左面参数
	CRect rectDistLeft;
	GetDlgItem(IDC_DISTANCE_LEFT)->GetWindowRect(&rectDistLeft);
	int nDistLeftHeight = rectDistLeft.Height();
	rectDistLeft.bottom = rect.bottom - PARAM_DIST-32;
	rectDistLeft.top = rectDistLeft.bottom - nDistLeftHeight;
	ScreenToClient(&rectDistLeft);
	GetDlgItem(IDC_DISTANCE_LEFT)->MoveWindow(&rectDistLeft);
	
	CRect rectCursorDB;
	GetDlgItem(IDC_CURSOR_DB)->GetWindowRect(&rectCursorDB);
	int nCursorDBHeight = rectDistLeft.Height();
	rectCursorDB.bottom = rect.bottom - PARAM_DIST-32;
	rectCursorDB.top = rectCursorDB.bottom - nCursorDBHeight;
	ScreenToClient(&rectCursorDB);
	GetDlgItem(IDC_CURSOR_DB)->MoveWindow(&rectCursorDB);

	CRect rectUnitDiv;
	GetDlgItem(IDC_UNIT_DIV)->GetWindowRect(&rectUnitDiv);
	int nUnitDivHeight = rectUnitDiv.Height();
	rectUnitDiv.bottom = rect.bottom - PARAM_DIST-32;
	rectUnitDiv.top = rectUnitDiv.bottom - nUnitDivHeight;
	ScreenToClient(&rectUnitDiv);
	GetDlgItem(IDC_UNIT_DIV)->MoveWindow(&rectUnitDiv);

	CRect rectDistRight;
	GetDlgItem(IDC_DISTANCE_RIGHT)->GetWindowRect(&rectDistRight);
	int nDistRightHeight = rectUnitDiv.Height();
	rectDistRight.bottom = rect.bottom - PARAM_DIST-32;
	rectDistRight.top = rectDistRight.bottom - nDistRightHeight;
	ScreenToClient(&rectDistRight);
	GetDlgItem(IDC_DISTANCE_RIGHT)->MoveWindow(&rectDistRight);
}

void CWaveform::SetMainSettingMode(BOOL bNoraml)
{
	if (bNoraml)
	{
		m_leftDlg.ShowWindow(SW_SHOW);
		m_sorDrawStatic.MoveWindow(m_rectSorDrawStatic, TRUE);
		m_editOffset.MoveWindow(m_rectOffset, TRUE);
		m_spinOffset.MoveWindow(m_rectSpinOffset, TRUE);
		m_unitDivV.MoveWindow(m_rectUnitDivV, TRUE);
		m_btnCursor.MoveWindow(m_rectCursor, TRUE);
		m_hAmplify.MoveWindow(m_recthAmplify, TRUE);
		m_hDeflate.MoveWindow(m_recthDeflate, TRUE);
		m_vAmplify.MoveWindow(m_rectvAmplify, TRUE);
		m_vDeflate.MoveWindow(m_rectvDeflate, TRUE);
		m_Return.MoveWindow(m_rectReturn, TRUE);
		m_EventPre.MoveWindow(m_rectEventPre, TRUE);
		m_EventNext.MoveWindow(m_rectEventNext, TRUE);
		m_EventAdd.MoveWindow(m_rectEventAdd, TRUE);
		m_EventDelete.MoveWindow(m_rectEventDelete, TRUE);
		m_Mk1.MoveWindow(m_rectMk1, TRUE);
		m_Mk2.MoveWindow(m_rectMk2, TRUE);
		m_Clear.MoveWindow(m_rectClear, TRUE);
		m_distance_left.MoveWindow(m_rectdistance_left,TRUE);
		m_unit_div.MoveWindow(m_rectunit_div,TRUE);
		m_distance_right.MoveWindow(m_rectdistance_right,TRUE);
		m_AorB.MoveWindow(m_rectAorB,TRUE);
		GetDlgItem(IDC_RADIO2)->MoveWindow(m_rectAandB,TRUE);
		m_ABCheck.MoveWindow(m_rectABCheck, TRUE);   //lzy 2015.2.11

	}
	else
	{
		m_leftDlg.ShowWindow(SW_HIDE);
		//m_rectSorDrawStatic
		//m_sorDrawStatic.MoveWindow(m_rectSorDrawStatic.left-LEFT_DLG_WIDTH,m_rectSorDrawStatic.top,m_rectSorDrawStatic.Width()+LEFT_DLG_WIDTH,m_rectSorDrawStatic.Height(), TRUE);
        //zll 2013.5.23 变更MoveWindow函数第3个参数，由m_rectSorDrawStatic.Width()+LEFT_DLG_WIDTH变为m_rectSorDrawStatic.Width()，否则使用列表向导时，绘制曲线框区域的右滚动条被遮挡
		m_sorDrawStatic.MoveWindow(m_rectSorDrawStatic.left-LEFT_DLG_WIDTH,m_rectSorDrawStatic.top,m_rectSorDrawStatic.Width(),m_rectSorDrawStatic.Height(), TRUE);
		//Unit Div V
		m_unitDivV.MoveWindow(m_rectUnitDivV.left-LEFT_DLG_WIDTH,m_rectUnitDivV.top,m_rectUnitDivV.Width(),m_rectUnitDivV.Height(), TRUE);
		//Vertical Offset
		m_editOffset.MoveWindow(m_rectOffset.left-LEFT_DLG_WIDTH,m_rectOffset.top,m_rectOffset.Width(),m_rectOffset.Height(), TRUE);
		//Vertical Spin Offset
		m_spinOffset.MoveWindow(m_rectSpinOffset.left-LEFT_DLG_WIDTH,m_rectSpinOffset.top,m_rectSpinOffset.Width(),m_rectSpinOffset.Height(), TRUE);
		//******设置各按钮位置
		m_btnCursor.MoveWindow(m_rectCursor.left-LEFT_DLG_WIDTH,m_rectCursor.top,m_rectCursor.Width(),m_rectCursor.Height(), TRUE);
		m_hAmplify.MoveWindow(m_recthAmplify.left-LEFT_DLG_WIDTH,m_recthAmplify.top,m_recthAmplify.Width(),m_recthAmplify.Height(), TRUE);
		m_hDeflate.MoveWindow(m_recthDeflate.left-LEFT_DLG_WIDTH,m_recthDeflate.top,m_recthDeflate.Width(),m_recthDeflate.Height(), TRUE);
		m_vAmplify.MoveWindow(m_rectvAmplify.left-LEFT_DLG_WIDTH,m_rectvAmplify.top,m_rectvAmplify.Width(),m_rectvAmplify.Height(), TRUE);
		m_vDeflate.MoveWindow(m_rectvDeflate.left-LEFT_DLG_WIDTH,m_rectvDeflate.top,m_rectvDeflate.Width(),m_rectvDeflate.Height(), TRUE);
		m_Return.MoveWindow(m_rectReturn.left-LEFT_DLG_WIDTH,m_rectReturn.top,m_rectReturn.Width(),m_rectReturn.Height(), TRUE);
		m_EventPre.MoveWindow(m_rectEventPre.left-LEFT_DLG_WIDTH,m_rectEventPre.top,m_rectEventPre.Width(),m_rectEventPre.Height(), TRUE);
		m_EventNext.MoveWindow(m_rectEventNext.left-LEFT_DLG_WIDTH,m_rectEventNext.top,m_rectEventNext.Width(),m_rectEventNext.Height(), TRUE);
		m_EventAdd.MoveWindow(m_rectEventAdd.left-LEFT_DLG_WIDTH,m_rectEventAdd.top,m_rectEventAdd.Width(),m_rectEventAdd.Height(), TRUE);
		m_EventDelete.MoveWindow(m_rectEventDelete.left-LEFT_DLG_WIDTH,m_rectEventDelete.top,m_rectEventDelete.Width(),m_rectEventDelete.Height(), TRUE);
		m_Mk1.MoveWindow(m_rectMk1.left-LEFT_DLG_WIDTH,m_rectMk1.top,m_rectMk1.Width(),m_rectMk1.Height(), TRUE);
		m_Mk2.MoveWindow(m_rectMk2.left-LEFT_DLG_WIDTH,m_rectMk2.top,m_rectMk2.Width(),m_rectMk2.Height(), TRUE);
		m_Clear.MoveWindow(m_rectClear.left-LEFT_DLG_WIDTH,m_rectClear.top,m_rectClear.Width(),m_rectClear.Height(), TRUE);
		m_distance_left.MoveWindow(m_rectdistance_left.left-LEFT_DLG_WIDTH,m_rectdistance_left.top,m_rectdistance_left.Width(),m_rectdistance_left.Height(), TRUE);
		m_unit_div.MoveWindow(m_rectunit_div.left-LEFT_DLG_WIDTH,m_rectunit_div.top,m_rectunit_div.Width(),m_rectunit_div.Height(), TRUE);
		m_distance_right.MoveWindow(m_rectdistance_right.left-LEFT_DLG_WIDTH,m_rectdistance_right.top,m_rectdistance_right.Width(),m_rectdistance_right.Height(), TRUE);
		m_AorB.MoveWindow(m_rectAorB.left-LEFT_DLG_WIDTH,m_rectAorB.top,m_rectAorB.Width(),m_rectAorB.Height(), TRUE);
		GetDlgItem(IDC_RADIO2)->MoveWindow(m_rectAandB.left-LEFT_DLG_WIDTH,m_rectAandB.top,m_rectAandB.Width(),m_rectAandB.Height(), TRUE);
		m_ABCheck.MoveWindow(m_rectABCheck.left-LEFT_DLG_WIDTH,m_rectABCheck.top,m_rectABCheck.Width(),m_rectABCheck.Height(), TRUE);   //lzy 2015.2.11
	}
}

UINT threadProc(LPVOID lParam)
{
	CWaveform* dlg=(CWaveform*)lParam;
	//dlg->decode();
	return 0;
}

void CWaveform::CreateLeftDlg()
{
	//创建
	m_leftDlg.Create(CWaveFormLeftDlg::IDD,this);
	m_leftDlg.SetWindowPos(NULL,0,0,0,0,SWP_SHOWWINDOW|SWP_NOSIZE);
	m_leftDlg.ShowWindow(SW_SHOW);
	CRect rect;
	m_leftDlg.GetClientRect(&rect);
}


//刷新文件列表
void CWaveform::PopulateData()
{
	m_leftDlg.PopulateData();
}

int CWaveform::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFormView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// 创建左面对话框
	CreateLeftDlg();

	return 0;
}
void CWaveform::OnBnClickedEventPre()
{
	//m_sorDrawStatic.EventSelectUp();
	POSITION pos = MAIN->m_pEventFormView->m_Eventlist.GetFirstSelectedItemPosition();
	int nIndex = MAIN->m_pEventFormView->m_Eventlist.GetNextSelectedItem(pos);
	if (nIndex > 0)
	{
		nIndex--;
		m_sorDrawStatic.MoveEventList(nIndex);
		//重置选中项
		MAIN->m_pEventFormView->m_Eventlist.SetItemState(nIndex, LVIS_SELECTED, LVIS_SELECTED);
	}

	m_EventPre.SetTooltipText(strPreEvent);
}

void CWaveform::OnBnClickedEventNext()
{
	//m_sorDrawStatic.EventSelectDown();
	POSITION pos = MAIN->m_pEventFormView->m_Eventlist.GetFirstSelectedItemPosition();
	int nIndex =  MAIN->m_pEventFormView->m_Eventlist.GetNextSelectedItem(pos);
	if (nIndex >= 0 && nIndex < MAIN->m_pEventFormView->m_Eventlist.GetItemCount()-1)
	{
		nIndex++;
		m_sorDrawStatic.MoveEventList(nIndex);
		//重置选中项
		MAIN->m_pEventFormView->m_Eventlist.SetItemState(nIndex, LVIS_SELECTED, LVIS_SELECTED);
	}
	else
	{
		//置第一个为选中项
		if (nIndex > 0)
			MAIN->m_pEventFormView->m_Eventlist.SetItemState(0, LVIS_SELECTED, LVIS_SELECTED);
	}

	m_EventNext.SetTooltipText(strNextEvent);
}

void CWaveform::OnBnClickedEventAdd()
{
	//m_sorDrawStatic.AddEvent();
	TestResultInfo* pData = g_sorFileArray.sorFileList[g_sorFileArray.curSelection].pTestResultInfo;
	if (0==pData->EventList.vEventInfo.size())//zll 2013.6.19 防止出错
	{
		return;
	}
	int nSelectPoint = 0;
	if (m_nSelectCursor == CURSOR_SELECT::CURSOR_A)
	{
		nSelectPoint = m_sorDrawStatic.GetCurrentCursorIndex();
	}
	else if (m_nSelectCursor == CURSOR_SELECT::CURSOR_B)
	{
		nSelectPoint = m_sorDrawStatic.GetCurrentCursorIndex();
	}
	else
		return;

	int waveNum = pData->WaveLength; 
	__try
	{
		pData->m_GaborResult = m_endgabor.insertEvent(nSelectPoint, //所要插入的事件点的数据点编号
			waveNum, //曲线波长
			pData->PulseWidth, //曲线脉宽
			pData->pDataPoints, //测试点数据
			pData->DataPointsNum, //点数
			pData->m_GaborResult,
			pData->Refraction,//折射率zll
			pData->Frequency,
			g_sorFileArray.eventAnalConfig.BackScatter);//g_sorFileArray.eventAnalConfig.BackScatter典型散射系数
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		return;
	}

	GaborResultToEvent(pData);
	m_sorAssist.SetEvents(*pData,&m_sorDrawStatic.m_vAllData[g_sorFileArray.curSelection]);//zll 2013.7.12 注意g_sorFileArray.curSelection 是当前曲线

	//重绘
	m_sorDrawStatic.DrawCurves(&m_sorDrawStatic.m_dcCurve,-1);
	InvalidateRect(m_sorDrawStatic.m_rectCurve);
	MAIN->m_pEventFormView->FreshEventList();//刷新事件列表
	//重置选中项
	MAIN->m_pEventFormView->m_Eventlist.SetItemState(MAIN->m_pEventFormView->m_Eventlist.GetItemCount()-1, LVIS_SELECTED, LVIS_SELECTED);

	m_EventAdd.SetTooltipText(strAddEvent);
}

void CWaveform::OnBnClickedEventDelete()
{
	//m_sorDrawStatic.RemoveEvent();
	TestResultInfo* pCurveData = g_sorFileArray.sorFileList[g_sorFileArray.curSelection].pTestResultInfo;
	POSITION pos = MAIN->m_pEventFormView->m_Eventlist.GetFirstSelectedItemPosition();
	int nIndex = MAIN->m_pEventFormView->m_Eventlist.GetNextSelectedItem(pos);
	if ((nIndex < 0) || (nIndex > pCurveData->m_GaborResult.Valid_Num))
	{
		return;
	}
	int waveNum = (1550 == pCurveData->WaveLength) ? 1:0;
	__try{
		pCurveData->m_GaborResult = m_endgabor.deleteEvent(nIndex,
			waveNum,
			pCurveData->PulseWidth,
			pCurveData->pDataPoints,
			pCurveData->m_GaborResult,
			pCurveData->Refraction,//折射率zll
			pCurveData->Frequency,
			0.0f);
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		return;
	}
	GaborResultToEvent(pCurveData);////调用dll得到数据后转化为事件列表 wcq2012.06.08
	m_sorAssist.SetEvents(*pCurveData,&m_sorDrawStatic.m_vAllData[g_sorFileArray.curSelection]);//zll 2013.5.7
	//重绘
	m_sorDrawStatic.DrawCurves(&m_sorDrawStatic.m_dcCurve,-1);//zll 2013.6.6
	InvalidateRect(m_sorDrawStatic.m_rectCurve);
	MAIN->m_pEventFormView->FreshEventList();//刷新界面事件列表 
	//
	nIndex--;
	if (nIndex < 0)
	{
		nIndex = 0;
	}
	if (MAIN->m_pEventFormView->m_Eventlist.GetItemCount() >= nIndex)
	{
		MAIN->m_pEventFormView->m_Eventlist.SetItemState(nIndex, LVIS_SELECTED, LVIS_SELECTED);
	}
	//EventCursor();//重绘

	m_EventDelete.SetTooltipText(strDeleteEvent);
}

//水平放大
void CWaveform::OnBnClickedHorAmplify()
{
	//m_sorDrawStatic.CurveHorizonZoomIn();
	m_sorDrawStatic.ZoomInHorizonForBig();
	MAIN->m_pThumbFormView->Invalidate();//缩略图
	//设置横坐标文本
	SetHorUnitText();
	
	m_hAmplify.SetTooltipText(strH_Zoom_in);
}

//水平缩小
void CWaveform::OnBnClickedHorDeflate()
{
	//m_sorDrawStatic.CurveHorizonZoomOut();
	m_sorDrawStatic.ZoomOutHorizonForSmall();
	MAIN->m_pThumbFormView->Invalidate();//缩略图
	//设置横坐标文本
	SetHorUnitText();

	m_hDeflate.SetTooltipText(strH_Zoom_out);
}
//返回未缩放状态
void CWaveform::OnBnClickedReturn()
{
	//m_sorDrawStatic.OnVKReturn();
	MAIN->m_pWaveFormView->m_spinOffset.SetPos(0.0f);//zll 2013.7.2 曲线全部恢复后，IDC_SPIN_OFFSET也要置为0
	m_sorDrawStatic.ZoomReset();
	MAIN->m_pThumbFormView->Invalidate();//缩略图
	//设置横坐标文本
	SetHorUnitText();
	//设置纵坐标文本
	SetVerUnitText();

	m_Return.SetTooltipText(strScaleInit);
}

void CWaveform::OnBnClickedVDeflate()
{
	//m_sorDrawStatic.CurveVerticalZoomOut();
	m_sorDrawStatic.ZoomOutVertialForSmall();
	MAIN->m_pThumbFormView->Invalidate();//缩略图
	SetVerUnitText();

	m_vDeflate.SetTooltipText(strV_Zoom_out);
}

void CWaveform::OnBnClickedVAmplify()
{
	//m_sorDrawStatic.CurveVerticalZoomIn();
	m_sorDrawStatic.ZoomInVertialForBig();
	MAIN->m_pThumbFormView->Invalidate();//缩略图
	SetVerUnitText();

	m_vAmplify.SetTooltipText(strV_Zoom_in);
}

void CWaveform::OnBnClickedCursorSelect()
{
	SetABType(_T("A"));
	//switch (m_sorDrawStatic.m_nSelectCursor)
	//{
	//case CURSOR_SELECT::CURSOR_A:
	//	m_sorDrawStatic.m_nSelectCursor = CURSOR_SELECT::CURSOR_B;
	//	m_btnCursor.SetIcon(IDI_L_B,32,32);
	//	m_sorDrawStatic.m_bZoomOK = FALSE;
	//	//m_btnCursor.SetIcon(IDI_CURSOR_B,32,32,IDI_CURSOR_B,32,32);
	//	break;
	//case CURSOR_SELECT::CURSOR_B:
	//	m_sorDrawStatic.m_nSelectCursor = CURSOR_SELECT::CURSOR_AB;
	//	m_btnCursor.SetIcon(IDI_L_AB,32,32);
	//	m_sorDrawStatic.m_bZoomOK = FALSE;
	//	//m_btnCursor.SetIcon(IDI_CURSOR_AB,32,32,IDI_CURSOR_AB,32,32);
	//	break;
	//case CURSOR_SELECT::CURSOR_AB:
	//	m_sorDrawStatic.m_nSelectCursor = CURSOR_SELECT::CURSOR_A;
	//	m_btnCursor.SetIcon(IDI_L_A,32,32);
	//	m_sorDrawStatic.m_bZoomOK = FALSE;
	//	//m_btnCursor.SetIcon(IDI_CURSOR_A,32,32,IDI_CURSOR_A,32,32);
	//	break;
	//}
	//m_btnCursor.SetTooltipText(_T("光标A\\B\\AB切换")); //设置ToolTip
	////重绘绘图区
	//m_sorDrawStatic.Invalidate();
}

void CWaveform::OnDeltaposSpinOffset(NMHDR *pNMHDR, LRESULT *pResult)
{
	if (0==m_sorDrawStatic.m_vAllData.size())
	{
		return;
	}
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	TestResultInfo* pData =  g_sorFileArray.sorFileList[g_sorFileArray.curSelection].pTestResultInfo;
	if (NULL == pData)
		return;

	//float fOldVal = g_sorFileArray.sorFileList[g_sorFileArray.curSelection].fOffset;
	float fOldVal=m_sorDrawStatic.m_vAllData[m_sorDrawStatic.m_nCurveIndex].m_nOffsetY;//zll 2013.7.2
	float fNewVal = m_spinOffset.GetPos();
	m_sorDrawStatic.m_vAllData[m_sorDrawStatic.m_nCurveIndex].m_nOffsetY=-fNewVal;
	m_sorDrawStatic.DrawCurves(&m_sorDrawStatic.m_dcCurve,-1);
	m_sorDrawStatic.Invalidate();
	//if (fOldVal < fNewVal) //Up spin//cwcq2013.03.29
	//{
	//	m_sorDrawStatic.CurveVerMove(pData, 1, true);
	//}
	//else if(fOldVal > fNewVal) //Down spin
	//{
	//	m_sorDrawStatic.CurveVerMove(pData, -1, true);
	//}
	//m_sorDrawStatic.InvalidateAll(); //重绘
	////保存新值
	//g_sorFileArray.sorFileList[g_sorFileArray.curSelection].fOffset = fNewVal;

	*pResult = 0;
}

void CWaveform::SetOffsetVal(float fOffset)
{
	m_spinOffset.SetPos(fOffset);
}

void CWaveform::SetHorUnitText() //设置横刻度文本
{
	if (g_sorFileArray.totalCurveNumber == 0) //没有曲线加载
		return;

	//当期选择曲线
	//TestResultInfo* pCurveData = g_sorFileArray.sorFileList[m_sorDrawStatic.m_nMaxIndex].pTestResultInfo;
	if(_T("m")==g_sorFileArray.waveConfig.DistUnit)//横坐标单位为km
	{
		//设置横坐标文本 开始距离
		//float fStartVal = m_sorDrawStatic.Get_XCoord(m_sorDrawStatic.m_nStartPoint, pCurveData)/1000;
		float fStartVal = m_sorDrawStatic.m_fRangeX[OTDR_TRACE_MIN]/1000;//zll 2013.5.10
		if(3==g_sorFileArray.waveConfig.UnitDistFig)
			m_strStartDis.Format(_T("%.3fkm"), fStartVal);
		else if(4==g_sorFileArray.waveConfig.UnitDistFig)
			m_strStartDis.Format(_T("%.4fkm"), fStartVal);
		else if(5==g_sorFileArray.waveConfig.UnitDistFig)
			m_strStartDis.Format(_T("%.5fkm"), fStartVal);
		GetDlgItem(IDC_DISTANCE_LEFT)->SetWindowText(m_strStartDis);
		//设置横坐标文本 结束距离
		float fEndVal = m_sorDrawStatic.m_fRangeX[OTDR_TRACE_MAX]/1000;//zll 2013.5.10
		if(3==g_sorFileArray.waveConfig.UnitDistFig)
			m_strEndDis.Format(_T("%.3fkm"), fEndVal);
		else if(4==g_sorFileArray.waveConfig.UnitDistFig)
			m_strEndDis.Format(_T("%.4fkm"), fEndVal);
		else if(5==g_sorFileArray.waveConfig.UnitDistFig)
			m_strEndDis.Format(_T("%.5fkm"), fEndVal);
		GetDlgItem(IDC_DISTANCE_RIGHT)->SetWindowText(m_strEndDis);
		//设置横坐标文本 单位长度
		if(3==g_sorFileArray.waveConfig.UnitDistFig)
			m_strUnitDis.Format(_T("%.3fkm/div"), (fEndVal-fStartVal)/10);
		else if(4==g_sorFileArray.waveConfig.UnitDistFig)
			m_strUnitDis.Format(_T("%.4fkm/div"), (fEndVal-fStartVal)/10);
		else if(5==g_sorFileArray.waveConfig.UnitDistFig)
			m_strUnitDis.Format(_T("%.5fkm/div"), (fEndVal-fStartVal)/10);
		GetDlgItem(IDC_UNIT_DIV)->SetWindowText(m_strUnitDis);
	}
	else if(_T("mi")==g_sorFileArray.waveConfig.DistUnit)//横坐标单位为miles
	{
		//设置横坐标文本 开始距离
		float fStartVal = m_sorDrawStatic.m_fRangeX[OTDR_TRACE_MIN]/1000;//zll 2013.5.10
		fStartVal=KmeterToOther(fStartVal,UNIT_SHOW_MILE);
		if(3==g_sorFileArray.waveConfig.UnitDistFig)
			m_strStartDis.Format(_T("%.3fmiles"), fStartVal);
		else if(4==g_sorFileArray.waveConfig.UnitDistFig)
			m_strStartDis.Format(_T("%.4fmiles"), fStartVal);
		else if(5==g_sorFileArray.waveConfig.UnitDistFig)
			m_strStartDis.Format(_T("%.5fmiles"), fStartVal);
		GetDlgItem(IDC_DISTANCE_LEFT)->SetWindowText(m_strStartDis);
		//设置横坐标文本 结束距离
		float fEndVal = m_sorDrawStatic.m_fRangeX[OTDR_TRACE_MAX]/1000;//zll 2013.5.10
		fEndVal=KmeterToOther(fEndVal,UNIT_SHOW_MILE);
		if(3==g_sorFileArray.waveConfig.UnitDistFig)
			m_strEndDis.Format(_T("%.3fmiles"), fEndVal);
		else if(4==g_sorFileArray.waveConfig.UnitDistFig)
			m_strEndDis.Format(_T("%.4fmiles"), fEndVal);
		else if(5==g_sorFileArray.waveConfig.UnitDistFig)
			m_strEndDis.Format(_T("%.5fmiles"), fEndVal);
		GetDlgItem(IDC_DISTANCE_RIGHT)->SetWindowText(m_strEndDis);
		//设置横坐标文本 单位长度
		if(3==g_sorFileArray.waveConfig.UnitDistFig)
			m_strUnitDis.Format(_T("%.3fmiles/div"), (fEndVal-fStartVal)/10);
		else if(4==g_sorFileArray.waveConfig.UnitDistFig)
			m_strUnitDis.Format(_T("%.4fmiles/div"), (fEndVal-fStartVal)/10);
		else if(5==g_sorFileArray.waveConfig.UnitDistFig)
			m_strUnitDis.Format(_T("%.5fmiles/div"), (fEndVal-fStartVal)/10);
		GetDlgItem(IDC_UNIT_DIV)->SetWindowText(m_strUnitDis);
	}
	else if(_T("ft")==g_sorFileArray.waveConfig.DistUnit)//横坐标单位为kft
	{
		//设置横坐标文本 开始距离
		float fStartVal = m_sorDrawStatic.m_fRangeX[OTDR_TRACE_MIN]/1000;//zll 2013.5.10
		fStartVal=KmeterToOther(fStartVal,UNIT_SHOW_KFEET);
		if(3==g_sorFileArray.waveConfig.UnitDistFig)
			m_strStartDis.Format(_T("%.3fkft"), fStartVal);
		else if(4==g_sorFileArray.waveConfig.UnitDistFig)
			m_strStartDis.Format(_T("%.4fkft"), fStartVal);
		else if(5==g_sorFileArray.waveConfig.UnitDistFig)
			m_strStartDis.Format(_T("%.5fkft"), fStartVal);
		GetDlgItem(IDC_DISTANCE_LEFT)->SetWindowText(m_strStartDis);
		//设置横坐标文本 结束距离
		float fEndVal = m_sorDrawStatic.m_fRangeX[OTDR_TRACE_MAX]/1000;//zll 2013.5.10
		fEndVal=KmeterToOther(fEndVal,UNIT_SHOW_KFEET);
		if(3==g_sorFileArray.waveConfig.UnitDistFig)
			m_strEndDis.Format(_T("%.3fkft"), fEndVal);
		else if(4==g_sorFileArray.waveConfig.UnitDistFig)
			m_strEndDis.Format(_T("%.4fkft"), fEndVal);
		else if(5==g_sorFileArray.waveConfig.UnitDistFig)
			m_strEndDis.Format(_T("%.5fkft"), fEndVal);
		GetDlgItem(IDC_DISTANCE_RIGHT)->SetWindowText(m_strEndDis);
		//设置横坐标文本 单位长度
		if(3==g_sorFileArray.waveConfig.UnitDistFig)
			m_strUnitDis.Format(_T("%.3fkft/div"), (fEndVal-fStartVal)/10);
		else if(4==g_sorFileArray.waveConfig.UnitDistFig)
			m_strUnitDis.Format(_T("%.4fkft/div"), (fEndVal-fStartVal)/10);
		else if(5==g_sorFileArray.waveConfig.UnitDistFig)
			m_strUnitDis.Format(_T("%.5fkft/div"), (fEndVal-fStartVal)/10);
		GetDlgItem(IDC_UNIT_DIV)->SetWindowText(m_strUnitDis);
	}
}

void CWaveform::SetHorUnitText(INT m_nCurveIndex) //设置横刻度文本
{
	if (g_sorFileArray.totalCurveNumber == 0) //没有曲线加载
		return;

	//当期选择曲线
	//TestResultInfo* pCurveData = g_sorFileArray.sorFileList[m_sorDrawStatic.m_nMaxIndex].pTestResultInfo;
	if(_T("m")==g_sorFileArray.waveConfig.DistUnit)//横坐标单位为km
	{
		//设置横坐标文本 开始距离
		//float fStartVal = m_sorDrawStatic.Get_XCoord(m_sorDrawStatic.m_nStartPoint, pCurveData)/1000;
		float fStartVal = m_sorDrawStatic.m_fRangeX[OTDR_TRACE_MIN]/1000;//zll 2013.5.10
		if(3==g_sorFileArray.waveConfig.UnitDistFig)
			m_strStartDis.Format(_T("%.3fkm"), fStartVal);
		else if(4==g_sorFileArray.waveConfig.UnitDistFig)
			m_strStartDis.Format(_T("%.4fkm"), fStartVal);
		else if(5==g_sorFileArray.waveConfig.UnitDistFig)
			m_strStartDis.Format(_T("%.5fkm"), fStartVal);
		GetDlgItem(IDC_DISTANCE_LEFT)->SetWindowText(m_strStartDis);
		//设置横坐标文本 结束距离
		float fEndVal = m_sorDrawStatic.m_vAllData[m_nCurveIndex].m_fLength/1000;//zll 2013.5.10
		if(3==g_sorFileArray.waveConfig.UnitDistFig)
			m_strEndDis.Format(_T("%.3fkm"), fEndVal);
		else if(4==g_sorFileArray.waveConfig.UnitDistFig)
			m_strEndDis.Format(_T("%.4fkm"), fEndVal);
		else if(5==g_sorFileArray.waveConfig.UnitDistFig)
			m_strEndDis.Format(_T("%.5fkm"), fEndVal);
		GetDlgItem(IDC_DISTANCE_RIGHT)->SetWindowText(m_strEndDis);
		//设置横坐标文本 单位长度
		if(3==g_sorFileArray.waveConfig.UnitDistFig)
			m_strUnitDis.Format(_T("%.3fkm/div"), (fEndVal-fStartVal)/10);
		else if(4==g_sorFileArray.waveConfig.UnitDistFig)
			m_strUnitDis.Format(_T("%.4fkm/div"), (fEndVal-fStartVal)/10);
		else if(5==g_sorFileArray.waveConfig.UnitDistFig)
			m_strUnitDis.Format(_T("%.5fkm/div"), (fEndVal-fStartVal)/10);
		GetDlgItem(IDC_UNIT_DIV)->SetWindowText(m_strUnitDis);
	}
	else if(_T("mi")==g_sorFileArray.waveConfig.DistUnit)//横坐标单位为miles
	{
		//设置横坐标文本 开始距离
		float fStartVal = m_sorDrawStatic.m_fRangeX[OTDR_TRACE_MIN]/1000;//zll 2013.5.10
		fStartVal=KmeterToOther(fStartVal,UNIT_SHOW_MILE);
		if(3==g_sorFileArray.waveConfig.UnitDistFig)
			m_strStartDis.Format(_T("%.3fmiles"), fStartVal);
		else if(4==g_sorFileArray.waveConfig.UnitDistFig)
			m_strStartDis.Format(_T("%.4fmiles"), fStartVal);
		else if(5==g_sorFileArray.waveConfig.UnitDistFig)
			m_strStartDis.Format(_T("%.5fmiles"), fStartVal);
		GetDlgItem(IDC_DISTANCE_LEFT)->SetWindowText(m_strStartDis);
		//设置横坐标文本 结束距离
		float fEndVal = m_sorDrawStatic.m_vAllData[m_nCurveIndex].m_fLength/1000;//zll 2013.5.10
		fEndVal=KmeterToOther(fEndVal,UNIT_SHOW_MILE);
		if(3==g_sorFileArray.waveConfig.UnitDistFig)
			m_strEndDis.Format(_T("%.3fmiles"), fEndVal);
		else if(4==g_sorFileArray.waveConfig.UnitDistFig)
			m_strEndDis.Format(_T("%.4fmiles"), fEndVal);
		else if(5==g_sorFileArray.waveConfig.UnitDistFig)
			m_strEndDis.Format(_T("%.5fmiles"), fEndVal);
		GetDlgItem(IDC_DISTANCE_RIGHT)->SetWindowText(m_strEndDis);
		//设置横坐标文本 单位长度
		if(3==g_sorFileArray.waveConfig.UnitDistFig)
			m_strUnitDis.Format(_T("%.3fmiles/div"), (fEndVal-fStartVal)/10);
		else if(4==g_sorFileArray.waveConfig.UnitDistFig)
			m_strUnitDis.Format(_T("%.4fmiles/div"), (fEndVal-fStartVal)/10);
		else if(5==g_sorFileArray.waveConfig.UnitDistFig)
			m_strUnitDis.Format(_T("%.5fmiles/div"), (fEndVal-fStartVal)/10);
		GetDlgItem(IDC_UNIT_DIV)->SetWindowText(m_strUnitDis);
	}
	else if(_T("ft")==g_sorFileArray.waveConfig.DistUnit)//横坐标单位为kft
	{
		//设置横坐标文本 开始距离
		float fStartVal = m_sorDrawStatic.m_fRangeX[OTDR_TRACE_MIN]/1000;//zll 2013.5.10
		fStartVal=KmeterToOther(fStartVal,UNIT_SHOW_KFEET);
		if(3==g_sorFileArray.waveConfig.UnitDistFig)
			m_strStartDis.Format(_T("%.3fkft"), fStartVal);
		else if(4==g_sorFileArray.waveConfig.UnitDistFig)
			m_strStartDis.Format(_T("%.4fkft"), fStartVal);
		else if(5==g_sorFileArray.waveConfig.UnitDistFig)
			m_strStartDis.Format(_T("%.5fkft"), fStartVal);
		GetDlgItem(IDC_DISTANCE_LEFT)->SetWindowText(m_strStartDis);
		//设置横坐标文本 结束距离
		float fEndVal = m_sorDrawStatic.m_vAllData[m_nCurveIndex].m_fLength/1000;//zll 2013.5.10
		fEndVal=KmeterToOther(fEndVal,UNIT_SHOW_KFEET);
		if(3==g_sorFileArray.waveConfig.UnitDistFig)
			m_strEndDis.Format(_T("%.3fkft"), fEndVal);
		else if(4==g_sorFileArray.waveConfig.UnitDistFig)
			m_strEndDis.Format(_T("%.4fkft"), fEndVal);
		else if(5==g_sorFileArray.waveConfig.UnitDistFig)
			m_strEndDis.Format(_T("%.5fkft"), fEndVal);
		GetDlgItem(IDC_DISTANCE_RIGHT)->SetWindowText(m_strEndDis);
		//设置横坐标文本 单位长度
		if(3==g_sorFileArray.waveConfig.UnitDistFig)
			m_strUnitDis.Format(_T("%.3fkft/div"), (fEndVal-fStartVal)/10);
		else if(4==g_sorFileArray.waveConfig.UnitDistFig)
			m_strUnitDis.Format(_T("%.4fkft/div"), (fEndVal-fStartVal)/10);
		else if(5==g_sorFileArray.waveConfig.UnitDistFig)
			m_strUnitDis.Format(_T("%.5fkft/div"), (fEndVal-fStartVal)/10);
		GetDlgItem(IDC_UNIT_DIV)->SetWindowText(m_strUnitDis);
	}
}

void CWaveform::SetHorUnitText(float nHorBalFactor) //设置横刻度文本
{
	////if (g_sorFileArray.totalCurveNumber == 0) //没有曲线加载
	////	return;

	//////当期选择曲线
	////TestResultInfo* pCurveData = g_sorFileArray.sorFileList[m_sorDrawStatic.m_nMaxIndex].pTestResultInfo;
	//////设置横坐标文本 开始距离
	////float fStartVal = m_sorDrawStatic.Get_XCoord(m_sorDrawStatic.m_nStartPoint, pCurveData)/1000;
	////float fEndVal = m_sorDrawStatic.Get_XCoord(m_sorDrawStatic.m_nMaxPoint, pCurveData)/1000;
	////float fTempStartVal = fStartVal + (fEndVal-fStartVal) * nHorBalFactor;
	////float fTempEndVal = fTempStartVal + (fEndVal-fStartVal);
	////CString strStartVal;
	////CString strEndVal;
	////if(3==g_sorFileArray.waveConfig.UnitDistFig)
	////{
	////	strStartVal.Format(_T("%.3fkm"), fTempStartVal);
	////	//设置横坐标文本 结束距离
	////	strEndVal.Format(_T("%.3fkm"), fTempEndVal);
	////}
	////else if(4==g_sorFileArray.waveConfig.UnitDistFig)
	////{
	////	strStartVal.Format(_T("%.4fkm"), fTempStartVal);
	////	//设置横坐标文本 结束距离
	////	strEndVal.Format(_T("%.4fkm"), fTempEndVal);
	////}
	////else if(5==g_sorFileArray.waveConfig.UnitDistFig)
	////{
	////	strStartVal.Format(_T("%.5fkm"), fTempStartVal);
	////	//设置横坐标文本 结束距离
	////	strEndVal.Format(_T("%.5fkm"), fTempEndVal);
	////}
	////GetDlgItem(IDC_DISTANCE_LEFT)->SetWindowText(strStartVal);
	////GetDlgItem(IDC_DISTANCE_RIGHT)->SetWindowText(strEndVal);
}

void CWaveform::SetVerUnitText() //设置纵刻度文本
{
	if (g_sorFileArray.totalCurveNumber == 0) //没有曲线加载
		return;

	//设置纵坐标文本 单位长度
	float fUnitDiv = (m_sorDrawStatic.m_fRangeY[OTDR_TRACE_MAX] - m_sorDrawStatic.m_fRangeY[OTDR_TRACE_MIN]) / COUNT_GRIDLINE_VERTICAL;
	if(1==g_sorFileArray.waveConfig.UnitDbFig)
		m_strUnitDiv.Format(_T("%.1fdB/Div"), fUnitDiv);
	else if(2==g_sorFileArray.waveConfig.UnitDbFig)
		m_strUnitDiv.Format(_T("%.2fdB/Div"), fUnitDiv);
	else if(3==g_sorFileArray.waveConfig.UnitDbFig)
		m_strUnitDiv.Format(_T("%.3fdB/Div"), fUnitDiv);
	GetDlgItem(IDC_UNIT_DIV_V)->SetWindowText(m_strUnitDiv);
}

//调用dll得到数据后转化为事件列表
void CWaveform::GaborResultToEvent(TestResultInfo* pCurveData)
{
	pCurveData->EventList.vEventInfo.clear();
	EventInfo eventinfo;
	int len = pCurveData->m_GaborResult.Valid_Num;//wcq2012.06.07
	float fDisTwo=m_sorDrawStatic.Get_XCoord(1,pCurveData);
	for(int i=0;i<len;i++)
	{
		if (0==fDisTwo)
		{
			int ff=0;
		}//eif
		eventinfo.index = pCurveData->m_GaborResult.Event_Array[i];//下标，第几点
		eventinfo.nEventType = pCurveData->m_GaborResult.Reflect_Type[i];//事件类型
		eventinfo.fLocation = fDisTwo * (pCurveData->m_GaborResult.Event_Array[i]+1);;//Get_XCoord(sGabor_Result..Event_Array[i],pCurveData);//两点间距离，以km为单位
		eventinfo.fEventloss = pCurveData->m_GaborResult.Event_Loss[i];
		eventinfo.fEventHigh = pCurveData->m_GaborResult.Event_Hight[i];
		eventinfo.fLineK = pCurveData->m_GaborResult.Line_k[i];// / fDisTwo;//2012.8.7 rzg修改
		eventinfo.fTotalLoss = pCurveData->m_GaborResult.Total_Loss[i];
		eventinfo.fReflect =pCurveData->m_GaborResult.Reflect;
		eventinfo.fTrailEnd = pCurveData->m_GaborResult.Trail_End;
		eventinfo.cEventType=pCurveData->m_GaborResult.Event_type[i];//event code
		pCurveData->EventList.vEventInfo.push_back(eventinfo);
	}//efor
}
//水平缩放所有曲线wcq 2011.09.13
void CWaveform::HorizonZooms()
{
	if (g_sorFileArray.totalCurveNumber == 0) //无曲线选择时
		return;

	if (!g_sorFileArray.waveConfig.MultiMode)
	{
		TestResultInfo* pCurve = g_sorFileArray.sorFileList[g_sorFileArray.curSelection].pTestResultInfo;
		HorizonZoom(pCurve);
	}
	else
	{
		//依次计算各条曲线起始点结束点
		for(int nCount = 0; nCount < g_sorFileArray.totalCurveNumber; nCount++)
		{
			TestResultInfo* pCurve = g_sorFileArray.sorFileList[nCount].pTestResultInfo;
			if (pCurve && pCurve->DataPointsNum > NUM_VALIDE_LEAST)
			{
				HorizonZoom(pCurve);
			}
		}
	}

	//重新设置绘图区域大小
	if (m_iHorizonZoomNum == 0) //无缩放
	{
		m_nDrawWidth = m_sorDrawStatic.m_rectCurve.Width();
	}
	else
	{
		m_nDrawWidth = m_sorDrawStatic.m_rectCurve.Width() * (m_iHorizonZoomNum+1);
	}
	//设置水平滚动条
	SetHorScroll();
}
//缩放一条曲线wcq 2011.09.21
void CWaveform::HorizonZoom(TestResultInfo* pData)
{
	if(CLEAR_DATAPOINTS_NUM == g_sorFileArray.sorFileList[g_sorFileArray.curSelection].pTestResultInfo->DataPointsNum)//刚开始测试时清空，则不缩放.wcq2011.12.23
	{
		return;
	}

	int m_nPointLabel; //m_nSelectCursor:Seleced标志,1选中A，2选中B, 3选中AB

	if (CURSOR_SELECT::CURSOR_A == m_nSelectCursor) //选中A
	{
		m_nPointLabel=m_sorDrawStatic.GetCurrentCursorIndex();
	}	
	else if (CURSOR_SELECT::CURSOR_B == m_nSelectCursor)
	{
		m_nPointLabel=m_sorDrawStatic.GetCurrentCursorIndex();
	}
	else//AB
	{
		m_nPointLabel=(m_sorDrawStatic.GetCurrentCursorIndex() + m_sorDrawStatic.GetCurrentCursorIndex())/2;
	}//eif

	float fLeftRightCoeff=m_nPointLabel * 1.0/g_sorFileArray.sorFileList[m_nMaxIndex].pTestResultInfo->DataPointsNum;//缩放时左右比例，左右不一
	//如果不是最长距离的曲线且多波形显示模式，则根据最长距离的曲线的A/B标位置进行转化
	if (!pData->IsMaxLength && g_sorFileArray.waveConfig.MultiMode)
	{
		float fTemp=pData->DataSpacing / g_sorFileArray.sorFileList[m_nMaxIndex].pTestResultInfo->DataSpacing;//根据两曲线到原点距离相等。wcq2011.12.01
		m_nPointLabel=m_nPointLabel * fTemp;
	}

	if(0 == m_iHorizonZoomNum) //未缩放
	{
		pData->StartPoint=0;
		pData->MaxPoint=pData->DataPointsNum;
	}
	else
	{
		//原来的cwcq2012.10.26
		//////float ZoomCoeff;
		//////ZoomCoeff=(float)pow(2.0,m_iHorizonZoomNum+1);//放大倍数的1/2
		//////if (m_nPointLabel >= pData->MaxPoint/ZoomCoeff)
		//////{
		//////	pData->StartPoint=(unsigned int)(m_nPointLabel - pData->MaxPoint/ZoomCoeff+1);
		//////	pData->MaxPoint=(unsigned int)(m_nPointLabel+pData->MaxPoint/ZoomCoeff);
		//////}				  
		//////else 
		//////{
		//////	pData->StartPoint=0;
		//////	pData->MaxPoint=(unsigned int)(2*pData->MaxPoint/ZoomCoeff);
		//////}
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////
		int multi=1;//多少倍曲线宽度
		int pointCount=0;
		if (m_iHorizonZoomNum ==m_nZoomCount)//最大缩放倍数
		{
			//pointCount=pData->m_TestResult.LastZoomPointCount;
			pointCount=pData->FullPointCountMin;
		}
		else
		{
			pointCount=pData->FullPointCountMin;//wcq2011.11.22
			int count= m_nZoomCount - m_iHorizonZoomNum;//最后一次缩放点数不一样
			double dPow=pow(2.0,m_iHorizonZoomNum);
			pointCount =pData->FullPointCountMin/dPow;
			//if (count < 20)//40++
			//{
			//	pointCount += count * pData->FullPointCountMin;
			//}
			//else if (count < 30)//30~39
			//{
			//	pointCount += ( 20 + 2 * (count - 19) ) * pData->FullPointCountMin;
			//}
			//else if (count < 40)//20~29
			//{
			//	pointCount +=( 40 + 3 * (count - 29) ) * pData->FullPointCountMin;
			//}
			//else// if (count < 50)//30~39
			//{
			//	pointCount +=( 70 + 4 * (count - 39) ) * pData->FullPointCountMin;
			//}
		}
		int itemp=pointCount * fLeftRightCoeff;//
		if (itemp > m_nPointLabel)
		{
			pData->StartPoint=0;
			pData->MaxPoint = m_nPointLabel + ( pointCount - itemp)-1;
		}
		else
		{
			pData->StartPoint = m_nPointLabel - itemp;//左边位置(起始点位置)
			pData->MaxPoint = m_nPointLabel + ( pointCount - itemp)-1;//右边位置(结束点位置),1:减去缩放中心点本身
		}

		if (pData->StartPoint > pData->DataPointsNum)
		{
			pData->StartPoint = pData->DataPointsNum-1;//wcq2011.11.29
		}
		if (pData->MaxPoint > pData->DataPointsNum)
		{
			pData->MaxPoint = pData->DataPointsNum;
		}		
		else if (pData->MaxPoint <0)
		{
			pData->MaxPoint = 0;
		}
		pData->FullPointCount=pointCount;//wcq2011.11.29
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	}//eif
	if (pData->StartPoint >= pData->MaxPoint)
	{
		pData->MaxPoint = pData->StartPoint + 1; //防止被0除
	}
	if (pData->IsMaxLength || (!g_sorFileArray.waveConfig.MultiMode))//但波形显示
	{
		m_nStartPoint = pData->StartPoint;
		m_nMaxPoint = pData->MaxPoint;
	}
}

void CWaveform::SetHorScroll() //设置水平滚动条
{
	//水平滚动条
	SCROLLINFO siHor;
	siHor.cbSize = sizeof(SCROLLINFO);
	siHor.fMask = SIF_ALL; 
	siHor.nMin = 0;
	siHor.nMax = m_nDrawWidth-1;//这么做后windows会把最大的滚动条位置限制为si.nmax-si.npage+1,而不是si.nmax
	siHor.nPage = m_sorDrawStatic.m_rectCurve.Width();
	siHor.nPos = 0;

	SetScrollInfo(SB_HORZ, &siHor, TRUE);
}
//刷新绘图区综略图波长脉宽显示区,方便调用
void CWaveform::InvalidateAll()
{
	InvalidateRect(m_sorDrawStatic.m_rectCurve,FALSE);//曲线
}

void CWaveform::SetABType(CString strAB)
{
	if ("AB" == strAB)
	{
		m_sorDrawStatic.SetCurrentTypeAB(OTDRTRACE_AB_SELE_DOUBLE_AB);
	}
	else
	{
		m_sorDrawStatic.SetCurrentTypeAB(OTDRTRACE_AB_TYPE_SINGLE_A_OR_B);
	}//eif
}

void CWaveform::OnBnClickedRadio1()
{
	// TODO: 在此添加控件通知处理程序代码
	SetABType(_T("A"));
}

void CWaveform::OnBnClickedRadio2()
{
	// TODO: 在此添加控件通知处理程序代码
	SetABType(_T("AB"));
}



void CWaveform::OnBnClickedCheckAbSelect()
{
	UpdateData(TRUE);
	if(!m_bCheckSelectAB)
	{
		ChangeABStatus(m_bCheckSelectAB);
		m_sorDrawStatic.m_bShowLineAB = TRUE;	       //先设为TRUE, 然后在OnShowAB中会处理成FALSE
		m_sorDrawStatic.OnShowAB();
		m_sorDrawStatic.Invalidate(FALSE);
	}
	else
	{
		ChangeABStatus(m_bCheckSelectAB);
		m_sorDrawStatic.m_bShowLineAB = FALSE;
		m_sorDrawStatic.OnShowAB();
		m_sorDrawStatic.Invalidate(FALSE);
	}
	 
}

//改变AB标有关的控件信息   lzy 2015.2.10
void CWaveform::ChangeABStatus(BOOL bSelected)
{
	if(!bSelected)     //未选中
	{
		m_bCheckSelectAB = bSelected;
		GetDlgItem(IDC_RADIO1)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO2)->EnableWindow(FALSE);
		
	}
	else			//选中
	{
		m_bCheckSelectAB = bSelected;
		GetDlgItem(IDC_RADIO1)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO2)->EnableWindow(TRUE);
	}

	UpdateData(FALSE);
}
