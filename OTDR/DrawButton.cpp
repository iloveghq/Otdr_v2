#include "StdAfx.h"
#include "DrawButton.h"
extern SorFileArray g_sorFileArray;

CDrawButton::CDrawButton(void):m_nStep(1),m_nCurveNumber(1),m_bReportDraw(false)
{
	//打印方向
	if (g_sorFileArray.reportConfig.Print.CompareNoCase(_T("0")) == 0)
	{
		m_bReportDraw = true;
	}
	else if (g_sorFileArray.reportConfig.Print.CompareNoCase(_T("1")) == 0)
	{
		m_bReportDraw = false;
	}
	//设置
	m_nCurveNumber = _wtoi(g_sorFileArray.reportConfig.WaveList);
	m_nListFormat = _wtoi(g_sorFileArray.reportConfig.ListFormat);
	//打印方向
	if (g_sorFileArray.reportConfig.Direction.CompareNoCase(_T("0")) == 0)
	{
		m_bHorLayout = true;
	}
	else if (g_sorFileArray.reportConfig.Direction.CompareNoCase(_T("1")) == 0)
	{
		m_bHorLayout = false;
	}
}

CDrawButton::~CDrawButton(void)
{
}

BEGIN_MESSAGE_MAP(CDrawButton, CButton)
	ON_WM_PAINT()
END_MESSAGE_MAP()

void CDrawButton::OnPaint()
{
	CPaintDC dc(this);

	//使用双缓冲技术画图
	GetClientRect(&m_rectDraw);

	CRect InvalidRect;
	dc.GetClipBox(&InvalidRect); //检取无效区,即需要重绘的区域

	CDC MemDC;
	CBitmap MemBitMap;
	CBitmap *pOldBitMap = NULL;

	//创建一个与dc兼容的内存内存设备环境
	if (MemDC.CreateCompatibleDC(&dc))
	{
		//创建一个与dc兼容的位图，大小为整个客户区
		if (MemBitMap.CreateCompatibleBitmap(&dc,m_rectDraw.Width(),m_rectDraw.Height()))
		{
			//将位图选入内存环境
			pOldBitMap=MemDC.SelectObject(&MemBitMap);

			//使MemDC的整个客户区都成无效区
			MemDC.SelectClipRgn(NULL);
			//再 "与上" 检取的无效区，使内存环境与dc检取的无效区相等
			MemDC.IntersectClipRect(InvalidRect);

			//用背景色将位图清除干净
			MemDC.FillSolidRect(&m_rectDraw, RGB(236,233,216));
		}
	}
	InnerDraw(&MemDC);

	//把绘制好的图形用BitBlt()"拷贝"到屏幕上
	dc.BitBlt(0,0,m_rectDraw.Width(),m_rectDraw.Height(),&MemDC, 0, 0,SRCCOPY); 
	if (pOldBitMap)
		MemDC.SelectObject(pOldBitMap);
	MemBitMap.DeleteObject();
	MemDC.DeleteDC();
}

void CDrawButton::Draw1Curve(CDC* pDC)
{
	int nSaveDC = pDC->SaveDC();
	CPen pen;
	pen.CreatePen (PS_SOLID, 1, RGB(0,0,0));
	pDC->SelectObject(&pen);//
	//画线
	int nVspace = m_rectDraw.Height() / 64;
	int nCurHeight = nVspace * 3;
	int nHleftspace = m_rectDraw.Width() / 10;
	int nHmidspace = m_rectDraw.Width() / 20;
	//标题
	pDC->MoveTo(m_rectDraw.Width() / 3, nCurHeight);
	pDC->LineTo(m_rectDraw.Width() * 2/3, nCurHeight);
	//内容
	for (int i=1; i<=6; i++)
	{
		nCurHeight = nCurHeight + nVspace;
		pDC->MoveTo(nHleftspace, nCurHeight);
		pDC->LineTo((m_rectDraw.Width() - nHmidspace)/2 ,nCurHeight);

		pDC->MoveTo((m_rectDraw.Width() + nHmidspace)/2, nCurHeight);
		pDC->LineTo((m_rectDraw.Width() - nHleftspace), nCurHeight);
	}
	//分割线
	{
		nCurHeight = nCurHeight + nVspace * 3;
		pDC->MoveTo(nHleftspace, nCurHeight);
		pDC->LineTo((m_rectDraw.Width() - nHmidspace)/3 ,nCurHeight);

		pDC->MoveTo((m_rectDraw.Width()+ nHmidspace)/3, nCurHeight);
		pDC->LineTo((m_rectDraw.Width() - nHmidspace) *2/3 , nCurHeight);

		pDC->MoveTo(m_rectDraw.Width()* 2/3, nCurHeight);
		pDC->LineTo((m_rectDraw.Width() - nHleftspace), nCurHeight);
	}
	//曲线
	{
		//大矩形
		nCurHeight = nCurHeight + nVspace * 2;
		CRect curveRect;
		curveRect.SetRect(nHleftspace*2 + nHmidspace/2, nCurHeight, m_rectDraw.Width() - nHleftspace, nCurHeight + nVspace * 20);
		pDC->SelectObject(GetStockObject(NULL_BRUSH));
		pDC->Rectangle(&curveRect);
		//左侧小矩形
		CRect leftRect;
		leftRect.SetRect(nHleftspace, curveRect.top + curveRect.Height()/2-nVspace, curveRect.left - nHmidspace/2, curveRect.top + curveRect.Height()/2+nVspace);
		pDC->Rectangle(&leftRect);
		//底侧小矩形
		CRect bottomRect;
		bottomRect.SetRect(curveRect.left + curveRect.Width()/2-nHleftspace, curveRect.bottom +nVspace/2, curveRect.left + curveRect.Width()/2+nHleftspace, curveRect.bottom +nVspace*3/2);
		pDC->Rectangle(&bottomRect);
	}
	//事件列表
	{
		nCurHeight = nCurHeight + nVspace * 27;
		int nCurLeft = 0;
		for (int nLine=0; nLine<=12;nLine++)
		{
			nCurLeft = nHleftspace;
			for (int nRow=0; nRow<8;nRow++)
			{
				pDC->MoveTo(nCurLeft, nCurHeight);
				pDC->LineTo(nCurLeft+nHleftspace/1.1, nCurHeight);
				nCurLeft = nCurLeft + nHleftspace;
			}
			nCurHeight = nCurHeight + nVspace * 1.5;
		}
	}

	pDC->RestoreDC(nSaveDC);
	pen.DeleteObject();
}


void CDrawButton::Draw1CurveTitle(CDC* pDC)
{
	int nSaveDC = pDC->SaveDC();
	CPen pen;
	pen.CreatePen (PS_DOT, 3, RGB(255,0,0));
	pDC->SelectObject(&pen);//
	//画线
	int nVspace = m_rectDraw.Height() / 64;
	int nCurHeight = nVspace * 3;
	int nHleftspace = m_rectDraw.Width() / 10;
	int nHmidspace = m_rectDraw.Width() / 20;
	//曲线标题矩形
	CRect m_rectTitle;
	nCurHeight = nCurHeight + nVspace * 8;
	m_rectTitle.left = nHleftspace - nHmidspace;
	m_rectTitle.top = nCurHeight;
	m_rectTitle.right = m_rectDraw.Width() - nHleftspace + nHmidspace;
	m_rectTitle.bottom = nCurHeight +  nVspace * 2;
	
	pDC->SelectObject(GetStockObject(NULL_BRUSH));
	pDC->Rectangle(&m_rectTitle);

	pDC->RestoreDC(nSaveDC);
	pen.DeleteObject();
}

void CDrawButton::Draw2Curve(CDC* pDC)
{
	int nSaveDC = pDC->SaveDC();
	CPen pen;
	pen.CreatePen (PS_SOLID, 1, RGB(0,0,0));
	pDC->SelectObject(&pen);//
	//画线
	int nVspace = m_rectDraw.Height() / 64;
	int nCurHeight = nVspace * 3;
	int nHleftspace = m_rectDraw.Width() / 10;
	int nHmidspace = m_rectDraw.Width() / 20;
	//标题
	pDC->MoveTo(m_rectDraw.Width() / 3, nCurHeight);
	pDC->LineTo(m_rectDraw.Width() * 2/3, nCurHeight);
	//内容
	for (int i=1; i<=6; i++)
	{
		nCurHeight = nCurHeight + nVspace;
		pDC->MoveTo(nHleftspace, nCurHeight);
		pDC->LineTo((m_rectDraw.Width() - nHmidspace)/2 ,nCurHeight);

		pDC->MoveTo((m_rectDraw.Width() + nHmidspace)/2, nCurHeight);
		pDC->LineTo((m_rectDraw.Width() - nHleftspace), nCurHeight);
	}
	//分割线
	{
		nCurHeight = nCurHeight + nVspace * 3;
		pDC->MoveTo(nHleftspace, nCurHeight);
		pDC->LineTo((m_rectDraw.Width() - nHmidspace)/3 ,nCurHeight);

		pDC->MoveTo((m_rectDraw.Width()+ nHmidspace)/3, nCurHeight);
		pDC->LineTo((m_rectDraw.Width() - nHmidspace) *2/3 , nCurHeight);

		pDC->MoveTo(m_rectDraw.Width()* 2/3, nCurHeight);
		pDC->LineTo((m_rectDraw.Width() - nHleftspace), nCurHeight);
	}
	//曲线
	{
		//大矩形
		nCurHeight = nCurHeight + nVspace * 2;
		CRect curveRect;
		curveRect.SetRect(nHleftspace*2 + nHmidspace/2, nCurHeight, m_rectDraw.Width() - nHleftspace, nCurHeight + nVspace * 20);
		pDC->SelectObject(GetStockObject(NULL_BRUSH));
		pDC->Rectangle(&curveRect);
		//左侧小矩形
		CRect leftRect;
		leftRect.SetRect(nHleftspace, curveRect.top + curveRect.Height()/2-nVspace, curveRect.left - nHmidspace/2, curveRect.top + curveRect.Height()/2+nVspace);
		pDC->Rectangle(&leftRect);
		//底侧小矩形
		CRect bottomRect;
		bottomRect.SetRect(curveRect.left + curveRect.Width()/2-nHleftspace, curveRect.bottom +nVspace/2, curveRect.left + curveRect.Width()/2+nHleftspace, curveRect.bottom +nVspace*3/2);
		pDC->Rectangle(&bottomRect);
	}
	//曲线2分割线
	{
		nCurHeight = nCurHeight + nVspace * 27;
		pDC->MoveTo(nHleftspace, nCurHeight);
		pDC->LineTo((m_rectDraw.Width() - nHmidspace)/3 ,nCurHeight);

		pDC->MoveTo((m_rectDraw.Width()+ nHmidspace)/3, nCurHeight);
		pDC->LineTo((m_rectDraw.Width() - nHmidspace) *2/3 , nCurHeight);

		pDC->MoveTo(m_rectDraw.Width()* 2/3, nCurHeight);
		pDC->LineTo((m_rectDraw.Width() - nHleftspace), nCurHeight);
	}
	//曲线2
	{
		//曲线2大矩形
		nCurHeight = nCurHeight + nVspace * 2;
		CRect curveRect;
		curveRect.SetRect(nHleftspace*2 + nHmidspace/2, nCurHeight, m_rectDraw.Width() - nHleftspace, nCurHeight + nVspace * 20);
		pDC->SelectObject(GetStockObject(NULL_BRUSH));
		pDC->Rectangle(&curveRect);
		//曲线2左侧小矩形
		CRect leftRect;
		leftRect.SetRect(nHleftspace, curveRect.top + curveRect.Height()/2-nVspace, curveRect.left - nHmidspace/2, curveRect.top + curveRect.Height()/2+nVspace);
		pDC->Rectangle(&leftRect);
		//曲线2底侧小矩形
		CRect bottomRect;
		bottomRect.SetRect(curveRect.left + curveRect.Width()/2-nHleftspace, curveRect.bottom +nVspace/2, curveRect.left + curveRect.Width()/2+nHleftspace, curveRect.bottom +nVspace*3/2);
		pDC->Rectangle(&bottomRect);
	}
	pDC->RestoreDC(nSaveDC);
	pen.DeleteObject();
}

