#pragma once
#include "afxwin.h"


// CItemSettingDlg 对话框

class CItemSettingDlg : public CDialog
{
	DECLARE_DYNAMIC(CItemSettingDlg)

public:
	CItemSettingDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CItemSettingDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_EDIT_ITEM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	virtual void OnOK();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnSetting();
	afx_msg void OnBnClickedBtnClearall();
	afx_msg void OnBnClickedBtnClear();
	afx_msg void OnLbnSelchangeListSelect();
	afx_msg void OnLbnDblclkListSelect();

private:
	CListBox m_list;
	CListBox m_listSelect;
};
