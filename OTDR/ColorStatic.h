#ifndef _CCOLOR_STATIC_H_
#define _CCOLOR_STATIC_H_

class CColorStatic : public CStatic
{

public:

	CColorStatic();
	virtual ~CColorStatic();
	COLORREF GetBkColor();
	void SetBkColor(COLORREF crBg);

	//{{AFX_VIRTUAL(CColorStatic)
	//}}AFX_VIRTUAL

protected:

	//{{AFX_MSG(CColorStatic)
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CBrush m_brush;
	COLORREF m_crBg;
};

//{{AFX_INSERT_LOCATION}}

#endif