void CDrawButton::Draw2CurveTitle(CDC* pDC)
{
	int nSaveDC = pDC->SaveDC();
	CPen pen;
	pen.CreatePen (PS_DOT, 3, RGB(255,0,0));
	pDC->SelectObject(&pen);//
	//画线
	int nVspace = m_rectDraw.Height() / 64;
	int nCurHeight = nVspace * 3;
	int nHleftspace = m_rectDraw.Width() / 10;
	int nHmidspace = m_rectDraw.Width() / 20;
	//曲线标题矩形1
	CRect m_rectTitle1;
	nCurHeight = nCurHeight + nVspace * 8;
	m_rectTitle1.left = nHleftspace - nHmidspace;
	m_rectTitle1.top = nCurHeight;
	m_rectTitle1.right = m_rectDraw.Width() - nHleftspace + nHmidspace;
	m_rectTitle1.bottom = nCurHeight +  nVspace * 2;

	pDC->SelectObject(GetStockObject(NULL_BRUSH));
	pDC->Rectangle(&m_rectTitle1);

	//曲线标题矩形2
	CRect m_rectTitle2;
	nCurHeight = nCurHeight + nVspace * 29;
	m_rectTitle1.left = nHleftspace - nHmidspace;
	m_rectTitle1.top = nCurHeight;
	m_rectTitle1.right = m_rectDraw.Width() - nHleftspace + nHmidspace;
	m_rectTitle1.bottom = nCurHeight +  nVspace * 2;
	pDC->Rectangle(&m_rectTitle1);

	pDC->RestoreDC(nSaveDC);
	pen.DeleteObject();
}

void CDrawButton::Draw4Curve(CDC* pDC)
{
	int nSaveDC = pDC->SaveDC();
	CPen pen;
	pen.CreatePen (PS_SOLID, 1, RGB(0,0,0));
	pDC->SelectObject(&pen);//
	//画线
	int nVspace = m_rectDraw.Height() / 64;
	int nCurHeight = nVspace * 3;
	int nHleftspace = m_rectDraw.Width() / 10;
	int nHmidspace = m_rectDraw.Width() / 20;
	int nSeparateSpace = (m_rectDraw.Width() - 2 * (nHleftspace + nHmidspace)) / 6;
	int nCurSeparate = nHleftspace;
	//打印方向,选四个点
	POINT point1;
	POINT point2;
	POINT point3;
	POINT point4;
	POINT arrowPoint[3];
	//标题
	pDC->MoveTo(m_rectDraw.Width() / 3, nCurHeight);
	pDC->LineTo(m_rectDraw.Width() * 2/3, nCurHeight);
	//内容
	for (int i=1; i<=6; i++)
	{
		nCurHeight = nCurHeight + nVspace;
		pDC->MoveTo(nHleftspace, nCurHeight);
		pDC->LineTo((m_rectDraw.Width() - nHmidspace)/2 ,nCurHeight);

		pDC->MoveTo((m_rectDraw.Width() + nHmidspace)/2, nCurHeight);
		pDC->LineTo((m_rectDraw.Width() - nHleftspace), nCurHeight);
	}
	//曲线12分割线
	nCurHeight = nCurHeight + nVspace * 3;
	for (int i=1; i<=6; i++)
	{
		pDC->MoveTo(nCurSeparate, nCurHeight);
		pDC->LineTo(nCurSeparate + nSeparateSpace, nCurHeight);
		nCurSeparate = nCurSeparate + nSeparateSpace + nHmidspace / 3;
	}

	//曲线1
	{
		//曲线1大矩形
		nCurHeight = nCurHeight + nVspace * 2;
		CRect curveRect;
		curveRect.SetRect(nHleftspace*2 + nHmidspace/2, nCurHeight, (m_rectDraw.Width() - nHleftspace)/2, nCurHeight + nVspace * 10);
		pDC->SelectObject(GetStockObject(NULL_BRUSH));
		pDC->Rectangle(&curveRect);
		//打印方向1点
		point1.x = curveRect.left + curveRect.Width()/4;
		point1.y = curveRect.top + curveRect.Height()/3;
		//左侧小矩形
		CRect leftRect;
		leftRect.SetRect(nHleftspace, curveRect.top + curveRect.Height()/2-nVspace, curveRect.left - nHmidspace/2, curveRect.top + curveRect.Height()/2+nVspace);
		pDC->Rectangle(&leftRect);
		//底侧小矩形
		CRect bottomRect;
		bottomRect.SetRect(curveRect.left + curveRect.Width()/2-nHmidspace, curveRect.bottom +nVspace/2, curveRect.left + curveRect.Width()/2+nHmidspace, curveRect.bottom +nVspace*3/2);
		pDC->Rectangle(&bottomRect);
	}
	//曲线2
	{
		//曲线2大矩形
		CRect curveRect;
		curveRect.SetRect(m_rectDraw.Width()/2 + nHleftspace, nCurHeight, m_rectDraw.Width() - nHmidspace - nHleftspace, nCurHeight + nVspace * 10);
		pDC->SelectObject(GetStockObject(NULL_BRUSH));
		pDC->Rectangle(&curveRect);
		//打印方向2点
		point2.x = curveRect.right - curveRect.Width()/4;
		point2.y = curveRect.top + curveRect.Height()/3;
		//左侧小矩形
		CRect leftRect;
		leftRect.SetRect(m_rectDraw.left + m_rectDraw.Width()/2, curveRect.top + curveRect.Height()/2-nVspace, curveRect.left - nHmidspace/2, curveRect.top + curveRect.Height()/2+nVspace);
		pDC->Rectangle(&leftRect);
		//底侧小矩形
		CRect bottomRect;
		bottomRect.SetRect(curveRect.left + curveRect.Width()/2-nHmidspace, curveRect.bottom +nVspace/2, curveRect.left + curveRect.Width()/2+nHmidspace, curveRect.bottom +nVspace*3/2);
		pDC->Rectangle(&bottomRect);
	}
	//曲线34分割线
	nCurHeight = nCurHeight + nVspace * 27;
	nCurSeparate = nHleftspace;
	for (int i=1; i<=6; i++)
	{
		pDC->MoveTo(nCurSeparate, nCurHeight);
		pDC->LineTo(nCurSeparate + nSeparateSpace, nCurHeight);
		nCurSeparate = nCurSeparate + nSeparateSpace + nHmidspace / 3;
	}
	//曲线3
	{
		//曲线3大矩形
		nCurHeight = nCurHeight + nVspace * 2;
		CRect curveRect;
		curveRect.SetRect(nHleftspace*2 + nHmidspace/2, nCurHeight, (m_rectDraw.Width() - nHleftspace)/2, nCurHeight + nVspace * 10);
		pDC->SelectObject(GetStockObject(NULL_BRUSH));
		pDC->Rectangle(&curveRect);
		//打印方向3点
		point3.x = curveRect.left + curveRect.Width()/4;
		point3.y = curveRect.top + curveRect.Height()/3;
		//左侧小矩形
		CRect leftRect;
		leftRect.SetRect(nHleftspace, curveRect.top + curveRect.Height()/2-nVspace, curveRect.left - nHmidspace/2, curveRect.top + curveRect.Height()/2+nVspace);
		pDC->Rectangle(&leftRect);
		//底侧小矩形
		CRect bottomRect;
		bottomRect.SetRect(curveRect.left + curveRect.Width()/2-nHmidspace, curveRect.bottom +nVspace/2, curveRect.left + curveRect.Width()/2+nHmidspace, curveRect.bottom +nVspace*3/2);
		pDC->Rectangle(&bottomRect);
	}
	//曲线4
	{
		//曲线4大矩形
		CRect curveRect;
		curveRect.SetRect(m_rectDraw.Width()/2 + nHleftspace, nCurHeight, m_rectDraw.Width() - nHmidspace - nHleftspace, nCurHeight + nVspace * 10);
		pDC->SelectObject(GetStockObject(NULL_BRUSH));
		pDC->Rectangle(&curveRect);
		//打印方向4点
		point4.x = curveRect.right - curveRect.Width()/4;
		point4.y = curveRect.top + curveRect.Height()/3;
		//左侧小矩形
		CRect leftRect;
		leftRect.SetRect(m_rectDraw.left + m_rectDraw.Width()/2, curveRect.top + curveRect.Height()/2-nVspace, curveRect.left - nHmidspace/2, curveRect.top + curveRect.Height()/2+nVspace);
		pDC->Rectangle(&leftRect);
		//底侧小矩形
		CRect bottomRect;
		bottomRect.SetRect(curveRect.left + curveRect.Width()/2-nHmidspace, curveRect.bottom +nVspace/2, curveRect.left + curveRect.Width()/2+nHmidspace, curveRect.bottom +nVspace*3/2);
		pDC->Rectangle(&bottomRect);
	}
	//打印方向
	CPen printPen;
	CPen arrowPen;
	CBrush printBrush;
	printPen.CreatePen (PS_SOLID, 10, RGB(127,127,127));
	arrowPen.CreatePen(PS_SOLID, 1, RGB(127,127,127));
	printBrush.CreateSolidBrush(RGB(127,127,127));
	pDC->SelectObject(&printPen);//
	{
		if (m_bHorLayout)//横向
		{
			pDC->MoveTo(point1);
			pDC->LineTo(point2);
			pDC->LineTo(point3);
			pDC->LineTo(point4.x -40, point4.y);
			//画箭头
			pDC->SelectObject(&arrowPen);
			pDC->SelectObject(&printBrush);
			arrowPoint[0].x=point4.x;
			arrowPoint[0].y=point4.y;
			arrowPoint[1].x=point4.x-40;
			arrowPoint[1].y=point4.y-10;
			arrowPoint[2].x=point4.x-40;
			arrowPoint[2].y=point4.y+10;
			pDC->Polygon(arrowPoint,3);

		}
		else//纵向
		{
			pDC->MoveTo(point1);
			pDC->LineTo(point3);
			pDC->LineTo(point2);
			pDC->LineTo(point4.x,point4.y-40);
			//画箭头
			pDC->SelectObject(&arrowPen);
			pDC->SelectObject(&printBrush);
			arrowPoint[0].x=point4.x;
			arrowPoint[0].y=point4.y;
			arrowPoint[1].x=point4.x-10;
			arrowPoint[1].y=point4.y-40;
			arrowPoint[2].x=point4.x+10;
			arrowPoint[2].y=point4.y-40;
			pDC->Polygon(arrowPoint,3);

		}
	}
	
	pDC->RestoreDC(nSaveDC);
	pen.DeleteObject();
	printPen.DeleteObject();
	arrowPen.DeleteObject();
	printBrush.DeleteObject();
}

