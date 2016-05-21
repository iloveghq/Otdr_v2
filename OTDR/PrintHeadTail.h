#pragma once
#include "afxcmn.h"
#include "NumSpinCtrl.h"
#include "afxwin.h"

// CPrintHeadTail 对话框

class CPrintHeadTail : public CPropertyPage
{
	DECLARE_DYNAMIC(CPrintHeadTail)

public:
	CPrintHeadTail();
	virtual ~CPrintHeadTail();

// 对话框数据
	enum { IDD = IDD_PAGE_PRINT_HEADTAIL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL OnApply();

private:
	CNumSpinCtrl m_spinStartNum;
	CNumSpinCtrl m_spinSectionNum;
	CComboBox m_comboxUpDown;
	CComboBox m_comboxLeftRight;
};
