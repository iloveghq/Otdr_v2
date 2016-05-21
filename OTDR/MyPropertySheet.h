#pragma once
#include "PropConfig.h"
#include "PropContent.h"
#include "PropJob.h"
#include "IniUtil.h"
#include "PropStep5New.h"
#include "MyPropertySheet.h"


// CMyPropertySheet

class CMyPropertySheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CMyPropertySheet)

public:
	CMyPropertySheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CMyPropertySheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CMyPropertySheet();
	
	void OnInitPages(); //初始化各属性页

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();

	CPropConfig m_propConfig;
	CPropContent m_propContent;
	CPropJob m_propJob;
	//HBRUSH m_bkBrush;   //对话框背景刷
	//CWnd *m_pWnd;
	
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	void SaveSetting();

	void SetLocalString();

private:
	CString m_strTabCotent;
	CString m_strTabConfig;
	CString m_strTabJob;
	CString m_strBtnOK;
	CString m_strBtnCancel;
};