void CDrawButton::Draw4CurveTitle(CDC* pDC)
{
	int nSaveDC = pDC->SaveDC();
	CPen pen;
	pen.CreatePen (PS_SOLID, 3, RGB(255,0,0));
	pDC->SelectObject(&pen);//
	//画线
	int nVspace = m_rectDraw.Height() / 64;
	int nCurHeight = nVspace * 3;
	int nHleftspace = m_rectDraw.Width() / 10;
	int nHmidspace = m_rectDraw.Width() / 20;

	//曲线标题矩形1
	CRect m_rectTitle1;
	nCurHeight = nCurHeight + nVspace * 8;
	m_rectTitle1.left = nHleftspace - nHmidspace;
	m_rectTitle1.top = nCurHeight;
	m_rectTitle1.right = m_rectDraw.Width() / 2 - nHmidspace;
	m_rectTitle1.bottom = nCurHeight +  nVspace * 2;
	pDC->SelectObject(GetStockObject(NULL_BRUSH));
	pDC->Rectangle(&m_rectTitle1);

	//曲线标题矩形2
	CRect m_rectTitle2;
	m_rectTitle2.left = m_rectDraw.Width() / 2 + nHmidspace;
	m_rectTitle2.top = nCurHeight;
	m_rectTitle2.right = m_rectDraw.Width() - nHleftspace;
	m_rectTitle2.bottom = nCurHeight +  nVspace * 2;
	pDC->Rectangle(&m_rectTitle2);

	//曲线标题矩形3
	CRect m_rectTitle3;
	nCurHeight = nCurHeight + nVspace * 29;
	m_rectTitle3.left = nHleftspace - nHmidspace;
	m_rectTitle3.top = nCurHeight;
	m_rectTitle3.right = m_rectDraw.Width() / 2 - nHmidspace;
	m_rectTitle3.bottom = nCurHeight +  nVspace * 2;
	pDC->Rectangle(&m_rectTitle3);

	//曲线标题矩形4
	CRect m_rectTitle4;
	m_rectTitle4.left = m_rectDraw.Width() / 2 + nHmidspace;
	m_rectTitle4.top = nCurHeight;
	m_rectTitle4.right = m_rectDraw.Width() - nHleftspace;
	m_rectTitle4.bottom = nCurHeight +  nVspace * 2;
	pDC->Rectangle(&m_rectTitle4);

	pDC->RestoreDC(nSaveDC);
	pen.DeleteObject();
}

void CDrawButton::Draw6Curve(CDC* pDC)
{
	int nSaveDC = pDC->SaveDC();
	CPen pen;
	pen.CreatePen (PS_SOLID, 1, RGB(0,0,0));
	pDC->SelectObject(&pen);//
	//画线
	int nVspace = m_rectDraw.Height() / 64;
	int nCurHeight = nVspace * 3;
	int nHleftspace = m_rectDraw.Width() / 10;
	int nHmidspace = m_rectDraw.Width() / 20;
	int nSeparateSpace = (m_rectDraw.Width() - 2 * (nHleftspace + nHmidspace)) / 6;
	int nCurSeparate = nHleftspace;
	//打印方向,选六个点
	POINT point1;
	POINT point2;
	POINT point3;
	POINT point4;
	POINT point5;
	POINT point6;
	POINT arrowPoint[3];
	//标题
	pDC->MoveTo(m_rectDraw.Width() / 3, nCurHeight);
	pDC->LineTo(m_rectDraw.Width() * 2/3, nCurHeight);
	//内容
	for (int i=1; i<=6; i++)
	{
		nCurHeight = nCurHeight + nVspace;
		pDC->MoveTo(nHleftspace, nCurHeight);
		pDC->LineTo((m_rectDraw.Width() - nHmidspace)/2 ,nCurHeight);

		pDC->MoveTo((m_rectDraw.Width() + nHmidspace)/2, nCurHeight);
		pDC->LineTo((m_rectDraw.Width() - nHleftspace), nCurHeight);
	}
	//曲线12分割线
	nCurHeight = nCurHeight + nVspace * 3;
	for (int i=1; i<=6; i++)
	{
		pDC->MoveTo(nCurSeparate, nCurHeight);
		pDC->LineTo(nCurSeparate + nSeparateSpace, nCurHeight);
		nCurSeparate = nCurSeparate + nSeparateSpace + nHmidspace / 3;
	}
	//曲线1
	{
		//曲线1大矩形
		nCurHeight = nCurHeight + nVspace * 2;
		CRect curveRect;
		curveRect.SetRect(nHleftspace*2 + nHmidspace/2, nCurHeight, (m_rectDraw.Width() - nHleftspace)/2, nCurHeight + nVspace * 10);
		pDC->SelectObject(GetStockObject(NULL_BRUSH));
		pDC->Rectangle(&curveRect);
		//打印方向1点
		point1.x = curveRect.left + curveRect.Width()/4;
		point1.y = curveRect.top + curveRect.Height()/3;
		//左侧小矩形
		CRect leftRect;
		leftRect.SetRect(nHleftspace, curveRect.top + curveRect.Height()/2-nVspace, curveRect.left - nHmidspace/2, curveRect.top + curveRect.Height()/2+nVspace);
		pDC->Rectangle(&leftRect);
		//底侧小矩形
		CRect bottomRect;
		bottomRect.SetRect(curveRect.left + curveRect.Width()/2-nHmidspace, curveRect.bottom +nVspace/2, curveRect.left + curveRect.Width()/2+nHmidspace, curveRect.bottom +nVspace*3/2);
		pDC->Rectangle(&bottomRect);
	}
	//曲线2
	{
		//曲线2大矩形
		CRect curveRect;
		curveRect.SetRect(m_rectDraw.Width()/2 + nHleftspace, nCurHeight, m_rectDraw.Width() - nHmidspace - nHleftspace, nCurHeight + nVspace * 10);
		pDC->SelectObject(GetStockObject(NULL_BRUSH));
		pDC->Rectangle(&curveRect);
		//打印方向2点
		point2.x = curveRect.right - curveRect.Width()/4;
		point2.y = curveRect.top + curveRect.Height()/3;
		//左侧小矩形
		CRect leftRect;
		leftRect.SetRect(m_rectDraw.left + m_rectDraw.Width()/2, curveRect.top + curveRect.Height()/2-nVspace, curveRect.left - nHmidspace/2, curveRect.top + curveRect.Height()/2+nVspace);
		pDC->Rectangle(&leftRect);
		//底侧小矩形
		CRect bottomRect;
		bottomRect.SetRect(curveRect.left + curveRect.Width()/2-nHmidspace, curveRect.bottom +nVspace/2, curveRect.left + curveRect.Width()/2+nHmidspace, curveRect.bottom +nVspace*3/2);
		pDC->Rectangle(&bottomRect);
	}
	//曲线34分割线
	nCurHeight = nCurHeight + nVspace * 17;
	nCurSeparate = nHleftspace;
	for (int i=1; i<=6; i++)
	{
		pDC->MoveTo(nCurSeparate, nCurHeight);
		pDC->LineTo(nCurSeparate + nSeparateSpace, nCurHeight);
		nCurSeparate = nCurSeparate + nSeparateSpace + nHmidspace / 3;
	}
	//曲线3
	{
		//曲线3大矩形
		nCurHeight = nCurHeight + nVspace * 2;
		CRect curveRect;
		curveRect.SetRect(nHleftspace*2 + nHmidspace/2, nCurHeight, (m_rectDraw.Width() - nHleftspace)/2, nCurHeight + nVspace * 10);
		pDC->SelectObject(GetStockObject(NULL_BRUSH));
		pDC->Rectangle(&curveRect);
		//打印方向3点
		point3.x = curveRect.left + curveRect.Width()/4;
		point3.y = curveRect.top + curveRect.Height()/3;
		//左侧小矩形
		CRect leftRect;
		leftRect.SetRect(nHleftspace, curveRect.top + curveRect.Height()/2-nVspace, curveRect.left - nHmidspace/2, curveRect.top + curveRect.Height()/2+nVspace);
		pDC->Rectangle(&leftRect);
		//底侧小矩形
		CRect bottomRect;
		bottomRect.SetRect(curveRect.left + curveRect.Width()/2-nHmidspace, curveRect.bottom +nVspace/2, curveRect.left + curveRect.Width()/2+nHmidspace, curveRect.bottom +nVspace*3/2);
		pDC->Rectangle(&bottomRect);
	}
	//曲线4
	{
		//曲线4大矩形
		CRect curveRect;
		curveRect.SetRect(m_rectDraw.Width()/2 + nHleftspace, nCurHeight, m_rectDraw.Width() - nHmidspace - nHleftspace, nCurHeight + nVspace * 10);
		pDC->SelectObject(GetStockObject(NULL_BRUSH));
		pDC->Rectangle(&curveRect);
		//打印方向4点
		point4.x = curveRect.right - curveRect.Width()/4;
		point4.y = curveRect.top + curveRect.Height()/3;
		//左侧小矩形
		CRect leftRect;
		leftRect.SetRect(m_rectDraw.left + m_rectDraw.Width()/2, curveRect.top + curveRect.Height()/2-nVspace, curveRect.left - nHmidspace/2, curveRect.top + curveRect.Height()/2+nVspace);
		pDC->Rectangle(&leftRect);
		//底侧小矩形
		CRect bottomRect;
		bottomRect.SetRect(curveRect.left + curveRect.Width()/2-nHmidspace, curveRect.bottom +nVspace/2, curveRect.left + curveRect.Width()/2+nHmidspace, curveRect.bottom +nVspace*3/2);
		pDC->Rectangle(&bottomRect);
	}
	//曲线56分割线
	nCurHeight = nCurHeight + nVspace * 17;
	nCurSeparate = nHleftspace;
	for (int i=1; i<=6; i++)
	{
		pDC->MoveTo(nCurSeparate, nCurHeight);
		pDC->LineTo(nCurSeparate + nSeparateSpace, nCurHeight);
		nCurSeparate = nCurSeparate + nSeparateSpace + nHmidspace / 3;
	}
	//曲线5
	{
		//曲线5大矩形
		nCurHeight = nCurHeight + nVspace * 2;
		CRect curveRect;
		curveRect.SetRect(nHleftspace*2 + nHmidspace/2, nCurHeight, (m_rectDraw.Width() - nHleftspace)/2, nCurHeight + nVspace * 10);
		pDC->SelectObject(GetStockObject(NULL_BRUSH));
		pDC->Rectangle(&curveRect);
		//打印方向5点
		point5.x = curveRect.left + curveRect.Width()/4;
		point5.y = curveRect.top + curveRect.Height()/3;
		//左侧小矩形
		CRect leftRect;
		leftRect.SetRect(nHleftspace, curveRect.top + curveRect.Height()/2-nVspace, curveRect.left - nHmidspace/2, curveRect.top + curveRect.Height()/2+nVspace);
		pDC->Rectangle(&leftRect);
		//底侧小矩形
		CRect bottomRect;
		bottomRect.SetRect(curveRect.left + curveRect.Width()/2-nHmidspace, curveRect.bottom +nVspace/2, curveRect.left + curveRect.Width()/2+nHmidspace, curveRect.bottom +nVspace*3/2);
		pDC->Rectangle(&bottomRect);
	}
	//曲线6
	{
		//曲线6大矩形
		CRect curveRect;
		curveRect.SetRect(m_rectDraw.Width()/2 + nHleftspace, nCurHeight, m_rectDraw.Width() - nHmidspace - nHleftspace, nCurHeight + nVspace * 10);
		pDC->SelectObject(GetStockObject(NULL_BRUSH));
		pDC->Rectangle(&curveRect);
		//打印方向6点
		point6.x = curveRect.right - curveRect.Width()/4;
		point6.y = curveRect.top + curveRect.Height()/3;
		//左侧小矩形
		CRect leftRect;
		leftRect.SetRect(m_rectDraw.left + m_rectDraw.Width()/2, curveRect.top + curveRect.Height()/2-nVspace, curveRect.left - nHmidspace/2, curveRect.top + curveRect.Height()/2+nVspace);
		pDC->Rectangle(&leftRect);
		//底侧小矩形
		CRect bottomRect;
		bottomRect.SetRect(curveRect.left + curveRect.Width()/2-nHmidspace, curveRect.bottom +nVspace/2, curveRect.left + curveRect.Width()/2+nHmidspace, curveRect.bottom +nVspace*3/2);
		pDC->Rectangle(&bottomRect);
	}
	//打印方向
	CPen printPen;
	CPen arrowPen;
	CBrush printBrush;
	printPen.CreatePen (PS_SOLID, 10, RGB(127,127,127));
	arrowPen.CreatePen(PS_SOLID, 1, RGB(127,127,127));
	printBrush.CreateSolidBrush(RGB(127,127,127));
	pDC->SelectObject(&printPen);//
	{
		if (m_bHorLayout)//横向
		{
			pDC->MoveTo(point1);
			pDC->LineTo(point2);
			pDC->LineTo(point3);
			pDC->LineTo(point4);
			pDC->LineTo(point5);
			pDC->LineTo(point6.x -40, point6.y);
			//画箭头
			pDC->SelectObject(&arrowPen);
			pDC->SelectObject(&printBrush);
			arrowPoint[0].x=point6.x;
			arrowPoint[0].y=point6.y;
			arrowPoint[1].x=point6.x-40;
			arrowPoint[1].y=point6.y-10;
			arrowPoint[2].x=point6.x-40;
			arrowPoint[2].y=point6.y+10;
			pDC->Polygon(arrowPoint,3);

		}
		else//纵向
		{
			pDC->MoveTo(point1);
			pDC->LineTo(point3);
			pDC->LineTo(point5);
			pDC->LineTo(point2);
			pDC->LineTo(point4);
			pDC->LineTo(point6.x,point6.y-40);

			//画箭头
			pDC->SelectObject(&arrowPen);
			pDC->SelectObject(&printBrush);
			arrowPoint[0].x=point6.x;
			arrowPoint[0].y=point6.y;
			arrowPoint[1].x=point6.x-10;
			arrowPoint[1].y=point6.y-40;
			arrowPoint[2].x=point6.x+10;
			arrowPoint[2].y=point6.y-40;
			pDC->Polygon(arrowPoint,3);

		}
	}
	pDC->RestoreDC(nSaveDC);
	pen.DeleteObject();
	printPen.DeleteObject();
	arrowPen.DeleteObject();
	printBrush.DeleteObject();
}

