// ReportWizard.cpp : 实现文件
//

#include "stdafx.h"
#include "OTDR.h"
#include "ReportWizard.h"


// CReportWizard 对话框

IMPLEMENT_DYNAMIC(CReportWizard, CDialog)

CReportWizard::CReportWizard(CWnd* pParent /*=NULL*/)
	: CDialog(CReportWizard::IDD, pParent)
{
	m_pPropSheet = NULL;
}

CReportWizard::~CReportWizard()
{
	if (NULL != m_pPropSheet)
	{
		delete m_pPropSheet;
		m_pPropSheet = NULL;
	}
}

void CReportWizard::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CReportWizard, CDialog)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
END_MESSAGE_MAP()


// CReportWizard 消息处理程序

int CReportWizard::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	//添加表头
	CreateReportHeader();
	//添加Report Wizard Sheet
	CreateReportSheet();

	return 0;
}

void CReportWizard::CreateReportHeader()
{
	m_ReportHeaderDlg.Create(CReportHead::IDD,this);
}

void CReportWizard::CreateReportSheet()
{
	if (NULL == m_pPropSheet)
		m_pPropSheet = new CPropSheet(_T("报表向导"), this);

	m_pPropSheet->SetWizardMode();
	m_pPropSheet->Create(this, WS_CHILD | WS_VISIBLE | WS_BORDER | DS_3DLOOK, WS_EX_CONTROLPARENT);
}

// CGrandwayProgress 消息处理程序

BOOL CReportWizard::OnInitDialog()
{
	CDialog::OnInitDialog();

	// initial scroll position
	m_nScrollPosVer = 0; 
	m_nScrollPosHor = 0; 

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CReportWizard::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	if (NULL == MAIN ||
		NULL == m_ReportHeaderDlg.GetSafeHwnd() ||
		NULL == m_pPropSheet->GetSafeHwnd())
		return;

	//报表头
	CRect reportHeaderRect;
	reportHeaderRect.SetRect(0, 0, REPORT_WIZARD_WIDTH, REPORT_HEADER_HEIGHT);
	m_ReportHeaderDlg.MoveWindow(&reportHeaderRect);//设置子窗口位置及大小
	m_ReportHeaderDlg.ShowWindow(SW_SHOW);

	//报表向导
	CRect reportSheetRect;
	reportSheetRect.SetRect(20, REPORT_HEADER_HEIGHT, REPORT_WIZARD_WIDTH, REPORT_WIZARD_HEIGHT);
	m_pPropSheet->MoveWindow(&reportSheetRect);//设置子窗口位置及大小
	m_pPropSheet->ShowWindow(SW_SHOW);

	//竖直滚动条
	m_nCurHeight = cy;
	SCROLLINFO siVer;
	siVer.cbSize = sizeof(SCROLLINFO);
	siVer.fMask = SIF_ALL; 
	siVer.nMin = 0;
	siVer.nMax = REPORT_WIZARD_HEIGHT;
	siVer.nPage = cy;
	siVer.nPos = 0;
	SetScrollInfo(SB_VERT, &siVer, TRUE);
	//水平滚动条
	m_nCurWidth = cx;
	SCROLLINFO siHor;
	siHor.cbSize = sizeof(SCROLLINFO);
	siHor.fMask = SIF_ALL; 
	siHor.nMin = 0;
	siHor.nMax = REPORT_WIZARD_WIDTH;
	siHor.nPage = cx;
	siHor.nPos = 0;
	SetScrollInfo(SB_HORZ, &siHor, TRUE);
}

void CReportWizard::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	int nDelta;
	int nMaxPos = REPORT_WIZARD_HEIGHT - m_nCurHeight;

	switch (nSBCode)
	{
	case SB_LINEDOWN:
		if (m_nScrollPosVer >= nMaxPos)
			return;

		nDelta = min(max(nMaxPos/20,5),nMaxPos-m_nScrollPosVer);
		break;

	case SB_LINEUP:
		if (m_nScrollPosVer <= 0)
			return;
		nDelta = -min(max(nMaxPos/20,5),m_nScrollPosVer);
		break;
	case SB_PAGEDOWN:
		if (m_nScrollPosVer >= nMaxPos)
			return;
		nDelta = min(max(nMaxPos/10,5),nMaxPos-m_nScrollPosVer);
		break;
	case SB_THUMBTRACK:
	case SB_THUMBPOSITION:
		nDelta = (int)nPos - m_nScrollPosVer;
		break;

	case SB_PAGEUP:
		if (m_nScrollPosVer <= 0)
			return;
		nDelta = -min(max(nMaxPos/10,5),m_nScrollPosVer);
		break;

	default:
		return;
	}
	m_nScrollPosVer += nDelta;
	SetScrollPos(SB_VERT,m_nScrollPosVer,TRUE);
	ScrollWindow(0,-nDelta);
	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CReportWizard::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	int nDelta;
	int nMaxPos = REPORT_WIZARD_WIDTH - m_nCurWidth;

	switch (nSBCode)
	{
	case SB_LINELEFT:
		if (m_nScrollPosHor >= nMaxPos)
			return;

		nDelta = min(max(nMaxPos/20,5),nMaxPos-m_nScrollPosHor);
		break;

	case SB_LINERIGHT:
		if (m_nScrollPosHor <= 0)
			return;
		nDelta = -min(max(nMaxPos/20,5),m_nScrollPosHor);
		break;
	case SB_PAGEDOWN:
		if (m_nScrollPosHor >= nMaxPos)
			return;
		nDelta = min(max(nMaxPos/10,5),nMaxPos-m_nScrollPosHor);
		break;
	case SB_THUMBTRACK:
	case SB_THUMBPOSITION:
		nDelta = (int)nPos - m_nScrollPosHor;
		break;

	case SB_PAGEUP:
		if (m_nScrollPosHor <= 0)
			return;
		nDelta = -min(max(nMaxPos/10,5),m_nScrollPosHor);
		break;

	default:
		return;
	}
	m_nScrollPosHor += nDelta;
	SetScrollPos(SB_HORZ,m_nScrollPosHor,TRUE);
	ScrollWindow(-nDelta,0);
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CReportWizard::SetLocalString()
{
	m_pPropSheet->SetLocalString();
}
