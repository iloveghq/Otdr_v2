#pragma once
#include "afxcmn.h"
#include "afxdtctl.h"

// CPageSystemSetting 对话框
class CClientSocket;
class CPageSystemSetting : public CPropertyPage
{
	DECLARE_DYNAMIC(CPageSystemSetting)

public:
	CPageSystemSetting();
	virtual ~CPageSystemSetting();
	virtual BOOL OnInitDialog();
	void SendConfigData();
	void GetConfigData();
	void SetLocaleString();

// 对话框数据
	enum { IDD = IDD_PAGE_SYSTEM_SETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnApply();

	DECLARE_MESSAGE_MAP()
	void Send();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar); 
	afx_msg void OnBnClickedBtnOtdr2pc();
	afx_msg void OnBnClickedBtnPc2otdr();

private:
	void RecordSetting();
	void FillSendMsg(); //填充发送消息文本接收数据
	void FillGetMsg(); //构造接收消息文本

public:
	CSliderCtrl m_slideTouchLight;

private:
	CString m_strSendMsg;
	CString m_strGetMsg;
	CString m_strConfig;
	CDateTimeCtrl m_date;
	CDateTimeCtrl m_time;
};
