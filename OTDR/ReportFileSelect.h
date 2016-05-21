#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CReportFileSelect 对话框

class CReportFileSelect : public CDialog
{
	DECLARE_DYNAMIC(CReportFileSelect)

public:
	CReportFileSelect(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CReportFileSelect();
	void InitListFile();
	void InitListFileSelect();
	void PopulateFileList();
	BOOL IsFileAlreadySelect(const CString& strFile);//判断文件是否已经选择
	bool GetBatchFileSelect();

// 对话框数据
	enum { IDD = IDD_DIALOG_FILE_SELECT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedBtnFileselect();
	afx_msg void OnBnClickedBtnBatch();
	afx_msg void OnBnClickedBtnSetting();
	afx_msg void OnBnClickedBtnClearall();
	afx_msg void OnBnClickedBtnClear();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCheckApply();

public:
	CStringArray m_strSelectFiles;//选择的文件

private:
	CListCtrl m_listFile;
	CListCtrl m_listFileSelect;
	CString m_strFolder;//文件夹路径
	CComboBox m_commboxWaveLength;
//	afx_msg void OnBnClickedCancel();
//	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedCancel();
};
