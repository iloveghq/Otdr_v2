#include "StdAfx.h"
#include "MyListCtrl.h"
#include "resource.h"

//HBITMAP CMyListCtrl::m_bmpNormal;//wcq2012.03.02
//HBITMAP CMyListCtrl::m_bmpSele;//wcq2012.03.02
//HBITMAP CMyListCtrl::m_bmpDisNormal;//wcq2012.03.02
//HBITMAP CMyListCtrl::m_bmpDisSele;//wcq2012.03.02
//const int LB_ITEM_HEIGHT = 20;
BEGIN_MESSAGE_MAP(CMyListCtrl, CListCtrl)
    //{{AFX_MSG_MAP(CCeListCtrlEx)
    ON_WM_SETFOCUS()
    ON_WM_KILLFOCUS()
    ON_WM_LBUTTONDOWN()
    ON_WM_CREATE()
    //}}AFX_MSG_MAP
    ON_WM_ERASEBKGND()
    ON_WM_MEASUREITEM_REFLECT( )
END_MESSAGE_MAP()

CMyListCtrl::CMyListCtrl(void)
{
	m_font = &g_fontSmall;
}

CMyListCtrl::~CMyListCtrl(void)
{
    if (m_bmpNormal)
    {
        DeleteObject(m_bmpNormal);
        m_bmpNormal = NULL;
    }

    if (m_bmpSele)
    {
        DeleteObject(m_bmpSele);
        m_bmpSele = NULL;
    }

    if (m_bmpDisNormal)
    {
        DeleteObject(m_bmpDisNormal);
        m_bmpDisNormal = NULL;
    }

    if (m_bmpDisSele)
    {
        DeleteObject(m_bmpDisSele);
        m_bmpDisSele = NULL;
    }
}

void CMyListCtrl::PreSubclassWindow()
{

    Init();
	//m_font.CreateFont(FONT_SIZE_SMALL, 0, 0, 0, FW_NORMAL, 0, 0, 0, GB2312_CHARSET, 0, 0, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_MODERN, FONT_NAME_CHS);////wcq2012.02.15//改为全局字体wcq2012.03.07

    CListCtrl::PreSubclassWindow();
}

void CMyListCtrl::DrawItem( LPDRAWITEMSTRUCT lpDrawItemStruct )
{
    ASSERT(::IsWindow(m_hWnd));
    ASSERT(lpDrawItemStruct != 0);
    CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
    CRect rcItem(lpDrawItemStruct->rcItem);
    int nItem = lpDrawItemStruct->itemID;

    LV_ITEM lvi;
    lvi.mask = LVIF_STATE;
    lvi.iItem = nItem;
    lvi.iSubItem = 0;
    lvi.stateMask = 0xFFFF;     // get all state flags
    GetItem(&lvi);

    BOOL bHighlight = lvi.state & LVIS_FOCUSED;

    CRect rcBounds;
    GetItemRect(nItem, rcBounds, LVIR_BOUNDS);


    HBITMAP		hbmOldBmp	= NULL;

    CDC bitmapDC;
    bitmapDC.CreateCompatibleDC(pDC);

   // PaintBk(pDC, rcItem);//cwcq
	pDC->FillSolidRect(&rcItem,COLOR_BG_GROUP);//背景
	if (IsWindowEnabled())
	{
		if (lvi.state & LVIS_FOCUSED)//选中
		{
			hbmOldBmp = (HBITMAP)bitmapDC.SelectObject(m_bmpSele);//图标
		}
		else
		{
			hbmOldBmp = (HBITMAP)bitmapDC.SelectObject(m_bmpNormal);//图标
		}//eif
		pDC->SetTextColor(COLOR_TEXT_RADIO);
	}
	else
	{
		if (lvi.state & LVIS_FOCUSED)//选中
		{
			hbmOldBmp = (HBITMAP)bitmapDC.SelectObject(m_bmpDisSele);//图标
		}
		else
		{
			hbmOldBmp = (HBITMAP)bitmapDC.SelectObject(m_bmpDisNormal);//图标
		}//eif
		pDC->SetTextColor(COLOR_TEXT_RADIO_DISABLE);
	}//eif
	pDC->BitBlt(rcItem.left, rcItem.top + (rcItem.Height() - WIDTH_RADIO)/2 +1 , WIDTH_RADIO, WIDTH_RADIO, &bitmapDC,0,0,SRCCOPY);//文本
	bitmapDC.SelectObject(hbmOldBmp);
	rcItem.OffsetRect(WIDTH_RADIO+2,0);
	CString sLabel = GetItemText(nItem, 0);
	//CFont* pOldFont = pDC->SelectObject(&m_font);//改为全局字体wcq2012.03.07
	CFont* pOldFont = pDC->SelectObject(&g_fontSmall);//wcq2012.03.07
	pDC->DrawText(sLabel,rcItem, DT_LEFT | DT_VCENTER |DT_SINGLELINE);
	pDC->SelectObject(pOldFont);
	return;
	///////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////
}

void CMyListCtrl::RepaintSelectedItems()
{

}

