#pragma once

#include "ReportWizard.h"
#include "ReportView.h"

// CReport 窗体视图

class CReport : public CFormView
{
	DECLARE_DYNCREATE(CReport)

protected:
	CReport();           // 动态创建所使用的受保护的构造函数
	virtual ~CReport();
	void CreateWizardDlg();
	void CreateReportViewDlg();

public:
	enum { IDD = IDD_REPORT };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

public:
	void SetLocalString();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual void OnInitialUpdate();

	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);

public:
	CReportWizard m_ReportWizardDlg;
	CReportView m_ReportViewDlg;
};


