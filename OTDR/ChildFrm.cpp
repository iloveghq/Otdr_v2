// ChildFrm.cpp : implementation of the CChildFrame class
//
#include "stdafx.h"
#include "OTDR.h"

#include "ChildFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWnd)
	ON_WM_GETMINMAXINFO()
END_MESSAGE_MAP()


// CChildFrame construction/destruction

CChildFrame::CChildFrame()
{
	// TODO: add member initialization code here
}

CChildFrame::~CChildFrame()
{
}


BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying the CREATESTRUCT cs
	if( !CMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}


// CChildFrame diagnostics

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
	CMDIChildWnd::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
}

#endif //_DEBUG


// CChildFrame message handlers

/*void CChildFrame::RecalcLayout(BOOL bNotify)
{
	if(IsZoomed())
	{
		LPMAIN->SetAllowLayout();
	}
	CMDIChildWnd::RecalcLayout(bNotify);
}*/

/*void CChildFrame::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO: Add your message handler code here and/or call default
	lpMMI->ptMaxTrackSize=CSize(400,300);
	lpMMI->ptMinTrackSize=CSize(400,300);

	CFrameWnd::OnGetMinMaxInfo(lpMMI);
}*/
