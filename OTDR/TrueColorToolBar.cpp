// TrueColorToolBar.cpp : implementation file
//

#include "stdafx.h"
#include "TrueColorToolBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTrueColorToolBar

CTrueColorToolBar::CTrueColorToolBar()
{
}

CTrueColorToolBar::~CTrueColorToolBar()
{
}


BEGIN_MESSAGE_MAP(CTrueColorToolBar, CToolBar)
	//{{AFX_MSG_MAP(CTrueColorToolBar)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTrueColorToolBar message handlers
BOOL CTrueColorToolBar::LoadTrueColorToolBar(int nBtnWidth,
											 UINT uToolBar,
											 UINT uToolBarHot,
											 UINT uToolBarDisabled)
{
	if (!SetTrueColorToolBar(TB_SETIMAGELIST, uToolBar, nBtnWidth))
		return FALSE;
	
	if (!SetTrueColorToolBar(TB_SETHOTIMAGELIST, uToolBarHot, nBtnWidth))
		return FALSE;

	if (uToolBarDisabled) {
	if (!SetTrueColorToolBar(TB_SETDISABLEDIMAGELIST, uToolBarDisabled, nBtnWidth))
		return FALSE;
	}

	return TRUE;
}


BOOL CTrueColorToolBar::SetTrueColorToolBar(UINT uToolBarType,
							     	        UINT uToolBar,
										    int nBtnWidth)
{
	CImageList	cImageList;
	CBitmap		cBitmap;
	BITMAP		bmBitmap;
	CSize		cSize;
	int			nNbBtn;
	
	if (!cBitmap.Attach(LoadImage(AfxGetInstanceHandle(),
								  MAKEINTRESOURCE(uToolBar),
								  IMAGE_BITMAP, 0, 0,
		                          LR_DEFAULTSIZE|LR_CREATEDIBSECTION)) ||
	    !cBitmap.GetBitmap(&bmBitmap))
		return FALSE;

	cSize  = CSize(bmBitmap.bmWidth, bmBitmap.bmHeight); 
	nNbBtn = cSize.cx/nBtnWidth;
	RGBTRIPLE* rgb		= (RGBTRIPLE*)(bmBitmap.bmBits);
	COLORREF   rgbMask	= RGB(rgb[0].rgbtRed,
		                      rgb[0].rgbtGreen,
							  rgb[0].rgbtBlue);
	
	if (!cImageList.Create(nBtnWidth, cSize.cy,
		                   ILC_COLOR24|ILC_MASK,
						   nNbBtn, 0))
		return FALSE;
	
	if (cImageList.Add(&cBitmap, rgbMask) == -1)
		return FALSE;

/*
	if (uToolBarType == TB_SETIMAGELIST)
		GetToolBarCtrl().SetImageList(&cImageList);
	else if (uToolBarType == TB_SETHOTIMAGELIST)
		GetToolBarCtrl().SetHotImageList(&cImageList);
	else if (uToolBarType == TB_SETDISABLEDIMAGELIST)
		GetToolBarCtrl().SetDisabledImageList(&cImageList);
	else 
		return FALSE;
*/

	SendMessage(uToolBarType, 0, (LPARAM)cImageList.m_hImageList);
	cImageList.Detach(); 
	cBitmap.Detach();
	
	return TRUE;
}

