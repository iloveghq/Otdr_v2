#pragma once
#include "resource.h"
#include "afxwin.h"

// CEventListPage 对话框

class CEventListPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CEventListPage)

public:
	CEventListPage();
	virtual ~CEventListPage();

// 对话框数据
	enum { IDD = IDD_PROPPAGE_EVENTLIST };
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	virtual BOOL OnApply();

	DECLARE_MESSAGE_MAP()

};
