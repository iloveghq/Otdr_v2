#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "NumSpinCtrl.h"

// CPageTestSetup 对话框
class CClientSocket;
class CPageTestSetup : public CPropertyPage
{
	DECLARE_DYNAMIC(CPageTestSetup)

public:
	CPageTestSetup();   // 标准构造函数
	virtual ~CPageTestSetup();
	virtual BOOL OnInitDialog();
	void SendConfigData();
	void GetConfigData();
	void SetLocaleString();

// 对话框数据
	enum { IDD = IDD_PAGE_TEST_SETUP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnApply();

	DECLARE_MESSAGE_MAP()

private:
	void RecordSetting();
	void FillSendMsg(); //填充发送消息文本接收数据
	void FillGetMsg(); //构造接收消息文本

public:
	afx_msg void OnBnClickedBtnOtdr2pc();
	afx_msg void OnBnClickedBtnPc2otdr();

public:
	CEdit m_editTestTime;
	CNumSpinCtrl m_spinTestTime;
	CString m_strSendMsg;
	CString m_strGetMsg;
	CString m_strConfig;
};
