#pragma once
// Prop.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// CPropStep1 dialog

class CPropStep1 : public CPropertyPage
{
	DECLARE_DYNCREATE(CPropStep1)

// Construction
public:
	CPropStep1();
	~CPropStep1();
	void SetLocalString();

// Dialog Data
	//{{AFX_DATA(CPropStep1)
	enum { IDD = IDD_PROP_STEP1 };
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPropStep1)
	public:
	virtual BOOL OnSetActive();
	virtual LRESULT OnWizardNext();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPropStep1)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedRdoStep1RptWave();
	afx_msg void OnBnClickedRdoStep1RptRpt();
	afx_msg void OnBnClickedRdoStep11wave();
	afx_msg void OnBnClickedRdoStep12wave();
	afx_msg void OnBnClickedRdoStep14wave();
	afx_msg void OnBnClickedRdoStep16wave();
	afx_msg void OnBnClickedRdoStep18wave();
	afx_msg void OnBnClickedRdoStep1PrintHor();
	afx_msg void OnBnClickedRdoStep1PrintVer();
	afx_msg void OnBnClickedRdoStep1A1();
	afx_msg void OnBnClickedRdoStep1A2();
	afx_msg void OnBnClickedRdoStep1A3();
	afx_msg void OnBnClickedRdoStep1B1();
	afx_msg void OnButtonRptSetting();
};
