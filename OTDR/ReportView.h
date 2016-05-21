#pragma once
#include "afxwin.h"
#include "ColorStatic.h"
#include "DrawButton.h"

// CReportView 对话框

class CReportView : public CDialog
{
	DECLARE_DYNAMIC(CReportView)

public:
	CReportView(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CReportView();
	virtual BOOL OnInitDialog();

// 对话框数据
	enum { IDD = IDD_REPORT_VIEW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

public:
	CDrawButton m_DrawButton;
	CColorStatic m_ImgStatic;
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
