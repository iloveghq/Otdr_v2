#pragma once
#include "resource.h"
#include "PrintMargin.h"
#include "PrintHeadTail.h"

class CPrintSheet :
	public CPropertySheet	
{
DECLARE_DYNAMIC(CPrintSheet)

public:
	CPrintSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	~CPrintSheet(void);
	bool CheckInputValid(const CString& strVal);
	BOOL SetPageTitle (int nPage, const CString& strText);

afx_msg LRESULT OnRecordConfigChange(WPARAM wParam, LPARAM lParam);
afx_msg LRESULT OnEnableApply(WPARAM wParam, LPARAM lParam);

protected:
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

private:
	CPrintMargin m_PrintMargin;
	CPrintHeadTail m_PrintHeadTail;
};