void CDrawButton::Draw6CurveTitle(CDC* pDC)
{
	int nSaveDC = pDC->SaveDC();
	CPen pen;
	pen.CreatePen (PS_SOLID, 3, RGB(255,0,0));
	pDC->SelectObject(&pen);//
	//画线
	int nVspace = m_rectDraw.Height() / 64;
	int nCurHeight = nVspace * 3;
	int nHleftspace = m_rectDraw.Width() / 10;
	int nHmidspace = m_rectDraw.Width() / 20;

	//曲线标题矩形1
	CRect m_rectTitle1;
	nCurHeight = nCurHeight + nVspace * 8;
	m_rectTitle1.left = nHleftspace - nHmidspace;
	m_rectTitle1.top = nCurHeight;
	m_rectTitle1.right = m_rectDraw.Width() / 2 - nHmidspace;
	m_rectTitle1.bottom = nCurHeight +  nVspace * 2;
	pDC->SelectObject(GetStockObject(NULL_BRUSH));
	pDC->Rectangle(&m_rectTitle1);

	//曲线标题矩形2
	CRect m_rectTitle2;
	m_rectTitle2.left = m_rectDraw.Width() / 2 + nHmidspace;
	m_rectTitle2.top = nCurHeight;
	m_rectTitle2.right = m_rectDraw.Width() - nHleftspace;
	m_rectTitle2.bottom = nCurHeight +  nVspace * 2;
	pDC->Rectangle(&m_rectTitle2);

	//曲线标题矩形3
	CRect m_rectTitle3;
	nCurHeight = nCurHeight + nVspace * 19;
	m_rectTitle3.left = nHleftspace - nHmidspace;
	m_rectTitle3.top = nCurHeight;
	m_rectTitle3.right = m_rectDraw.Width() / 2 - nHmidspace;
	m_rectTitle3.bottom = nCurHeight +  nVspace * 2;
	pDC->Rectangle(&m_rectTitle3);

	//曲线标题矩形4
	CRect m_rectTitle4;
	m_rectTitle4.left = m_rectDraw.Width() / 2 + nHmidspace;
	m_rectTitle4.top = nCurHeight;
	m_rectTitle4.right = m_rectDraw.Width() - nHleftspace;
	m_rectTitle4.bottom = nCurHeight +  nVspace * 2;
	pDC->Rectangle(&m_rectTitle4);

	//曲线标题矩形5
	CRect m_rectTitle5;
	nCurHeight = nCurHeight + nVspace * 19;
	m_rectTitle5.left = nHleftspace - nHmidspace;
	m_rectTitle5.top = nCurHeight;
	m_rectTitle5.right = m_rectDraw.Width() / 2 - nHmidspace;
	m_rectTitle5.bottom = nCurHeight +  nVspace * 2;
	pDC->Rectangle(&m_rectTitle5);

	//曲线标题矩形6
	CRect m_rectTitle6;
	m_rectTitle6.left = m_rectDraw.Width() / 2 + nHmidspace;
	m_rectTitle6.top = nCurHeight;
	m_rectTitle6.right = m_rectDraw.Width() - nHleftspace;
	m_rectTitle6.bottom = nCurHeight +  nVspace * 2;
	pDC->Rectangle(&m_rectTitle6);

	pDC->RestoreDC(nSaveDC);
	pen.DeleteObject();
}

