#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include"Step4FileSelect.h"
#include "NumSpinCtrl.h"

// CPropStep4 对话框

class CPropStep4 : public CPropertyPage
{
	DECLARE_DYNAMIC(CPropStep4)

public:
	CPropStep4();
	virtual ~CPropStep4();
	void SetLocalString();

	// 对话框数据
	enum { IDD = IDD_PROP_STEP4 };

public:
	virtual BOOL OnSetActive();
	virtual LRESULT OnWizardNext();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

private:
	void InitTab();
	void RecordSetting(); //保存配置信息

	DECLARE_MESSAGE_MAP()
	afx_msg void OnDeltaposSpinFiberNum(NMHDR *pNMHDR, LRESULT *pResult);

public:
	CNumSpinCtrl m_spinFiberNum; //芯数
	CNumSpinCtrl m_spinStartFiber; // 起始芯号
	CEdit m_editFiberNum;
	CEdit m_editStartFiber;
	CTabCtrl m_tab;
	CStep4FileSelect m_pageFileSelect;
};
