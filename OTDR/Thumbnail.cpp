// Thumbnail.cpp : 实现文件
//

#include "stdafx.h"
#include "OTDR.h"
#include "Thumbnail.h"
#include "SorFileInfo.h"
#include "SorDrawStatic.h"
#include "MainFrm.h"

extern SorFileArray g_sorFileArray;
// CThumbnail

IMPLEMENT_DYNCREATE(CThumbnail, CFormView)

CThumbnail::CThumbnail()
	: CFormView(CThumbnail::IDD)
{
}

CThumbnail::~CThumbnail()
{
}

void CThumbnail::SetLocalString()
{
	CString strTitle = IniUtil::ReadResString(_T("IDS_FRM_THUMBNAIL_TITLE"));
	GetParentFrame()->SetWindowText(strTitle);
}

void CThumbnail::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CThumbnail, CFormView)
	ON_WM_PAINT()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CThumbnail 诊断

#ifdef _DEBUG
void CThumbnail::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CThumbnail::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CThumbnail 消息处理程序

void CThumbnail::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
}

void CThumbnail::OnPaint()
{
	CPaintDC dc(this);

	//使用双缓冲技术画图
	GetClientRect(&m_rectThumbnail);

	CRect InvalidRect;
		dc.GetClipBox(&InvalidRect); //检取无效区,即需要重绘的区域

	CDC MemDC;
	CBitmap MemBitMap;
	CBitmap *pOldBitMap;

	//创建一个与dc兼容的内存内存设备环境
	if (MemDC.CreateCompatibleDC(&dc))
	{
		//创建一个与dc兼容的位图，大小为整个客户区
		if (MemBitMap.CreateCompatibleBitmap(&dc,m_rectThumbnail.Width(),m_rectThumbnail.Height()))
		{
			//将位图选入内存环境
			pOldBitMap=MemDC.SelectObject(&MemBitMap);

			//使MemDC的整个客户区都成无效区
			MemDC.SelectClipRgn(NULL);
			//再 "与上" 检取的无效区，使内存环境与dc检取的无效区相等
			MemDC.IntersectClipRect(InvalidRect);

			//用背景色将位图清除干净
			MemDC.FillSolidRect(&m_rectThumbnail, RGB(236,233,216));
		}
	}
	DrawThumbnail(&MemDC);

	//把绘制好的图形用BitBlt()"拷贝"到屏幕上
	dc.BitBlt(0,0,m_rectThumbnail.Width(),m_rectThumbnail.Height(),&MemDC, 0, 0,SRCCOPY); 
	MemDC.SelectObject(pOldBitMap);
	MemBitMap.DeleteObject();
	MemDC.DeleteDC();
}

/*
***************************************************************
函数名称:             DrawThumbnail()                                                                               
函数说明:             绘制参数显示区                                                                              
入口参数:             CDC                                                                         
出口参数:             无                                                                         
建立时间:             2012-04-02                                                                  
Write By:             XJF                                                                                          
***************************************************************
*/
void CThumbnail::DrawThumbnail(CDC *pDC)
{
	DrawThumbCoord(pDC);
	DrawThumbCurve(pDC);

}
/*
***************************************************************
函数名称:             DrawThumbCoord()                                                                               
函数说明:             绘制缩略图坐标                                                                              
入口参数:             CDC                                                                         
出口参数:             无                                                                         
建立时间:             2011-07-24                                                                  
Write By:             XJF                                                                                          
***************************************************************
*/
void CThumbnail::DrawThumbCoord(CDC *pDC)
{
	//填充背景
	pDC->FillSolidRect(&m_rectThumbnail,g_sorFileArray.waveConfig.ColorBack);  //坐标轴内的矩形区域
	if (g_sorFileArray.totalCurveNumber == 0)
		return;

	//绘制坐标系
	TestResultInfo* pTestResultInfo = g_sorFileArray.sorFileList[g_sorFileArray.curSelection].pTestResultInfo;
	int nSaveDC = pDC->SaveDC();

	//zll 2013.7.3注释
	/*CPen penThumbZoom;
	penThumbZoom.CreatePen (PS_SOLID, 1, RGB(0,255,0));*/

	CBrush brushThumbBG;
	brushThumbBG.CreateSolidBrush(g_sorFileArray.waveConfig.ColorBack);
	pDC->SelectObject(&brushThumbBG);//

	 //缩略图的矩形区域
	pDC->Rectangle(&CRect(0, 0, m_rectThumbnail.Width(), m_rectThumbnail.Height()));
	if (NULL == pTestResultInfo->pThumbData ||
		pTestResultInfo->DataPointsNum < NUM_VALIDE_LEAST)
	{

	}
	//zll 2013.7.3注释
	//pDC->SelectObject (&penThumbZoom);
	//CRect rect;
	//GetMoveRect(&rect, pTestResultInfo);//得到缩放矩形框//2013.5.10 晚
	//pDC->SelectStockObject(NULL_BRUSH);
	//pDC->Rectangle(&rect);
	
	//恢复
	pDC->RestoreDC(nSaveDC);
	//删除资源
	//penThumbZoom.DeleteObject();//zll 2013.7.3注释
	brushThumbBG.DeleteObject();
}

