// Report.cpp : 实现文件
//

#include "stdafx.h"
#include "OTDR.h"
#include "Report.h"


// CReport

IMPLEMENT_DYNCREATE(CReport, CFormView)

CReport::CReport()
	: CFormView(CReport::IDD)
{

}

CReport::~CReport()
{
}

void CReport::SetLocalString()
{
	CString strTitle = IniUtil::ReadResString(_T("IDS_FRM_REPORT_TITLE"));
	GetParentFrame()->SetWindowText(strTitle);
	//
	m_ReportWizardDlg.SetLocalString();
}

void CReport::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CReport, CFormView)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CReport 诊断

#ifdef _DEBUG
void CReport::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CReport::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CReport 消息处理程序

int CReport::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFormView::OnCreate(lpCreateStruct) == -1)
		return -1;

	//添加左面对话框
	CreateWizardDlg();
	//添加右面对话框
	CreateReportViewDlg();

	return 0;
}

void CReport::CreateWizardDlg()
{
	//创建
	m_ReportWizardDlg.Create(CReportWizard::IDD,this);
}

void CReport::CreateReportViewDlg()
{
	//创建
	m_ReportViewDlg.Create(CReportView::IDD,this);
}

void CReport::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
}

void CReport::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);
	if (NULL == m_ReportViewDlg.GetSafeHwnd() ||
		NULL == m_ReportWizardDlg.GetSafeHwnd() ||
		NULL == MAIN)
		return;

	CRect clientRect;
	MAIN->GetClientRect(clientRect);
	ScreenToClient(clientRect);
	//左侧wizard
	CRect wizardRect;
	wizardRect.SetRect(0, 0, clientRect.Width() / 2, clientRect.bottom/* - DEFAULT_STATUSBAR_HEIGHT*/);
	m_ReportWizardDlg.MoveWindow(&wizardRect,TRUE);//设置子窗口位置及大小
	m_ReportWizardDlg.ShowWindow(SW_SHOW);

	//右侧图形
	CRect reportRect;
	reportRect.SetRect(clientRect.Width() / 2, 0, clientRect.right, clientRect.bottom/* - DEFAULT_STATUSBAR_HEIGHT*/);
	m_ReportViewDlg.MoveWindow(&reportRect);//设置子窗口位置及大小
	m_ReportViewDlg.ShowWindow(SW_SHOW);
}
