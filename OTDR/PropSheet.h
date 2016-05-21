#pragma once

// PropSheet.h : header file
//


/////////////////////////////////////////////////////////////////////////////
// CPropSheet
#include "PropStep1.h"
#include "PropStep2.h"
#include "PropStep2A.h"
#include "PropStep3.h"
#include "PropStep3Rpt.h"
#include "PropStep4.h"
#include "PropStep5New.h"
#include "PropStep6.h"
#include "Step4FileSelect.h"

class CPropSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CPropSheet)

// Construction
public:
	CPropSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CPropSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CPropSheet();

// Attributes
public:

// Operations
public:
	void SetLocalString();
	virtual BOOL OnInitDialog();
	BOOL EnablePrevBtn(BOOL bEnable);
	BOOL EnableNextBtn(BOOL bEnable);
	LRESULT WizardBack();//前一页面
	LRESULT WizardNext();//后一页面
	void SetReportHeadBtn(int nCurPageIndex);//设置报表头按钮显示
// Overrides
	// ClassWizard generated virtual function overrides

// Implementation
public:
	CPropStep1 m_PropStep1;
	CPropStep2 m_PropStep2;
	//CPropStep2A m_PropStep2;
	CPropStep3 m_PropStep3;
	CPropStep3Rpt m_PropStep3Rpt;
	CPropStep4 m_PropStep4;
	CPropStep5New m_PropStep5;  //lzy2014.9.12
	CPropStep6 m_PropStep6;
	CStep4FileSelect m_Step4FileSelect;
	CButton m_BtnQuit; //退出按钮
	CButton m_BtnPrev; //向前按钮
	CButton m_BtnNext; //向后按钮
	int m_nAlreaySettingPageIndex;//已配置page索引号

	// Generated message map functions
public:

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBtnQuit();
	afx_msg void OnBtnPrev();
	afx_msg void OnBtnNext();
	
};