void CMyListCtrl::Init()
{
    // Change the style of the control
    VERIFY(ModifyStyle(LVS_TYPEMASK       | // this styles are removed
        LVS_SHOWSELALWAYS  |
        LVS_EDITLABELS,
        LVS_REPORT         | // this styles are added
        LVS_OWNERDRAWFIXED |
        LVS_NOCOLUMNHEADER |
        LVS_SINGLESEL));

    // Insert at least one column to the list control
    VERIFY(InsertColumn(0, _T("Column"), LVCFMT_LEFT) == 0);
    SetColumnWidth(0, LVSCW_AUTOSIZE_USEHEADER);
}

void CMyListCtrl::OnSetFocus( CWnd* pOldWnd )
{

}

void CMyListCtrl::OnKillFocus( CWnd* pNewWnd )
{

}

void CMyListCtrl::OnLButtonDown( UINT nFlags, CPoint point )
{
    Default();
}

int CMyListCtrl::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
    if (CListCtrl::OnCreate(lpCreateStruct) == -1)
        return -1;

    // TODO: Add your specialized creation code here

    Init();

    return 0;
}
//
//void MyListCtrl::SetBk( CDC *pDC )
//{
//    if (m_dcBk.m_hDC != NULL && m_pbmpOldBk != NULL)
//    {
//        m_dcBk.SelectObject(m_pbmpOldBk);
//    } // if
//
//    m_bmpBk.DeleteObject();
//    m_dcBk.DeleteDC();
//
//    CRect rect;
//    CRect rect1;
//
//    GetClientRect(rect);
//
//    GetWindowRect(rect1);
//    GetParent()->ScreenToClient(rect1);
//
//    m_dcBk.CreateCompatibleDC(pDC);
//    m_bmpBk.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
//    m_pbmpOldBk = m_dcBk.SelectObject(&m_bmpBk);
//    m_dcBk.BitBlt(0, 0, rect.Width(), rect.Height(), pDC, rect1.left, rect1.top, SRCCOPY);
//}

void CMyListCtrl::PaintBk(CDC *pDC, CRect& rect)
{
  //  pDC->BitBlt(rect.left,rect.top,rect.Width(),rect.Height(),&m_dcBk,rect.left, rect.top,SRCCOPY);//cwcq
	pDC->FillSolidRect(&rect,COLOR_BG_GROUP);//wcq2012.03.01
}

void CMyListCtrl::MeasureItem( LPMEASUREITEMSTRUCT lpMeasureItemStruct )
{
 lpMeasureItemStruct->itemHeight=RADIO_HEIGHT_LINE;
}

BOOL CMyListCtrl::OnEraseBkgnd(CDC* pDC)
{
    // TODO: ?????????????/??????
    CListCtrl::OnEraseBkgnd(pDC);
    CRect rect;
    GetClientRect(rect);
    PaintBk(pDC, rect);
    return TRUE;
}
//
//void MyListCtrl::SetItemHighlightImg( UINT id )
//{
//    if (m_SelImg != NULL)
//    {
//        DeleteObject(m_SelImg);
//    }
//    AssignImg(id, &m_SelImg);
//}
//
//void MyListCtrl::SetItemIcon( UINT id )
//{
//    if (m_ItemIcon != NULL)
//    {
//        DeleteObject(m_ItemIcon);
//    }
//    AssignImg(id, &m_ItemIcon);
//}
//
//void MyListCtrl::SetItemCheckedImg( UINT id )
//{
//    if (m_CbChecked != NULL)
//    {
//        DeleteObject(m_CbChecked);
//    }
//    AssignImg(id, &m_CbChecked);
//}
//
//void MyListCtrl::SetItemUnCheckedImg( UINT id )
//{
//    if (m_CbUnChecked != NULL)
//    {
//        DeleteObject(m_CbUnChecked);
//    }
//    AssignImg(id, &m_CbUnChecked);
//}

void CMyListCtrl::SetItemImg(UINT idNormal,UINT idSele,UINT idDisableNormal,UINT idDisableSele)
{
	if (m_bmpNormal != NULL)
	{
		DeleteObject(m_bmpNormal);
	}
	AssignImg(idNormal, &m_bmpNormal);

	if (m_bmpSele != NULL)
	{
		DeleteObject(m_bmpSele);
	}
	AssignImg(idSele, &m_bmpSele);

	if (m_bmpDisNormal != NULL)
	{
		DeleteObject(m_bmpDisNormal);
	}
	AssignImg(idDisableNormal, &m_bmpDisNormal);

	if (m_bmpDisSele != NULL)
	{
		DeleteObject(m_bmpDisSele);
	}
	AssignImg(idDisableSele, &m_bmpDisSele);

}
void CMyListCtrl::AssignImg( UINT id, HBITMAP* hBMP )
{
    *hBMP = (HBITMAP)::LoadImage(AfxGetApp()->m_hInstance, MAKEINTRESOURCE(id), IMAGE_BITMAP, 0, 0, 0);
}

//选择下标从0开始wcq2012.03.01
INT CMyListCtrl::GetSeleIndex()
{
	POSITION pos;
	pos=GetFirstSelectedItemPosition();
	return GetNextSelectedItem(pos);
}
//设置选中下标从0开始wcq2012.03.01
void CMyListCtrl::SetSeleIndex(int nIndex)
{
	SetItemState(nIndex, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
}