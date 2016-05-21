#pragma once
#include "resource.h"
#include "ColorPage.h"
#include "EventListPage.h"
#include "UnitPage.h"
#include "WavePage.h"

class CConfigSheet :
	public CPropertySheet	
{
DECLARE_DYNAMIC(CConfigSheet)

public:
	CConfigSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	~CConfigSheet(void);
	bool CheckInputValid(const CString& strVal);
	BOOL SetPageTitle (int nPage, const CString& strText);

afx_msg LRESULT OnRecordConfigChange(WPARAM wParam, LPARAM lParam);
afx_msg LRESULT OnEnableApply(WPARAM wParam, LPARAM lParam);

protected:
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

private:
	CEventListPage m_EventListPage;
	CColorPage m_ColorPage;
	CUnitPage m_UnitPage;
	CWavePage m_WavePage;
};
