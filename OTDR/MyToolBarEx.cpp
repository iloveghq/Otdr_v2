// MyToolBarEx.cpp : implementation file
//

#include "stdafx.h"
//#include "smanage.h"
#include "MyToolBarEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyToolBarEx

CMyToolBarEx::CMyToolBarEx()
{

}

CMyToolBarEx::~CMyToolBarEx()
{
	cImageList[0].DeleteImageList ();
	cImageList[1].DeleteImageList ();
	cImageList[2].DeleteImageList ();
	cBitmap.DeleteObject ();
}


BEGIN_MESSAGE_MAP(CMyToolBarEx, CToolBar)
	//{{AFX_MSG_MAP(CMyToolBarEx)
	ON_WM_CREATE()
	//ON_NOTIFY_REFLECT(TBN_DROPDOWN, OnToolbarDropDown)//暂时无解决方法,所以zll注释,否则报错
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyToolBar message handlers

int CMyToolBarEx::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CToolBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

void CMyToolBarEx::OnToolbarDropDown(NMTOOLBAR* pnmtb, LRESULT *plr)
{
	for (int i = 0; i < m_lstDropDownButton.GetSize(); i++) {

		stDropDownInfo DropDownInfo = m_lstDropDownButton.GetAt(i);

		if (DropDownInfo.uButtonID == UINT(pnmtb->iItem)) {

			CMenu menu;
			menu.LoadMenu(DropDownInfo.uMenuID);
			CMenu* pPopup = menu.GetSubMenu(0);

			CRect rc;
			SendMessage(TB_GETRECT, (WPARAM)pnmtb->iItem, (LPARAM)&rc);
			ClientToScreen(&rc);

			pPopup->TrackPopupMenu(TPM_LEFTALIGN|TPM_LEFTBUTTON|TPM_VERTICAL,
				rc.left, rc.bottom, DropDownInfo.pParent, &rc);
			break;
		}
	}
}


BOOL CMyToolBarEx::SetTrueColorToolBar(CImageList& cImageList,UINT uToolBarType, UINT uToolBar, int nBtnWidth)
{

	BITMAP		bmBitmap;

	if (!cBitmap.Attach(LoadImage(AfxGetResourceHandle(), MAKEINTRESOURCE(uToolBar),
		IMAGE_BITMAP, 0, 0,
		LR_DEFAULTSIZE|LR_CREATEDIBSECTION)) ||
		!cBitmap.GetBitmap(&bmBitmap))
		return FALSE;

	CSize		cSize(bmBitmap.bmWidth, bmBitmap.bmHeight); 
	int			nNbBtn	= cSize.cx/nBtnWidth;
	RGBTRIPLE*	rgb		= (RGBTRIPLE*)(bmBitmap.bmBits);
	COLORREF	rgbMask	= RGB(rgb[0].rgbtRed, rgb[0].rgbtGreen, rgb[0].rgbtBlue);

	if (!cImageList.Create(nBtnWidth, cSize.cy, ILC_COLOR24|ILC_MASK, nNbBtn, 0))
		return FALSE;

	if (cImageList.Add(&cBitmap, rgbMask) == -1)
		return FALSE;

	SendMessage(uToolBarType, 0, (LPARAM)cImageList.m_hImageList);
	cBitmap.DeleteObject ();
	return TRUE;
}

void CMyToolBarEx::AddDropDownButton(CWnd *pParent, UINT uButtonID, UINT uMenuID)
{
	if (!m_bDropDown) {
		GetToolBarCtrl().SendMessage(TB_SETEXTENDEDSTYLE, 0, (LPARAM)TBSTYLE_EX_DRAWDDARROWS);
		m_bDropDown = TRUE;
	}

	SetButtonStyle(CommandToIndex(uButtonID), TBSTYLE_DROPDOWN);

	stDropDownInfo DropDownInfo;
	DropDownInfo.pParent	= pParent;
	DropDownInfo.uButtonID	= uButtonID;
	DropDownInfo.uMenuID	= uMenuID;
	m_lstDropDownButton.Add(DropDownInfo);

}

BOOL CMyToolBarEx::LoadTrueColorToolBar(int nBtnWidth, UINT uToolBar, UINT uToolBarHot, UINT uToolBarDisabled)
{
	if (!SetTrueColorToolBar(cImageList[0],TB_SETIMAGELIST, uToolBar, nBtnWidth))
		return FALSE;

	if (uToolBarHot) {
		if (!SetTrueColorToolBar(cImageList[1],TB_SETHOTIMAGELIST, uToolBarHot, nBtnWidth))
			return FALSE;
	}

	if (uToolBarDisabled) {
		if (!SetTrueColorToolBar(cImageList[2],TB_SETDISABLEDIMAGELIST, uToolBarDisabled, nBtnWidth))
			return FALSE;
	}

	return TRUE;
}

