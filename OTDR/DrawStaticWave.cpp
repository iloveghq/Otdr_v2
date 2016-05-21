#include "stdafx.h"
#include "DrawStaticWave.h"

extern SorFileArray g_sorFileArray;

IMPLEMENT_DYNAMIC(CDrawStaticWave, CStatic)

CDrawStaticWave::CDrawStaticWave()
{
}

CDrawStaticWave::~CDrawStaticWave()
{
}

BEGIN_MESSAGE_MAP(CDrawStaticWave, CStatic)
	ON_WM_PAINT()
END_MESSAGE_MAP()



void CDrawStaticWave::DrawBackGround(CDC *pDC) //绘制背景
{
	//获取背景颜色
	CString strBack = IniUtil::ReadSingleConfigInfo(_T("Color"), _T("Back"));
	COLORREF colBack = _wtol(strBack);
	GetClientRect(m_drawRect);
	pDC->FillSolidRect(&m_drawRect, colBack);  //坐标轴内的矩形区域
}

void CDrawStaticWave::DrawGridLine(CDC *pDC) //绘制网格线
{
	//不显示网格
	if (!g_sorFileArray.waveConfig.ShowGrid)
		return;

	//获取栅格线颜色
	CString strGrid = IniUtil::ReadSingleConfigInfo(_T("Color"), _T("Grid"));
	COLORREF colGrid = _wtol(strGrid);
	GetClientRect(m_drawRect);

	CPen penGridLine;
	penGridLine.CreatePen (PS_DOT/*点*/, 1, colGrid);

	float ndx = m_drawRect.Width () / 5;
	float ndy = m_drawRect.Height () / 4;

	CPen* pOldPen = pDC->SelectObject (&penGridLine);
	//pDC->SetBkColor(CLR_OTDR_BACKGROUND);
	int bottom = m_drawRect.Height();
	int left = 0;
	int itemp;//
	for ( int i = 1; i <5; i++)
	{	
		itemp=left + ndx * i;
		//横坐标刻度从上到下-------------------------------------------------------
		pDC->MoveTo (itemp,bottom);
		pDC->LineTo (itemp,0);
	}
	//纵坐标:::	从左到右
	for (int i=1; i < 4; i++)
	{
		itemp = bottom - ndy * i;

		pDC->MoveTo (left , itemp);
		pDC->LineTo (m_drawRect.right,itemp);

	}

	pDC->SelectObject(pOldPen);
	//release the gdi
	penGridLine.DeleteObject();
}

void CDrawStaticWave::DrawLine(CDC *pDC) //绘制AB线
{
	//获取光标线颜色
	CString strCursor = IniUtil::ReadSingleConfigInfo(_T("Color"), _T("Cursor"));
	COLORREF colCursor = _wtol(strCursor);
	GetClientRect(m_drawRect);

	CPen penCursorA, penCursorB;
	penCursorA.CreatePen (PS_SOLID/*实线*/, 1, colCursor);
	penCursorB.CreatePen (PS_DOT/*点*/, 1, colCursor);

	float ndx = m_drawRect.Width() / 3;

	CPen* pOldPen = pDC->SelectObject (&penCursorA);
	int left1 = ndx;
	int left2 = 2 * ndx;
	int top = 0;
	int height = m_drawRect.Height();
	//Line 1
	pDC->MoveTo (left1, top);
	pDC->LineTo (left1, height);
	//Line 2
	pOldPen = pDC->SelectObject (&penCursorB);
	//double cursor
	if (g_sorFileArray.waveConfig.ShowDoubleCursor)
	{
		pDC->MoveTo (left2, top);
		pDC->LineTo (left2, height);
	}
	pDC->SelectObject(pOldPen);
	//release the gdi
	penCursorA.DeleteObject();
	penCursorB.DeleteObject();
}

void CDrawStaticWave::DrawCursor(CDC *pDC) //绘制光标
{
	if (!g_sorFileArray.waveConfig.ShowCursor)
		return;

	//获取光标颜色
	CString strCursor = IniUtil::ReadSingleConfigInfo(_T("Color"), _T("Cursor"));
	COLORREF colCursor = _wtol(strCursor);
	GetClientRect(m_drawRect);

	CPen penCursor;
	penCursor.CreatePen (PS_SOLID/*点*/, 1, colCursor);

	float ndy = m_drawRect.Height () / 3;

	CPen* pOldPen = pDC->SelectObject (&penCursor);
	int right = m_drawRect.Width();
	int left = 0;
	int height = ndy;

	pDC->MoveTo (left,height);
	pDC->LineTo (right,height);

	pDC->SelectObject(pOldPen);
	//release the gdi
	penCursor.DeleteObject();
}

void CDrawStaticWave::OnPaint()
{
	CPaintDC dc(this);

	//使用双缓冲技术画图
	CRect ClientRect;
	GetClientRect(&ClientRect);

	CRect InvalidRect;
	dc.GetClipBox(&InvalidRect); //检取无效区,即需要重绘的区域

	CDC MemDC;
	CBitmap MemBitMap;
	CBitmap *pOldBitMap;

	//创建一个与dc兼容的内存内存设备环境
	if (MemDC.CreateCompatibleDC(&dc))
	{
		//创建一个与dc兼容的位图，大小为整个客户区
		if (MemBitMap.CreateCompatibleBitmap(&dc,ClientRect.Width(),ClientRect.Height()))
		{
			//将位图选入内存环境
			pOldBitMap=MemDC.SelectObject(&MemBitMap);

			//使MemDC的整个客户区都成无效区
			MemDC.SelectClipRgn(NULL);
			//再 "与上" 检取的无效区，使内存环境与dc检取的无效区相等
			MemDC.IntersectClipRect(InvalidRect);

			//用背景色将位图清除干净
			MemDC.FillSolidRect(&ClientRect,RGB(236,233,216));
		}

	}

	DrawBackGround(&MemDC);
	DrawGridLine(&MemDC);
	DrawLine(&MemDC);
	DrawCursor(&MemDC);

	//把绘制好的图形用BitBlt()"拷贝"到屏幕上
	dc.BitBlt(0,0,ClientRect.Width(),ClientRect.Height(),&MemDC, 0, 0,SRCCOPY); 
	MemDC.SelectObject(pOldBitMap);
	MemBitMap.DeleteObject();
	MemDC.DeleteDC();
}
