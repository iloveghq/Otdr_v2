// ReportView.cpp : 实现文件
//

#include "stdafx.h"
#include "OTDR.h"
#include "ReportView.h"


// CReportView 对话框

IMPLEMENT_DYNAMIC(CReportView, CDialog)

CReportView::CReportView(CWnd* pParent /*=NULL*/)
	: CDialog(CReportView::IDD, pParent)
{

}

CReportView::~CReportView()
{
}

void CReportView::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_IMG, m_ImgStatic);
	DDX_Control(pDX, IDC_BUTTON_DRAW, m_DrawButton);
}


BEGIN_MESSAGE_MAP(CReportView, CDialog)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CReportView 消息处理程序

BOOL CReportView::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_ImgStatic.SetBkColor(RGB(172, 168, 153));

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CReportView::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	if (NULL == m_ImgStatic.GetSafeHwnd() ||
		NULL == m_DrawButton.GetSafeHwnd())
		return;

	//背景Static
	CRect clientRect;
	GetWindowRect(clientRect);
	ScreenToClient(clientRect);
	clientRect.DeflateRect(8, 8, 8, 8);
	m_ImgStatic.MoveWindow(clientRect);
	//绘图按钮
	clientRect.DeflateRect(80, 8, 80, 8);
	m_DrawButton.MoveWindow(clientRect);
}
