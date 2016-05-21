#pragma once

#include "resource.h"
#include "afxwin.h"
#include "ColorStatic.h"
#include "DrawStaticColor.h"
// CColorPage 对话框

class CColorPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CColorPage)

public:
	CColorPage();
	virtual ~CColorPage();

// 对话框数据
	enum { IDD = IDD_PROPPAGE_COLOR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	virtual BOOL OnApply();

	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedBtnMod1();
	afx_msg void OnBnClickedBtnMod2();
	afx_msg void OnBnClickedBtnMod3();
	afx_msg void OnBnClickedBtnMod4();
	afx_msg void OnBnClickedBtnMod5();
	afx_msg void OnBnClickedBtnMod6();
	afx_msg void OnBnClickedBtnMod7();
	afx_msg void OnBnClickedBtnMod8();
	afx_msg void OnBnClickedBtnBgMod();
	afx_msg void OnBnClickedBtnBgGirdMod();
	afx_msg void OnBnClickedBtnOtherEventMarkerMod();
	afx_msg void OnBnClickedBtnOtherCursorMod();

private:
	void ModifyWaveColor(CColorStatic& control, int nIndex);

public:
	CComboBox m_ComboColSel;
	CColorStatic m_colorStatic1;
	CColorStatic m_colorStatic2;
	CColorStatic m_colorStatic3;
	CColorStatic m_colorStatic4;
	CColorStatic m_colorStatic5;
	CColorStatic m_colorStatic6;
	CColorStatic m_colorStatic7;
	CColorStatic m_colorStatic8;
	CColorStatic m_colorStaticBack;
	CColorStatic m_colorStaticGrid;
	CColorStatic m_colorStaticEventMarker;
	CColorStatic m_colorStaticCursor;

	CDrawStaticColor m_drawStatic;
};
