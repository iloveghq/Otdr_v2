#pragma once
/////////////////////////////////////////////////////////////////////////////
// CPropStep3 dialog

class CPropStep3 : public CPropertyPage
{
	DECLARE_DYNCREATE(CPropStep3)

// Construction
public:
	CPropStep3();
	~CPropStep3();
	void SetLocalString();
	void InitCombox();
	void InitCombox(CComboBox& combox, CString strText);
	void RecordSetting(); //±£¥Ê≈‰÷√–≈œ¢

// Dialog Data
	//{{AFX_DATA(CPropStep3)
	enum { IDD = IDD_PROP3 };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPropStep3)
	public:
	virtual BOOL OnSetActive();
	virtual LRESULT OnWizardNext();

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPropStep3)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CComboBox m_comboxWave1;
	CComboBox m_comboxWave2;
	CComboBox m_comboxWave3;
	CComboBox m_comboxEvent1;
	CComboBox m_comboxEvent2;
	CComboBox m_comboxEvent3;

};
