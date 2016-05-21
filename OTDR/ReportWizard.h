#pragma once

#include "ReportHead.h"
#include "PropSheet.h"

// CReportWizard 对话框

class CReportWizard : public CDialog
{
	DECLARE_DYNAMIC(CReportWizard)

public:
	CReportWizard(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CReportWizard();
	void CreateReportHeader();
	void CreateReportSheet();
	void SetLocalString();

// 对话框数据
	enum { IDD = IDD_REPORT_WIZARD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

public:
	CReportHead m_ReportHeaderDlg;
	CPropSheet* m_pPropSheet;

	// actual scroll position
	int m_nScrollPosVer;
	int m_nScrollPosHor;
	// actual dialog height
	int m_nCurHeight;
	// actual dialog width
	int m_nCurWidth;
};