//平移曲线时得到当缩略图中表示缩放程度的矩形框(m_rectMoveThumb).2011.12.14//zll 2013.5.10 晚 需要修改
void CThumbnail::GetMoveRect(CRect* rectMove, TestResultInfo* pTestResultInfo)
{
	if (pTestResultInfo->DataPointsNum < NUM_VALIDE_LEAST)//wcq2011.12.22//wcq2011.12.23
	{
		return;
	}
    /////zll 2013.5.11
	INT nWidth;//矩形宽度
	nWidth = m_rectThumbnail.Width();//矩形宽度
	INT nHeight;//矩形宽度
	nHeight = m_rectThumbnail.Height();//矩形宽度
	CGraphElement* element=MAIN->m_pWaveFormView->m_sorDrawStatic.GetSeleCurve();
	COtdrTrace* pOtdrTrace=&MAIN->m_pWaveFormView->m_sorDrawStatic;
	int left = 0;
	int top  = 0;
	int right = m_rectThumbnail.Width();
	int bottom  = m_rectThumbnail.Height();
	left=pOtdrTrace->m_fRangeX[OTDR_TRACE_MIN]*nWidth/element->m_fLength;
	right=pOtdrTrace->m_fRangeX[OTDR_TRACE_MAX]*nWidth/element->m_fLength;
	top=nHeight-pOtdrTrace->m_fRangeY[OTDR_TRACE_MIN]*nHeight/g_nTraceYmax;
	bottom=nHeight-pOtdrTrace->m_fRangeY[OTDR_TRACE_MAX]*nHeight/g_nTraceYmax;
	rectMove->left=left;
    rectMove->right=right;
	rectMove->bottom=bottom;
	rectMove->top=top;

	////if (0 == MAIN->m_pWaveFormView->m_sorDrawStatic.m_iHorizonZoomNum && //cwcq2013.03.29
	////	0 == MAIN->m_pWaveFormView->m_sorDrawStatic.m_iVerticalZoomNum)//水平竖直都没缩放
	////{
	////	rectMove->SetRect(0, 0, m_rectThumbnail.Width(),m_rectThumbnail.Height());
	////}
	////else
	////{
	////	float fLeftRightCoeff;//左右比例
	////	INT nWidth;//矩形宽度
	////	INT nCenter;//矩形中心点
	////	//x轴
	////	int m_nPointLabel; //m_nSelectCursor:Seleced标志,1选中A，2选中B, 3选中AB
	////	if (CURSOR_SELECT::CURSOR_A == MAIN->m_pWaveFormView->m_sorDrawStatic.m_nSelectCursor) //选中A
	////	{
	////		m_nPointLabel = MAIN->m_pWaveFormView->m_sorDrawStatic.m_nPointA;
	////	}	
	////	else if (CURSOR_SELECT::CURSOR_B == MAIN->m_pWaveFormView->m_sorDrawStatic.m_nSelectCursor)
	////	{
	////		m_nPointLabel = MAIN->m_pWaveFormView->m_sorDrawStatic.m_nPointB;
	////	}
	////	else//AB
	////	{
	////		m_nPointLabel=(MAIN->m_pWaveFormView->m_sorDrawStatic.m_nPointA + MAIN->m_pWaveFormView->m_sorDrawStatic.m_nPointB)/2;
	////	}//eif
	////	//防止越界
	////	if (m_nPointLabel > pTestResultInfo->DataPointsNum)
	////	{
	////		m_nPointLabel = pTestResultInfo->DataPointsNum - 1;
	////	}
	////	int left = 0;//wqc2011.12.13
	////	int top  = 0;//wqc2011.12.13
	////	int right = m_rectThumbnail.Width();//wqc2011.12.13
	////	int bottom  = m_rectThumbnail.Height();//wqc2011.12.13
	////	if (0 == MAIN->m_pWaveFormView->m_sorDrawStatic.m_iHorizonZoomNum)
	////	{
	////		rectMove->left = left;//wqc2011.12.13
	////		rectMove->right = right;//wqc2011.12.13
	////	}
	////	else//水平无缩放
	////	{
	////		fLeftRightCoeff = 0;//左右比例
	////		nWidth = m_rectThumbnail.Width();//矩形宽度
	////		nCenter= nWidth / 2;//矩形中心点

	////		fLeftRightCoeff = m_nPointLabel * 1.0 / pTestResultInfo->DataPointsNum;//缩放时左右比例，左右不一样
	////		nCenter = m_rectThumbnail.Width() * fLeftRightCoeff;//矩形中心点
	////		nWidth = (MAIN->m_pWaveFormView->m_sorDrawStatic.m_nZoomCount - 
	////			MAIN->m_pWaveFormView->m_sorDrawStatic.m_iHorizonZoomNum) * 
	////			nWidth / MAIN->m_pWaveFormView->m_sorDrawStatic.m_nZoomCount;//矩形宽度
	////		rectMove->left = left + nCenter - nWidth * fLeftRightCoeff;//wqc2011.12.13
	////		if ( 0 == nWidth)//防止绘不出矩形来
	////		{
	////			nWidth++;
	////			nWidth++;
	////		}
	////		rectMove->right = rectMove->left + nWidth;
	////	}

	////	//y轴
	////	if (0 == MAIN->m_pWaveFormView->m_sorDrawStatic.m_iVerticalZoomNum)
	////	{
	////		rectMove->top = top;//wqc2011.12.13
	////		rectMove->bottom = bottom;//wqc2011.12.13
	////	}
	////	else
	////	{
	////		fLeftRightCoeff= (-pTestResultInfo->pDataPoints[m_nPointLabel]/1000.0 - 
	////			MAIN->m_pWaveFormView->m_sorDrawStatic.m_fStartDB) * 
	////			1.0/(MAIN->m_pWaveFormView->m_sorDrawStatic.m_fMaxDB - 
	////			MAIN->m_pWaveFormView->m_sorDrawStatic.m_fStartDB);//缩放时左右比例，左右不一样
	////		nCenter = m_rectThumbnail.Height() * (1 - fLeftRightCoeff);//fLeftRightCoeff;
	////		nWidth = (ZOOM_VERTICAL_COUNT - 
	////			MAIN->m_pWaveFormView->m_sorDrawStatic.m_iVerticalZoomNum)*m_rectThumbnail.Height()/ZOOM_VERTICAL_COUNT;//矩形高度
	////		rectMove->top = top + nCenter - nWidth * (1 - fLeftRightCoeff);//wqc2011.12.13
	////		if ( 0 == nWidth)//防止绘不出矩形来
	////		{
	////			nWidth++;
	////		}
	////		rectMove->bottom = rectMove->top + nWidth;
	////	}//eif
	////}////eif

	//////加上竖直方向的平移wcq2011.12.14
	//////int yOffset = (m_rectThumbnail.Height() / m_rectCoord.Height())*m_pMapCurveData[m_curABWave]->m_TestResult.YOffset;
	////int yOffset = (m_rectThumbnail.Height() * pTestResultInfo->YOffset / 
	////	MAIN->m_pWaveFormView->m_sorDrawStatic.m_rectCoord.Height());
	////rectMove->OffsetRect(0, yOffset);
}

