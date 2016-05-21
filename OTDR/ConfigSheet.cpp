#include "StdAfx.h"
#include "ConfigSheet.h"

IMPLEMENT_DYNAMIC(CConfigSheet, CPropertySheet)

CConfigSheet::CConfigSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
: CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	//AddPage(&m_EventListPage);
	AddPage(&m_ColorPage);
	AddPage(&m_UnitPage);
	AddPage(&m_WavePage);
}

CConfigSheet::~CConfigSheet(void)
{

}

BEGIN_MESSAGE_MAP(CConfigSheet, CPropertySheet)

END_MESSAGE_MAP()

BOOL CConfigSheet::OnInitDialog()
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

	//确定按钮
	CString strBtnText = IniUtil::ReadResString(_T("IDS_SCREEN_OK"));
	GetDlgItem(IDOK)->SetWindowText(strBtnText);

	//取消按钮
	strBtnText = IniUtil::ReadResString(_T("IDS_SCREEN_CANCAL"));
	GetDlgItem(IDCANCEL)->SetWindowText(strBtnText);

	//设置标题
	CString strTitle = IniUtil::ReadResString(_T("IDS_SCREEN_TITLE"));
	SetWindowText(strTitle);

	//事件列表标题
	//strTitle = IniUtil::ReadResString(_T("IDS_SCREEN_EVENT_TITLE"));
	//SetPageTitle(0, strTitle);

	//颜色标题
	strTitle = IniUtil::ReadResString(_T("IDS_SCREEN_COLOR_TITLE"));
	SetPageTitle(0, strTitle);


	//单位标题
	strTitle = IniUtil::ReadResString(_T("IDS_SCREEN_UNIT_TITLE"));
	SetPageTitle(1, strTitle);

	//波形标题
	strTitle = IniUtil::ReadResString(_T("IDS_SCREEN_WAVE_TITLE"));
	SetPageTitle(2, strTitle);


	return TRUE;
}

/* /记录用户配置信息到Config.ini
* @param wParam  配置信息类型
* @param lParam 信息是否改变
*/
LRESULT CConfigSheet::OnRecordConfigChange(WPARAM wParam, LPARAM lParam)
{
	
	return 1;
}

bool CConfigSheet::CheckInputValid(const CString& strVal)
{


	return true;
}

LRESULT CConfigSheet::OnEnableApply(WPARAM wParam, LPARAM lParam)
{
	GetDlgItem(ID_APPLY_NOW)->EnableWindow(TRUE);
	GetDlgItem(IDOK)->EnableWindow(TRUE);

	return 0;
}

BOOL CConfigSheet::SetPageTitle (int nPage, const CString& strText)
{
	CTabCtrl* pTab = GetTabControl();
	ASSERT (pTab);

	TC_ITEM ti;
	ti.mask = TCIF_TEXT;
	ti.pszText = (LPWSTR)(LPCTSTR)strText;
	VERIFY (pTab->SetItem (nPage, &ti));

	return TRUE;
} 
