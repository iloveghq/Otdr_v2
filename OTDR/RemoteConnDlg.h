#pragma once

// CRemoteConnDlg 对话框
class CClientSocket;

class CRemoteConnDlg : public CDialog
{
	DECLARE_DYNAMIC(CRemoteConnDlg)

public:
	CRemoteConnDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRemoteConnDlg();
	void SetLocaleString();

// 对话框数据
	enum { IDD = IDD_DIALOG_CONNECT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnConnect();
	afx_msg void OnBnClickedBtnDisconnect();
	afx_msg LRESULT OnReceive(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDisconnect(WPARAM wParam, LPARAM lParam);

private:
	void RecordSetting();

public:
	CClientSocket *m_pClientSocket;
	CString m_strIP;
	UINT m_nPort;
	DWORD m_dwIP;
};
