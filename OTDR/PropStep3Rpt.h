#pragma once

// CPropStep3Rpt 对话框

class CPropStep3Rpt : public CPropertyPage
{
	DECLARE_DYNAMIC(CPropStep3Rpt)

public:
	CPropStep3Rpt();
	virtual ~CPropStep3Rpt();
	void SetLocalString();
	void RecordSetting();

// 对话框数据
	enum { IDD = IDD_PROP_STEP3_RPT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnSetActive();
	virtual LRESULT OnWizardNext();

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};
