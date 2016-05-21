#pragma once


// CPropJob 对话框

class CPropJob : public CPropertyPage
{
	DECLARE_DYNAMIC(CPropJob)

public:
	CPropJob();
	virtual ~CPropJob();

// 对话框数据
	enum { IDD = IDD_PROP_STEP2_JOB };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	// 任务编号,EDIT控件变量
	CString m_strJobID;
	// 承包商，EDIT控件变量
	CString m_strContractor;
	// 用户，EDIT控件变量
	CString m_strCustomer;
	// 操作者，EDIT控件变量
	CString m_strOperator;
	// 文件名，EDIT控件变量
	CString m_strFile;
	// 测试时间，EDIT控件变量
	CString m_strTestDate;
	// 注释，EDIT控件变量
	CString m_strComment;
	afx_msg void OnBnClickedBtnJobClear();

private:
	CString m_strStInfo;  //Job info static text
	CString m_strStID;   //text of job index
	CString m_strStContractor;
	CString m_strStCustomer;
	CString m_strStOperator;
	CString m_strStFile;
	CString m_strStTime;
	CString m_strStComment;
	CString m_strBtnClear;

	void SetLocalString();

public:
	virtual BOOL OnInitDialog();
};
