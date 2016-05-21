#if !defined(AFX_MYTOOLBAREX_H__0D3EE008_B5D2_4FDA_896F_2AC3DFDF5EB0__INCLUDED_)
#define AFX_MYTOOLBAREX_H__0D3EE008_B5D2_4FDA_896F_2AC3DFDF5EB0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyToolBarEx.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMyToolBarEx window

class CMyToolBarEx : public CToolBar
{
// Construction
public:
	CMyToolBarEx();
    
// Attributes
public:

// Operations
public:
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyToolBarEx)
	//}}AFX_VIRTUAL

// Implementation
public:

	virtual BOOL LoadTrueColorToolBar(int  nBtnWidth,UINT uToolBar,UINT uToolBarHot		= 0,UINT uToolBarDisabled = 0);
	void AddDropDownButton(CWnd* pParent, UINT uButtonID, UINT uMenuID);
	virtual ~CMyToolBarEx();

	// Generated message map functions
protected:
	CBitmap		cBitmap;
	CImageList	cImageList[3];

protected:
	
	//{{AFX_MSG(CMyToolBarEx)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnToolbarDropDown(NMTOOLBAR* pnmh, LRESULT* plRes);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	BOOL SetTrueColorToolBar(CImageList& cImageList,UINT uToolBarType,UINT uToolBar,int  nBtnWidth);
	BOOL m_bDropDown;
	struct stDropDownInfo {
	public:
		UINT  uButtonID;
		UINT  uMenuID;
		CWnd* pParent;

	};
	

	CArray <stDropDownInfo, stDropDownInfo&> m_lstDropDownButton;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYTOOLBAREX_H__0D3EE008_B5D2_4FDA_896F_2AC3DFDF5EB0__INCLUDED_)
