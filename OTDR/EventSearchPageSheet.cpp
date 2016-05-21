#include "StdAfx.h"
#include "EventSearchPageSheet.h"

IMPLEMENT_DYNAMIC(CEventSearchPageSheet, CPropertySheet)

CEventSearchPageSheet::CEventSearchPageSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
: CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	AddPage(&m_EventSearchCon);
	AddPage(&m_EventAnalCon);
}

CEventSearchPageSheet::~CEventSearchPageSheet(void)
{

}

BEGIN_MESSAGE_MAP(CEventSearchPageSheet, CPropertySheet)
END_MESSAGE_MAP()

BOOL CEventSearchPageSheet::OnInitDialog()
{
	CPropertySheet::OnInitDialog();
	//隐藏Help按钮
	GetDlgItem(IDHELP)->ShowWindow(SW_HIDE);

	//移动OK，CANCEL 和 Apply位置
	CRect helpRect;
	CRect applyRect;
	CRect cancelRect;
	GetDlgItem(IDHELP)->GetWindowRect(&helpRect);
	GetDlgItem(ID_APPLY_NOW)->GetWindowRect(&applyRect);
	GetDlgItem(IDCANCEL)->GetWindowRect(&cancelRect);
	ScreenToClient(helpRect);
	GetDlgItem(ID_APPLY_NOW)->MoveWindow(helpRect);
	ScreenToClient(applyRect);
	GetDlgItem(IDCANCEL)->MoveWindow(&applyRect);
	ScreenToClient(cancelRect);
	GetDlgItem(IDOK)->MoveWindow(&cancelRect);
	/*                    设置标题                       */
	//确定按钮
	CString strOK = IniUtil::ReadResString(_T("IDS_EVENT_CON_OK"));
	GetDlgItem(IDOK)->SetWindowText(strOK);

	//取消按钮
	CString strCancel = IniUtil::ReadResString(_T("IDS_EVENT_CON_CANCAL"));
	GetDlgItem(IDCANCEL)->SetWindowText(strCancel);

	//设置标题
	CString strTitle = IniUtil::ReadResString(_T("IDS_EVENT_CON_TITLE"));
	SetWindowText(strTitle);

	//搜索条件标题
	strTitle = IniUtil::ReadResString(_T("IDS_EVENT_CON_SEARCH_TITLE"));
	SetPageTitle(0, strTitle);

	//分析条件标题
	strTitle = IniUtil::ReadResString(_T("IDS_EVENT_CON_ANAL_TITLE"));
	SetPageTitle(1, strTitle);

	return TRUE;
}

BOOL CEventSearchPageSheet::SetPageTitle (int nPage, const CString& strText)
{
	CTabCtrl* pTab = GetTabControl();
	ASSERT (pTab);

	TC_ITEM ti;
	ti.mask = TCIF_TEXT;
	ti.pszText = (LPWSTR)(LPCTSTR)strText;
	VERIFY (pTab->SetItem (nPage, &ti));

	return TRUE;
} 