void CDrawButton::Draw8Curve(CDC* pDC)
{
	int nSaveDC = pDC->SaveDC();
	CPen pen;
	pen.CreatePen (PS_SOLID, 1, RGB(0,0,0));
	pDC->SelectObject(&pen);//
	//画线
	int nVspace = m_rectDraw.Height() / 64;
	int nCurHeight = nVspace * 3;
	int nHleftspace = m_rectDraw.Width() / 10;
	int nHmidspace = m_rectDraw.Width() / 20;
	int nSeparateSpace = (m_rectDraw.Width() - 2 * (nHleftspace + nHmidspace)) / 6;
	int nCurSeparate = nHleftspace;
	//打印方向,选八个点
	POINT point1;
	POINT point2;
	POINT point3;
	POINT point4;
	POINT point5;
	POINT point6;
	POINT point7;
	POINT point8;
	POINT arrowPoint[3];
	//标题
	pDC->MoveTo(m_rectDraw.Width() / 3, nCurHeight);
	pDC->LineTo(m_rectDraw.Width() * 2/3, nCurHeight);
	//曲线12分割线
	nCurHeight = nCurHeight + nVspace * 3;
	nCurSeparate = nHleftspace;
	for (int i=1; i<=6; i++)
	{
		pDC->MoveTo(nCurSeparate, nCurHeight);
		pDC->LineTo(nCurSeparate + nSeparateSpace, nCurHeight);
		nCurSeparate = nCurSeparate + nSeparateSpace + nHmidspace / 3;
	}
	//曲线1
	{
		//曲线1大矩形
		nCurHeight = nCurHeight + nVspace * 2;
		CRect curveRect;
		curveRect.SetRect(nHleftspace*2 + nHmidspace/2, nCurHeight, (m_rectDraw.Width() - nHleftspace)/2, nCurHeight + nVspace * 10);
		pDC->SelectObject(GetStockObject(NULL_BRUSH));
		pDC->Rectangle(&curveRect);
		//打印方向1点
		point1.x = curveRect.left + curveRect.Width()/4;
		point1.y = curveRect.top + curveRect.Height()/3;
		//左侧小矩形
		CRect leftRect;
		leftRect.SetRect(nHleftspace, curveRect.top + curveRect.Height()/2-nVspace, curveRect.left - nHmidspace/2, curveRect.top + curveRect.Height()/2+nVspace);
		pDC->Rectangle(&leftRect);
		//底侧小矩形
		CRect bottomRect;
		bottomRect.SetRect(curveRect.left + curveRect.Width()/2-nHmidspace, curveRect.bottom +nVspace/2, curveRect.left + curveRect.Width()/2+nHmidspace, curveRect.bottom +nVspace*3/2);
		pDC->Rectangle(&bottomRect);
	}
	//曲线2
	{
		//曲线2大矩形
		CRect curveRect;
		curveRect.SetRect(m_rectDraw.Width()/2 + nHleftspace, nCurHeight, m_rectDraw.Width() - nHmidspace - nHleftspace, nCurHeight + nVspace * 10);
		pDC->SelectObject(GetStockObject(NULL_BRUSH));
		pDC->Rectangle(&curveRect);
		//打印方向2点
		point2.x = curveRect.right - curveRect.Width()/4;
		point2.y = curveRect.top + curveRect.Height()/3;
		//左侧小矩形
		CRect leftRect;
		leftRect.SetRect(m_rectDraw.left + m_rectDraw.Width()/2, curveRect.top + curveRect.Height()/2-nVspace, curveRect.left - nHmidspace/2, curveRect.top + curveRect.Height()/2+nVspace);
		pDC->Rectangle(&leftRect);
		//底侧小矩形
		CRect bottomRect;
		bottomRect.SetRect(curveRect.left + curveRect.Width()/2-nHmidspace, curveRect.bottom +nVspace/2, curveRect.left + curveRect.Width()/2+nHmidspace, curveRect.bottom +nVspace*3/2);
		pDC->Rectangle(&bottomRect);
	}
	//曲线34分割线
	nCurHeight = nCurHeight + nVspace * 13;
	nCurSeparate = nHleftspace;
	for (int i=1; i<=6; i++)
	{
		pDC->MoveTo(nCurSeparate, nCurHeight);
		pDC->LineTo(nCurSeparate + nSeparateSpace, nCurHeight);
		nCurSeparate = nCurSeparate + nSeparateSpace + nHmidspace / 3;
	}
	//曲线3
	{
		//曲线3大矩形
		nCurHeight = nCurHeight + nVspace * 2;
		CRect curveRect;
		curveRect.SetRect(nHleftspace*2 + nHmidspace/2, nCurHeight, (m_rectDraw.Width() - nHleftspace)/2, nCurHeight + nVspace * 10);
		pDC->SelectObject(GetStockObject(NULL_BRUSH));
		pDC->Rectangle(&curveRect);
		//打印方向3点
		point3.x = curveRect.left + curveRect.Width()/4;
		point3.y = curveRect.top + curveRect.Height()/3;
		//左侧小矩形
		CRect leftRect;
		leftRect.SetRect(nHleftspace, curveRect.top + curveRect.Height()/2-nVspace, curveRect.left - nHmidspace/2, curveRect.top + curveRect.Height()/2+nVspace);
		pDC->Rectangle(&leftRect);
		//底侧小矩形
		CRect bottomRect;
		bottomRect.SetRect(curveRect.left + curveRect.Width()/2-nHmidspace, curveRect.bottom +nVspace/2, curveRect.left + curveRect.Width()/2+nHmidspace, curveRect.bottom +nVspace*3/2);
		pDC->Rectangle(&bottomRect);
	}
	//曲线4
	{
		//曲线4大矩形
		CRect curveRect;
		curveRect.SetRect(m_rectDraw.Width()/2 + nHleftspace, nCurHeight, m_rectDraw.Width() - nHmidspace - nHleftspace, nCurHeight + nVspace * 10);
		pDC->SelectObject(GetStockObject(NULL_BRUSH));
		pDC->Rectangle(&curveRect);
		//打印方向4点
		point4.x = curveRect.right - curveRect.Width()/4;
		point4.y = curveRect.top + curveRect.Height()/3;
		//左侧小矩形
		CRect leftRect;
		leftRect.SetRect(m_rectDraw.left + m_rectDraw.Width()/2, curveRect.top + curveRect.Height()/2-nVspace, curveRect.left - nHmidspace/2, curveRect.top + curveRect.Height()/2+nVspace);
		pDC->Rectangle(&leftRect);
		//底侧小矩形
		CRect bottomRect;
		bottomRect.SetRect(curveRect.left + curveRect.Width()/2-nHmidspace, curveRect.bottom +nVspace/2, curveRect.left + curveRect.Width()/2+nHmidspace, curveRect.bottom +nVspace*3/2);
		pDC->Rectangle(&bottomRect);
	}
	//曲线56分割线
	nCurHeight = nCurHeight + nVspace * 13;
	nCurSeparate = nHleftspace;
	for (int i=1; i<=6; i++)
	{
		pDC->MoveTo(nCurSeparate, nCurHeight);
		pDC->LineTo(nCurSeparate + nSeparateSpace, nCurHeight);
		nCurSeparate = nCurSeparate + nSeparateSpace + nHmidspace / 3;
	}
	//曲线5
	{
		//曲线5大矩形
		nCurHeight = nCurHeight + nVspace * 2;
		CRect curveRect;
		curveRect.SetRect(nHleftspace*2 + nHmidspace/2, nCurHeight, (m_rectDraw.Width() - nHleftspace)/2, nCurHeight + nVspace * 10);
		pDC->SelectObject(GetStockObject(NULL_BRUSH));
		pDC->Rectangle(&curveRect);
		//打印方向5点
		point5.x = curveRect.left + curveRect.Width()/4;
		point5.y = curveRect.top + curveRect.Height()/3;
		//左侧小矩形
		CRect leftRect;
		leftRect.SetRect(nHleftspace, curveRect.top + curveRect.Height()/2-nVspace, curveRect.left - nHmidspace/2, curveRect.top + curveRect.Height()/2+nVspace);
		pDC->Rectangle(&leftRect);
		//底侧小矩形
		CRect bottomRect;
		bottomRect.SetRect(curveRect.left + curveRect.Width()/2-nHmidspace, curveRect.bottom +nVspace/2, curveRect.left + curveRect.Width()/2+nHmidspace, curveRect.bottom +nVspace*3/2);
		pDC->Rectangle(&bottomRect);
	}
	//曲线6
	{
		//曲线6大矩形
		CRect curveRect;
		curveRect.SetRect(m_rectDraw.Width()/2 + nHleftspace, nCurHeight, m_rectDraw.Width() - nHmidspace - nHleftspace, nCurHeight + nVspace * 10);
		pDC->SelectObject(GetStockObject(NULL_BRUSH));
		pDC->Rectangle(&curveRect);
		//打印方向6点
		point6.x = curveRect.right - curveRect.Width()/4;
		point6.y = curveRect.top + curveRect.Height()/3;
		//左侧小矩形
		CRect leftRect;
		leftRect.SetRect(m_rectDraw.left + m_rectDraw.Width()/2, curveRect.top + curveRect.Height()/2-nVspace, curveRect.left - nHmidspace/2, curveRect.top + curveRect.Height()/2+nVspace);
		pDC->Rectangle(&leftRect);
		//底侧小矩形
		CRect bottomRect;
		bottomRect.SetRect(curveRect.left + curveRect.Width()/2-nHmidspace, curveRect.bottom +nVspace/2, curveRect.left + curveRect.Width()/2+nHmidspace, curveRect.bottom +nVspace*3/2);
		pDC->Rectangle(&bottomRect);
	}
	//曲线78分割线
	nCurHeight = nCurHeight + nVspace * 13;
	nCurSeparate = nHleftspace;
	for (int i=1; i<=6; i++)
	{
		pDC->MoveTo(nCurSeparate, nCurHeight);
		pDC->LineTo(nCurSeparate + nSeparateSpace, nCurHeight);
		nCurSeparate = nCurSeparate + nSeparateSpace + nHmidspace / 3;
	}
	//曲线7
	{
		//曲线7大矩形
		nCurHeight = nCurHeight + nVspace * 2;
		CRect curveRect;
		curveRect.SetRect(nHleftspace*2 + nHmidspace/2, nCurHeight, (m_rectDraw.Width() - nHleftspace)/2, nCurHeight + nVspace * 10);
		pDC->SelectObject(GetStockObject(NULL_BRUSH));
		pDC->Rectangle(&curveRect);
		//打印方向7点
		point7.x = curveRect.left + curveRect.Width()/4;
		point7.y = curveRect.top + curveRect.Height()/3;
		//左侧小矩形
		CRect leftRect;
		leftRect.SetRect(nHleftspace, curveRect.top + curveRect.Height()/2-nVspace, curveRect.left - nHmidspace/2, curveRect.top + curveRect.Height()/2+nVspace);
		pDC->Rectangle(&leftRect);
		//底侧小矩形
		CRect bottomRect;
		bottomRect.SetRect(curveRect.left + curveRect.Width()/2-nHmidspace, curveRect.bottom +nVspace/2, curveRect.left + curveRect.Width()/2+nHmidspace, curveRect.bottom +nVspace*3/2);
		pDC->Rectangle(&bottomRect);
	}
	//曲线8
	{
		//曲线8大矩形
		CRect curveRect;
		curveRect.SetRect(m_rectDraw.Width()/2 + nHleftspace, nCurHeight, m_rectDraw.Width() - nHmidspace - nHleftspace, nCurHeight + nVspace * 10);
		pDC->SelectObject(GetStockObject(NULL_BRUSH));
		pDC->Rectangle(&curveRect);
		//打印方向6点
		point8.x = curveRect.right - curveRect.Width()/4;
		point8.y = curveRect.top + curveRect.Height()/3;
		//左侧小矩形
		CRect leftRect;
		leftRect.SetRect(m_rectDraw.left + m_rectDraw.Width()/2, curveRect.top + curveRect.Height()/2-nVspace, curveRect.left - nHmidspace/2, curveRect.top + curveRect.Height()/2+nVspace);
		pDC->Rectangle(&leftRect);
		//底侧小矩形
		CRect bottomRect;
		bottomRect.SetRect(curveRect.left + curveRect.Width()/2-nHmidspace, curveRect.bottom +nVspace/2, curveRect.left + curveRect.Width()/2+nHmidspace, curveRect.bottom +nVspace*3/2);
		pDC->Rectangle(&bottomRect);
	}
	//打印方向
	CPen printPen;
	CPen arrowPen;
	CBrush printBrush;
	printPen.CreatePen (PS_SOLID, 10, RGB(127,127,127));
	arrowPen.CreatePen(PS_SOLID, 1, RGB(127,127,127));
	printBrush.CreateSolidBrush(RGB(127,127,127));
	pDC->SelectObject(&printPen);//
	{
		if (m_bHorLayout)//横向
		{
			pDC->MoveTo(point1);
			pDC->LineTo(point2);
			pDC->LineTo(point3);
			pDC->LineTo(point4);
			pDC->LineTo(point5);
			pDC->LineTo(point6);
			pDC->LineTo(point7);
			pDC->LineTo(point8.x -40, point8.y);
			//画箭头
			pDC->SelectObject(&arrowPen);
			pDC->SelectObject(&printBrush);
			arrowPoint[0].x=point8.x;
			arrowPoint[0].y=point8.y;
			arrowPoint[1].x=point8.x-40;
			arrowPoint[1].y=point8.y-10;
			arrowPoint[2].x=point8.x-40;
			arrowPoint[2].y=point8.y+10;
			pDC->Polygon(arrowPoint,3);

		}
		else//纵向
		{
			pDC->MoveTo(point1);
			pDC->LineTo(point3);
			pDC->LineTo(point5);
			pDC->LineTo(point7);
			pDC->LineTo(point2);
			pDC->LineTo(point4);
			pDC->LineTo(point6);
			pDC->LineTo(point8.x,point8.y-40);

			//画箭头
			pDC->SelectObject(&arrowPen);
			pDC->SelectObject(&printBrush);
			arrowPoint[0].x=point8.x;
			arrowPoint[0].y=point8.y;
			arrowPoint[1].x=point8.x-10;
			arrowPoint[1].y=point8.y-40;
			arrowPoint[2].x=point8.x+10;
			arrowPoint[2].y=point8.y-40;
			pDC->Polygon(arrowPoint,3);

		}
	}
	pDC->RestoreDC(nSaveDC);
	pen.DeleteObject();
	printPen.DeleteObject();
	arrowPen.DeleteObject();
	printBrush.DeleteObject();
}

