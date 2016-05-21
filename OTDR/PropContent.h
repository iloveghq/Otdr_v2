#pragma once


// CPropContent 对话框

class CPropContent : public CPropertyPage
{
	DECLARE_DYNAMIC(CPropContent)

public:
	CPropContent();
	virtual ~CPropContent();

// 对话框数据
	enum { IDD = IDD_PROP_STEP2_CONTENT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	// 是否打印grandway logo
	BOOL m_bCheckLogo;
	// 是否打印任务信息
	BOOL m_bCheckJob;
	// 是否打印配置信息
	BOOL m_bCheckConfig;
	// 是否打印注释信息
	BOOL m_bCheckComment;
	// 是否打印曲线
	BOOL m_bCheckTrace;
	// 是否打印事件
	BOOL m_bCheckEvent;

	void SetLocalString();

private:
	CString m_strSelect;
	CString m_strLogo;
	CString m_strJob;
	CString m_strCfg;
	CString m_strCmt;
	CString m_strTrace;
	CString m_strEvent;
public:
	virtual BOOL OnInitDialog();
};
