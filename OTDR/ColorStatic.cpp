#include "stdafx.h"
#include "ColorStatic.h"

CColorStatic::CColorStatic()
{
}

CColorStatic::~CColorStatic()
{
}

COLORREF CColorStatic::GetBkColor()
{
	return m_crBg;
}

void CColorStatic::SetBkColor(COLORREF crBg)
{
	m_crBg = crBg;

	m_brush.DeleteObject();
	m_brush.CreateSolidBrush(crBg);
	RedrawWindow();
}

BEGIN_MESSAGE_MAP(CColorStatic, CStatic)
	//{{AFX_MSG_MAP(CColorStatic)
	ON_WM_CTLCOLOR_REFLECT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

HBRUSH CColorStatic::CtlColor(CDC* pDC, UINT nCtlColor) 
{
	HBRUSH hbr;
	//pDC->SetBkColor(m_crBg);
	hbr = m_brush;
	return hbr;	
}
