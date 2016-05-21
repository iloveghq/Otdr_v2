#include "StdAfx.h"
#include "EventSearchSheet.h"

IMPLEMENT_DYNAMIC(CEventSearchSheet, CPropertySheet)

CEventSearchSheet::CEventSearchSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
: CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	AddPage(&m_EventListPage);
	AddPage(&m_ColorPage);
	AddPage(&m_UnitPage);
	AddPage(&m_WavePage);
}

CEventSearchSheet::~CEventSearchSheet(void)
{

}

BEGIN_MESSAGE_MAP(CEventSearchSheet, CPropertySheet)
	//ON_MESSAGE(WM_NOTIFY_RECORD_CHANGE, &CEventSearchSheet::OnRecordConfigChange)
	//ON_MESSAGE(WM_NOTIFY_ENABLE_APPLY, &CEventSearchSheet::OnEnableApply)
END_MESSAGE_MAP()

BOOL CEventSearchSheet::OnInitDialog()
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

	//设置标题
	CString strTitle;
	//strTitle.LoadString(IDS_SETTING);
	//SetWindowText(strTitle);

	return TRUE;
}

/* /记录用户配置信息到Config.ini
* @param wParam  配置信息类型
* @param lParam 信息是否改变
*/
LRESULT CEventSearchSheet::OnRecordConfigChange(WPARAM wParam, LPARAM lParam)
{
	
	return 1;
}

bool CEventSearchSheet::CheckInputValid(const CString& strVal)
{


	return true;
}

LRESULT CEventSearchSheet::OnEnableApply(WPARAM wParam, LPARAM lParam)
{
	GetDlgItem(ID_APPLY_NOW)->EnableWindow(TRUE);
	GetDlgItem(IDOK)->EnableWindow(TRUE);

	return 0;
}