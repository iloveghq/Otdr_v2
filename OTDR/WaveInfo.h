#pragma once


// CWaveInfo 对话框

class CWaveInfo : public CDialog
{
	DECLARE_DYNAMIC(CWaveInfo)

public:
	CWaveInfo(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CWaveInfo();

// 对话框数据
	enum { IDD = IDD_DIALOG_WAVEINFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};
