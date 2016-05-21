#pragma once
#include "resource.h"
#include "afxwin.h"
#include "DrawStaticWave.h"

// CWavePage 对话框

class CWavePage : public CPropertyPage
{
	DECLARE_DYNAMIC(CWavePage)

public:
	CWavePage();   // 标准构造函数
	virtual ~CWavePage();

// 对话框数据
	enum { IDD = IDD_PROPPAGE_WAVE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	virtual BOOL OnApply();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedRadioAvgprocShow();
	afx_msg void OnBnClickedRadioAvgprocHide();
	afx_msg void OnBnClickedRadioCursorCrossline();
	afx_msg void OnBnClickedRadioCursorLine();
	afx_msg void OnBnClickedRadioGirdShow();
	afx_msg void OnBnClickedRadioGridHide();
	afx_msg void OnBnClickedRadioDoubleCursorShow();
	afx_msg void OnBnClickedRadioDoubleCursorHide();

private:
	bool m_bAvgProc; //是否在平均化处理中
	CRect m_drawRect; //绘制区域大小
public:
	CDrawStaticWave m_drawStatic;
};
