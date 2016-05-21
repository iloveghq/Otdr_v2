#pragma once
#include "afxcmn.h"


// CGrandwayProgress 对话框

class CGrandwayProgress : public CDialog
{
	DECLARE_DYNAMIC(CGrandwayProgress)

public:
	CGrandwayProgress(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CGrandwayProgress();
	void SetPos(int nPos);

// 对话框数据
	enum { IDD = IDD_DIALOG_PROGRRESS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

private:
	CProgressCtrl m_progress;
};
