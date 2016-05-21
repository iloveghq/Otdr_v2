#pragma once

#include "resource.h"
#include "Eventlist.h"
// CUnitPage 对话框

class CUnitPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CUnitPage)

public:
	CUnitPage();
	virtual ~CUnitPage();

// 对话框数据
	enum { IDD = IDD_PROPPAGE_UNIT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	virtual BOOL OnApply();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedRadioUnitKm();
	afx_msg void OnBnClickedRadioUnitMile();
	afx_msg void OnBnClickedRadioUnitKf();
public:
	CWaveform *m_pWaveFormView;//波形视图
	CEventlist *m_pEventFormView;//事件列表视图
};
