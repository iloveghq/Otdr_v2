#pragma once
#include "afxcmn.h"
#include "NumSpinCtrl.h"

// CEventSearchCon 对话框

class CEventSearchCon : public CPropertyPage
{
	DECLARE_DYNAMIC(CEventSearchCon)

public:
	CEventSearchCon();
	virtual ~CEventSearchCon();
	void SetLocaleString();

// 对话框数据
	enum { IDD = IDD_PAGE_EVENT_CON };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	CNumSpinCtrl m_spinBendTH;
	CNumSpinCtrl m_spinReflectTH;
	CNumSpinCtrl m_spinTermail;
	CNumSpinCtrl m_spinBackScatter;
	virtual BOOL OnApply();
public:
	CNumSpinCtrl m_spinBackScatter1550;
	CNumSpinCtrl m_spinBackScatter1625;
//	afx_msg void OnBnClickedCheckBend();
//	afx_msg void OnBnClickedCheckRetrunLoss();
//	afx_msg void OnBnClickedCheckTermail();
	afx_msg void OnBnClickedCheckBend();
	afx_msg void OnBnClickedCheckRetrunLoss();
	afx_msg void OnBnClickedCheckTermail();
public:
	CButton* pBtnBend;
	CEdit* pEditBend;
	CSpinButtonCtrl* pSpinBend;
	CButton* pBtnReturnLoss;
	CEdit* pEditReturnLoss;
	CSpinButtonCtrl* pSpinReturnLoss;
	CButton* pBtnTerminal;
	CEdit* pEditTerminal;
	CSpinButtonCtrl* pSpinTerminal;
};
