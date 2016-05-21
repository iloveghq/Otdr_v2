#pragma once
#include "resource.h"
#include "EventGenerateCon.h"
#include "EventSearchCon.h"
#include "EventAnalCon.h"

class CEventSearchPageSheet :
	public CPropertySheet	
{
DECLARE_DYNAMIC(CEventSearchPageSheet)

public:
	CEventSearchPageSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	~CEventSearchPageSheet(void);
	BOOL SetPageTitle (int nPage, const CString& strText);

protected:
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

private:
	CEventSearchCon m_EventSearchCon;
	CEventAnalCon m_EventAnalCon;
};