/*
***************************************************************
函数名称:             DrawThumbCurve()                                                                               
函数说明:             绘制缩略图曲线                                                                              
入口参数:             CDC                                                                         
出口参数:             无                                                                         
建立时间:             2012-04-02                                                                  
Write By:             XJF                                                                                          
***************************************************************
*/
void CThumbnail::DrawThumbCurve(CDC *pDC)
{	
	//当前选择曲线
	TestResultInfo* pTestResultInfo = g_sorFileArray.sorFileList[g_sorFileArray.curSelection].pTestResultInfo;
	if (NULL == pTestResultInfo)
		return;

	int nSaveDC=pDC->SaveDC();
	unsigned int i;
	CPen penCurve;
	penCurve.CreatePen (PS_SOLID, 1, pTestResultInfo->curveColor);
	pDC->SelectObject(&penCurve);
	pDC->Polyline(pTestResultInfo->pThumbData, pTestResultInfo->ThumbDrawXCount);

	//int index = pTestResultInfo->ThumbDrawXCount > 0 ? pTestResultInfo->ThumbDrawXCount : 0;
	//if(index >0)
		//TRACE(_T("last point value = %d\n"), (*(pTestResultInfo->pThumbData+index-1)).y);

    ////////////////zll 2013.7.3 缩放矩形框绘在缩略图曲线上 人性化一些
	CPen penThumbZoom;
	penThumbZoom.CreatePen (PS_SOLID, 1, RGB(0,255,0));
	pDC->SelectObject (&penThumbZoom);
	CRect rect;
	GetMoveRect(&rect, pTestResultInfo);//得到缩放矩形框//2013.5.10 晚
	pDC->SelectStockObject(NULL_BRUSH);
	pDC->Rectangle(&rect);  
	////////////////

	//恢复
	pDC->RestoreDC(nSaveDC);
	penCurve.DeleteObject();
	penThumbZoom.DeleteObject();
}

