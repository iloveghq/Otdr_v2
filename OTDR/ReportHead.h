#pragma once
#include "afxwin.h"
#include "BtnST.h"

// CReportHead 对话框

class CReportHead : public CDialog
{
	DECLARE_DYNAMIC(CReportHead)

public:
	CReportHead(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CReportHead();
	void SetReportHeadBtn(int nAlreadyPageIndex);//设置报表头按钮显示

// 对话框数据
	enum { IDD = IDD_REPORT_HEAD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedBtnBack();
	afx_msg void OnBnClickedBtnNext();
	afx_msg void OnBnClickedBtnStep1();
	afx_msg void OnBnClickedBtnStep2();
	afx_msg void OnBnClickedBtnStep3();
	afx_msg void OnBnClickedBtnStep4();
	afx_msg void OnBnClickedBtnStep5();
	afx_msg void OnBnClickedBtnStep6();

public:
	CButtonST m_btnBack;
	CButtonST m_btnNext;
	CButtonST m_btnStep1;
	CButtonST m_btnStep2;
	CButtonST m_btnStep3;
	CButtonST m_btnStep4;
	CButtonST m_btnStep5;
	CButtonST m_btnStep6;
};
