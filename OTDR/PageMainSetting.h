#pragma once
#include "afxwin.h"
#include "OtdrTrace.h"


// CPageMainSetting 对话框

class CPageMainSetting : public CPropertyPage
{
	DECLARE_DYNAMIC(CPageMainSetting)

public:
	CPageMainSetting();
	virtual ~CPageMainSetting();
	void SetLocalString();
	void InitListFile();
	void PopulateFileList();
	void AddFileList();

// 对话框数据
	enum { IDD = IDD_MAIN_SETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedBtnStep5Openfile();
	afx_msg void OnBnClickedBtnStep5Auto();

private:
	CListCtrl m_fileList;
public:
	COtdrTrace* m_potdrTrace;
};
