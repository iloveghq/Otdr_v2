#pragma once
#include "resource.h"
#include "EventGenerateCon.h"
#include "EventSearchCon.h"
#include "EventAnalCon.h"

class CEventGenPageSheet :
	public CPropertySheet	
{
DECLARE_DYNAMIC(CEventGenPageSheet)

public:
	CEventGenPageSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	~CEventGenPageSheet(void);
	BOOL SetPageTitle (int nPage, const CString& strText);

protected:
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

private:
	CEventGenerateCon m_EventGenerateCon;
	CEventSearchCon m_EventSearchCon;
	CEventAnalCon m_EventAnalCon;
};
