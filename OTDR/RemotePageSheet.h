#pragma once
#include "resource.h"
#include "PageSystemSetting.h"
#include "PageTestSetup.h"

class CRemotePageSheet :
	public CPropertySheet	
{
DECLARE_DYNAMIC(CRemotePageSheet)

public:
	CRemotePageSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	~CRemotePageSheet(void);
	bool CheckInputValid(const CString& strVal);
	BOOL SetPageTitle (int nPage, const CString& strText);

afx_msg LRESULT OnRecordConfigChange(WPARAM wParam, LPARAM lParam);
afx_msg LRESULT OnEnableApply(WPARAM wParam, LPARAM lParam);
afx_msg LRESULT OnSocketDataReady(WPARAM wParam, LPARAM lParam);

protected:
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

private:
	CPageSystemSetting m_PageSystemSetting;
	CPageTestSetup m_PageTestSetup;
};
