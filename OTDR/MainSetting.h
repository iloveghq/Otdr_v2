#pragma once
#include "afxwin.h"
#include "BtnST.h"
#include "PageMainSetting.h"
#include "afxcmn.h"

// CMainSetting 窗体视图

class CMainSetting : public CFormView
{
	DECLARE_DYNCREATE(CMainSetting)

protected:
	CMainSetting();           // 动态创建所使用的受保护的构造函数
	virtual ~CMainSetting();
	void InitTab();

public:
	enum { IDD = IDD_MAIN_SETTING_DLG };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	virtual void OnInitialUpdate();

	DECLARE_MESSAGE_MAP()
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedBtnPrev();
	afx_msg void OnBnClickedBtnNext();
	afx_msg void OnBnClickedBtnCancel();
	afx_msg void OnBnClickedBtnStep3();
	afx_msg void OnBnClickedBtnStep4();
	afx_msg void OnBnClickedBtnStep5();

public:
	void SetLocalString();

	CButtonST m_btnStep3;
	CButtonST m_btnStep4;
	CButtonST m_btnStep5;
	CTabCtrl m_tab;
	CPageMainSetting m_pageMainSetting;
};