void CDrawButton::Draw8CurveTitle(CDC* pDC)
{
	int nSaveDC = pDC->SaveDC();
	CPen pen;
	pen.CreatePen (PS_SOLID, 3, RGB(255,0,0));
	pDC->SelectObject(&pen);//
	//画线
	int nVspace = m_rectDraw.Height() / 64;
	int nCurHeight = nVspace * 3;
	int nHleftspace = m_rectDraw.Width() / 10;
	int nHmidspace = m_rectDraw.Width() / 20;

	//曲线标题矩形1
	CRect m_rectTitle1;
	nCurHeight = nCurHeight + nVspace * 2;
	m_rectTitle1.left = nHleftspace - nHmidspace;
	m_rectTitle1.top = nCurHeight;
	m_rectTitle1.right = m_rectDraw.Width() / 2 - nHmidspace;
	m_rectTitle1.bottom = nCurHeight +  nVspace * 2;
	pDC->SelectObject(GetStockObject(NULL_BRUSH));
	pDC->Rectangle(&m_rectTitle1);

	//曲线标题矩形2
	CRect m_rectTitle2;
	m_rectTitle2.left = m_rectDraw.Width() / 2 + nHmidspace;
	m_rectTitle2.top = nCurHeight;
	m_rectTitle2.right = m_rectDraw.Width() - nHleftspace;
	m_rectTitle2.bottom = nCurHeight +  nVspace * 2;
	pDC->Rectangle(&m_rectTitle2);

	//曲线标题矩形3
	CRect m_rectTitle3;
	nCurHeight = nCurHeight + nVspace * 15;
	m_rectTitle3.left = nHleftspace - nHmidspace;
	m_rectTitle3.top = nCurHeight;
	m_rectTitle3.right = m_rectDraw.Width() / 2 - nHmidspace;
	m_rectTitle3.bottom = nCurHeight +  nVspace * 2;
	pDC->Rectangle(&m_rectTitle3);

	//曲线标题矩形4
	CRect m_rectTitle4;
	m_rectTitle4.left = m_rectDraw.Width() / 2 + nHmidspace;
	m_rectTitle4.top = nCurHeight;
	m_rectTitle4.right = m_rectDraw.Width() - nHleftspace;
	m_rectTitle4.bottom = nCurHeight +  nVspace * 2;
	pDC->Rectangle(&m_rectTitle4);

	//曲线标题矩形5
	CRect m_rectTitle5;
	nCurHeight = nCurHeight + nVspace * 15;
	m_rectTitle5.left = nHleftspace - nHmidspace;
	m_rectTitle5.top = nCurHeight;
	m_rectTitle5.right = m_rectDraw.Width() / 2 - nHmidspace;
	m_rectTitle5.bottom = nCurHeight +  nVspace * 2;
	pDC->Rectangle(&m_rectTitle5);

	//曲线标题矩形6
	CRect m_rectTitle6;
	m_rectTitle6.left = m_rectDraw.Width() / 2 + nHmidspace;
	m_rectTitle6.top = nCurHeight;
	m_rectTitle6.right = m_rectDraw.Width() - nHleftspace;
	m_rectTitle6.bottom = nCurHeight +  nVspace * 2;
	pDC->Rectangle(&m_rectTitle6);

	//曲线标题矩形7
	CRect m_rectTitle7;
	nCurHeight = nCurHeight + nVspace * 15;
	m_rectTitle7.left = nHleftspace - nHmidspace;
	m_rectTitle7.top = nCurHeight;
	m_rectTitle7.right = m_rectDraw.Width() / 2 - nHmidspace;
	m_rectTitle7.bottom = nCurHeight +  nVspace * 2;
	pDC->Rectangle(&m_rectTitle7);

	//曲线标题矩形8
	CRect m_rectTitle8;
	m_rectTitle8.left = m_rectDraw.Width() / 2 + nHmidspace;
	m_rectTitle8.top = nCurHeight;
	m_rectTitle8.right = m_rectDraw.Width() - nHleftspace;
	m_rectTitle8.bottom = nCurHeight +  nVspace * 2;
	pDC->Rectangle(&m_rectTitle8);

	pDC->RestoreDC(nSaveDC);
	pen.DeleteObject();
}

void CDrawButton::InnerDraw(CDC* pDC)
{
	switch (m_nStep)
	{
	case 1: //Step1
		if (m_bReportDraw)
		{
			DrawReport(pDC);
		}
		else
		{
			if (m_nCurveNumber == 0)
				Draw1Curve(pDC);
			else if (m_nCurveNumber == 1)
				Draw2Curve(pDC);
			else if (m_nCurveNumber == 2)
				Draw4Curve(pDC);
			else if (m_nCurveNumber == 3)
				Draw6Curve(pDC);
			else if (m_nCurveNumber == 4)
				Draw8Curve(pDC);
		}
			break;
	case 2: //Step2
		if (m_bReportDraw)
		{
			DrawReport(pDC);
			DrawRptTitleRect(pDC);
		}
		else
		{
			if (m_nCurveNumber == 0)
			{
				Draw1Curve(pDC);
				DrawCurveTitleRect(pDC);
			}

			else if (m_nCurveNumber == 1)
			{
				Draw2Curve(pDC);
				DrawCurveTitleRect(pDC);
			}
			else if (m_nCurveNumber == 2)
			{
				Draw4Curve(pDC);
				DrawCurveTitleRect(pDC);
			}
			else if (m_nCurveNumber == 3)
			{
				Draw6Curve(pDC);
				DrawCurveTitleRect(pDC);
			}
			else if (m_nCurveNumber == 4)
			{
				Draw8Curve(pDC);
				DrawLittlesCurveTitleRect(pDC);
			}
		}
		break;
	case 3: //Step3
		if (m_bReportDraw)
		{
			DrawReport(pDC);
		}
		else
		{
			if (m_nCurveNumber == 0)
			{
				Draw1Curve(pDC);
				Draw1CurveTitle(pDC);
			}
			else if (m_nCurveNumber == 1)
			{
				Draw2Curve(pDC);
				Draw2CurveTitle(pDC);
			}
			else if (m_nCurveNumber == 2)
			{
				Draw4Curve(pDC);
				Draw4CurveTitle(pDC);
			}
			else if (m_nCurveNumber == 3)
			{
				Draw6Curve(pDC);
				Draw6CurveTitle(pDC);
			}
			else if (m_nCurveNumber == 4)
			{
				Draw8Curve(pDC);
				Draw8CurveTitle(pDC);
			}
		}
		break;
	case 4: //Step4
		if (m_bReportDraw)
		{
			DrawReport(pDC);
		}
		else
		{
			if (m_nCurveNumber == 0)
				Draw1Curve(pDC);
			else if (m_nCurveNumber == 1)
				Draw2Curve(pDC);
			else if (m_nCurveNumber == 2)
				Draw4Curve(pDC);
			else if (m_nCurveNumber == 3)
				Draw6Curve(pDC);
			else if (m_nCurveNumber == 4)
				Draw8Curve(pDC);
		}
		break;
	case 5: //Step5
		if (m_bReportDraw)
		{
			DrawReport(pDC);
		}
		else
		{
			if (m_nCurveNumber == 0)
				Draw1Curve(pDC);
			else if (m_nCurveNumber == 1)
				Draw2Curve(pDC);
			else if (m_nCurveNumber == 2)
				Draw4Curve(pDC);
			else if (m_nCurveNumber == 3)
				Draw6Curve(pDC);
			else if (m_nCurveNumber == 4)
				Draw8Curve(pDC);
		}
		break;
	case 6: //Step6
		if (m_bReportDraw)
		{
			DrawReport(pDC);
		}
		else
		{
			if (m_nCurveNumber == 0)
				Draw1Curve(pDC);
			else if (m_nCurveNumber == 1)
				Draw2Curve(pDC);
			else if (m_nCurveNumber == 2)
				Draw4Curve(pDC);
			else if (m_nCurveNumber == 3)
				Draw6Curve(pDC);
			else if (m_nCurveNumber == 4)
				Draw8Curve(pDC);
		}
		break;
	}
}

void CDrawButton::SetDrawMode(BOOL bReportDraw)
{
	m_bReportDraw = bReportDraw;
	InvalidateRect(&m_rectDraw);
}

void CDrawButton::ReDrawRect(int nStep/*当前步骤*/)//重绘矩形区
{
	m_nStep = nStep;
	InvalidateRect(&m_rectDraw);
}

void CDrawButton::Setlayout(BOOL bHorLayout)
{
	m_bHorLayout = bHorLayout;
	InvalidateRect(&m_rectDraw);
}

void CDrawButton::SetListFormat(int nListFormat) //0:A-1,1:A-2,2:A-3,3:B-1
{
	m_nListFormat = nListFormat;
	InvalidateRect(&m_rectDraw);
}

void CDrawButton::Draw(unsigned int nStep, unsigned int nCurveNumber)
{
	m_nStep = nStep;
	m_nCurveNumber = nCurveNumber;
	//重绘矩形区
	InvalidateRect(&m_rectDraw);
}