/*
***************************************************************
函数名称:             GetAllThumbData
入口参数:             
出口参数:             
建立时间:             2011-12-14
Write By:             xjf
函数说明:             重新获取所有曲线的缩略图数据
***************************************************************
*/
void CThumbnail::GetAllThumbData()
{
	//计算
	for(int nCount = 0; nCount < g_sorFileArray.totalCurveNumber; nCount++)
	{
		TestResultInfo* pCurve = g_sorFileArray.sorFileList[nCount].pTestResultInfo;
		if (!pCurve)
		{
			continue;
		}//eif
		GetThumbData(pCurve);
	}//efor
}

//每收到一条新数据，都计算下用于绘制缩略图的点.
void CThumbnail::GetThumbData(TestResultInfo* pTestResultInfo)
{
	if (pTestResultInfo->DataPointsNum < NUM_VALIDE_LEAST)//没有或数据很少
		return;

	if (pTestResultInfo->pThumbData)//缩略图数据
	{
		delete[] pTestResultInfo->pThumbData;
		pTestResultInfo->pThumbData = NULL;
	}
	pTestResultInfo->pThumbData = new CPoint[m_rectThumbnail.Width() * 2];//x上选二点

	float fXCoeff;//计算x坐标时系数
	float fYConst;//计算y坐标时不变的部分
	float fYCoeff;//计算y坐标时系数
	float width;//曲线绘制区域宽度单位：px
	unsigned int nStartPoint = 0;
	unsigned int nMaxPoint = pTestResultInfo->DataPointsNum;//所有点
	//if (pTestResultInfo->IsMaxLength)//是距离最长曲线
	//{
		width = m_rectThumbnail.Width();
	//}
	//else/*不是最长曲线*/
	//{
	//	width = m_rectThumbnail.Width() * pTestResultInfo->Length / pTestResultInfo->MaxTestDistance;
	//}

	float MaxDB,StartDB;
	//if (IsLowerStandardAll())//
	{
		StartDB = M_FSTARTDB0;
		MaxDB = M_FMAXDB0;
	}
/*	else
	{
		StartDB = M_FSTARTDB1;
		MaxDB = M_FMAXDB1;
	}*/
	int top,left;//,right,bottom;//因为缩略图绘制时会先绘制到一个位图里，所以以左上角作为起始点wcq2011.12.13
	left = 0;//wcq2011.12.13
	top = 0;//wcq2011.12.13

	fYConst = top + m_rectThumbnail.Height()+ ( StartDB/( MaxDB - StartDB ) )*m_rectThumbnail.Height();//计算y坐标时不变的部分//wcq2011.12.13
	//fYCoeff = m_rectThumbnail.Height() /((MaxDB - StartDB) * 1000);//计算y坐标时系数
	//fYCoeff = m_rectThumbnail.Height()*9/10 /((MaxDB - StartDB) * 1000);//计算y坐标时系数 zll 2013.6.19 比例缩小到9/10
	fYCoeff = m_rectThumbnail.Height() /((MaxDB - StartDB) * 1000);//计算y坐标时系数 lzy2014.7.16


	//WORD * pword = pTestResultInfo->pDataPoints;
#ifdef COMPILE_SHOW_SMOOTH_DATA////显示时用平滑过的数据，保存时用实际数据
	WORD * pword = pTestResultInfo->pDataPointsSmooth;
#else
	WORD * pword = pTestResultInfo->pDataPoints;
#endif
	CPoint * pPoint = pTestResultInfo->pThumbData;
	float fStepCoeff = pTestResultInfo->DataPointsNum / width;
	int index = 0;
	int maxIndex, minIndex;//取点时某一象素对应的最大最小值点的数据数组下标.wcq2011.09.29
	int wMax, wMin;//某一象素对应的最大最小值点的值
	int step = 0;
	int lastStepPoint = 0;
	int maxLen = width;//m_rectThumbnail.Width();//
	int i;
	//int left = m_rectThumbnail.left;//已在前面声明 wcq2011.12.13
	pTestResultInfo->ThumbDrawXCount = maxLen * 2;//与下面的计算无关，主要是用于绘制缩略图。*2:每一点取两个//wcq2011.12.08
	for(i=0; i< maxLen; i++)
	{
		maxIndex = lastStepPoint;//默认每一象素第一点
		minIndex = lastStepPoint;//默认每一象素第一点
		wMax = pword[lastStepPoint];//默认每一象素第一点
		wMin = pword[lastStepPoint];//默认每一象素第一点
		step =(i+1)*fStepCoeff;//不会超过最大点个数??????
		//DEBUGMSG(true,(_T("step:%d\n"),step));
		for(int j=lastStepPoint+1;j<step;j++)
		{
			if (j==nMaxPoint)
			{
				break;
			}
			if (pword[j] > wMax)
			{
				wMax = pword[j];
				maxIndex=j;
			}
			else if (pword[j] < wMin)
			{
				wMin = pword[j];
				minIndex=j;
			}
		}
		if (minIndex < maxIndex)//根据下标先后将相应最大最小值添加到数组
		{
			//最大点
			pPoint[index].x = i + left;//m_rectThumbnail.left + maxIndex*fXCoeff;//wcq2011.12.13
			pPoint[index].y = fYConst+pword[minIndex]*fYCoeff;//wcq2011.12.13
			index++;
			//最小点
			pPoint[index].x = i + left;//m_rectThumbnail.left + minIndex*fXCoeff;
			pPoint[index].y = fYConst+pword[maxIndex]*fYCoeff;//wcq2011.12.13
		}
		else
		{//wcq2011.12.13
			//最大点
			pPoint[index].x = i + left;//m_rectThumbnail.left + maxIndex*fXCoeff;//wcq2011.12.13
			pPoint[index].y = fYConst+pword[maxIndex]*fYCoeff;//wcq2011.12.13
			index++;
			//最小点
			pPoint[index].x = i + left;//m_rectThumbnail.left + minIndex*fXCoeff;
			pPoint[index].y = fYConst+pword[minIndex]*fYCoeff;//wcq2011.12.13

		}
		index++;
		//
		lastStepPoint= step;//保存本次的最后一点
	}//endfor
}
void CThumbnail::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);
	//重新计算绘图区域
	GetClientRect(&m_rectThumbnail);
	//重新获取thumbnail数据
	GetAllThumbData();
}
