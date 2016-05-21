#if !defined(AFX_TRUECOLORTOOLBAR_H__4D431A79_0E34_4E0C_BE1C_387E90FCA1F3__INCLUDED_)
#define AFX_TRUECOLORTOOLBAR_H__4D431A79_0E34_4E0C_BE1C_387E90FCA1F3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TrueColorToolBar.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTrueColorToolBar window

class CTrueColorToolBar : public CToolBar
{
// Construction
public:
	CTrueColorToolBar();

// Attributes
private:

	
// Operations
public:
	BOOL LoadTrueColorToolBar(int nBtnWidth,
							  UINT uToolBar,
							  UINT uToolBarHot,
							  UINT uToolBarDisabled = 0);

private:
	BOOL SetTrueColorToolBar(UINT uToolBarType,
		                     UINT uToolBar,
						     int nBtnWidth);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTrueColorToolBar)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTrueColorToolBar();

	// Generated message map functions
protected:
	//{{AFX_MSG(CTrueColorToolBar)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRUECOLORTOOLBAR_H__4D431A79_0E34_4E0C_BE1C_387E90FCA1F3__INCLUDED_)