void CDrawButton::DrawReport(CDC* pDC)
{
	int nSaveDC = pDC->SaveDC();
	CPen pen;
	pen.CreatePen (PS_SOLID, 1, RGB(0,0,0));
	pDC->SelectObject(&pen);//
	//画线
	int nVspace = m_rectDraw.Height() / 64;
	int nCurHeight = nVspace * 3;
	int nHleftspace = m_rectDraw.Width() / 10;
	int nHmidspace = m_rectDraw.Width() / 20;
	int nLittleTableMaxRowCoord = 0;
	//标题
	pDC->MoveTo(m_rectDraw.Width() / 3, nCurHeight);
	pDC->LineTo(m_rectDraw.Width() * 2/3, nCurHeight);
	//内容
	for (int i=1; i<=6; i++)
	{
		nCurHeight = nCurHeight + nVspace;
		pDC->MoveTo(nHleftspace, nCurHeight);
		pDC->LineTo((m_rectDraw.Width() - nHmidspace)/2 ,nCurHeight);

		pDC->MoveTo((m_rectDraw.Width() + nHmidspace)/2, nCurHeight);
		pDC->LineTo((m_rectDraw.Width() - nHleftspace), nCurHeight);
	}
	//大表格
	{
		nCurHeight = nCurHeight + nVspace * 1.5;
		int nCurColCoord = nCurHeight + nVspace * 1.5;//当前表格纵坐标
		//表格横线
		for (int row=1; row<=30; row++)
		{
			nCurHeight = nCurHeight + nVspace * 1.5;
			pDC->MoveTo(nHleftspace, nCurHeight);
			pDC->LineTo(m_rectDraw.Width() - nHleftspace * 2, nCurHeight);
		}
		//表格纵线
		int nCurRowCoord = nHleftspace;//当前表格横坐标
		int nRowSpan = (m_rectDraw.Width() - nHleftspace * 3)/9;
		for (int col=1; col<=10; col++)
		{
			if (col != 1 &&
				col != 10)
			{
				pDC->MoveTo(nCurRowCoord, nCurColCoord + nVspace*1.5);
				pDC->LineTo(nCurRowCoord, nCurHeight);
	
			}
			else
			{
				pDC->MoveTo(nCurRowCoord, nCurColCoord);
				pDC->LineTo(nCurRowCoord, nCurHeight);
			}

			if (col == 8)//最后两列
			{
				nCurRowCoord = nCurRowCoord + nRowSpan;
				nLittleTableMaxRowCoord = nCurRowCoord;
			}
			else if(col == 9)//最后一列
				nCurRowCoord = m_rectDraw.Width() - nHleftspace * 2;
			else
				nCurRowCoord = nCurRowCoord + nRowSpan;
		}
	}
	//小表格
	{
		nCurHeight = nCurHeight + nVspace * 1.5;
		int nCurColCoord = nCurHeight + nVspace * 1.5;//当前表格纵坐标
		//表格横线
		for (int row=1; row<=4; row++)
		{
			nCurHeight = nCurHeight + nVspace * 1.5;
			pDC->MoveTo(nHleftspace, nCurHeight);
			pDC->LineTo(nLittleTableMaxRowCoord, nCurHeight);
		}
		//表格纵线
		int nCurRowCoord = nHleftspace;//当前表格横坐标
		int nRowSpan = (m_rectDraw.Width() - nHleftspace * 3)/9;
		for (int col=1; col<=9; col++)
		{
			if (col == 2 ||
				col == 4 )//不需要画线
			{
				nCurRowCoord = nCurRowCoord + nRowSpan;
				continue;
			}
			pDC->MoveTo(nCurRowCoord, nCurColCoord);
			pDC->LineTo(nCurRowCoord, nCurHeight);
			nCurRowCoord = nCurRowCoord + nRowSpan;
		}
	}

	pDC->RestoreDC(nSaveDC);
	pen.DeleteObject();
}

void CDrawButton::DrawCurveTitleRect(CDC* pDC) //绘制标题框
{
	int nSaveDC = pDC->SaveDC();
	CPen pen;
	pen.CreatePen (PS_DOT, 3, RGB(255,0,0));
	pDC->SelectObject(&pen);//
	//pDC->SetROP2(R2_NOT);

	//画线
	int nVspace = m_rectDraw.Height() / 64;
	int nCurHeight = nVspace * 3;
	int nHleftspace = m_rectDraw.Width() / 10;
	int nHmidspace = m_rectDraw.Width() / 20;
	CRect titleRect;
	titleRect.left = nHleftspace - nHmidspace;
	titleRect.top = nCurHeight - nVspace;
	titleRect.right = m_rectDraw.Width() - nHleftspace + nHmidspace/2;
	titleRect.bottom = nVspace * 9 + nVspace;

	pDC->SelectObject(GetStockObject(NULL_BRUSH));
	pDC->Rectangle(&titleRect);

	pDC->RestoreDC(nSaveDC);
	pen.DeleteObject();
}

void CDrawButton::DrawLittlesCurveTitleRect(CDC* pDC) //绘制小标题框
{
	int nSaveDC = pDC->SaveDC();
	CPen pen;
	pen.CreatePen (PS_DOT, 2, RGB(255,0,0));
	pDC->SelectObject(&pen);//
	//pDC->SetROP2(R2_NOT);

	//画线
	int nVspace = m_rectDraw.Height() / 64;
	int nCurHeight = nVspace * 3;
	int nHleftspace = m_rectDraw.Width() / 10;
	int nHmidspace = m_rectDraw.Width() / 20;
	CRect titleRect;
	titleRect.left = m_rectDraw.Width() / 3 - nHmidspace;
	titleRect.top = nCurHeight - nVspace;
	titleRect.right = m_rectDraw.Width()*2/3 + nHmidspace ;
	titleRect.bottom = nVspace * 4;

	pDC->SelectObject(GetStockObject(NULL_BRUSH));
	pDC->Rectangle(&titleRect);

	pDC->RestoreDC(nSaveDC);
	pen.DeleteObject();
}

void CDrawButton::DrawRptTitleRect(CDC* pDC) //绘制总表标题框
{
	int nSaveDC = pDC->SaveDC();
	CPen pen;
	pen.CreatePen (PS_DOT, 3, RGB(255,0,0));
	pDC->SelectObject(&pen);//

	//画线
	int nVspace = m_rectDraw.Height() / 64;
	int nCurHeight = nVspace * 3;
	int nHleftspace = m_rectDraw.Width() / 10;
	int nHmidspace = m_rectDraw.Width() / 20;
	CRect titleRect;
	titleRect.left = nHleftspace - nHmidspace/2;
	titleRect.top = nCurHeight - nVspace;
	titleRect.right = m_rectDraw.Width() - nHleftspace + nHmidspace/2;
	titleRect.bottom = nVspace * 10;

	pDC->SelectObject(GetStockObject(NULL_BRUSH));
	pDC->Rectangle(&titleRect);

	pDC->RestoreDC(nSaveDC);
	pen.DeleteObject();
}

void CDrawButton::DrawReportFailRect(CDC* pDC) //绘制总表合格不合格框
{
	int nSaveDC = pDC->SaveDC();
	CPen pen;
	pen.CreatePen (PS_DOT, 3, RGB(255,0,0));
	pDC->SelectObject(&pen);//
	//pDC->SetROP2(R2_NOT);

	//画线
	int nVspace = m_rectDraw.Height() / 64;
	int nCurHeight = nVspace * 3;
	int nHleftspace = m_rectDraw.Width() / 10;
	int nHmidspace = m_rectDraw.Width() / 20;
	CRect titleRect;
	titleRect.left = nHleftspace - nHmidspace;
	titleRect.top = nCurHeight - nVspace;
	titleRect.right = m_rectDraw.Width() - nHleftspace + nHmidspace/2;
	titleRect.bottom = nVspace * 9 + nVspace;

	pDC->SelectObject(GetStockObject(NULL_BRUSH));
	pDC->Rectangle(&titleRect);

	pDC->RestoreDC(nSaveDC);
	pen.DeleteObject();
}

void CDrawButton::DrawReportRptRect(CDC* pDC) //绘制总表总结表框
{
	int nSaveDC = pDC->SaveDC();
	CPen pen;
	pen.CreatePen (PS_DOT, 3, RGB(255,0,0));
	pDC->SelectObject(&pen);//
	//pDC->SetROP2(R2_NOT);

	//画线
	int nVspace = m_rectDraw.Height() / 64;
	int nCurHeight = nVspace * 3;
	int nHleftspace = m_rectDraw.Width() / 10;
	int nHmidspace = m_rectDraw.Width() / 20;
	CRect titleRect;
	titleRect.left = nHleftspace - nHmidspace;
	titleRect.top = nCurHeight - nVspace;
	titleRect.right = m_rectDraw.Width() - nHleftspace + nHmidspace/2;
	titleRect.bottom = nVspace * 9 + nVspace;

	pDC->SelectObject(GetStockObject(NULL_BRUSH));
	pDC->Rectangle(&titleRect);

	pDC->RestoreDC(nSaveDC);
	pen.DeleteObject();
}

//输出位图
void CDrawButton::OutPutBmp()
{
	if (m_bReportDraw)//总表不需要输出位图
		return;
		
		if (m_nCurveNumber == 0) //输出1张位图
		{
			for (int i=0;i<g_sorFileArray.totalCurveNumber;i++)
			{
				SaveCurveToLocal(i);//zll 2012.9.24
			}
			//SaveCurveToLocal(0);
		}
		else if (m_nCurveNumber == 1)//输出2张位图
		{
			for (int i=0;i<g_sorFileArray.totalCurveNumber;i++)
			{
				SaveCurveToLocal(i);//zll 2012.9.24
			}
			/*SaveCurveToLocal(0);
			SaveCurveToLocal(1);*/
		}
		else if (m_nCurveNumber == 2) //输出4张位图
		{
			for (int i=0;i<g_sorFileArray.totalCurveNumber;i++)
			{
				SaveCurveToLocal(i);//zll 2012.9.24
			}
			/*SaveCurveToLocal(0);
			SaveCurveToLocal(1);
			SaveCurveToLocal(2);
			SaveCurveToLocal(3);*/
		}
		else if (m_nCurveNumber == 3)//输出6张位图
		{
			for (int i=0;i<g_sorFileArray.totalCurveNumber;i++)
			{
				SaveCurveToLocal(i);//zll 2012.9.24
			}
			/*SaveCurveToLocal(0);
			SaveCurveToLocal(1);
			SaveCurveToLocal(2);
			SaveCurveToLocal(3);
			SaveCurveToLocal(4);
			SaveCurveToLocal(5);*/ //zllzll
		}
		else if (m_nCurveNumber == 4) //输出8张位图
		{
			for (int i=0;i<g_sorFileArray.totalCurveNumber;i++)
			{
				SaveCurveToLocal(i);//zll 2012.9.24
			}
			/*SaveCurveToLocal(0);
			SaveCurveToLocal(1);
			SaveCurveToLocal(2);
			SaveCurveToLocal(3);
			SaveCurveToLocal(4);
			SaveCurveToLocal(5);
			SaveCurveToLocal(6);
			SaveCurveToLocal(7);*/
		}
}

