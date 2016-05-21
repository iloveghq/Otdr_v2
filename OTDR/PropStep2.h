#pragma once
// PropStep2.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// CPropStep2 dialog

class CPropStep2 : public CPropertyPage
{
	DECLARE_DYNCREATE(CPropStep2)

// Construction
public:
	CPropStep2();
	~CPropStep2();
	void SetLocalString();
	void InitCombox();//初始化Combox	
	void InitEdit();//初始化所有EDIT的值lzy2014.7.14

// Dialog Data
enum { IDD = IDD_PROP_STEP2 };
// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPropStep2)
	public:
	virtual BOOL OnSetActive();
	virtual LRESULT OnWizardNext();

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL
private:
	void InitCombox(CComboBox& combox, CString strText);
	void RecordSetting(); //保存配置信息
// Implementation
protected:
	// Generated message map functions
	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedBtnStep2Edit();
	afx_msg void OnBnClickedBtnStep2Copy();

public:
	CComboBox m_combox1;
	CComboBox m_combox2;
	CComboBox m_combox3;
	CComboBox m_combox4;
	CComboBox m_combox5;
	CComboBox m_combox6;
	CComboBox m_combox7;
	CComboBox m_combox8;
	CComboBox m_combox9;
	CComboBox m_combox10;
	CComboBox m_combox11;
	CComboBox m_combox12;
};