void CDrawButton::SaveCurveToLocal(int nCurveIndex)
{
	//定义图形大小
	int iWidth = 320;//320;
	int iHeight = 200;

	iWidth = 642;  //所有图片尺寸都相同 lzy2014.9.3
	iHeight = 350;
	//if (m_nCurveNumber ==0) //每页1个波形
	//{	
	//	iWidth = 642;  //600
	//	iHeight = 350;
	//}
	//else if (m_nCurveNumber == 1) //每页2个波形 //zll 2013.5.28
	//{	
	//	//iWidth = 600;
	//	//iHeight = 280;
	//	iWidth = 642;  //600
	//	iHeight = 350;
	//}
	//else if (m_nCurveNumber == 4) //每页8个波形 //zll 2013.5.28
	//{	
	//	iHeight = 180;
	//}

	int iPixel  = 16;
	//图形格式参数
	LPBITMAPINFO lpbmih = new BITMAPINFO;
	lpbmih->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	lpbmih->bmiHeader.biWidth = iWidth;
	lpbmih->bmiHeader.biHeight = iHeight;
	lpbmih->bmiHeader.biPlanes = 1;
	lpbmih->bmiHeader.biBitCount = iPixel;
	lpbmih->bmiHeader.biCompression = BI_RGB;
	lpbmih->bmiHeader.biSizeImage = 0;
	lpbmih->bmiHeader.biXPelsPerMeter = 0;
	lpbmih->bmiHeader.biYPelsPerMeter = 0;
	lpbmih->bmiHeader.biClrUsed = 0;
	lpbmih->bmiHeader.biClrImportant = 0;

	//创建位图数据
	HDC hdc,hdcMem;
	HBITMAP hBitMap = NULL;
	CBitmap *pBitMap = NULL;
	CDC *pMemDC = NULL;
	BYTE *pBits;

	hdc = CreateIC(TEXT("DISPLAY"),NULL,NULL,NULL);
	hdcMem = CreateCompatibleDC(hdc);
	hBitMap = CreateDIBSection(hdcMem,lpbmih,DIB_PAL_COLORS,(void **)&pBits,NULL,0);
	pBitMap = new CBitmap;
	pBitMap->Attach(hBitMap);
	pMemDC = new CDC;
	pMemDC->Attach(hdcMem);
	pMemDC->SelectObject(pBitMap);
	//
	CRect rc(0,0,iWidth,iHeight);
	pMemDC->SetBkMode(TRANSPARENT);
	//添加自绘图形 lzy2014.8.25	
	//DrawCurve(pMemDC,rc, nCurveIndex);
	//CRect curveRect;
	//curveRect.SetRect(nHleftspace*1.5 + nHmidspace/2, nCurHeight, m_rectDraw.Width() - 1.5 * nHleftspace - nHmidspace/2, nBottom - nVspace * 1.5);
	//pDC->Rectangle(&curveRect);
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	//页面背景色
	CBrush brushCtl;
	brushCtl.CreateSolidBrush(RGB(255,255,255));
	pMemDC->Rectangle(rc);
	pMemDC->FillRect(rc,&brushCtl) ;
	brushCtl.DeleteObject();
	CRect rcInner = CRect(rc.left + rc.Width()*0.08, rc.top + rc.Height()*0.05, rc.right - rc.Width()*0.02, rc.bottom - rc.Height()*0.1);

	if(0 == m_nCurveNumber)  //单张
		MAIN->m_pPrintView->DrawTraceBmp(pMemDC, rcInner, g_sorFileArray.curSelection, 0.4);//lzy2014.8.26
	else
		MAIN->m_pPrintView->DrawTraceBmp(pMemDC, rcInner, nCurveIndex, 0.4);//lzy2014.8.26

	//MAIN->m_pPrintView->DrawGridLine(pMemDC, rc);//绘制栅格 wcq2012.10.31
	//MAIN->m_pPrintView->DrawCurve(pMemDC, rc, nCurveIndex);
	//MAIN->m_pPrintView->DrawCursorAB(pMemDC, rc); //绘制AB光标 xjf2014.03.03		
	//float flo_Length=g_sorFileArray.sorFileList[g_sorFileArray.curSelection].pTestResultInfo->Length;	
	//MAIN->m_pPrintView->DrawScale(pMemDC, rc, 0, flo_Length);

	//保存到文件并创建位图结构
	BITMAPFILEHEADER bmfh;
	ZeroMemory(&bmfh,sizeof(BITMAPFILEHEADER));
	*((char *)&bmfh.bfType) = 'B';
	*(((char *)&bmfh.bfType) + 1) = 'M';
	bmfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	bmfh.bfSize = bmfh.bfOffBits + (iWidth * iHeight) * iPixel / 8;

	TCHAR szBMPFileName[128];
	int iBMPBytes = iWidth * iHeight * iPixel / 8;
	//创建位图文件 
	CString filePath;
	filePath.Format(_T("%s\\trace%d.bmp"), CUtil::getExePath(), nCurveIndex+1);
	//strcpy(szBMPFileName,filePath);
	CFile file;
	if(file.Open(filePath,CFile::modeWrite | CFile::modeCreate))
	{
		file.Write(&bmfh,sizeof(BITMAPFILEHEADER));
		file.Write(&(lpbmih->bmiHeader),sizeof(BITMAPINFOHEADER));
		file.Write(pBits,iBMPBytes);
		file.Close();
	}

	pMemDC->DeleteDC();
	delete pMemDC;	pMemDC  = NULL;
	delete pBitMap; pBitMap = NULL;
	delete lpbmih;  lpbmih  = NULL;
}

/*
***************************************************************
函数名称:             DrawCurve()                                                                               
函数说明:             画测量曲线                                                                              
入口参数:             CDC                                                                                           
出口参数:             无                                                                                        
建立时间:             2011-07-24                                                                  
Write By:             XJF                                                                                          
***************************************************************
*/
void CDrawButton::DrawCurve(CDC* pDC, CRect& rect, int nCurveIndex)
{
	TestResultInfo* pData = g_sorFileArray.sorFileList[nCurveIndex].pTestResultInfo;
	if (pData == NULL ||
		pData->DataPointsNum < NUM_VALIDE_LEAST ||
		CLEAR_DATAPOINTS_NUM == pData->DataPointsNum)//没有或数据很少
		return;

	//页面背景色
	CBrush brushCtl;
	brushCtl.CreateSolidBrush(RGB(255,255,255));
	pDC->Rectangle(rect);
	pDC->FillRect(rect,&brushCtl) ;
	brushCtl.DeleteObject();

	unsigned int i;
	//转化为绘图像素坐标
	float fXConst;//计算x坐标时不变的部分
	float fXCoeff;//计算x坐标时系数
	//double fXCoeff;//计算x坐标时系数wcq2011.09.23
	float fYConst;//计算y坐标时不变的部分
	float fYCoeff;//计算y坐标时系数
	float width;//曲线绘制区域宽度单位：px
	pData->StartPoint = 0;//2013.05.12
	pData->MaxPoint = pData->DataPointsNum;//2013.05.12

	if (NULL == pData->ptDrawData)//2013.05.12
	{
		pData->ptDrawData=new CPoint[pData->MaxPoint-pData->StartPoint];
	}//eif

	unsigned int nStartPoint = pData->StartPoint;
	unsigned int nMaxPoint = pData->MaxPoint;
	width = rect.Width();

	int left = rect.left;
	int top;
	int right;
	int bottom = rect.bottom;

	fXConst=left-( (nStartPoint * 1.0) /(nMaxPoint - nStartPoint) ) * width;//计算x坐标时不变的部分//wcq2011.12.13
	fXCoeff=width * 1.0/(nMaxPoint - nStartPoint);////计算x坐标时系数//wcq2011.12.13
	fYConst=bottom - ((-M_FSTARTDB0)/(M_FMAXDB0 - M_FSTARTDB0)) * rect.Height() - pData->YOffset;//计算y坐标时不变的部分wcq2011.12.05//wcq2011.12.13
	fYCoeff=rect.Height()/((M_FMAXDB0 - M_FSTARTDB0) * 1000.0);//计算y坐标时系数//wcq2011.12.13
	WORD * pword ;//= pData->pDataPoints;
#ifdef COMPILE_SHOW_SMOOTH_DATA////显示时用平滑过的数据，保存时用实际数据 wcq2013.04.26
	pword = pData->pDataPointsSmooth;
#else
	pword = pData->pDataPoints;
#endif
	//CPoint * pPoint=pData->m_TestResult.ptDrawData;//wcq2011.09.23
	CPoint * pDrawData = pData->ptDrawData;//wcq2011.09.23
	CPoint * pDataCurve ;//= pData->pDataCurve;//wcq2011.09.23//2013.05.12

	int maxLen = nMaxPoint;//仅为方便循环wcq2011.09.22
	int pointCountToDraw = nMaxPoint - nStartPoint;//需要绘制的点的个数wcq2011.09.28
	int index = 0;

	m_PrintView.DrawGridLine(pDC,rect);//绘栅格

	int nIndex = 0;
	for(i = pData->StartPoint; i < pData->MaxPoint; i++)//2013.05.12
	{
		pData->ptDrawData[nIndex].x = int(rect.left + rect.Width () * (i - pData->StartPoint)/(pData->MaxPoint - pData->StartPoint));
		//pData->ptDrawData[i].y = int(rect.bottom - (((-pData->pDataPoints[i]/1000.0) - M_FSTARTDB0)/(M_FMAXDB0 - M_FSTARTDB0)) * rect.Height());
		pData->ptDrawData[nIndex].y = int(rect.bottom - (((-pword[i]/1000.0) - M_FSTARTDB0)/(M_FMAXDB0 - M_FSTARTDB0)) * rect.Height() - HIKE_UP_HEIGHT);//zll 2013.6.3 -50 使曲线沿y轴向上移动

		if(pData->ptDrawData[nIndex].x > rect.right)
			pData->ptDrawData[nIndex].x = rect.right;
		if(pData->ptDrawData[nIndex].y < rect.top)
			pData->ptDrawData[nIndex].y = rect.top;
		if(pData->ptDrawData[nIndex].y > rect.bottom)
			pData->ptDrawData[nIndex].y = rect.bottom; 

		nIndex++;
	}

	//画曲线
	pDC->Polyline(&pData->ptDrawData[pData->StartPoint], pointCountToDraw);

	//绘事件点竖线及下方矩形框 zll 2013.5.23 晚
	int fX,fY;
	for (int i=0; i<pData->EventList.vEventInfo.size(); i++)
	{		
		CRect rectLineBottom;
		CString temp;
		fX=int(pData->EventList.vEventInfo[i].fLocation*rect.Width()/pData->Length);
		fY=int(rect.bottom - (65535-pData->pDataPoints[pData->EventList.vEventInfo[i].index])/1000.0*rect.Height()/(g_nTraceYmax-OTDR_TRACE_Y_MIN) - HIKE_UP_HEIGHT);//zll 2013.6.3 -50 使事件点标示沿y轴向上移动
		//画竖直线
		pDC->MoveTo(fX,fY);
		pDC->LineTo(fX,fY+44);
		//竖直线下面的正方形
		rectLineBottom.SetRect(fX - 8,fY + 34,fX - 8 + 16, fY + 34 + 18);
		if (i>8)//两位数
		{
			rectLineBottom.right += 2;
		}
		pDC->Rectangle(&rectLineBottom);

		temp.Format(_T("%d"),i + 1);
		//pDC->SelectObject(&fontValue);////zll 2013.5.11 以下是A或B标旁边的字体属性
		rectLineBottom.DeflateRect(2,2);
		pDC->DrawText(temp,&rectLineBottom,DT_CENTER | DT_VCENTER | DT_SINGLELINE);//矩形框中的数值
	}
}

void CDrawButton::DeleteBmpFiles() //删除位图文件
{
	CString strExePath = CUtil::getExePath();
	for (int nIndex = 0; nIndex < g_sorFileArray.totalCurveNumber; nIndex++)
	{
		CString fileName;
		fileName.Format(_T("%s\\trace%d.bmp"), strExePath, nIndex+1);
		if (CUtil::isFileExist(fileName))
		{
			DeleteFile(fileName);
		}
	}
}