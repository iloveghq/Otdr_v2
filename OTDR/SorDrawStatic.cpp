// SorDrawStatic.cpp : 实现文件
//

#include "stdafx.h"
#include <math.h>
#include "SorDrawStatic.h"
#include "MainFrm.h"
#include "MyFont.h"
#include "EndGabor.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define ZOOM_VERTICAL_STANDARD 30*1000//wcq2011.09.26用于竖直方向缩放时确定最大量程，大于该值时为-65~30,小于时为-55～40
#define Y_StartEndPoint 12//zll2012.8.15 画事件点所在的竖线的纵坐标上下距离
#define X_StartEndPoint 5//zll2012.8.15 画事件点所在的竖线下的方格所在横坐标左距离
#define CursorAB_Coefficient 1000.0//系数 zll 2013.3.15
const unsigned C_zll = 299792458 ; //真空中的光速m

int g_nMaxPointFromFile=0;  //从文件读取的最大采样点数
int g_iMaxFlag=-1;  //两条曲线中的最大距离,网络中的距离长用0，文件中的大用1

// CSorDrawStatic 对话框
extern SorFileArray g_sorFileArray;

IMPLEMENT_DYNAMIC(CSorDrawStatic, CButton)

CSorDrawStatic::CSorDrawStatic()
{
	m_nStartPoint=0;
	m_nMaxPoint= INVALID_MAX_POINT;//原为0,wcq2012.02.20
	m_fStartDB=M_FSTARTDB0;
	m_fMaxDB=M_FMAXDB0;
	m_ptRecord=NULL;
	m_bReceiveResultOK=FALSE;
	m_iHorizonZoomNum=0; //初始化为0，表示无水平放大缩小操作
	m_iVerticalZoomNum=0; //初始化为0，表示无纵向放大操作
	m_bReadFromFile=FALSE;
	m_nStartPointFromFile=0;
	m_nMaxPointFromFile=0;
	m_iCurveZoomMode = 0; //曲线缩放
	m_iTestModeForFile=0;
	m_nSelectCursor = CURSOR_SELECT::CURSOR_A; //默认选中A
	m_nSelectIndex = 0;
	m_pRecvbuf = NULL;
	m_bEventCursor = FALSE;
	m_dwWaveTesting = 0;
	m_dwStartTestTime = 0;

	m_bBusy = FALSE;
	m_dwMaxPointCount=1;

	m_dwWaveSelected = INVALID_WAVELENGTH;
	m_nZoomCount=0;//如果为0则未进行过曲线缩放次数计算
	m_nWaveToDevice = 1550;//wcq2011.12.19
	m_nStopTestNormal = TRUE;
	m_tempInt=0;
	m_lastPageType = -1;//wcq2012.02.14
	m_bNeedHorizonZoom = FALSE;//是否需要进行缩放，由下面三个参数决定 wcq2012.02.17
	m_lastWaveLength = 0;//上一次波长
	m_lastDataLength = 0;//上一次测试点数
	m_lastRefraction = 0;//上一次测试的折射率
	m_bFirstRead = TRUE;
	m_bZoomOK = FALSE; //AB线是否可以拖动

	m_nMaxIndex = 0;
	m_fRefraction = 1.465f;
	m_cursorPoint.x = 0;
	m_cursorPoint.y = 0;
	m_nPointA = 0;
	m_nPointB = 0;
	m_fCoeff = 1.0;
	// initial scroll position
	m_nScrollPosVer = 0; 
	m_nScrollPosHor = 0; 
	// original scroll size
	m_nDrawHeight = 0;
	m_nDrawWidth = 0;

	m_nCurHeight = 0;
	m_nCurWidth = 0;

	m_RectSelected.SetRectEmpty();
	m_PLMouseDown.x = 0;
	m_PLMouseDown.y = 0;
}

CSorDrawStatic::~CSorDrawStatic()
{
}

BEGIN_MESSAGE_MAP(CSorDrawStatic, CButton)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_HSCROLL()
	ON_WM_SIZE()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

void CSorDrawStatic::SetPageTitle()
{

}

//设置页面文本
void CSorDrawStatic::SetCurPageText()
{
}

void CSorDrawStatic::AddEvent()
{
	TestResultInfo* pData = g_sorFileArray.sorFileList[g_sorFileArray.curSelection].pTestResultInfo;
	int nSelectPoint = 0;
	if (m_nSelectCursor == CURSOR_SELECT::CURSOR_A)
	{
		nSelectPoint = m_nPointA;
	}
	else if (m_nSelectCursor == CURSOR_SELECT::CURSOR_B)
	{
		nSelectPoint = m_nPointB;
	}
	else
		return;

	int waveNum = pData->WaveLength; 
	__try
	{
		pData->m_GaborResult = m_endgabor.insertEvent(nSelectPoint, //所要插入的事件点的数据点编号
			waveNum, //曲线波长
			pData->PulseWidth, //曲线脉宽
			pData->pDataPoints, //测试点数据
			pData->DataPointsNum, //点数
			pData->m_GaborResult,
			pData->Refraction,//折射率zll
			pData->Frequency,
			g_sorFileArray.eventAnalConfig.BackScatter);//g_sorFileArray.eventAnalConfig.BackScatter典型散射系数
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		return;
	}
	GaborResultToEvent(pData);
	//重绘
	InvalidateRect(m_rectCurve);
	MAIN->m_pEventFormView->FreshEventList();//刷新事件列表
	//重置选中项
	MAIN->m_pEventFormView->m_Eventlist.SetItemState(MAIN->m_pEventFormView->m_Eventlist.GetItemCount()-1, LVIS_SELECTED, LVIS_SELECTED);
}

//调用dll得到数据后转化为事件列表
void CSorDrawStatic::GaborResultToEvent(TestResultInfo* pCurveData)
{
	pCurveData->EventList.vEventInfo.clear();
	EventInfo eventinfo;
	int len = pCurveData->m_GaborResult.Valid_Num;//wcq2012.06.07
	float fDisTwo=Get_XCoord(1,pCurveData);
	for(int i=0;i<len;i++)
	{
		if (0==fDisTwo)
		{
			int ff=0;
		}//eif
		eventinfo.index = pCurveData->m_GaborResult.Event_Array[i];//下标，第几点
		eventinfo.nEventType = pCurveData->m_GaborResult.Reflect_Type[i];//事件类型
		eventinfo.fLocation = fDisTwo * pCurveData->m_GaborResult.Event_Array[i];//Get_XCoord(sGabor_Result..Event_Array[i],pCurveData);//两点间距离，以km为单位
		eventinfo.fEventloss = pCurveData->m_GaborResult.Event_Loss[i];
		eventinfo.fEventHigh = pCurveData->m_GaborResult.Event_Hight[i];
		eventinfo.fLineK = pCurveData->m_GaborResult.Line_k[i];// / fDisTwo;//2012.8.7 rzg修改
		eventinfo.fTotalLoss = pCurveData->m_GaborResult.Total_Loss[i];
		eventinfo.fReflect =pCurveData->m_GaborResult.Reflect;
		eventinfo.fTrailEnd = pCurveData->m_GaborResult.Trail_End;
		eventinfo.cEventType=pCurveData->m_GaborResult.Event_type[i];//event code
		pCurveData->EventList.vEventInfo.push_back(eventinfo);
	}//efor
}



//从文件读取事件列表后转化为使用的GaborResult结构体 wcq2012.06.11
void CSorDrawStatic::EventToGaborResult(TestResultInfo* pCurveData)
{
	EventInfo eventinfo;
	int len = pCurveData->EventList.vEventInfo.size();//wcq2012.06.07
	if ((len < 0)  || (len > OTDR_EVENT_NUM_MAX))//防止出现意外 wcq2012.07.16
	{
		return;
	}//eif
	pCurveData->m_GaborResult.Valid_Num = len;
	float fDisTwo=Get_XCoord(1,pCurveData);
	//float ddd=0;//zll 2012.12.26注释
	for(int i=0;i<len;i++)
	{
		eventinfo = pCurveData->EventList.vEventInfo[i];
		//	pCurveData->m_GaborResult.Event_Array[i] = eventinfo.index;//下标，第几点//???wcq2012.07.09

		//pCurveData->m_GaborResult.Reflect_Type[i] = !eventinfo.nEventType;//事件类型//标准里有3种，这里只显示反射、非反射两种cwcq2012.06.12//cwcq2012.07.09
		pCurveData->m_GaborResult.Reflect_Type[i] = eventinfo.nEventType;//事件类型//标准里有3种，这里只显示反射、非反射两种cwcq2012.06.12//wcq2012.07.09
		pCurveData->m_GaborResult.Event_Array[i] = eventinfo.index;//Get_XCoord(pCurveData->m_GaborResult..Event_Array[i],pCurveData);//两点间距离，以km为单位

		//以下两行 zll 2012.12.26注释
		/*ddd=eventinfo.fLocation/fDisTwo;
		pCurveData->m_GaborResult.Event_Array[i]=ddd;*/

		pCurveData->m_GaborResult.Event_Loss[i] = eventinfo.fEventloss;
		pCurveData->m_GaborResult.Event_Hight[i] = eventinfo.fEventHigh ;
		//pCurveData->m_GaborResult.Line_k[i] =  fDisTwo * eventinfo.fLineK;
		pCurveData->m_GaborResult.Line_k[i] =  eventinfo.fLineK;//wcq2012.06.29 ?????
		pCurveData->m_GaborResult.Total_Loss[i] = eventinfo.fTotalLoss;
		pCurveData->m_GaborResult.Reflect = eventinfo.fReflect;
		pCurveData->m_GaborResult.Trail_End = eventinfo.fTrailEnd;
		pCurveData->m_GaborResult.Event_type[i] = eventinfo.cEventType;//wcq2012.07.09
	}//efor
}

void CSorDrawStatic::RemoveEvent()
{
	TestResultInfo* pCurveData = g_sorFileArray.sorFileList[g_sorFileArray.curSelection].pTestResultInfo;
	POSITION pos = MAIN->m_pEventFormView->m_Eventlist.GetFirstSelectedItemPosition();
	int nIndex = MAIN->m_pEventFormView->m_Eventlist.GetNextSelectedItem(pos);
	if ((nIndex < 0) || (nIndex > pCurveData->m_GaborResult.Valid_Num))
	{
		return;
	}
	int waveNum = (1550 == pCurveData->WaveLength) ? 1:0;
	__try{
		pCurveData->m_GaborResult = m_endgabor.deleteEvent(nIndex,
												waveNum,
												pCurveData->PulseWidth,
												pCurveData->pDataPoints,
												pCurveData->m_GaborResult,
												pCurveData->Refraction,//折射率zll
												pCurveData->Frequency,
												0.0f);
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		return;
	}
	GaborResultToEvent(pCurveData);////调用dll得到数据后转化为事件列表 wcq2012.06.08
	MAIN->m_pEventFormView->FreshEventList();//刷新界面事件列表 
	//
	nIndex--;
	if (nIndex < 0)
	{
		nIndex = 0;
	}
	if (MAIN->m_pEventFormView->m_Eventlist.GetItemCount() >= nIndex)
	{
		MAIN->m_pEventFormView->m_Eventlist.SetItemState(nIndex, LVIS_SELECTED, LVIS_SELECTED);
	}
	//EventCursor();//重绘
	InvalidateAll();// wcq2012.06.08
}

//按下测试后初始化一些参数，如A/B标位置 wcq2012.02.21
void CSorDrawStatic::NewTestInit(TestResultInfo* pData)
{
	if (pData->Length < m_lastTestInfo.m_distanceA)//原来的A标位置比本次测试光纤长度大
	{
		m_nPointA = pData->DataPointsNum / 2;//取中间位置
		m_iHorizonZoomNum = 0;
	}
	else
	{
		m_nPointA = Get_PosPoint(m_lastTestInfo.m_distanceA,pData);//求对应点位置
	}

	if (pData->Length < m_lastTestInfo.m_distanceB)//原来的A标位置比本次测试光纤长度大
	{
		m_nPointB = pData->DataPointsNum / 2;//取中间位置
		m_iHorizonZoomNum =0;
	}
	else
	{
		m_nPointB = Get_PosPoint(m_lastTestInfo.m_distanceB,pData);//求对应点位置
	}
}

BOOL CSorDrawStatic::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}

void CSorDrawStatic::OnPaint() 
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

	CalCulDispRect();
	DrawCoord(&MemDC);
	DrawCurves(&MemDC);
	//DrawLineAB(&MemDC);
	DrawCursor(&MemDC);

	//把绘制好的图形用BitBlt()"拷贝"到屏幕上
	dc.BitBlt(0,0,ClientRect.Width()*2,ClientRect.Height() * 2,&MemDC, 0, 0,SRCCOPY); 
	MemDC.SelectObject(pOldBitMap);
	MemBitMap.DeleteObject();
	MemDC.DeleteDC();
}

//光标移动时需要重绘区域,该函数只确定左右边界。//wcq2011.12.26
void CSorDrawStatic::GetCursorRect(CRect* rect)//光标移动时需要重绘区域。//wcq2011.12.26
{
	POINT ptA,ptB, ptEvent;
	int nXCoord=0;
	int nHalfWidth = m_rectCoord.right / 2;
	int lineWidth = WIDTH_CURSOR_LEFT;

	if (m_nSelectCursor == CURSOR_SELECT::CURSOR_A)//选中A
	{
		nXCoord = int(m_rectCoord.left + m_rectCoord.Width () * (m_nPointA - m_nStartPoint) * 1.0/(m_nMaxPoint-m_nStartPoint));
		if (nXCoord < nHalfWidth)
		{
			rect->left = nXCoord - lineWidth*2;//竖线本身宽度
			rect->right = nXCoord + WIDTH_CURSOR_INVALID;//多少m 文本
		}
		else if (nXCoord > nHalfWidth)
		{
			rect->right = nXCoord + lineWidth;
			rect->left = nXCoord - WIDTH_CURSOR_INVALID;
		}
		else//=中间时，无效区域为两倍
		{
			rect->right = nXCoord + WIDTH_CURSOR_INVALID + lineWidth + lineWidth;
			rect->left = nXCoord - WIDTH_CURSOR_INVALID;
		}
	}
	else if (m_nSelectCursor == CURSOR_SELECT::CURSOR_B) //选中B
	{
		nXCoord = int(m_rectCoord.left + m_rectCoord.Width () * (m_nPointB - m_nStartPoint) * 1.0/(m_nMaxPoint-m_nStartPoint));
		if (nXCoord < nHalfWidth)
		{
			rect->left = nXCoord - lineWidth*2;//竖线本身宽度
			rect->right = nXCoord + WIDTH_CURSOR_INVALID;//多少m 文本
		}
		else if (nXCoord > nHalfWidth)
		{
			rect->right = nXCoord + lineWidth;
			rect->left = nXCoord - WIDTH_CURSOR_INVALID;
		}
		else//=中间时，无效区域为两倍
		{
			rect->right = nXCoord + WIDTH_CURSOR_INVALID ;
			rect->left = nXCoord - WIDTH_CURSOR_INVALID - lineWidth*3;
		}
	}
	else if (m_nSelectCursor == CURSOR_SELECT::CURSOR_AB) //选中AB
	{
		nXCoord = int(m_rectCoord.left + m_rectCoord.Width () * (m_nPointA - m_nStartPoint) * 1.0/(m_nMaxPoint-m_nStartPoint));
		int nXCoordB = int(m_rectCoord.left + m_rectCoord.Width () * (m_nPointB - m_nStartPoint) * 1.0/(m_nMaxPoint-m_nStartPoint));
		if (nXCoord < nXCoordB)//A在B左边
		{
			if (nXCoord < nHalfWidth)//文本在右边
			{
				rect->left = nXCoord - lineWidth;
			}
			else//文本在左边
			{
				rect->left = nXCoord - WIDTH_CURSOR_INVALID;
			}//eif
			if (nXCoordB < nHalfWidth)//B,文本在右边
			{
				rect->right = nXCoordB + WIDTH_CURSOR_INVALID +lineWidth;
			}
			else//文本在左边
			{
				rect->right = nXCoordB +lineWidth;

			}//eif
		}
		else//B左A右
		{
			if (nXCoordB < nHalfWidth)//文本在右边
			{
				rect->left = nXCoordB - lineWidth;
			}
			else//文本在左边
			{
				rect->left = nXCoordB - WIDTH_CURSOR_INVALID;
			}//eif
			if (nXCoord < nHalfWidth)//A,文本在右边
			{
				rect->right = nXCoord + WIDTH_CURSOR_INVALID;
			}
			else//文本在左边
			{
				rect->right = nXCoord + lineWidth;

			}//eif
		}//eif //选中AB
	}//eif 选中
}

//刷新绘图区综略图波长脉宽显示区,方便调用
void CSorDrawStatic::InvalidateAll()
{
	InvalidateRect(m_rectCurve,FALSE);//曲线
}

void CSorDrawStatic::DrawCursor(CDC *pDC)
{
	TestResultInfo* pCurveData = GetMaxLengthCurveData();

	int nSaveDC = pDC->SaveDC();

	POINT ptA,ptB, ptEvent;
	float nDistanceA;
	CString strTextA;

	float nDistanceB;
	CString strTextB;

	if(pCurveData)//借用来判断是否已经测试或加载过数据
	{
		ptA.x= int(m_rectCoord.left + m_rectCoord.Width () * (m_nPointA - m_nStartPoint) * 1.0/(m_nMaxPoint-m_nStartPoint));
		ptB.x= int(m_rectCoord.left + m_rectCoord.Width () * (m_nPointB - m_nStartPoint) * 1.0/(m_nMaxPoint-m_nStartPoint));

		nDistanceA=Get_XCoord(m_nPointA, pCurveData);
		nDistanceB=Get_XCoord(m_nPointB, pCurveData);

		if (ptA.x>=m_rectCoord.left && ptA.x<=m_rectCoord.right)
		{
			if(_T("m")==g_sorFileArray.waveConfig.DistUnit)//横坐标单位为km
			{
				strTextA.Format(_T("%.3f"),nDistanceA/CursorAB_Coefficient);
				strTextA=_T("A ")+strTextA+_T("km");
			}
			else if(_T("mi")==g_sorFileArray.waveConfig.DistUnit)//横坐标单位为miles
			{
				nDistanceA=KmeterToOther(nDistanceA,UNIT_SHOW_MILE);
				strTextA.Format(_T("%.3f"),nDistanceA/CursorAB_Coefficient);
				strTextA=_T("A ")+strTextA+_T("miles");
			}
			else if(_T("ft")==g_sorFileArray.waveConfig.DistUnit)//横坐标单位为kft
			{
				nDistanceA=KmeterToOther(nDistanceA,UNIT_SHOW_KFEET);
				strTextA.Format(_T("%.3f"),nDistanceA/CursorAB_Coefficient);
				strTextA=_T("A ")+strTextA+_T("kft");
			}
		}
		if (ptB.x>=m_rectCoord.left && ptB.x<=m_rectCoord.right)
		{
			if(_T("m")==g_sorFileArray.waveConfig.DistUnit)//横坐标单位为km
			{
				strTextB.Format(_T("%.3f"),nDistanceB/CursorAB_Coefficient);
				strTextB=_T("B ") +strTextB+ _T("km");
			}
			else if(_T("mi")==g_sorFileArray.waveConfig.DistUnit)//横坐标单位为miles
			{
				nDistanceB=KmeterToOther(nDistanceB,UNIT_SHOW_MILE);
				strTextB.Format(_T("%.3f"),nDistanceB/CursorAB_Coefficient);
				strTextB=_T("B ") +strTextB+ _T("miles");
			}
			else if(_T("ft")==g_sorFileArray.waveConfig.DistUnit)//横坐标单位为kft
			{
				nDistanceB=KmeterToOther(nDistanceB,UNIT_SHOW_KFEET);
				strTextB.Format(_T("%.3f"),nDistanceB/CursorAB_Coefficient);
				strTextB=_T("B ") +strTextB+ _T("kft");
			}
		}
	}
	else //还没有采集到数据的情况
	{
		ptA.x=m_rectCoord.left;
		ptB.x=m_rectCoord.left+m_rectCoord.Width()/5;

		nDistanceA=0;
		nDistanceB=60/5;
		//nDistanceB=60000/5;//zll 2012.10.30注释

		if (ptA.x>=m_rectCoord.left && ptA.x<=m_rectCoord.right)
		{
			if(_T("m")==g_sorFileArray.waveConfig.DistUnit)//横坐标单位为km
			{
				strTextA.Format(_T("%.3f"),nDistanceA/CursorAB_Coefficient);
				strTextA=_T("A ")+strTextA+ _T("km");
			}
			else if(_T("mi")==g_sorFileArray.waveConfig.DistUnit)//横坐标单位为miles
			{
				nDistanceA=KmeterToOther(nDistanceA,UNIT_SHOW_MILE);
				strTextA.Format(_T("%.3f"),nDistanceA/CursorAB_Coefficient);
				strTextA=_T("A ")+strTextA+ _T("miles");
			}
			else if(_T("ft")==g_sorFileArray.waveConfig.DistUnit)//横坐标单位为kft
			{
				nDistanceA=KmeterToOther(nDistanceA,UNIT_SHOW_KFEET);
				strTextA.Format(_T("%.3f"),nDistanceA/CursorAB_Coefficient);
				strTextA=_T("A ")+strTextA+ _T("kft");
			}
		}
		if (ptB.x>=m_rectCoord.left && ptB.x<=m_rectCoord.right)
		{
			if(_T("m")==g_sorFileArray.waveConfig.DistUnit)//横坐标单位为km
			{
				strTextB.Format(_T("%.3f"),nDistanceB/CursorAB_Coefficient);
				strTextB= _T("B ")+strTextB+ _T("km");
			}
			else if(_T("mi")==g_sorFileArray.waveConfig.DistUnit)//横坐标单位为miles
			{
				nDistanceB=KmeterToOther(nDistanceB,UNIT_SHOW_MILE);
				strTextB.Format(_T("%.3f"),nDistanceB/CursorAB_Coefficient);
				strTextB= _T("B ")+strTextB+ _T("miles");
			}
			else if(_T("ft")==g_sorFileArray.waveConfig.DistUnit)//横坐标单位为kft
			{
				nDistanceB=KmeterToOther(nDistanceB,UNIT_SHOW_KFEET);
				strTextB.Format(_T("%.3f"),nDistanceB/CursorAB_Coefficient);
				strTextB= _T("B ")+strTextB+ _T("kft");
			}
		}
	}

	CPen penSelect;
	penSelect.CreatePen (PS_SOLID, 2, RGB(255,0,0));

	CPen penUnselect;
	penUnselect.CreatePen (PS_SOLID,2,RGB(255,255,255));

	CPen* pOldPen;//wcq2011.12.08
	if (m_nSelectCursor == CURSOR_SELECT::CURSOR_A)//选中A
	{
		pOldPen = pDC->SelectObject(&penSelect);
		if (ptA.x>=m_rectCoord.left && ptA.x<=m_rectCoord.right)
		{
			pDC->MoveTo(ptA.x,m_rectCoord.bottom);
			pDC->LineTo(ptA.x,m_rectCoord.top+14);
		}
		pDC->SelectObject(&penUnselect);
		if (ptB.x>=m_rectCoord.left && ptB.x<=m_rectCoord.right)
		{
			pDC->MoveTo(ptB.x,m_rectCoord.bottom);
			pDC->LineTo(ptB.x,m_rectCoord.top+34);
		}
	}
	else if (m_nSelectCursor == CURSOR_SELECT::CURSOR_B) //选中B
	{
		pOldPen = pDC->SelectObject(&penUnselect);
		if (ptA.x>=m_rectCoord.left && ptA.x<=m_rectCoord.right)
		{
			pDC->MoveTo(ptA.x,m_rectCoord.bottom);
			pDC->LineTo(ptA.x,m_rectCoord.top+14);
		}
		pDC->SelectObject(&penSelect);
		if (ptB.x>=m_rectCoord.left && ptB.x<=m_rectCoord.right)
		{
			pDC->MoveTo(ptB.x,m_rectCoord.bottom);
			pDC->LineTo(ptB.x,m_rectCoord.top+34);
		}
	}
	else if (m_nSelectCursor == CURSOR_SELECT::CURSOR_AB) //选中AB
	{
		pOldPen = pDC->SelectObject(&penSelect);
		if (ptA.x>=m_rectCoord.left && ptA.x<=m_rectCoord.right)
		{
			pDC->MoveTo(ptA.x,m_rectCoord.bottom);
			pDC->LineTo(ptA.x,m_rectCoord.top+14);
		}

		if (ptB.x>=m_rectCoord.left && ptB.x<=m_rectCoord.right)
		{
			pDC->MoveTo(ptB.x,m_rectCoord.bottom);
			pDC->LineTo(ptB.x,m_rectCoord.top+34);
		}
	}
	else if (m_bEventCursor) //事件光标
	{
		pOldPen = pDC->SelectObject(&penSelect);
		if (ptEvent.x>=m_rectCoord.left && ptEvent.x<=m_rectCoord.right)
		{
			pDC->MoveTo(ptEvent.x,m_rectCoord.bottom);
			pDC->LineTo(ptEvent.x,m_rectCoord.top+14);
		}
	}

	CRect rtA,rtB, rtEvent;
	/////////////////////当A,B线超过中线时，向右时距离矩形框在左边，否则在右边	
	if(ptA.x < m_rectCoord.right / 2)
	{
		rtA.left=ptA.x;
		rtA.top=m_rectCoord.top;
		rtA.right=ptA.x+100;
		rtA.bottom=m_rectCoord.top+14;
	}
	else
	{
		rtA.left=ptA.x-100;
		rtA.top=m_rectCoord.top;
		rtA.right=ptA.x;
		rtA.bottom=m_rectCoord.top+14;
	}
	if(ptB.x < m_rectCoord.right / 2)
	{
		rtB.left=ptB.x;
		rtB.top=m_rectCoord.top+20;
		rtB.right=ptB.x+100;
		rtB.bottom=m_rectCoord.top+34;
	}
	else
	{
		rtB.left=ptB.x-100;
		rtB.top=m_rectCoord.top+20;
		rtB.right=ptB.x;
		rtB.bottom=m_rectCoord.top+34;
	}

	if(m_nSelectCursor == CURSOR_SELECT::CURSOR_A)//选中A
	{
		pDC->SetTextColor (RGB(0,0,255));//no select cursor blue
		pDC->SetBkMode(TRANSPARENT);
		pDC->ExtTextOut(rtB.left+2,m_rectCoord.top+20,ETO_CLIPPED,rtB,strTextB,NULL);
		pDC->SetTextColor (RGB(255,0,0)); //select cursor red
		pDC->ExtTextOut(rtA.left+2,m_rectCoord.top,ETO_CLIPPED,rtA,strTextA,NULL);
	}
	else if(m_nSelectCursor == CURSOR_SELECT::CURSOR_B)//选中B
	{
		pDC->SetTextColor (RGB(0,0,255)); //no select cursor blue
		pDC->SetBkMode(TRANSPARENT);
		pDC->ExtTextOut(rtA.left+2,m_rectCoord.top,ETO_CLIPPED,rtA,strTextA,NULL);
		pDC->SetTextColor (RGB(255,0,0));
		pDC->ExtTextOut(rtB.left+2,m_rectCoord.top+20,ETO_CLIPPED,rtB,strTextB,NULL);
	}
	else if(m_nSelectCursor == CURSOR_SELECT::CURSOR_AB)//选中AB
	{
		pDC->SetTextColor (RGB(255,0,0));
		pDC->SetBkMode(TRANSPARENT);
		pDC->ExtTextOut(rtA.left+2,m_rectCoord.top,ETO_CLIPPED,rtA,strTextA,NULL);
		pDC->ExtTextOut(rtB.left+2,m_rectCoord.top+20,ETO_CLIPPED,rtB,strTextB,NULL);
	}
	//释放资源
	pDC->RestoreDC(nSaveDC);//wcq2011.12.08
	penSelect.DeleteObject();
	penUnselect.DeleteObject();
}

void CSorDrawStatic::DrawCurves(CDC *pDC) //绘制所有曲线
{
	int SingleorMulti=0;//绘单条迹线
	//判断是否为多曲线绘制模式
	if (!g_sorFileArray.waveConfig.MultiMode)
	{
		TestResultInfo* pCurve = g_sorFileArray.sorFileList[g_sorFileArray.curSelection].pTestResultInfo;
		DrawCurve(pDC, pCurve,SingleorMulti);
		return;
	}

	//多曲线绘制模式
	for(int nCount = 0; nCount < 8; nCount++)
	{
		//
		TestResultInfo* pCurve = g_sorFileArray.sorFileList[nCount].pTestResultInfo;
		SingleorMulti=nCount;//绘单条迹线
		if (pCurve)
		{
			DrawCurve(pDC, pCurve,SingleorMulti);
		}//eif
	}//efor
}
/*
***************************************************************
入口参数:
出口参数:
建立时间:2011-09-09
Write By:wcq
函数说明:计算所有曲线中最长的距离并赋给所有曲线结构相应变量
***************************************************************
*/
void CSorDrawStatic::CalMaxDistance()
{
	int nMaxIndex=0;//最长距离曲线对应的索引
	float fMaxDistance=0;//最大距离
	float fTemp=0;//临时变量
	bool bFirst=true;//是否第一个含有有效数据的曲线
	//计算
	for(int nCount = 0; nCount < g_sorFileArray.totalCurveNumber; nCount++)
	{
		TestResultInfo* pCurve = g_sorFileArray.sorFileList[nCount].pTestResultInfo;
		if ((!pCurve) || (CLEAR_DATAPOINTS_NUM == pCurve->DataPointsNum) )//wcq2011.12.23
		{
			continue;
		}
		if (bFirst)
		{
			fMaxDistance= pCurve->Length;//wcq2011.11.25//(C/(2 * pCurve->m_TestResult.Frequency * 1000000 * pCurve->m_TestResult.Refraction)) * (pCurve->m_TestResult.DataPointsNum);
			bFirst=false;
		}
		else{
			fTemp= pCurve->Length;//wcq2011.11.25//(C/(2 * pCurve->m_TestResult.Frequency * 1000000 * pCurve->m_TestResult.Refraction)) * (pCurve->m_TestResult.DataPointsNum);
			if(fTemp > (fMaxDistance+0.0001) )//0.0001:两个很接近的float比较时结果不好确定
			{
				fMaxDistance = fTemp;
				nMaxIndex = nCount;//索引
			}
		}
		//pCurve->m_TestResult.Length = fMaxDistance;//曲线长度wcq2011.11.10
	}
	//赋值
	for(int nCount = 0; nCount < g_sorFileArray.totalCurveNumber; nCount++)
	{
		TestResultInfo* pCurve = g_sorFileArray.sorFileList[nCount].pTestResultInfo;
		if (pCurve)
		{
			//是否为最长距离变量
			if (nMaxIndex == nCount)
			{
				pCurve->IsMaxLength=TRUE;
				//m_curABWave = it->first;
				m_nMaxIndex = nMaxIndex;
			}
			else
			{
				pCurve->IsMaxLength=FALSE;
			}
			//最长距离
			pCurve->MaxTestDistance=fMaxDistance;
		}
	}
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
void CSorDrawStatic::DrawCurve(CDC *pDC, TestResultInfo* pData,int SingleorMulti)
{
	if (pData == NULL ||
		pData->DataPointsNum < NUM_VALIDE_LEAST ||
		pData->pDataPoints == NULL ||
		CLEAR_DATAPOINTS_NUM == pData->DataPointsNum)//没有或数据很少wcq2011.09.09 //0 == m_pMapCurveData[m_curABWave]->m_TestResult.DataPointsNum.重测时会清零//wcq2011.12.22
		return;


	int nSaveDC=pDC->SaveDC();
	unsigned int i;
	CPen penCurve;
	penCurve.CreatePen (PS_SOLID, 1, pData->curveColor);

	CBrush brushEventPoint;
	brushEventPoint.CreateSolidBrush(CLR_YELLOW);  //事件点笔刷，颜色

	//转化为绘图像素坐标
	float fXConst;//计算x坐标时不变的部分
	float fXCoeff;//计算x坐标时系数
	//double fXCoeff;//计算x坐标时系数wcq2011.09.23
	float fYConst;//计算y坐标时不变的部分
	float fYCoeff;//计算y坐标时系数
	float width;//曲线绘制区域宽度单位：px
	unsigned int nStartPoint = pData->StartPoint;
	unsigned int nMaxPoint = pData->MaxPoint;

	if(g_sorFileArray.waveConfig.MultiMode && !pData->IsMaxLength)/*不是最长曲线且是多迹线显示*/
	{
		if(0 == m_iHorizonZoomNum) 
		{
			//宽度计算 w=( (当前曲线长度km/最大曲线长度km)*绘图区域宽度px )*(绘图点数/总共点数)
			width = ( pData->Length / pData->MaxTestDistance ) * m_rectCoord.Width();
		}
		else
		{
			//根据距离原点一样 wcq2012.02.22
			width=( ((pData->MaxPoint - pData->StartPoint) * pData->DataSpacing )/ ( (m_nMaxPoint - m_nStartPoint) * pData->DataSpacing ) ) * m_rectCoord.Width();//
		}
	}
	else// if (pData->IsMaxLength)//距离最长曲线,或者单迹线显示
	{
		width = m_rectCoord.Width();
	}
	int left = 0,top = 0,right,bottom;//左上
	right = m_rectCoord.Width();
	bottom = m_rectCoord.Height();
	//fYConst=bottom - ((-m_fStartDB)/(m_fMaxDB - m_fStartDB)) * m_rectCoord.Height() - pData->YOffset;//计算y坐标偏距
	//转化为绘图像素坐标
	for(i = pData->StartPoint; i < pData->MaxPoint; i++)
	{
		pData->ptDrawData[i].x = int(m_rectCoord.left + width * (i - pData->StartPoint)/(pData->MaxPoint - pData->StartPoint));
		pData->ptDrawData[i].y = int(m_rectCoord.bottom - pData->YOffset - (((-pData->pDataPoints[i]/1000.0) - m_fStartDB)/(m_fMaxDB - m_fStartDB)) * m_rectCoord.Height());

		if(pData->ptDrawData[i].x > m_rectCoord.right)
			pData->ptDrawData[i].x = m_rectCoord.right;
		if(pData->ptDrawData[i].y < m_rectCoord.top)
			pData->ptDrawData[i].y = m_rectCoord.top;
		if(pData->ptDrawData[i].y > m_rectCoord.bottom)
			pData->ptDrawData[i].y = m_rectCoord.bottom; 
	}

	TRACE(_T("Dis:%d\n"),pData->MaxPoint-pData->StartPoint);
	//画曲线
	pDC->SelectObject (&penCurve);
	pDC->Polyline(&pData->ptDrawData[pData->StartPoint], pData->MaxPoint - pData->StartPoint);

	//画事件点
	if (g_sorFileArray.totalCurveNumber-1==SingleorMulti)
	{
		pDC->SelectObject(&brushEventPoint);
		int nEventNum = pData->EventList.vEventInfo.size();
		for(i=0;i<nEventNum;i++)
		{
			CPen m_penBack;
			m_penBack.CreatePen (PS_SOLID,0,CLR_YELLOW);
			CPen *pOldPen = pDC->SelectObject (&m_penBack);
			EventInfo eventInfo = pData->EventList.vEventInfo[i];
			CRect rectLineBottom;//rectLineBottom:直线下方的矩形wcq2011.11.16
			CString temp;
			pDC->SetBkMode(TRANSPARENT);
			if(eventInfo.index<m_nMaxPoint && eventInfo.index>=m_nStartPoint)
			{
				int nLocaltion = eventInfo.index;

				//竖直细线
				pDC->MoveTo(pData->ptDrawData[nLocaltion].x,pData->ptDrawData[nLocaltion].y-Y_StartEndPoint);
				pDC->LineTo(pData->ptDrawData[nLocaltion].x,pData->ptDrawData[nLocaltion].y+Y_StartEndPoint);

				//下方矩形
				rectLineBottom.SetRect(pData->ptDrawData[nLocaltion].x-X_StartEndPoint,pData->ptDrawData[nLocaltion].y+Y_StartEndPoint,pData->ptDrawData[nLocaltion].x-X_StartEndPoint + 18,pData->ptDrawData[nLocaltion].y+Y_StartEndPoint + 18);

				pDC->FillSolidRect(&rectLineBottom,CLR_YELLOW);

				if (i>8)//两位数
				{
					rectLineBottom.right += 2;
				}
				pDC->Rectangle(rectLineBottom);//后面矩形框
				//rectLineBottom.left -=6;
				//rectLineBottom.right +=6;
				//rectTemp.OffsetRect(2,0);//获得居中效果
				temp.Format(_T("%d"),i + 1);
				pDC->DrawText(temp,&rectLineBottom,DT_CENTER | DT_VCENTER | DT_SINGLELINE);//矩形框中的数值

				pDC->SelectObject (pOldPen);
				m_penBack.DeleteObject();
			}
		}
	}

	//恢复
	pDC->RestoreDC(nSaveDC);
	//释放资源
	penCurve.DeleteObject();
	brushEventPoint.DeleteObject();
}

////wcq2011.09.06得到所曲线中最长的距离,以km为单位
float CSorDrawStatic::GetMaxDistance()
{
	//暂时只考虑一条曲线，如果有两条以上要做相应修改
	DWORD dwWave = _wtoi(_T("1310"));
	TestResultInfo* pCurveData = g_sorFileArray.sorFileList[m_nMaxIndex].pTestResultInfo;

	float fResult=0;
	fResult=Get_XCoord(pCurveData->DataPointsNum, pCurveData);//>=Get_XCoordForFile(g_nMaxPointFromFile)
	return fResult;
}
//////////////////////////////////////////////////////////////////////////////////////////////
//入口参数:curveWidth:当前曲线画图时曲线在水平方向上的宽度,当为最长曲线时，单位：象素;yOffset：y轴方向上的偏移；yMin:y轴坐标最小值;yMax:y轴坐标最大值;
//			rectCurve画图区域；pData:数据指针，不一定从0开始，传递时可加偏移；dataCount：数据点个数
//出口参数:
//建立时间:2011-09-05
//Write By:wcq
//函数说明:计算参数wcq 2011.09.6
//算法：x位置=x基+(x-xMin)*宽度/(xMax-xMin)
//      y位置=y基+(y-yMin)*高度/(yMax-yMin)
//      根据软件需要，作了一些转化：x基=0,即水平方向不移动，xMin=0,即从原点开始,不偏移
//////////////////////////////////////////////////////////////////////////////////////////////
void CSorDrawStatic::CalDrawCurveParams(int curveWidth,CRect rectCurve,int dataCount)
{

}

void CSorDrawStatic::DrawCoord(CDC *pDC)
{
	//填充背景
	pDC->FillSolidRect(&m_rectCoord,g_sorFileArray.waveConfig.ColorBack);  //坐标轴内的矩形区域

	TestResultInfo* pCurveData = GetMaxLengthCurveData();
	int nSaveDC = pDC->SaveDC();

	CPen penGridLine,penCoord;
	penCoord.CreatePen (PS_SOLID/*实线*/,1, g_sorFileArray.waveConfig.ColorCursor);
	penGridLine.CreatePen (PS_DOT/*虚线*/,1, g_sorFileArray.waveConfig.ColorGrid);
	pDC->SelectObject (&penCoord);
	pDC->SelectObject(GetStockObject(NULL_BRUSH));
	if (!m_RectSelected.IsRectEmpty()) 
	{
		pDC->Rectangle(&m_RectSelected);
	}

	pDC->SelectObject (&penGridLine);

	if (g_sorFileArray.waveConfig.ShowGrid)
	{
		//横坐标刻度
		int ndx = m_rectCoord.Width () / COOR_SPLIT_COUNT_H;
		int ndy = m_rectCoord.Height () / COOR_SPLIT_COUNT_V;
		pDC->SelectObject (&penGridLine);
		for ( int i = 1; i< COOR_SPLIT_COUNT_H; i++)
		{	
			//横坐标刻度
			pDC->MoveTo(m_rectCoord.left + ndx * i,m_rectCoord.bottom );
			pDC->LineTo(m_rectCoord.left + ndx * i,m_rectCoord.top );
		}

		//纵坐标刻度
		pDC->SetTextAlign(TA_LEFT | TA_TOP);
		for (int i=1; i<COOR_SPLIT_COUNT_V; i++)
		{
			pDC->MoveTo(m_rectCoord.left , m_rectCoord.bottom - ndy * i);
			pDC->LineTo(m_rectCoord.right, m_rectCoord.bottom - ndy * i);
		}
	}

	//恢复
	pDC->RestoreDC(nSaveDC);
	//释放资源
	penGridLine.DeleteObject();
	penCoord.DeleteObject();
}

/*
***************************************************************
函数名称:             Get_Xcoord()                                                                               
函数说明:             将采集点的序号转换成曲线的横坐标点                                                                              
入口参数:             xpoint：数据点的序号                                                                                             
出口参数:             X：转换后的横坐标点   单位  m                                                                                          
建立时间:             2010-06-24                                                                  
Write By:             XJF                                                                                          
***************************************************************
*/
float CSorDrawStatic::Get_XCoord(int xpoint)
{
	//1.4685是光纤折射率的缺省值，测试用

	float X=(float)(C_zll/(2*m_fFrequency*m_fRefraction * 1000000/*MHz*/)*xpoint);
	return X;
}

/*
***************************************************************
函数名称:             Get_Xcoord()                                                                               
函数说明:             将采集点的序号转换成曲线的横坐标点                                                                              
入口参数:             xpoint：数据点的序号                                                                                             
出口参数:             X：转换后的横坐标点   单位  km                                                                                          
建立时间:             2010-06-24                                                                  
Write By:             XJF                                                                                          
***************************************************************
*/
float CSorDrawStatic::Get_XCoord(unsigned int xpoint,TestResultInfo* pData)
{
	//return xpoint * pData->DataSpacing * 1.0/100000000;//wcq2011.11.25
	//return C/(5000 * pData->Refraction * 1000000/*MHz*/) * xpoint
	return C_zll/( pData->Refraction)/pData->Frequency/1000000/2* xpoint;//2012.8.3 rzg修改公式 /1000表示换算为以Km为单位
}
//计算距离,以c/2nf公式计算
float CSorDrawStatic::Get_XCoord2nf(unsigned int xpoint,TestResultInfo* pData)
{
	return C_zll/(2*pData->Frequency * pData->Refraction * 1000000/*MHz*/) * xpoint;
}
//已知距离反求数组下标
UINT CSorDrawStatic::Get_PosPoint(float distance,TestResultInfo* pData)
{
	return distance/(C_zll/(2*pData->Frequency * pData->Refraction * 1000000/*MHz*/));
}

/*
***************************************************************
函数名称:             CalculDispRect()                                                                               
函数说明:             划分曲线显示区，XY轴信息显示区，光纤信息显示区的矩形                                            
入口参数:             bDisplayEventList,是否需要显示事件列表                                                          
出口参数:             无                                                                                             
建立时间:             2011-07-23                                                                
Write By:             XJF                                                                                           
***************************************************************
*/
void CSorDrawStatic::CalCulDispRect()
{
	//曲线显示区
	GetClientRect(m_rectCurve);
	//坐标显示区
	m_rectCoord.left=m_rectCurve.left;
	m_rectCoord.right=m_rectCurve.right;//wcq2012.02.14
	m_rectCoord.top=m_rectCurve.top;//wcq2011.12.09
	m_rectCoord.bottom=m_rectCurve.bottom;
}

/*
***************************************************************
函数名称:             CurveHorizonZoomIn                                                                              
函数说明:             水平放大                                                                          
入口参数:             无                                                                                      
出口参数:             无                                                                                        
建立时间:             2011-07-25                                                                  
Write By:             XJF                                                                                          
***************************************************************
*/
void CSorDrawStatic::CurveHorizonZoomIn()
{
	if (m_iHorizonZoomNum < m_nZoomCount)
	{
		m_iHorizonZoomNum++;
		HorizonZooms();
		InvalidateAll();
	}
}

/*
***************************************************************
函数名称:             CurveHorizonZoomOut                                                                              
函数说明:             水平缩小                                                                          
入口参数:             无                                                                                      
出口参数:             无                                                                                        
建立时间:             2011-07-25                                                                  
Write By:             XJF                                                                                          
***************************************************************
*/
void CSorDrawStatic::CurveHorizonZoomOut()
{
	if (m_iHorizonZoomNum>0)
	{
		m_iHorizonZoomNum--;
		HorizonZooms();//wcq2011.09.13
		InvalidateAll();//wcq2011.11.16
	}
}
//水平缩放所有曲线wcq 2011.09.13
void CSorDrawStatic::HorizonZooms()
{
	if (g_sorFileArray.totalCurveNumber == 0) //无曲线选择时
		return;
	
	if (!g_sorFileArray.waveConfig.MultiMode)
	{
		TestResultInfo* pCurve = g_sorFileArray.sorFileList[g_sorFileArray.curSelection].pTestResultInfo;
		HorizonZoom(pCurve);
	}
	else
	{
		//依次计算各条曲线起始点结束点
		for(int nCount = 0; nCount < g_sorFileArray.totalCurveNumber; nCount++)
		{
			TestResultInfo* pCurve = g_sorFileArray.sorFileList[nCount].pTestResultInfo;
			if (pCurve && pCurve->DataPointsNum > NUM_VALIDE_LEAST)
			{
				HorizonZoom(pCurve);
			}
		}
	}

	//重新设置绘图区域大小
	if (m_iHorizonZoomNum == 0) //无缩放
	{
		m_nDrawWidth = m_rectCurve.Width();
	}
	else
	{
		m_nDrawWidth = m_rectCurve.Width() * (m_iHorizonZoomNum+1);
	}
	//设置水平滚动条
	SetHorScroll();
}
//缩放一条曲线wcq 2011.09.21
void CSorDrawStatic::HorizonZoom(TestResultInfo* pData)
{
	if(CLEAR_DATAPOINTS_NUM == g_sorFileArray.sorFileList[g_sorFileArray.curSelection].pTestResultInfo->DataPointsNum)//刚开始测试时清空，则不缩放.wcq2011.12.23
	{
		return;
	}

	int m_nPointLabel; //m_nSelectCursor:Seleced标志,1选中A，2选中B, 3选中AB

	if (CURSOR_SELECT::CURSOR_A == m_nSelectCursor) //选中A
	{
		m_nPointLabel=m_nPointA;
	}	
	else if (CURSOR_SELECT::CURSOR_B == m_nSelectCursor)
	{
		m_nPointLabel=m_nPointB;
	}
	else//AB
	{
		m_nPointLabel=(m_nPointA + m_nPointB)/2;
	}//eif

	float fLeftRightCoeff=m_nPointLabel * 1.0/g_sorFileArray.sorFileList[m_nMaxIndex].pTestResultInfo->DataPointsNum;//缩放时左右比例，左右不一
	//如果不是最长距离的曲线且多波形显示模式，则根据最长距离的曲线的A/B标位置进行转化
	if (!pData->IsMaxLength && g_sorFileArray.waveConfig.MultiMode)
	{
		float fTemp=pData->DataSpacing / g_sorFileArray.sorFileList[m_nMaxIndex].pTestResultInfo->DataSpacing;//根据两曲线到原点距离相等。wcq2011.12.01
		m_nPointLabel=m_nPointLabel * fTemp;
	}

	if(0 == m_iHorizonZoomNum) //未缩放
	{
		pData->StartPoint=0;
		pData->MaxPoint=pData->DataPointsNum;
	}
	else
	{
		//原来的cwcq2012.10.26
		//////float ZoomCoeff;
		//////ZoomCoeff=(float)pow(2.0,m_iHorizonZoomNum+1);//放大倍数的1/2
		//////if (m_nPointLabel >= pData->MaxPoint/ZoomCoeff)
		//////{
		//////	pData->StartPoint=(unsigned int)(m_nPointLabel - pData->MaxPoint/ZoomCoeff+1);
		//////	pData->MaxPoint=(unsigned int)(m_nPointLabel+pData->MaxPoint/ZoomCoeff);
		//////}				  
		//////else 
		//////{
		//////	pData->StartPoint=0;
		//////	pData->MaxPoint=(unsigned int)(2*pData->MaxPoint/ZoomCoeff);
		//////}
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////
		int multi=1;//多少倍曲线宽度
		int pointCount=0;
		if (m_iHorizonZoomNum ==m_nZoomCount)//最大缩放倍数
		{
			//pointCount=pData->m_TestResult.LastZoomPointCount;
			pointCount=pData->FullPointCountMin;
		}
		else
		{
			pointCount=pData->FullPointCountMin;//wcq2011.11.22
			int count= m_nZoomCount - m_iHorizonZoomNum;//最后一次缩放点数不一样
			double dPow=pow(2.0,m_iHorizonZoomNum);
			pointCount =pData->FullPointCountMin/dPow;
			//if (count < 20)//40++
			//{
			//	pointCount += count * pData->FullPointCountMin;
			//}
			//else if (count < 30)//30~39
			//{
			//	pointCount += ( 20 + 2 * (count - 19) ) * pData->FullPointCountMin;
			//}
			//else if (count < 40)//20~29
			//{
			//	pointCount +=( 40 + 3 * (count - 29) ) * pData->FullPointCountMin;
			//}
			//else// if (count < 50)//30~39
			//{
			//	pointCount +=( 70 + 4 * (count - 39) ) * pData->FullPointCountMin;
			//}
		}
		int itemp=pointCount * fLeftRightCoeff;//
		if (itemp > m_nPointLabel)
		{
			pData->StartPoint=0;
			pData->MaxPoint = m_nPointLabel + ( pointCount - itemp)-1;
		}
		else
		{
			pData->StartPoint = m_nPointLabel - itemp;//左边位置(起始点位置)
			pData->MaxPoint = m_nPointLabel + ( pointCount - itemp)-1;//右边位置(结束点位置),1:减去缩放中心点本身
		}

		if (pData->StartPoint > pData->DataPointsNum)
		{
			pData->StartPoint = pData->DataPointsNum-1;//wcq2011.11.29
		}
		if (pData->MaxPoint > pData->DataPointsNum)
		{
			pData->MaxPoint = pData->DataPointsNum;
		}		
		else if (pData->MaxPoint <0)
		{
			pData->MaxPoint = 0;
		}
		pData->FullPointCount=pointCount;//wcq2011.11.29
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	}//eif
	if (pData->StartPoint >= pData->MaxPoint)
	{
		pData->MaxPoint = pData->StartPoint + 1; //防止被0除
	}
	if (pData->IsMaxLength || (!g_sorFileArray.waveConfig.MultiMode))//但波形显示
	{
		m_nStartPoint = pData->StartPoint;
		m_nMaxPoint = pData->MaxPoint;
	}
}

//平移一条曲线
void CSorDrawStatic::CurveHorMove(TestResultInfo* pData, int nRange, float fHorBalFactor)
{
	if(CLEAR_DATAPOINTS_NUM == pData->DataPointsNum)
	{
		return;
	}

	if(0 == m_iHorizonZoomNum) //未缩放
	{
		pData->StartPoint=0;
		pData->MaxPoint=pData->DataPointsNum;
	}
	else
	{
		int nDrawPointNumber = pData->DataPointsNum/pow(2.0,m_iHorizonZoomNum-1);
		//计算起始位置
		//pData->StartPoint= (fHorBalFactor * pData->DataPointsNum/nRange) * nDrawPointNumber;
		pData->StartPoint= pData->DataPointsNum * fHorBalFactor-nDrawPointNumber;
		if (pData->StartPoint<0)
		{
			pData->StartPoint=0;
		}
		pData->MaxPoint = pData->StartPoint + nDrawPointNumber;

		TRACE(_T("DataPointsNum:%d"),pData->DataPointsNum);
		TRACE(_T("StartPoint:%d"),pData->StartPoint);
		TRACE(_T("MaxPoint:%d"),pData->MaxPoint);


		if (pData->StartPoint > pData->DataPointsNum)
		{
			pData->StartPoint = pData->DataPointsNum-1;//wcq2011.11.29
		}
		if (pData->MaxPoint > pData->DataPointsNum)
		{
			pData->MaxPoint = pData->DataPointsNum;
		}		
		else if (pData->MaxPoint <0)
		{
			pData->MaxPoint = 0;
		}
		//pData->FullPointCount=pointCount;
	}//eif

	//重新设置横坐标文本
	//MAIN->m_pWaveFormView->SetHorUnitText();
}

/*
***************************************************************
函数名称:             CurveVerticalZoomIn                                                                              
函数说明:             垂直方向放大                                                                          
入口参数:             无                                                                                      
出口参数:             无                                                                                        
建立时间:             2011-07-25                                                                  
Write By:             XJF                                                                                          
***************************************************************
*/
void CSorDrawStatic::CurveVerticalZoomIn()
{
	if (m_iVerticalZoomNum < ZOOM_VERTICAL_COUNT)
	{
		m_iVerticalZoomNum++;
		VerticalZoom();
		InvalidateRect(m_rectCurve,FALSE);
	}
}
/*
***************************************************************
函数名称:             CurveVerticalZoomOut                                                                              
函数说明:             垂直方向缩小                                                                          
入口参数:             无                                                                                      
出口参数:             无                                                                                        
建立时间:             2011-07-25                                                                  
Write By:             XJF                                                                                          
***************************************************************
*/
void CSorDrawStatic::CurveVerticalZoomOut()
{
	if (m_iVerticalZoomNum > 0)
	{
		m_iVerticalZoomNum--;
		VerticalZoom();
		InvalidateRect(m_rectCurve,FALSE);
	}
}
/*
***************************************************************
函数名称:             VerticalZoom                                                                              
函数说明:             垂直方向变化控制                                                                           
入口参数:             无                                                                                      
出口参数:             无                                                                                        
建立时间:             2011-07-25                                                                  
Write By:             XJF                                                                                          
***************************************************************
*/
void CSorDrawStatic::VerticalZoom()
{
	if (g_sorFileArray.totalCurveNumber == 0) //没有曲线时
		return;

	int m_nPointLabel; //m_nSelectCursor:Seleced标志,1选中A，2选中B, 3选中AB
	//确定选中的是A/B
	if (CURSOR_SELECT::CURSOR_A == m_nSelectCursor) //选中A
	{
		m_nPointLabel=m_nPointA;
	}
	else if (CURSOR_SELECT::CURSOR_B == m_nSelectCursor)
	{
		m_nPointLabel=m_nPointB;
	}
	else//AB
	{
		m_nPointLabel=(m_nPointA + m_nPointB)/2;
	}//eif

	//防止越界
	if (m_nPointLabel<0)
	{
		m_nPointLabel=0;
	}//eif
	if (m_nPointLabel>g_sorFileArray.sorFileList[m_nMaxIndex].pTestResultInfo->DataPointsNum)
	{
		m_nPointLabel=g_sorFileArray.sorFileList[m_nMaxIndex].pTestResultInfo->DataPointsNum;
	}//eif
	//
	float fPointLabelDb;
	//fPointLabelDb=m_pMapCurveData[m_curABWave]->m_TestResult.pDataPoints[m_nPointLabel];//???????????
	fPointLabelDb=g_sorFileArray.sorFileList[m_nMaxIndex].pTestResultInfo->pDataPoints[m_nPointLabel]/(-1000.0);//?????是否还要除-1000

	CalVerticalRange(m_iVerticalZoomNum, fPointLabelDb);//计算竖直方向缩放时最大值最小值.赋给m_fStartDB,m_fMaxDB.wcq2011.09.28
	
	//重新设置绘图区域大小
	if (m_iVerticalZoomNum == 0) //无缩放
	{
		m_fCoeff = 1.0;
		m_nDrawHeight = m_rectCurve.Height();
	}
	else
	{
		m_nDrawHeight = m_rectCurve.Height()/m_fCoeff;
	}
	SetVerScoll();
}

/*
***************************************************************
函数名称:             OnButtonMoveLeft                                                                              
函数说明:             被选中的标杆左移                                                 
入口参数:             无                                                                                      
出口参数:             无                                                                                        
建立时间:             2010-06-28                                                                 
Write By:             XJF                                                                                          
***************************************************************
*/
void CSorDrawStatic::CursorMoveLeft()
{
	int moveFactor =1;//每次移动几象素//wcq2011.12.26
	//if (m_pMainDlg->checkLongClick(VK_LEFT))//wcq2011.12.26
	//{
	//	moveFactor = WIDTH_CURSOR_MOVE_LONG;
	//	//DEBUGMSG(TRUE,(_T("CursorMoveRight   is LongClick\n")));
	//}
	long l=GetTickCount();
	// TODO: 在此添加控件通知处理程序代码
	if (m_nSelectCursor == CURSOR_SELECT::CURSOR_A) //选中了A
	{
		//m_nPointA=m_nPointA - 5*(m_nMaxPoint-m_nStartPoint)/m_rectCoord.Width();//wcq2011.11.16
		//m_nPointA=m_nPointA - 1*(m_nMaxPoint-m_nStartPoint + 1)/m_rectCoord.Width();//wcq2011.11.24
		m_nPointA=m_nPointA - moveFactor*(m_nMaxPoint-m_nStartPoint + 1)/m_rectCoord.Width();////wcq2011.12.26
		//如果移动距离为0了，那么就每次移动一个点。LDL2010-7-22加入
		//if (((m_nMaxPoint-m_nStartPoint)/m_rectCoord.Width())==0)//wcq2011.11.24
		//{
		//	m_nPointA=m_nPointA+1;
		//}
		if (m_nMaxPoint-m_nStartPoint < m_rectCoord.Width())
		{
			m_nPointA --;
		}
		if (m_nPointA <0)//wcq2011.11.24
		{
			m_nPointA = 0;
		}
		if (m_nPointA>=m_nMaxPoint)////改为下面的wcq2011.11.24
		{
			m_nPointA=m_nMaxPoint-1;
		}
		//if (m_nPointA>=m_pMapCurveData[m_curABWave].m_TestResult.DataPointsNum)//wcq2011.11.24
		//{
		//	m_nPointA=m_pMapCurveData[m_curABWave].m_TestResult.DataPointsNum -1;
		//}
		//InvalidateRect(m_rectCurve);//放到后面wcq2011.11.16
		//移动时保证A要在B的左边
		if (m_nPointA >= m_nPointB)
			m_nPointB = m_nPointA;
		if (m_nPointA < m_nStartPoint)//移到当前屏幕外面，则重新缩放//wcq2011.11.24
		{
			HorizonZooms();
		}
	}
	else if(m_nSelectCursor == CURSOR_SELECT::CURSOR_B) //选中了B
	{
		//m_nPointB=m_nPointB - 5*(m_nMaxPoint-m_nStartPoint)/m_rectCoord.Width();//wcq2011.11.16
		//m_nPointB=m_nPointB - 1*(m_nMaxPoint-m_nStartPoint + 1)/m_rectCoord.Width();//wcq2011.11.24
		m_nPointB=m_nPointB - moveFactor*(m_nMaxPoint-m_nStartPoint + 1)/m_rectCoord.Width();//wcq2011.12.26

		//如果移动距离为0了，那么就每次移动一个点。LDL2010-7-22加入
		if (m_nMaxPoint-m_nStartPoint < m_rectCoord.Width())
		{
			m_nPointB --;
		}

		if (m_nPointB <0)//wcq2011.11.24
		{
			m_nPointB = 0;
		}
		//if ((unsigned int)m_nPointB>=m_nMaxPoint)//改为下面的wcq2011.11.24
		//{
		//	m_nPointB=m_nMaxPoint-1;
		//}
		//if (m_nPointB>=m_pMapCurveData[m_curABWave].m_TestResult.DataPointsNum)//wcq2011.11.24
		//{
		//	m_nPointB=m_pMapCurveData[m_curABWave].m_TestResult.DataPointsNum -1;
		//}
		//InvalidateRect(m_rectCurve);//放到后面wcq2011.11.16
		//移动时保证A要在B的左边
		if (m_nPointA >= m_nPointB)
			m_nPointA = m_nPointB;
		if (m_nPointB < m_nStartPoint)//移到当前屏幕外面，则重新缩放//wcq2011.11.24
		{
			HorizonZooms();
		}
	}
	else if(m_nSelectCursor == CURSOR_SELECT::CURSOR_AB) //选中了AB
	{
		//如果已经移动到右边界，不在移动
		if (m_nPointA >= m_nMaxPoint ||
			m_nPointB >= m_nMaxPoint)
			return;

		//m_nPointA=m_nPointA - 5*(m_nMaxPoint-m_nStartPoint)/m_rectCoord.Width();//wcq2011.11.16
		//m_nPointB=m_nPointB - 5*(m_nMaxPoint-m_nStartPoint)/m_rectCoord.Width();//wcq2011.11.16
		//m_nPointA=m_nPointA - 1*(m_nMaxPoint-m_nStartPoint + 1)/m_rectCoord.Width();//wcq2011.11.24
		//m_nPointB=m_nPointB - 1*(m_nMaxPoint-m_nStartPoint + 1)/m_rectCoord.Width();//wcq2011.11.24
		m_nPointA=m_nPointA - moveFactor*(m_nMaxPoint-m_nStartPoint + 1)/m_rectCoord.Width();//wcq2011.12.26
		m_nPointB=m_nPointB - moveFactor*(m_nMaxPoint-m_nStartPoint + 1)/m_rectCoord.Width();//wcq2011.12.26
		//如果移动距离为0了，那么就每次移动一个点。LDL2010-7-22加入
		/*if (((m_nMaxPoint-m_nStartPoint)/m_rectCoord.Width())==0)//wcq2012.02.22
		{
		m_nPointA=m_nPointA+1;
		m_nPointB=m_nPointB+1;
		}*/
		if (m_nMaxPoint-m_nStartPoint < m_rectCoord.Width())//wcq2012.02.22
		{
			m_nPointA --;
			m_nPointB --;
		}
		if (m_nPointA <0)//wcq2011.11.24
		{
			m_nPointA = 0;
		}
		if (m_nPointB <0)//wcq2011.11.24
		{
			m_nPointB = 0;
		}
		//if (m_nPointA>=m_pMapCurveData[m_curABWave].m_TestResult.DataPointsNum)//wcq2011.11.24
		//{
		//	m_nPointA=m_pMapCurveData[m_curABWave].m_TestResult.DataPointsNum -1;
		//}
		//if (m_nPointB>=m_pMapCurveData[m_curABWave].m_TestResult.DataPointsNum)//wcq2011.11.24
		//{
		//	m_nPointB=m_pMapCurveData[m_curABWave].m_TestResult.DataPointsNum -1;
		//}
		int nCenterAB = (m_nPointB - m_nPointA)/2;
		if (nCenterAB < m_nStartPoint)//中心移到当前屏幕外面，则重新缩放//wcq2011.11.24
		{
			HorizonZooms();
		}
		//InvalidateRect(m_rectCurve);//放到后面wcq2011.11.16
	}
	else if(m_bEventCursor) //事件列表
	{
		if (m_nSelectIndex > 0)
			m_nSelectIndex --;
		else 
			m_nSelectIndex = 0;

		////if (m_nSelectIndex < m_iEventNum)//??????????????
		////	m_nPointEvent= m_CArrayEventInfoFromNode[m_nSelectIndex].nLocation;
		////else
		////	m_nPointEvent = 0;

		EventIsSelected(m_nPointEvent);
		//InvalidateRect(m_rectCurve);//放到后面wcq2011.11.16
	}
	//InvalidateAll();//wcq2011.11.16
	InvalidateRect(m_rectSubInfo,FALSE);//wcq2012.03.08
	InvalidateRect(m_rectCurve,FALSE);//曲线
	//DEBUGMSG(true,(_T("CursorMoveLeft time:%d\n"),GetTickCount()-l));
	//InvalidateRect(m_rectSubInfo);////wcq2011.12.22
	//以下为以前的.wcq2011.09.23
	//if (m_bReceiveResultOK)
	//{
	//	if (m_nSelectCursor == 1) //选中了A
	//	{
	//		m_nPointA=m_nPointA-5*(m_nMaxPoint-m_nStartPoint)/m_rectCoord.Width();

	//		//如果移动距离为0了，那么就每次移动一个点。LDL2010-7-22加入
	//		if (((m_nMaxPoint-m_nStartPoint)/m_rectCoord.Width())==0)
	//		{
	//			m_nPointA=m_nPointA-1;
	//		}

	//		if ((int)(m_nPointA-m_nStartPoint)<=0)
	//		{
	//			m_nPointA=m_nStartPoint;
	//		}
	//		InvalidateRect(m_rectCurve);
	//		//移动时保证A要在B的左边
	//		if (m_nPointA > m_nPointB)
	//			m_nPointB = m_nPointA;
	//	}
	//	else if(m_nSelectCursor == 2) //选中B
	//	{
	//		m_nPointB=m_nPointB-5*(m_nMaxPoint-m_nStartPoint)/m_rectCoord.Width();

	//		//如果移动距离为0了，那么就每次移动一个点。LDL2010-7-22加入
	//		if (((m_nMaxPoint-m_nStartPoint)/m_rectCoord.Width())==0)
	//		{
	//			m_nPointB=m_nPointB-1;
	//		}

	//		if ((int)(m_nPointB-m_nStartPoint)<=0)
	//		{
	//			m_nPointB=m_nStartPoint;
	//		}
	//		InvalidateRect(m_rectCurve);
	//		//移动时保证A要在B的左边
	//		if (m_nPointA > m_nPointB)
	//			m_nPointA = m_nPointB;
	//	}
	//	else if(m_nSelectCursor == 3) //选中AB
	//	{
	//		//如果已经移动到左边界，不能向左移动
	//		if ((int)(m_nPointA-m_nStartPoint) <= 0 ||
	//			(int)(m_nPointB-m_nStartPoint) <= 0)
	//		{
	//			return;
	//		}

	//		m_nPointA=m_nPointA-5*(m_nMaxPoint-m_nStartPoint)/m_rectCoord.Width();
	//		m_nPointB=m_nPointB-5*(m_nMaxPoint-m_nStartPoint)/m_rectCoord.Width();

	//		//如果移动距离为0了，那么就每次移动一个点。LDL2010-7-22加入
	//		if (((m_nMaxPoint-m_nStartPoint)/m_rectCoord.Width())==0)
	//		{
	//			m_nPointA=m_nPointA-1;
	//			m_nPointB=m_nPointB-1;
	//		}
	//		InvalidateRect(m_rectCurve);
	//	}
	//	else if(m_bEventCursor) //事件列表
	//	{
	//		if (m_nSelectIndex > 0)
	//			m_nSelectIndex --;
	//		else 
	//			m_nSelectIndex = m_iEventNum;

	//		if (m_nSelectIndex < m_iEventNum)
	//			m_nPointEvent= m_CArrayEventInfoFromNode[m_nSelectIndex].nLocation;
	//		else
	//			m_nPointEvent = 0;

	//		EventIsSelected(m_nPointEvent);
	//		InvalidateRect(m_rectCurve);
	//	}
	//}

	//if (m_bReadFromFile)
	//{
	//	if (m_nSelectCursor == 1) //选中了A
	//	{
	//		m_nPointA=m_nPointA-5*(m_nMaxPointFromFile-m_nStartPointFromFile)/m_rectCoord.Width();

	//		//如果移动距离为0了，那么就每次移动一个点。LDL2010-7-22加入
	//		if (((m_nMaxPointFromFile-m_nStartPointFromFile)/m_rectCoord.Width())==0)
	//		{
	//			m_nPointA=m_nPointA-1;
	//		}

	//		if ((int)(m_nPointA-m_nStartPointFromFile)<=0)
	//		{
	//			m_nPointA=m_nStartPointFromFile;
	//		}
	//		InvalidateRect(m_rectCurve);
	//		//移动时保证A要在B的左边
	//		if (m_nPointA > m_nPointB)
	//			m_nPointB = m_nPointA;
	//	}
	//	else if(m_nSelectCursor == 2) //选中B
	//	{
	//		m_nPointB=m_nPointB-5*(m_nMaxPointFromFile-m_nStartPointFromFile)/m_rectCoord.Width();

	//		//如果移动距离为0了，那么就每次移动一个点。LDL2010-7-22加入
	//		if (((m_nMaxPointFromFile-m_nStartPointFromFile)/m_rectCoord.Width())==0)
	//		{
	//			m_nPointB=m_nPointB-1;
	//		}

	//		if ((int)(m_nPointB-m_nStartPointFromFile)<=0)
	//		{
	//			m_nPointB=m_nStartPointFromFile;
	//		}
	//		InvalidateRect(m_rectCurve);
	//		//移动时保证A要在B的左边
	//		if (m_nPointA > m_nPointB)
	//			m_nPointA = m_nPointB;
	//	}
	//	else if(m_nSelectCursor == 3) //选中AB
	//	{
	//		//如果已经移动到左边界，不能向左移动
	//		if ((int)(m_nPointA-m_nStartPointFromFile) <= 0 ||
	//			(int)(m_nPointB-m_nStartPointFromFile) <= 0)
	//		{
	//			return;
	//		}

	//		m_nPointA=m_nPointA-5*(m_nMaxPointFromFile-m_nStartPointFromFile)/m_rectCoord.Width();
	//		m_nPointB=m_nPointB-5*(m_nMaxPointFromFile-m_nStartPointFromFile)/m_rectCoord.Width();

	//		//如果移动距离为0了，那么就每次移动一个点。LDL2010-7-22加入
	//		if (((m_nMaxPointFromFile-m_nStartPointFromFile)/m_rectCoord.Width())==0)
	//		{
	//			m_nPointA=m_nPointA-1;
	//			m_nPointB=m_nPointB-1;
	//		}
	//		InvalidateRect(m_rectCurve);
	//	}
	//	else if(m_bEventCursor) //事件列表
	//	{
	//		if (m_nSelectIndex > 0)
	//			m_nSelectIndex --;
	//		else 
	//			m_nSelectIndex = m_iEventNumFromFile;

	//		if (m_nSelectIndex < m_iEventNumFromFile)
	//			m_nPointEvent= m_CArrayEventInfoFromFile[m_nSelectIndex].nLocation;
	//		else
	//			m_nPointEvent = 0;

	//		EventIsSelected(m_nPointEvent);
	//		InvalidateRect(m_rectCurve);
	//	}
	//}
}


/*
***************************************************************
函数名称:             EventIsSelected                                                                              
函数说明:             移动标杆时如果标杆距离某个事件的距离小于其他事件，则这个事件被选中                                              
入口参数:             PointEvent, 事件列表杆的点数                                                                                     
出口参数:             无                                                                                        
建立时间:             2011-07-06                                                              
Write By:             XJF                                                                                          
***************************************************************
*/
void CSorDrawStatic::EventIsSelected(int PointEvent)
{
	//int iPoint=0;
	//int RowNum; //iPointA为标杆Event代表的实际距离,RowNum表示事件表中的事件数目
	//int i,j,k; //i,j控制循环
	//RowNum = m_lstEventList.GetItemCount();

	//////if (m_bISCurveFromNodeSelected)//wcq2012.03.09
	//////{
	//////	iPoint=Get_XCoord(PointEvent);
	//////}
	//////if (m_bISCurveFromFileSelected)
	//////{
	//////	iPoint=Get_XCoordForFile(PointEvent);
	//////}
	//for (i = 0;i< RowNum; i++)
	//{	
	//	for (k = 0;k < RowNum; k++)
	//	{
	//		if (k != i)
	//		{
	//			if (abs(_wtoi(m_lstEventList.GetItemText(i,2)) - iPoint) < abs(_wtoi(m_lstEventList.GetItemText(k,2))-iPoint)) //找出距离标杆最近的事件点i 
	//			{
	//				continue;
	//			}
	//			else
	//			{
	//				break;
	//			}
	//		}

	//		else
	//		{
	//			continue;
	//		}

	//	}
	//	if (k == RowNum)
	//	{
	//		for (j = 0;j < RowNum; j++)
	//		{	
	//			m_lstEventList.SetItemState(j, 0, LVIS_SELECTED|LVIS_FOCUSED); //消除选中状态
	//		}
	//		m_lstEventList.SetItemState(i,LVIS_SELECTED|LVIS_FOCUSED,LVIS_SELECTED|LVIS_FOCUSED); //设为选中状态
	//	}
	//}
}

void CSorDrawStatic::CurvesHorMove(int nRange, float nHorBalFactor)
{
	if (g_sorFileArray.totalCurveNumber == 0)//还没测试
		return;

	//单曲线绘制模式
	if (!g_sorFileArray.waveConfig.MultiMode)
	{
		TestResultInfo* pCurve = g_sorFileArray.sorFileList[g_sorFileArray.curSelection].pTestResultInfo;
		CurveHorMove(pCurve, nRange, nHorBalFactor);
	}
	else
	{
		//多曲线绘制模式
		for(int nCount = 0; nCount < g_sorFileArray.totalCurveNumber; nCount++)
		{
			TestResultInfo* pData = g_sorFileArray.sorFileList[nCount].pTestResultInfo;
			if (!pData || pData->DataPointsNum < NUM_VALIDE_LEAST || CLEAR_DATAPOINTS_NUM == g_sorFileArray.sorFileList[m_nMaxIndex].pTestResultInfo->DataPointsNum)//没有或数据很少wcq2011.09.09 //0 == m_pMapCurveData[m_curABWave]->m_TestResult.DataPointsNum.重测时会清零//wcq2011.12.22
			{
				continue;
			}
			CurveHorMove(pData, nRange, nHorBalFactor);
		}
	}
	MAIN->m_pWaveFormView->SetHorUnitText(nHorBalFactor);
	InvalidateRect(m_rectCurve,FALSE);//wcq2011.09.23
}

/*
***************************************************************
函数名称:             CurveVerMove
入口参数:             无
出口参数:             无
建立时间:             2011-12-14
Write By:             XJF
函数说明:             wcq 曲线上下平移时会平移所有曲线
平移方式：以当前缩略图中的矩形框为准，下边框最上移到缩略图区域的上边缘，
上边框最下移到缩略图区域的下边缘。切换到平移模式时，先记下缩略图中的矩形框
平移时以中间大图曲线往上（下）移动一象素。
***************************************************************
*/
void CSorDrawStatic::CurveVerMove(TestResultInfo* pData, int nSetp, bool bSpin)
{
	CRect rect;
	MAIN->m_pThumbFormView->GetMoveRect(&rect, pData);
	if (bSpin)
		pData->YOffset += nSetp;//通过旋钮调整位置
	else
		pData->YOffset = nSetp;//通过滚动条调整位置
}

void CSorDrawStatic::CurvesVerMove(int nSetp)
{
	if (!g_sorFileArray.waveConfig.MultiMode)
	{
		TestResultInfo* pCurve = g_sorFileArray.sorFileList[g_sorFileArray.curSelection].pTestResultInfo;
		CurveVerMove(pCurve, nSetp);
	}
	else
	{
		//多曲线绘制模式
		for(int nCount = 0; nCount < g_sorFileArray.totalCurveNumber; nCount++)
		{
			TestResultInfo* pCurve = g_sorFileArray.sorFileList[nCount].pTestResultInfo;
			if (pCurve)
			{
				CurveVerMove(pCurve, nSetp);
			}//eif
		}//efor
	}

	InvalidateAll();
}

////从平移到缩放时把竖直方向的平移恢复为0;wcq2011.12.14
void CSorDrawStatic::ResetYOffset()
{
	//计算
	for(int nCount = 0; nCount < g_sorFileArray.totalCurveNumber; nCount++)
	{
		TestResultInfo* pCurve = g_sorFileArray.sorFileList[nCount].pTestResultInfo;
		if (pCurve)//wcq2011.12.23
		{
			pCurve->YOffset = 0;
			continue;
		}
	}
}
/*
***************************************************************
函数名称:             InitialEventListFromFile                                                                              
函数说明:             用文件数据初始化事件表信息                                                
入口参数:             无                                                                                      
出口参数:             无                                                                                        
建立时间:             2010-07-02                                                                
Write By:             XJF                                                                                          
***************************************************************
//*/
//void CSorDrawStatic::ReadEventListFromFile()
//{
//	m_lstEventList.DeleteAllItems();
//	int i;
//	CString str;
//	for (i=0;i<m_CArrayEventInfoFromFile.GetSize();i++)
//	{
//		str.Format(_T("%d"),i+1);
//		m_lstEventList.InsertItem(i,str,1);
//
//		if (m_CArrayEventInfoFromFile[i].nEventType==0)
//		{
//			m_lstEventList.SetItemText(i,1,_T("EventType_0"));//开始事件
//		}
//		else if (m_CArrayEventInfoFromFile[i].nEventType==1)
//		{
//			m_lstEventList.SetItemText(i,1,_T("EventType_0")); //反射事件
//		}
//		else if (m_CArrayEventInfoFromFile[i].nEventType==2)
//		{
//			m_lstEventList.SetItemText(i,1,_T("EventType_1")); //非反射事件
//		}
//		else if (m_CArrayEventInfoFromFile[i].nEventType==3)
//		{
//			m_lstEventList.SetItemText(i,1,_T("EventType_1")); //结束事件
//		}
//
//		str.Format(_T("%.2f"),Get_XCoordForFile(m_CArrayEventInfoFromFile[i].nLocation));
//		m_lstEventList.SetItemText(i,2,str);
//
//		if (m_CArrayEventInfoFromFile[i].fReflectValue==100)
//		{
//			m_lstEventList.SetItemText(i,3,_T("----"));
//		}
//		else
//		{
//			str.Format(_T("%.3f"),m_CArrayEventInfoFromFile[i].fReflectValue);
//			m_lstEventList.SetItemText(i,3,str);
//		}
//
//		if (m_CArrayEventInfoFromFile[i].fInsertloss==100)
//		{
//			m_lstEventList.SetItemText(i,4,_T("----"));
//		}
//		else
//		{
//			str.Format(_T("%.3f"),m_CArrayEventInfoFromFile[i].fInsertloss);
//			m_lstEventList.SetItemText(i,4,str);
//		}
//
//		str.Format(_T("%.3f"),m_CArrayEventInfoFromFile[i].fAttenuCoefficient);
//		m_lstEventList.SetItemText(i,5,str);
//
//		str.Format(_T("%.3f"),m_CArrayEventInfoFromFile[i].fCumulativeLoss);
//		m_lstEventList.SetItemText(i,6,str);
//
//		//////////////////////////////////////////////////////////////////////////
//		//处理开始事件和结束事件
//		if (m_CArrayEventInfoFromFile[i].nEventType==0)
//		{
//			m_lstEventList.SetItemText(i,4,_T("----")); //插入损耗
//		}
//		if (m_CArrayEventInfoFromFile[i].nEventType==3)
//		{
//			m_lstEventList.SetItemText(i,4,_T("----")); //插入损耗
//			m_lstEventList.SetItemText(i,5,_T("----")); //衰减系数
//		}
//		if (m_CArrayEventInfoFromFile[i].nEventType==2)//非反射事件的反射值为空
//		{
//			m_lstEventList.SetItemText(i,3,_T("----")); 
//		}
//
//	}
//}
//
///*
//***************************************************************
//函数名称:             ReadCurveDataFromNode                                                                              
//函数说明:             读取缓存区数据                                                  
//入口参数:             无                                                                                      
//出口参数:             无                                                                                        
//建立时间:             2011-06-28                                                                 
//Write By:             XJF                                                                                          
//***************************************************************
//*/
//void CSorDrawStatic::ReadCurveDataFromNode()
//{
//	// TODO: 在此添加控件通知处理程序代码
//	CString strPath = L"/NandFlash/OTDR/OTDR-data/sor.txt";
//	FILE *fp;
//	double fTemp;
//	char pszFileName[MAX_PATH] = {""};
//	WideCharToMultiByte(CP_OEMCP, 0, strPath, -1, pszFileName, strPath.GetLength() * 2, NULL, NULL);
//	if((fp = fopen(pszFileName,"r")) != NULL)
//	{
//		//写数据到缓冲区
//		if(NULL != m_pRecvbuf)
//		{
//			delete[] m_pRecvbuf;
//		}
//		//从文件中加载默认测量波长是2000
//
//		CCurveData* pCurveData = m_pMapCurveData[2000];
//		if (pCurveData != NULL)
//		{
//			delete pCurveData;
//			pCurveData = NULL;
//		}
//		pCurveData = new CCurveData();
//
//		m_pMapCurveData[2000] = pCurveData;
//		int nDataLen = 0;
//		fscanf(fp,"%d",&nDataLen);
//		pCurveData->DataPointsNum = nDataLen;
//		pCurveData->MaxPoint = nDataLen;
//		pCurveData->IsMaxLength = TRUE;
//		m_fFrequency = 50;
//		m_pRecvbuf = new WORD[nDataLen];
//		WORD wTemp;
//		for (unsigned int i = 0;i < nDataLen; i++)
//		{
//			fscanf(fp,"%d",&wTemp);
//			m_pRecvbuf[i] = wTemp;
//		}
//		pCurveData->pDataPoints = m_pRecvbuf;
//		m_bReadFromFile = TRUE;
//		m_bISCurveFromFileSelected=TRUE;
//		if (m_ptRecord!=NULL)
//		{
//			delete []m_ptRecord;
//		}
//		m_ptRecord = new CPoint[pCurveData->DataPointsNum];//申请空间保存，画图的点
//		fclose(fp);
//	}
//}
/*

***************************************************************
函数名称:             OnClickListEvent                                                                              
函数说明:             点击事件表中的一个事件后，被选中的标杆移动到该事件点                                              
入口参数:             NMHDR* pNMHDR, LRESULT* pResult                                                                                      
出口参数:             无                                                                                        
建立时间:             2011-07-06                                                              
Write By:             XJF                                                                                          
***************************************************************
*/

void CSorDrawStatic::OnClickListEvent(NMHDR* pNMHDR, LRESULT* pResult) 
{
	//CCurveData* pCurveData = m_pMapCurveData[m_dwWaveTesting];
	//// TODO: Add your control notification handler code here
	//POSITION pos = m_lstEventList.GetFirstSelectedItemPosition();
	//int nIndex = m_lstEventList.GetNextSelectedItem(pos);
	//if (nIndex >= 0 && nIndex < m_lstEventList.GetItemCount()-1)
	//{
	//	//m_lstEventList.SetItemState(nIndex, NULL, NULL);
	//	MoveEventList(nIndex);
	//}
	//////int i;
	//////POSITION pos=m_lstEventList.GetFirstSelectedItemPosition();
	//////while(pos)
	//////{
	//////	i=m_lstEventList.GetNextSelectedItem(pos);

	/////////	if (m_bISCurveFromFileSelected) //如果文件曲线被选中//wcq2012.03.09
	/////////	{
	//////		if (m_nSelectCursor == 1) //选中了A杆
	//////		{
	//////			m_nPointA=m_CArrayEventInfoFromFile[i].nLocation;
	//////			if (m_nPointA<g_nMaxPointFromFile)
	//////			{
	//////				HorizonZoom();
	//////				VerticalZoom();
	//////			}

	//////			if (m_nPointA>=g_nMaxPointFromFile)
	//////			{
	//////				m_nPointA=g_nMaxPointFromFile-1;
	//////			}
	//////		}
	//////		if (m_nSelectCursor == 2) //如果选中了B杆
	//////		{
	//////			m_nPointB=m_CArrayEventInfoFromFile[i].nLocation;
	//////			if (m_nPointB<g_nMaxPointFromFile)
	//////			{
	//////				HorizonZoom();
	//////				VerticalZoom();
	//////			}

	//////			if (m_nPointB>=g_nMaxPointFromFile)
	//////			{
	//////				m_nPointB=g_nMaxPointFromFile-1;
	//////			}

	//////		}
	//////		InvalidateRect(&m_rectClient,FALSE);
	//////		//InitialReturnInfo();
	//////	///}//wcq2012.03.09

	//////	////if (m_bISCurveFromNodeSelected)
	//////	////{
	//////	////	if (m_nSelectCursor == 1) //选中了A杆
	//////	////	{
	//////	////		m_nPointA=m_CArrayEventInfoFromNode[i].nLocation;
	//////	////		if (m_nPointA<pCurveData->DataPointsNum)
	//////	////		{
	//////	////			HorizonZoom();
	//////	////			VerticalZoom();
	//////	////		}

	//////	////		if (m_nPointA>=pCurveData->DataPointsNum)
	//////	////		{
	//////	////			m_nPointA=pCurveData->DataPointsNum-1;
	//////	////		}

	//////	////	}
	//////	////	if (m_nSelectCursor == 2) //如果选中了B杆
	//////	////	{
	//////	////		m_nPointB=m_CArrayEventInfoFromNode[i].nLocation;
	//////	////		if (m_nPointB<pCurveData->DataPointsNum)
	//////	////		{
	//////	////			HorizonZoom();
	//////	////			VerticalZoom();
	//////	////		}
	//////	////		if (m_nPointB>=pCurveData->DataPointsNum)
	//////	////		{
	//////	////			m_nPointB=pCurveData->DataPointsNum-1;
	//////	////		}
	//////	////	}
	//////	////	InvalidateRect(&m_rectClient,FALSE);
	//////	////	//InitialReturnInfo();
	//////	////}
	//////}

	*pResult = 0;
}

/*
***************************************************************
函数名称:             InitialReturnInfo                                                                              
函数说明:             初始化返回信息                                              
入口参数:             无                                                                                      
出口参数:             无                                                                                        
建立时间:             2011-07-02                                                                
Write By:             XJF                                                                                          
***************************************************************
*/
//void CSorDrawStatic::InitialReturnInfo()
//{
//	//CString str;
//	////Y
//	//GetDlgItem(IDC_STATIC_X)->SetWindowText(_T("Y: 6dB/div"));
//	////X
//	//GetDlgItem(IDC_STATIC_Y)->SetWindowText(_T("X:"));
//	////波长
//	//CString strTestWave;
//	//strTestWave.LoadString(m_pMainDlg->GetCurLangHandle(), IDS_TEST_WAVE);
//	//str.Format(_T("%s%d nm"),strTestWave, m_iWaveLengthFromFile);
//	//GetDlgItem(IDC_STATIC_WAVE)->SetWindowText(str);
//	////测试时间
//	//CString strTestTime;
//	//strTestTime.LoadString(m_pMainDlg->GetCurLangHandle(), IDS_TEST_TIME);
//	//str.Format(_T("%s%d s"),strTestTime, m_iMeasureTimeFromFile);
//	//GetDlgItem(IDC_STATIC_TIME)->SetWindowText(str);
//	////脉宽
//	//CString strPulseWidth;
//	//strPulseWidth.LoadString(m_pMainDlg->GetCurLangHandle(), IDS_PULSE_WIDTH);
//	//str.Format(_T("%s%d ns"),strPulseWidth, m_iPulseWidthFromFile);
//	//GetDlgItem(IDC_STATIC_PULSE_WIDTH)->SetWindowText(str);
//	////光纤折射率
//	//CString strReflectConff;
//	//strReflectConff.LoadString(m_pMainDlg->GetCurLangHandle(), IDS_REFLECTION);
//	//str.Format(_T("%s%d ns"),strReflectConff, m_fRefractionFromFile);
//	//GetDlgItem(IDC_STATIC_REFLECT_COFF)->SetWindowText(str);
//	////光标A点位置
//	//int nDistanceA=Get_XCoordForFile(m_nPointA);
//	//str.Format(_T("A: %dm"),nDistanceA);
//	//GetDlgItem(IDC_STATIC_COORD_A)->SetWindowText(str);
//	////光标B位置
//	//int nDistanceB=Get_XCoordForFile(m_nPointB);
//	//str.Format(_T("B: %dm"),nDistanceB);
//	//GetDlgItem(IDC_STATIC_COORD_B)->SetWindowText(str);
//	////光标B-A距离
//	//str.Format(_T("B-A: %dm"),nDistanceB - nDistanceA);
//	//GetDlgItem(IDC_STATIC_AB_DISTANCE)->SetWindowText(str);
//}

/*
***************************************************************
函数名称:             CalculABParaFromNode                                                                              
函数说明:             计算网络曲线AB段参数                                              
入口参数:             无                                                                                      
出口参数:             无                                                                                        
建立时间:             2011-07-02                                                                
Write By:             XJF                                                                                          
***************************************************************
//*/
//void CSorDrawStatic::CalculABParaFromNode()
//{
//	CCurveData* pCurveData = m_pMapCurveData[m_dwWaveTesting];
//
//	int RealPointA; // A点实际点数
//	int RealPointB; //B点实际点数
//	if (m_bReceiveResultOK && m_bReadFromFile)
//	{
//		if (Get_XCoord(pCurveData->DataPointsNum)>Get_XCoordForFile(g_nMaxPointFromFile))
//		{
//			m_iABSegmentLen=(long int)(fabs(Get_XCoord(m_nPointA)-Get_XCoord(m_nPointB)));//AB之间的距离
//			m_fABLoss=(float)(fabs(m_CArraySampleLoss[m_nPointA]-m_CArraySampleLoss[m_nPointB]));//两点之间的损耗
//			if (m_iABSegmentLen!=0)
//			{
//				m_fABSegmentAttenu=(float)(m_fABLoss/(m_iABSegmentLen/1000.0));//两点之间的衰减	
//			}
//			else
//			{
//				m_fABSegmentAttenu=0;//两点之间的衰减
//			}
//			//下面两个if7月6号加入
//			if (m_nSelectCursor == 1) //如果选中A线，显示A点纵坐标
//			{
//				//GetDlgItem(IDC_STATIC_APOINT_ID)->SetWindowText("A点纵坐标:");
//				m_fAPointCoord=(float)m_CArraySampleLoss[m_nPointA]; //A点纵坐标
//			}
//			if (m_nSelectCursor == 2) //如果选中B线，显示B点纵坐标
//			{
//				//GetDlgItem(IDC_STATIC_APOINT_ID)->SetWindowText("B点纵坐标:");
//				m_fAPointCoord=(float)m_CArraySampleLoss[m_nPointB]; //B点纵坐标
//			}
//		}
//		else
//		{
//			//这个时候要使用两个标杆在网络曲线中的实际点数
//			RealPointA=GetSerialNum(Get_XCoordForFile(m_nPointA));
//			RealPointB=GetSerialNum(Get_XCoordForFile(m_nPointB));
//			m_iABSegmentLen=(long int)(fabs(Get_XCoordForFile(m_nPointA)-Get_XCoordForFile(m_nPointB))); //AB间的距离
//			if ((Get_XCoordForFile(m_nPointA)>Get_XCoord(pCurveData->DataPointsNum)) && (Get_XCoordForFile(m_nPointB)<=Get_XCoord(pCurveData->DataPointsNum)))
//			{
//				//A杆超过了网络曲线的最大距离
//				//下面两个if7月6号加入
//				if (m_nSelectCursor == 1) //如果选中A线，显示A点纵坐标
//				{
//					//GetDlgItem(IDC_STATIC_APOINT_ID)->SetWindowText("A点纵坐标:");
//					m_fAPointCoord=0; //A点纵坐标
//				}
//				if (m_nSelectCursor == 2) //如果选中B线，显示B点纵坐标
//				{
//					//GetDlgItem(IDC_STATIC_APOINT_ID)->SetWindowText("B点纵坐标:");
//					m_fAPointCoord=(float)m_CArraySampleLoss[RealPointB]; //B点纵坐标
//				}
//				m_fABLoss=(float)fabs(m_CArraySampleLoss[RealPointB]);
//			}
//			else if ((Get_XCoordForFile(m_nPointB)>Get_XCoord(pCurveData->DataPointsNum)) && (Get_XCoordForFile(m_nPointA)<=Get_XCoord(pCurveData->DataPointsNum)))
//			{
//				//B杆超过了网络曲线的最大距离
//				//下面两个if7月6号加入
//				if (m_nSelectCursor == 1) //如果选中A线，显示A点纵坐标
//				{
//					//GetDlgItem(IDC_STATIC_APOINT_ID)->SetWindowText("A点纵坐标:");
//					m_fAPointCoord=(float)m_CArraySampleLoss[RealPointA]; //A点纵坐标
//				}
//				if (m_nSelectCursor == 2) //如果选中B线，显示B点纵坐标
//				{
//					//GetDlgItem(IDC_STATIC_APOINT_ID)->SetWindowText("B点纵坐标:");
//					m_fAPointCoord=0; //B点纵坐标
//				}
//				m_fABLoss=(float)fabs(m_CArraySampleLoss[RealPointA]);
//			}
//			else if ((Get_XCoordForFile(m_nPointA)>Get_XCoord(pCurveData->DataPointsNum)) && (Get_XCoordForFile(m_nPointB)>Get_XCoord(pCurveData->DataPointsNum)))
//			{
//				//A和B杆都超过了网络曲线的最大距离
//				//下面两个if7月6号加入
//				if (m_nSelectCursor == 1) //如果选中A线，显示A点纵坐标
//				{
//					//GetDlgItem(IDC_STATIC_APOINT_ID)->SetWindowText("A点纵坐标:");
//					m_fAPointCoord=0; //A点纵坐标
//				}
//				if (m_nSelectCursor == 2) //如果选中B线，显示B点纵坐标
//				{
//					//GetDlgItem(IDC_STATIC_APOINT_ID)->SetWindowText("B点纵坐标:");
//					m_fAPointCoord=0; //B点纵坐标
//				}
//				m_fABLoss=0;
//			}
//			else
//			{
//				//A和B杆都没有超过文件曲线的最大距离
//				//下面两个if7月6号加入
//				if (m_nSelectCursor == 1) //如果选中A线，显示A点纵坐标
//				{
//					//GetDlgItem(IDC_STATIC_APOINT_ID)->SetWindowText("A点纵坐标:");
//					m_fAPointCoord=(float)m_CArraySampleLoss[RealPointA]; //A点纵坐标
//				}
//				if (m_nSelectCursor == 2) //如果选中B线，显示B点纵坐标
//				{
//					//GetDlgItem(IDC_STATIC_APOINT_ID)->SetWindowText("B点纵坐标:");
//					m_fAPointCoord=(float)m_CArraySampleLoss[RealPointB]; //B点纵坐标
//				}
//				m_fABLoss=(float)(fabs(m_CArraySampleLoss[RealPointA]-m_CArraySampleLoss[RealPointB]));//两点之间的损耗
//			}
//
//			if (m_iABSegmentLen!=0)
//			{
//				m_fABSegmentAttenu=(float)(m_fABLoss/(m_iABSegmentLen/1000.0));
//			}
//			else
//			{
//				m_fABSegmentAttenu=0;
//			}
//		}
//	}
//
//	if(m_bReceiveResultOK)
//	{
//		m_iABSegmentLen=(long int)fabs(Get_XCoord(m_nPointA)-Get_XCoord(m_nPointB));//AB之间的距离
//		m_fABLoss=(float)fabs(m_CArraySampleLoss[m_nPointA]-m_CArraySampleLoss[m_nPointB]);//两点之间的损耗
//		if (m_iABSegmentLen!=0)
//		{
//			m_fABSegmentAttenu=(float)(m_fABLoss/(m_iABSegmentLen/1000.0));//两点之间的衰减
//
//		}
//		else
//		{
//			m_fABSegmentAttenu=0;//两点之间的衰减
//		}
//		//下面两个if7月6号加入
//		if (m_nSelectCursor == 1) //如果选中A线，显示A点纵坐标
//		{
//			//GetDlgItem(IDC_STATIC_APOINT_ID)->SetWindowText("A点纵坐标:");
//			m_fAPointCoord=(float)m_CArraySampleLoss[m_nPointA]; //A点纵坐标
//		}
//		if (m_nSelectCursor == 2) //如果选中B线，显示B点纵坐标
//		{
//			//GetDlgItem(IDC_STATIC_APOINT_ID)->SetWindowText("B点纵坐标:");
//			m_fAPointCoord=(float)m_CArraySampleLoss[m_nPointB]; //B点纵坐标
//		}
//	}
//
//}

/*
***************************************************************
函数名称:             CalculABParaFromFile                                                                              
函数说明:             计算文件曲线AB段参数                                              
入口参数:             无                                                                                      
出口参数:             无                                                                                        
建立时间:             2011-07-02                                                                
Write By:             XJF                                                                                          
***************************************************************
//*/
//void CSorDrawStatic::CalculABParaFromFile()
//{
//	CCurveData* pCurveData = m_pMapCurveData[m_dwWaveTesting];
//	
//	int RealPointA; // A点实际点数
//	int RealPointB; //B点实际点数
//	if (m_bReceiveResultOK && m_bReadFromFile)
//	{
//		if (Get_XCoordForFile(g_nMaxPointFromFile)>Get_XCoord(pCurveData->DataPointsNum))
//		{
//			m_iABSegmentLenFromFile=(long int)fabs(Get_XCoordForFile(m_nPointA)-Get_XCoordForFile(m_nPointB));//AB之间的距离
//			m_fABLossFromFile=(float)fabs(m_CArraySampleLossFromFile[m_nPointA]-m_CArraySampleLossFromFile[m_nPointB]);//两点之间的损耗
//			if (m_iABSegmentLenFromFile!=0)
//			{
//				m_fABSegmentAttenuFromFile=(float)(m_fABLossFromFile/(m_iABSegmentLenFromFile/1000.0));//两点之间的衰减
//
//			}
//			else
//			{
//				m_fABSegmentAttenuFromFile=0;//两点之间的衰减
//			}
//			//下面两个if7月6号加入
//			if (m_nSelectCursor == 1) //如果选中A线，显示A点纵坐标
//			{
//				//GetDlgItem(IDC_STATIC_APOINT_ID)->SetWindowText("A点纵坐标:");
//				m_fAPointCoordFromFile=(float)m_CArraySampleLossFromFile[m_nPointA]; //A点纵坐标
//			}
//			if (m_nSelectCursor == 2) //如果选中B线，显示B点纵坐标
//			{
//				//GetDlgItem(IDC_STATIC_APOINT_ID)->SetWindowText("B点纵坐标:");
//				m_fAPointCoordFromFile=(float)m_CArraySampleLossFromFile[m_nPointB]; //B点纵坐标
//			}
//
//		}
//		else
//		{
//			//这个时候要使用两个标杆在文件曲线中的实际点数
//			RealPointA=GetSerialNumForFile(Get_XCoord(m_nPointA));
//			RealPointB=GetSerialNumForFile(Get_XCoord(m_nPointB));
//			m_iABSegmentLenFromFile=(long int)fabs(Get_XCoord(m_nPointA)-Get_XCoord(m_nPointB)); //AB间的距离
//			if ((Get_XCoord(m_nPointA)>Get_XCoordForFile(g_nMaxPointFromFile)) && (Get_XCoord(m_nPointB)<=Get_XCoordForFile(g_nMaxPointFromFile)))
//			{
//				//A杆超过了文件曲线的最大距离
//				if (m_nSelectCursor == 1) //选中A线
//				{
//					//GetDlgItem(IDC_STATIC_APOINT_ID)->SetWindowText("A点纵坐标:");
//					m_fAPointCoordFromFile=0;
//				}
//				if (m_nSelectCursor == 2)
//				{
//					//GetDlgItem(IDC_STATIC_APOINT_ID)->SetWindowText("B点纵坐标:");
//					m_fAPointCoordFromFile=(float)m_CArraySampleLossFromFile[RealPointB];
//				}
//
//				m_fABLossFromFile=(float)fabs(m_CArraySampleLossFromFile[RealPointB]);
//			}
//			else if ((Get_XCoord(m_nPointB)>Get_XCoordForFile(g_nMaxPointFromFile)) && (Get_XCoord(m_nPointA)<=Get_XCoordForFile(g_nMaxPointFromFile)))
//			{
//				//B杆超过了文件曲线的最大距离
//				if (m_nSelectCursor == 1)
//				{
//					//GetDlgItem(IDC_STATIC_APOINT_ID)->SetWindowText("A点纵坐标:");
//					m_fAPointCoordFromFile=(float)m_CArraySampleLossFromFile[RealPointA];
//				}
//				if (m_nSelectCursor == 2)
//				{
//					//GetDlgItem(IDC_STATIC_APOINT_ID)->SetWindowText("B点纵坐标:");
//					m_fAPointCoordFromFile=0;
//				}
//
//				m_fABLossFromFile=(float)fabs(m_CArraySampleLossFromFile[RealPointA]);
//			}
//			else if ((Get_XCoord(m_nPointA)>Get_XCoordForFile(g_nMaxPointFromFile)) && (Get_XCoord(m_nPointB)>Get_XCoordForFile(g_nMaxPointFromFile)))
//			{
//				//A和B杆都超过了文件曲线的最大距离
//				if (m_nSelectCursor == 1)
//				{
//					//GetDlgItem(IDC_STATIC_APOINT_ID)->SetWindowText("A点纵坐标:");
//					m_fAPointCoordFromFile=0;
//				}
//				if (m_nSelectCursor == 2)
//				{
//					//GetDlgItem(IDC_STATIC_APOINT_ID)->SetWindowText("B点纵坐标:");
//					m_fAPointCoordFromFile=0;
//				}
//				m_fABLossFromFile=0;
//			}
//			else
//			{
//				//A和B杆都没有超过文件曲线的最大距离
//				if (m_nSelectCursor == 1)
//				{
//					//GetDlgItem(IDC_STATIC_APOINT_ID)->SetWindowText("A点纵坐标:");
//					m_fAPointCoordFromFile=(float)m_CArraySampleLossFromFile[RealPointA];
//				}
//				if (m_nSelectCursor == 2)
//				{
//					//GetDlgItem(IDC_STATIC_APOINT_ID)->SetWindowText("B点纵坐标:");
//					m_fAPointCoordFromFile=(float)m_CArraySampleLossFromFile[RealPointB];
//				}
//
//				m_fABLossFromFile=(float)fabs(m_CArraySampleLossFromFile[RealPointA]-m_CArraySampleLossFromFile[RealPointB]);//两点之间的损耗
//			}
//
//			if (m_iABSegmentLenFromFile!=0)
//			{
//				m_fABSegmentAttenuFromFile=(float)(m_fABLossFromFile/(m_iABSegmentLenFromFile/1000.0));
//			}
//			else
//			{
//				m_fABSegmentAttenuFromFile=0;
//			}
//		}
//	}
//
//	if(m_bReadFromFile && !m_bReceiveResultOK)
//	{
//		m_iABSegmentLenFromFile=(long int)fabs(Get_XCoordForFile(m_nPointA)-Get_XCoordForFile(m_nPointB));//AB之间的距离
//		m_fABLossFromFile=(float)fabs(m_CArraySampleLossFromFile[m_nPointA]-m_CArraySampleLossFromFile[m_nPointB]);//两点之间的损耗
//		if (m_iABSegmentLenFromFile!=0)
//		{
//			m_fABSegmentAttenuFromFile=(float)(m_fABLossFromFile/(m_iABSegmentLenFromFile/1000.0));//两点之间的衰减
//
//		}
//		else
//		{
//			m_fABSegmentAttenuFromFile=0;//两点之间的衰减
//		}
//		//下面两个if7月6号加入
//		if (m_nSelectCursor == 1) //如果选中A线，显示A点纵坐标
//		{
//			//GetDlgItem(IDC_STATIC_APOINT_ID)->SetWindowText("A点纵坐标:");
//			m_fAPointCoordFromFile=(float)m_CArraySampleLossFromFile[m_nPointA]; //A点纵坐标
//		}
//		if (m_nSelectCursor == 2) //如果选中B线，显示B点纵坐标
//		{
//			//GetDlgItem(IDC_STATIC_APOINT_ID)->SetWindowText("B点纵坐标:");
//			m_fAPointCoordFromFile=(float)m_CArraySampleLossFromFile[m_nPointB]; //B点纵坐标
//		}
//	}
//}
//
//

/*
***************************************************************
函数名称:             GetSerialNumForFile                                                                              
函数说明:             获取与距离相对应的文件曲线点数                                              
入口参数:             无                                                                                      
出口参数:             距离对应点数                                                                                        
建立时间:             2011-07-02                                                                
Write By:             XJF                                                                                          
***************************************************************
*//*
int CSorDrawStatic::GetSerialNumForFile(long int theDistance)
{
int X=(int)((2*m_fFrequencyFromFile*m_fRefractionFromFile*theDistance)/C);
return X;
}
*/

/*
***************************************************************
函数名称:             GetSerialNum                                                                              
函数说明:             获取与距离相对应的网络曲线点数                                              
入口参数:             无                                                                                      
出口参数:             距离对应点数                                                                                        
建立时间:             2010-07-02                                                                
Write By:             XJF                                                                                          
***************************************************************
*/
//int CSorDrawStatic::GetSerialNum(long int theDistance)
//{
//	int X=(int)((2*m_fFrequency*m_fRefraction*theDistance)/C);
//	return X;
//}
/*

//返回当前曲线缩放模式 0：缩放；1：平移
int CSorDrawStatic::GetCurCurveZoomMode() const
{
	return m_iCurveZoomMode;
}
//清空所有曲线数据，设置DataPointsNum为0.//wqc2011.12.19
void CSorDrawStatic::ClearAllCurveData()
{
	for(int nCount = 0; nCount < g_sorFileArray.totalCurveNumber; nCount++)
	{
		TestResultInfo* pCurve = g_sorFileArray.sorFileList[nCount].pTestResultInfo;
		if (pCurve)
		{
			//pCurve->m_TestResult.DataPointsNum = 0;//wcq2011.12.23
			pCurve->DataPointsNum = CLEAR_DATAPOINTS_NUM;//wcq2011.12.23
			pCurve->IsMaxLength = FALSE;//????wcq2011.12.23
		}
	}
}
//设置当前曲线缩放模式 0：缩放；1：平移
void CSorDrawStatic::SetCurCurveZoomMode(int nCurveZoomMode)
{
	m_iCurveZoomMode = nCurveZoomMode;
}


//事件列表
void CSorDrawStatic::EventList()
{
	//m_bDisplayEventList = !m_bDisplayEventList;

	//CalCulDispRect(m_bDisplayEventList);
	//CRect ClientRect;
	//GetClientRect(&ClientRect);
	////InvalidateRect(&ClientRect);//wcq2011.11.16////放到后面wcq2011.12.15
	////InvalidateAll();//wcq2011.11.16
	////加默认选中项
	//POSITION pos = m_lstEventList.GetFirstSelectedItemPosition();
	//int nIndex = m_lstEventList.GetNextSelectedItem(pos);
	//if (m_lstEventList.GetItemCount() > 0 && nIndex == -1)
	//{
	//	m_lstEventList.SetItemState(0, LVIS_SELECTED, LVIS_SELECTED);
	//	//MoveEventList(0);	//拿到外面 wcq2011.12.15
	//}//eif
	//m_lstEventList.MoveWindow(&m_rectEventArea);//原来在OnPaint里，//wcq2011.12.15
	//m_bReDrawGrid = TRUE;//因为大小改变了，所以要重绘Grid.//wcq2011.12.15
	////InvalidateRect(&ClientRect);//wcq2011.12.15
	//Invalidate(FALSE);//wcq2011.12.15
}

//曲线对比
void CSorDrawStatic::CurveCompare(const CString strFileName)
{
	//if (strFileName == _T(""))
	//{
	//	return;
	//}
	//if (_T(".sor") !=strFileName.Right(4) && _T(".SOR") !=strFileName.Right(4))//不是sor文件wcq2011.12.27
	//{
	//	return;
	//}

	////从文件中加载默认测量波长是2000
	//m_curABWave=2000;
	//CCurveData* pCurveData = m_pMapCurveData[2000];
	//if (pCurveData != NULL)
	//{
	//	delete pCurveData;
	//	pCurveData = NULL;
	//}
	//pCurveData = new CCurveData();

	//m_pMapCurveData[2000] = pCurveData;

	//CSorFile sorFile;
	////int iiii=sorFile.readSorFile(strFileName, pCurveData->m_TestResult);
	//if(1 != sorFile.readSorFile(strFileName, pCurveData->m_TestResult))//wcq2011.12.01
	//{
	//	return;
	//}
	//pCurveData->Length = pCurveData->DataSpacing * pCurveData->DataPointsNum;//计算曲线（光纤）长度。wcq2011.12.27
	////for (int i=0;i<pCurveData->DataPointsNum;i++)
	////{
	////	DEBUGMSG(true,(_T("%d \n"),pCurveData->pDataPoints[i]));
	////}
	//if (pCurveData->DataPointsNum != 1)//第一次(DataPointsNum初始为1)或数据点数比前面大,重新申请空间
	//{
	//	if (NULL != pCurveData->ptDrawData)
	//	{
	//		delete[] pCurveData->ptDrawData;
	//	}
	//	//if (NULL != pCurveData->pDataPoints)
	//	//{
	//	//	delete[] pCurveData->pDataPoints;
	//	//}
	//}

	//if (NULL == pCurveData->pDataCurve)
	//{
	//	pCurveData->pDataCurve= new CPoint[COUNT_POINT_MAX_MIN * 2];//每一象素取两个点//wcq2011.11.23
	//}

	////pCurveData->pDataPoints= new WORD[pCurveData->DataPointsNum];
	//pCurveData->ptDrawData=new CPoint[pCurveData->DataPointsNum];
	////pCurveData->DataPointsNum = pCurveData->DataPointsNum;
	///*

	//if (0 == m_nZoomCount)//如果为0则未进行过曲线缩放次数计算
	//{
	//ReCalZoomParas();//第一次有数据到来，重新计算缩放相关参数。//wcq2011.09.23
	//}
	//pCurveData->Length = Get_XCoord2nf(pCurveData->DataPointsNum,pCurveData);//得到光纤长度2011.11.25
	//SetLowerStandard(pCurveData);
	//ReCalABLabelPos();//每次加载或读到一条新的曲线数据时重新计算A/B标位置//wcq2011.09.13
	//SetLowerStandard(pCurveData);//设置CCurveData中的IsLowerStandard变量，以方便曲线竖直方向缩放.wcq2011.09.28

	////DEBUGMSG(true,(_T("OnDataReady VerticalZoom HorizonZooms\n")));
	//VerticalZoom();//得到竖直方向范围wcq2011.09.28
	//HorizonZooms();//水平缩放函数，这里用来计算各曲线的MaxPoint/StartPoint //wcq2011.09.14

	//*
	//BOOL bMaxDistance = pCurveData->IsMaxLength;//用于确定是否要更新测试曲线的缩略图，加载的曲线比测试长，则需要重新计算所有曲线的缩略图数据wcq2011.12.14
	//ReCalZoomParas();//第一次有数据到来，重新计算缩放相关参数。//wcq2011.11.25
	////pCurveData->Length = Get_XCoord(pCurveData->DataPointsNum,pCurveData);//得到光纤长度会在读取的时候赋值2011.11.25
	//ReCalABLabelPos();//每次加载或读到一条新的曲线数据时重新计算A/B标位置//wcq2011.09.13
	//SetLowerStandard(pCurveData);//设置CCurveData中的IsLowerStandard变量，以方便曲线竖直方向缩放.wcq2011.09.28
	//VerticalZoom();//得到竖直方向范围wcq2011.09.28
	////l2=::GetTickCount() -l;
	//HorizonZooms();//水平缩放函数，这里用来计算各曲线的MaxPoint/StartPoint //wcq2011.09.14
	////GetThumbData(pCurveData);//每收到一条新数据，都计算下用于绘制缩略图的点.wcq2011-11.24
	//if (!(bMaxDistance && pCurveData->IsMaxLength))//计算最大距离曲线前后，当前加载的曲线是否为最长距离曲线标示不一样，则要重新计算所有缩略图数据wcq2011.12.14
	//{
	//	GetAllThumbData();
	//}
	//else//只需要计算当前曲线的缩略图
	//{
	//	GetThumbData(pCurveData);//每收到一条新数据，都计算下用于绘制缩略图的点.wcq2011-11.24
	//}
	//if (INVALID_WAVELENGTH == m_dwWaveSelected)
	//{
	//	m_dwWaveSelected = 2000;
	//}
	//GetDataCurve(pCurveData);//wcq2011-11.24
	////InitialEventListFromNode(); //初始化事件表信息
	////EventIsSelected(m_nPointA);
	////InvalidateRect(m_rectCurve);//wcq2011.09.13
	//InvalidateAll();//wcq2011.11.16
//}

//事件光标
void CSorDrawStatic::EventCursor()
{
	//if (!HasTested())//还没测试不能移动//wcq2012.01.05
	//{
	//	return;
	//}
	//m_bEventCursor = TRUE;
	//int iSelectPoint=-1;
	//POSITION pos=m_lstEventList.GetFirstSelectedItemPosition();
	//while(pos)
	//{
	//	iSelectPoint = m_lstEventList.GetNextSelectedItem(pos);
	//	break;
	//}
	//if (-1 == iSelectPoint)//防止未选中 wcq2012.03.15
	//{
	//	if ( 0==m_lstEventList.GetItemCount())
	//	{
	//		return;
	//	}
	//	iSelectPoint = 0;
	//	m_lstEventList.SetItemState(0, LVIS_SELECTED, LVIS_SELECTED);//
	//}
	////m_nPointA = m_CArrayEventInfoFromFile[iSelectPoint + 1].nLocation;//c wcq 2011.12.16
	//std::vector<EventInfo> ve = m_pMapCurveData[m_dwWaveTesting]->m_TestResult.EventList.vEventInfo;//速度??//c wcq 2011.12.16
	////InvalidateRect(&m_rectCurve);
	//int eventCount = m_pMapCurveData[m_dwWaveTesting]->m_TestResult.EventList.vEventInfo.size();
	//if (0 == eventCount)//无事件。wcq2011.12.16
	//{
	//	return;
	//}
	//if (iSelectPoint == eventCount -1)//最后一个//wcq2011.12.16
	//{
	//	iSelectPoint = 0;
	//}
	//else//不是最后一个，把光标移到后一个事件点//wcq2011.12.16
	//{
	//	iSelectPoint++;
	//}
	//m_lstEventList.SetItemState(iSelectPoint, LVIS_SELECTED, LVIS_SELECTED);
	//m_nPointA = ve[iSelectPoint].index;
	//InvalidateAll();//wcq2011.11.16
}

LRESULT CSorDrawStatic::OnDataReady(WPARAM wParam,LPARAM lParam)
{
	//	//DEBUGMSG(TRUE,(_T("OnDataReady  ****************enter \n")));
	//#ifdef TRACE_FIND_MEMORY
	//	GlobalMemoryStatus(&buffer);//wcq
	//	DEBUGMSG(TRUE,(_T("OnDataReady enter mem: %d\n"),buffer.dwTotalPhys-buffer.dwAvailPhys));
	//#endif
	//	///m_nCount++;//wcq2012.03.09
	//	long l=GetTickCount();//?????????????????
	//	m_bBusy=TRUE;
	//	INT nDataLenMsg = (int)lParam;
	//	if(0 == nDataLenMsg)
	//	{
	//		return 1;
	//	}
	//	//if(NULL != m_pRecvbuf)//2011.09.21
	//	//{
	//	//	delete[] m_pRecvbuf;
	//	//}
	//	if(NULL == m_pRecvbuf)//第一次
	//	{
	//		m_pRecvbuf = new WORD[nDataLenMsg + COUNT_READ_DATA_OTHER];//2011.09.21//nDataLenMsg不包含其它数据wcq2011.12.05
	//#ifdef TRACE_FIND_MEMORY
	//		m_pMainDlg->SetStatusBarText(_T("OnDataReady m_pRecvbuf new") );
	//#endif
	//	}
	//	else if (nDataLenMsg>m_dwMaxPointCount)//比前面大,包括所有曲线
	//	{
	//		if(NULL != m_pRecvbuf)//2011.09.21
	//		{
	//			delete[] m_pRecvbuf;
	//		}
	//		m_pRecvbuf = new WORD[nDataLenMsg + COUNT_READ_DATA_OTHER];//2011.09.21
	//#ifdef TRACE_FIND_MEMORY
	//		DEBUGMSG(true,(_T("OnDataReady m_pRecvbuf new\n")));
	//		//m_pMainDlg->SetStatusBarText(_T("OnDataReady m_pRecvbuf new else") );
	//#endif
	//	}
	//	//if (1310 == m_nWaveToDevice)
	//	//{
	//	//	int a=0;
	//	//}
	//	//DEBUGMSG(true,(_T("OnDataReady m_pMapCurveData size:%d  m_curABWave:%d,m_dwWaveTesting:%d\n"),m_pMapCurveData.size(),m_curABWave,m_dwWaveTesting));
	//	//获取测量波长
	//	if (m_dwWaveTesting == 0)
	//		return -1;
	//	if (-1 == m_curABWave)
	//	{
	//		m_curABWave = m_dwWaveTesting;
	//	}
	//	CCurveData* pCurveData = m_pMapCurveData[m_dwWaveTesting];
	//	//DEBUGMSG(true,(_T("OnDataReady m_pMapCurveData size:%d  m_curABWave:%d,m_dwWaveTesting:%d\n"),m_pMapCurveData.size(),m_curABWave,m_dwWaveTesting));
	//	///*if (pCurveData != NULL)//wcq2011.09.16
	//	//{
	//	//	delete pCurveData;
	//	//	pCurveData = NULL;
	//	//}
	//	//pCurveData = new CCurveData();*/
	//
	//	if (NULL == pCurveData)//wcq2011.09.16
	//	{
	//		//delete pCurveData;
	//		//pCurveData = NULL;
	//		pCurveData = new CCurveData();//wcq2011.09.16
	//		///pCurveData->pDataPoints= new WORD[nDataLenMsg];
	//		m_pMapCurveData[m_dwWaveTesting] = pCurveData;//wcq2011.09.16
	//#ifdef TRACE_FIND_MEMORY
	//		//m_pMainDlg->SetStatusBarText(_T("OnDataReady new CCurveData") );
	//		DEBUGMSG(true,(_T("OnDataReady CCurveData new\n")));
	//#endif
	//	}
	//
	//	if (pCurveData->DataPointsNum < nDataLenMsg)//第一次(DataPointsNum初始为1)或数据点数比前面大,重新申请空间
	//	{
	//		if (NULL != pCurveData->ptDrawData)
	//		{
	//			delete[] pCurveData->ptDrawData;
	//			pCurveData->ptDrawData=NULL;
	//		}
	//		if (NULL != pCurveData->pDataPoints)
	//		{
	//			delete[] pCurveData->pDataPoints;
	//			pCurveData->pDataPoints=NULL;
	//		}
	//		//if (NULL == pCurveData->pDataCurve)
	//		//{
	//		//	pCurveData->pDataCurve= new WORD[COUNT_POINT_MAX_MIN];//每一象素取两个点//wcq2011.09.30
	//		//}
	//		//if (NULL == pCurveData->pThumbData)//wcq2011.11.25缩略图数据
	//		//{
	//		//	pCurveData->pThumbData = new CPoint[m_rectThumbnail.Width() * 2];//x上选二点
	//		//}
	//
	//#ifdef TRACE_FIND_MEMORY
	//		//m_pMainDlg->SetStatusBarText(_T("OnDataReady new CCurveData") );
	//		DEBUGMSG(true,(_T("OnDataReady pDataPoints new\n")));
	//#endif
	//
	//		pCurveData->ptDrawData=new CPoint[nDataLenMsg];
	//		pCurveData->pDataPoints= new WORD[nDataLenMsg];
	//		//pCurveData->ptDrawData=new CPoint[nDataLenMsg+4];//wcq2012.01.12
	//		pCurveData->DataPointsNum = nDataLenMsg;
	//	}
	//	///ASSERTMSG(_T("memery zero\n"),(NULL == m_pRecvbuf));
	//	///	DEBUGMSG(true,(_T("OnDataReady before readData test is NULL\n")));
	//	////m_pRecvbuf[0] = 0;
	//	//DEBUGMSG(true,(_T("OnDataReady before readData\n")));
	//
	//	CString ss;
	//	//if (!m_frmDriver.readData(m_pRecvbuf, nDataLenMsg ))//wcq2011.12.05
	//	if (!m_frmDriver.readData(m_pRecvbuf, nDataLenMsg  + COUNT_READ_DATA_OTHER))//wcq2011.12.05
	//		return -1; //接收失败
	//
	//	//wcq2012.02.07添加//////////////////////////////////////////////////////
	//	m_nDataLenMsg = nDataLenMsg;
	//	::SetEvent(hEeventDealData);
	//	///ConnectDeviceToTest();//wcq2012.02.07//cwcq2012.02.21
	//	//DEBUGMSG(TRUE,(_T("OnDataReady  ****************exit \n")));
	//	return 1;
	//	//wcq2012.02.07添加结束//////////////////////////////////////////////////////
	//	//后面为以前的，改为放到一个线程里处理//wcq2012.02.07
	//
	//
	//
	//
	//	////////////////////////////////////////////////////////////////////////////////////////////////
	//	//DEBUGMSG(true,(_T("OnDataReady after readData nDataLenMsg:%d\n"),nDataLenMsg));
	//	//读取缓冲区前七位数据
	//	pCurveData->DataType = *(m_pRecvbuf); //数据类型
	//	pCurveData->WaveLength = *(m_pRecvbuf + 1); //波长
	//	pCurveData->PulseWidth = *(m_pRecvbuf + 2); //脉宽
	//	pCurveData->TestMode = *(m_pRecvbuf + 3); //测试模式
	//	pCurveData->Frequency = *(m_pRecvbuf + 4); //测试频率
	//	pCurveData->Relay = *(m_pRecvbuf + 5); //继电器wcq2011.12.06
	//	pCurveData->Joint1 = *(m_pRecvbuf + 6); //拼接点1 wcq2011.12.05
	//	pCurveData->Joint2 = *(m_pRecvbuf + 7); //拼接点2 wcq2011.12.05
	//	pCurveData->DataSerialNum = *(m_pRecvbuf + 8); //数据包序号
	//	pCurveData->DataPointsNum = *(m_pRecvbuf + 9); //采样点数
	//	//pCurveData->pDataPoints = m_pRecvbuf + 7;
	//	//memcpy(pCurveData->pDataPoints,m_pRecvbuf+9,nDataLenMsg * sizeof(WORD));//wcq2011.09.21
	//	memcpy(pCurveData->pDataPoints,m_pRecvbuf+COUNT_READ_DATA_OTHER,nDataLenMsg * sizeof(WORD));//wcq2011.12.06
	//	///	DEBUGMSG(true,(_T("On dataready time memcpy\n")));
	//	//清空事件列表wcq2011.11.15
	//	pCurveData->EventList.vEventInfo.clear();//wcq2011.11.15
	//
	//
	//#ifdef TRACE_FIND_MEMORY
	//	GlobalMemoryStatus(&buffer);//wcq
	//	DEBUGMSG(TRUE,(_T("OnDataReady after read data: %d\n"),buffer.dwTotalPhys-buffer.dwAvailPhys));
	//#endif
	//	//DEBUGMSG(true,(_T("OnDataReady after readData pCurveData->DataPointsNum:%d\n"),pCurveData->DataPointsNum));
	//	m_nMaxPoint = pCurveData->DataPointsNum; //采样点数
	//
	//	if (nDataLenMsg>m_dwMaxPointCount)//wcq.2011.09.23
	//	{
	//		m_dwMaxPointCount=nDataLenMsg;//wcq2011.09.21
	//	}
	//	BOOL bMaxDistance = pCurveData->IsMaxLength;//用于确定是否要更新测试曲线的缩略图，如果曲线加载前后是否为最长距离曲线标示不一样，则需要重新计算所有曲线的缩略图数据wcq2011.12.14
	//
	//	pCurveData->Length = Get_XCoord2nf(pCurveData->DataPointsNum,pCurveData);//得到光纤长度2011.11.25//wcq2011.12.19
	//	pCurveData->DataSpacing = Get_XCoord2nf(1,pCurveData);//
	//
	//	if (0 == m_nZoomCount)//如果为0则未进行过曲线缩放次数计算
	//	{
	//		//if (1310 == m_nWaveToDevice)
	//		//{
	//		//	int a=0;
	//		//}
	//		ReCalZoomParas();//第一次有数据到来，重新计算缩放相关参数。//wcq2011.09.23
	//	}
	//	///	DEBUGMSG(true,(_T("On dataready time Get_XCoord2nf\n")));
	//	//pCurveData->Length = Get_XCoord2nf(pCurveData->DataPointsNum,pCurveData);//得到光纤长度2011.11.25//移到前面wcq2011.12.19
	//	//pCurveData->DataSpacing = Get_XCoord2nf(1,pCurveData);//移到前面wcq2011.12.19
	//	SetLowerStandard(pCurveData);
	//	ReCalABLabelPos();//每次加载或读到一条新的曲线数据时重新计算A/B标位置//wcq2011.09.13
	//	SetLowerStandard(pCurveData);//设置CCurveData中的IsLowerStandard变量，以方便曲线竖直方向缩放.wcq2011.09.28
	//
	//	///	DEBUGMSG(true,(_T("On dataready time VerticalZoom\n")));
	//	//DEBUGMSG(true,(_T("OnDataReady VerticalZoom HorizonZooms\n")));
	//	VerticalZoom();//得到竖直方向范围wcq2011.09.28
	//	HorizonZooms();//水平缩放函数，这里用来计算各曲线的MaxPoint/StartPoint //wcq2011.09.14
	//	//DEBUGMSG(true,(_T("OnDataReady VerticalZoom HorizonZooms2\n")));
	//	//l=::GetTickCount();
	//	//GetThumbData(pCurveData);//每收到一条新数据，都计算下用于绘制缩略图的点.wcq2011-11.10	//wcq2011.12.14
	//	if (!(bMaxDistance && pCurveData->IsMaxLength))//计算最大距离曲线前后，当前加载的曲线是否为最长距离曲线标示不一样，则要重新计算所有缩略图数据wcq2011.12.14
	//	{
	//		GetAllThumbData();
	//	}
	//	else//只需要计算当前曲线的缩略图
	//	{
	//		GetThumbData(pCurveData);//每收到一条新数据，都计算下用于绘制缩略图的点.wcq2011-11.24
	//	}
	//	GetDataCurve(pCurveData);//给CurveData中的 pDataCurve变量赋值，得到计算过后的绘制曲线数据wcq2011.11.21
	//	//DEBUGMSG(true,(_T("On dataready time:%d\n"),GetTickCount() - l));
	//	//InvalidateRect(m_rectCurve);//wcq2011.09.13
	//	//InvalidateRect(m_rectThumbnail);
	//	//InvalidateRect(m_rectSubInfo);
	//	//InvalidateAll();//wcq2011.11.16
	//	m_bBusy=FALSE;
	//	//GetEventList();//平均测试完后，通过dll的计算公式得到事件列表wcq2011-12.05
	//
	//#ifdef TRACE_FIND_MEMORY
	//	GlobalMemoryStatus(&buffer);//wcq
	//	DEBUGMSG(TRUE,(_T("OnDataReady  end: %d\n"),buffer.dwTotalPhys-buffer.dwAvailPhys));
	//#endif
	//
	//	Invalidate(FALSE);//wcq2011.12.02
	//	long ll=GetTickCount()-l;//?????????????????
	//	//DEBUGMSG(TRUE,(_T("OnDataReady  end time: %d\n"),ll));
	//	if (!m_bStopTest)
	//	{
	//		//ConnectDeviceToTest(TRUE);//开始测试
	//		//ConnectDeviceToTest();//开始测试wcq2011.12.13
	//	}
	//	else//停止测试,得到事件列表wcq2011.11.15
	//	{
	//		long l=GetTickCount();
	//		int ll2=GetTickCount()-l;
	//		//DEBUGMSG(TRUE,(L"gorbal time(ms):%d\n",ll2));
	//	}
	//
	//return 1;
//}


//打开文件
void CSorDrawStatic::FillCurveDataFromFile(TestResultInfo* pCurveData)
{
	if (NULL == pCurveData)
	{
		return;
	}
	pCurveData->IsLoadFromFile = 1; //从文件加载
	pCurveData->IsMaxLength = 1;
	pCurveData->Length = pCurveData->DataSpacing * pCurveData->DataPointsNum;//计算曲线（光纤）长度。wcq2011.12.27
	if (pCurveData->DataPointsNum != 1)//第一次(DataPointsNum初始为1)或数据点数比前面大,重新申请空间
	{
		pCurveData->MaxPoint = pCurveData->DataPointsNum;
	}

	if (NULL != pCurveData->pDataCurve)
	{
		delete[] pCurveData->pDataCurve;
		pCurveData->pDataCurve = NULL;
	}
	pCurveData->pDataCurve= new CPoint[pCurveData->DataPointsNum];

	if (NULL != pCurveData->ptDrawData)
	{
		delete[] pCurveData->ptDrawData;
		pCurveData->ptDrawData = NULL;
	}
	pCurveData->ptDrawData=new CPoint[pCurveData->DataPointsNum];

	long a=GetTickCount();
	BOOL bMaxDistance = pCurveData->IsMaxLength;//用于确定是否要更新测试曲线的缩略图，加载的曲线比测试长，则需要重新计算所有曲线的缩略图数据wcq2011.12.14
	ReCalZoomParas();//第一次有数据到来，重新计算缩放相关参数。//wcq2011.11.25
	//pCurveData->Length = Get_XCoord(pCurveData->DataPointsNum,pCurveData);//得到光纤长度会在读取的时候赋值2011.11.25
	ReCalABLabelPos();//每次加载或读到一条新的曲线数据时重新计算A/B标位置//wcq2011.09.13
	SetLowerStandard(pCurveData);//设置CCurveData中的IsLowerStandard变量，以方便曲线竖直方向缩放.wcq2011.09.28
	VerticalZoom();//得到竖直方向范围wcq2011.09.28
	//l2=::GetTickCount() -l;
	HorizonZooms();//水平缩放函数，这里用来计算各曲线的MaxPoint/StartPoint //wcq2011.09.14
	NewTestInit(pCurveData);
	if (!(bMaxDistance && pCurveData->IsMaxLength))//计算最大距离曲线前后，当前加载的曲线是否为最长距离曲线标示不一样，则要重新计算所有缩略图数据wcq2011.12.14
	{
		MAIN->m_pThumbFormView->GetAllThumbData();
	}
	else//只需要计算当前曲线的缩略图
	{
		MAIN->m_pThumbFormView->GetThumbData(pCurveData);//每收到一条新数据，都计算下用于绘制缩略图的点.wcq2011-11.24
	}
	if (INVALID_WAVELENGTH == m_dwWaveSelected)
	{
		m_dwWaveSelected = 2000;
	}
	InvalidateAll();//wcq2011.11.16
	TRACE(_T("222 %d\n"),GetTickCount()-a);
	a=GetTickCount();
	GetDataCurve(pCurveData);//wcq2011-11.24
	TRACE(_T("333 %d\n"),GetTickCount()-a);
}

//重新计算曲线数据
void CSorDrawStatic::ResetCurveData()
{
	ReCalZoomParas();//第一次有数据到来，重新计算缩放相关参数。//wcq2011.11.25
	ReCalABLabelPos();//每次加载或读到一条新的曲线数据时重新计算A/B标位置//wcq2011.09.13
	VerticalZoom();//得到竖直方向范围wcq2011.09.28
	HorizonZooms();//水平缩放函数，这里用来计算各曲线的MaxPoint/StartPoint //wcq2011.09.14
	InvalidateAll();
}

void CSorDrawStatic::DealDataDetail()
{
	INT nDataLenMsg = m_nDataLenMsg;
	TestResultInfo* pCurveData = g_sorFileArray.sorFileList[m_nMaxIndex].pTestResultInfo;
	pCurveData->DataType = *(m_pRecvbuf); //数据类型
	pCurveData->WaveLength = *(m_pRecvbuf + 1); //波长
	pCurveData->PulseWidth = *(m_pRecvbuf + 2); //脉宽
	pCurveData->TestMode = *(m_pRecvbuf + 3); //测试模式
	pCurveData->Frequency = *(m_pRecvbuf + 4); //测试频率
	pCurveData->Relay = *(m_pRecvbuf + 5); //继电器wcq2011.12.06
	pCurveData->Joint1 = *(m_pRecvbuf + 6); //拼接点1 wcq2011.12.05
	pCurveData->Joint2 = *(m_pRecvbuf + 7); //拼接点2 wcq2011.12.05
	pCurveData->DataSerialNum = *(m_pRecvbuf + 8); //数据包序号
	pCurveData->DataPointsNum = *(m_pRecvbuf + 9); //采样点数
	memcpy(pCurveData->pDataPoints,m_pRecvbuf+COUNT_READ_DATA_OTHER,nDataLenMsg * sizeof(WORD));
	//清空事件列表wcq2011.11.15
	pCurveData->EventList.vEventInfo.clear();//wcq2011.11.15

	if(INVALID_MAX_POINT == m_nMaxPoint)//第一次赋值
	{
		m_nMaxPoint = pCurveData->DataPointsNum; //采样点数//wcq2012.02.20
	}
	if (nDataLenMsg>m_dwMaxPointCount)//wcq.2011.09.23
	{
		m_dwMaxPointCount=nDataLenMsg;//wcq2011.09.21
	}
	BOOL bMaxDistance = pCurveData->IsMaxLength;//用于确定是否要更新测试曲线的缩略图，如果曲线加载前后是否为最长距离曲线标示不一样，则需要重新计算所有曲线的缩略图数据wcq2011.12.14

	if (m_bFirstRead)//第一次收到数据 wcq2012.02.22
	{
		pCurveData->Length = Get_XCoord2nf(pCurveData->DataPointsNum,pCurveData);//得到光纤长度2011.11.25//wcq2011.12.19
		pCurveData->DataSpacing = Get_XCoord2nf(1,pCurveData);//
	}

	SetLowerStandard(pCurveData);
	ReCalABLabelPos();//每次加载或读到一条新的曲线数据时重新计算A/B标位置//wcq2011.09.13
	SetLowerStandard(pCurveData);//设置CCurveData中的IsLowerStandard变量，以方便曲线竖直方向缩放.wcq2011.09.28

	if (m_bFirstRead)//第一次收到数据 wcq2012.02.21
	{
		m_bFirstRead = FALSE;
		ReCalZoomParas();//第一次有数据到来，重新计算缩放相关参数。//wcq2011.09.23//wcq2012.02.22
		VerticalZoom();//得到竖直方向范围
		HorizonZooms();//水平缩放函数，这里用来计算各曲线的MaxPoint/StartPoint //
		NewTestInit(pCurveData);//按下测试后初始化一些参数
	}

	m_bBusy=FALSE;

	m_bNeedHorizonZoom = FALSE;//wcq2012.02.20

	Invalidate(FALSE);//wcq2011.12.02
}

//每收到一条新数据，都计算下用于绘制曲线的点.wcq2011.11.21
void CSorDrawStatic::GetDataCurve(TestResultInfo* pCurveData)
{
	if (!pCurveData || pCurveData->DataPointsNum<NUM_VALIDE_LEAST)//没有或数据很少
		return;

	//计算个数
	int pointCount;//点个数
	int dataPointNum = pCurveData->DataPointsNum;//原始数据点个数
	if (dataPointNum < COUNT_POINT_MAX_MIN * 2)//点数较少，直接复制，不进行特别处理
	{
		//pointCount = dataPointNum;
		//分配内存
		if (pCurveData->dataCurveCount < dataPointNum)//上次比本次点个数少，或第一次(此时为0)
		{
			if (NULL!= pCurveData->pDataCurve)//如果已存在，则删除
			{
				delete[] pCurveData->pDataCurve;
				pCurveData->pDataCurve = NULL;
			}
			pCurveData->pDataCurve= new CPoint[dataPointNum];

#ifdef TRACE_FIND_MEMORY
			m_pMainDlg->SetStatusBarText(_T("OnDataReady GetDataCurve") );
#endif
		}
		//
		pCurveData->dataCurveCount = dataPointNum;
		CPoint* pData = pCurveData->pDataCurve;
		WORD* pWord = pCurveData->pDataPoints;
		int i=dataPointNum-1;
		while(i >= 0)//2011.12.01
		{
			pData[i].x = i;
			pData[i].y = pWord[i];
			i--;//2011.12.01
		}//ewhile
	}
	else//
	{//总共取COUNT_POINT_MAX_MIN * 2个点，包括最大最小值，x:代表在原数据数组中的下标，y:代表对应的功率值（原始未除以1000）
		pointCount = COUNT_POINT_MAX_MIN;
		//分配内存
		if (pCurveData->dataCurveCount < COUNT_POINT_MAX_MIN * 2)//上次比本次点个数少，或第一次(此时为0)
		{
			if (NULL!= pCurveData->pDataCurve)//如果已存在，则删除
			{
				delete[] pCurveData->pDataCurve;
				pCurveData->pDataCurve = NULL;
			}
			pCurveData->pDataCurve= new CPoint[COUNT_POINT_MAX_MIN * 2];//2*:取最大最小两个点；2:首尾wcq2011.11.22

#ifdef TRACE_FIND_MEMORY
			m_pMainDlg->SetStatusBarText(_T("OnDataReady GetDataCurve else") );
#endif
		}//eif
		//
		pCurveData->dataCurveCount = COUNT_POINT_MAX_MIN * 2;//2*:取最大最小两个点；wcq2011.11.22
		CPoint* pData = pCurveData->pDataCurve;
		WORD* pWord = pCurveData->pDataPoints;
		float fStepCoeff = dataPointNum * 1.0 / (COUNT_POINT_MAX_MIN);
		int lastStepPoint = 0;
		int step = 0;
		int maxIndex,minIndex;//取点时某一象素对应的最大最小值点的数据数组下标.wcq2011.09.29
		int wMax,wMin;//某一象素对应的最大最小值点的值
		int index = 0;//下标
		for(int i=0; i< COUNT_POINT_MAX_MIN; i++)
		{
			maxIndex=lastStepPoint;//默认每一象素第一点
			minIndex=lastStepPoint;//默认每一象素第一点
			wMax=pWord[lastStepPoint];//默认每一象素第一点
			wMin=pWord[lastStepPoint];//默认每一象素第一点
			step =(i+1)*fStepCoeff;//不会超过最大点个数??????
			//DEBUGMSG(true,(_T("step:%d\n"),step));
			for(int j=lastStepPoint+1;j<step;j++)
			{
				if (j == dataPointNum)//防止越界
				{
					break;
				}//eif
				if (pWord[j] > wMax)
				{
					wMax = pWord[j];
					maxIndex=j;
				}
				else if (pWord[j] < wMin)
				{
					wMin = pWord[j];
					minIndex=j;
				}//eif
				//DEBUGMSG(true,(_T("CSorDrawStatic::GetDataCurve dataPointNum:%d  i:%d  j:%d index:%d\n"),dataPointNum,i,j,i * 2 + 1));
			}//efor
			//顺序放到数组
			if (minIndex < maxIndex)
			{
				pData[index].x = minIndex;
				pData[index++].y = wMin;//++:转到下一点
				pData[index].x = maxIndex;
				pData[index++].y = wMax;
			}
			else
			{
				pData[index].x = maxIndex;
				pData[index++].y = wMax;
				pData[index].x = minIndex;
				pData[index++].y = wMin;
			}//eif
			lastStepPoint= step;//保存本次的最后一点
		}//efor
		//std::vector<CPoint> vp;
		//vp.assign(pData,pData+pointCount);
	}//eif
}
//每次加载或读到一条新的曲线数据时重新计算A/B标位置，并确定最长距离的曲线波长，赋给变量m_curABWave.
//如果有更长距离曲线到来，则重新计算缩放参数
//wcq 2011.0913
void CSorDrawStatic::ReCalABLabelPos()
{
	CalMaxDistance();//先计算所有曲线中最长的距离并赋给所有曲线结构相应变量

	BOOL bTheSame=TRUE;//最长距离的曲线是否和上次的一样，默认一样
	DWORD maxWave=0;//最长距离曲线对应的map的波长键值
	//通过遍历确定是否一样
	for(int nCount = 0; nCount < g_sorFileArray.totalCurveNumber; nCount++)
	{
		TestResultInfo* pCurve = g_sorFileArray.sorFileList[nCount].pTestResultInfo;
		if (pCurve && pCurve->IsMaxLength)
		{
			if (nCount != m_nMaxIndex)
			{
				bTheSame = FALSE;
				m_nMaxIndex = nCount;
				break;
			}
		}
	}

	//如果不一样，依次计算除最长距离外相应曲线的A/B标位置
	if (bTheSame)//一样，返回
	{
		return;
	}
	ReCalZoomParas();//有更长距离曲线数据到来，重新计算缩放相关参数。//wcq2011.09.23
	//先计算新的最长距离的A/B标位置
	//距离作为比例
	//float fTemp=m_pMapCurveData[maxWave]->m_TestResult.Frequency * m_pMapCurveData[maxWave]->m_TestResult.Refraction * m_pMapCurveData[m_curABWave]->m_TestResult.DataPointsNum;//wcq2011.11.25
	//fTemp=fTemp/(m_pMapCurveData[m_curABWave]->m_TestResult.Frequency * m_pMapCurveData[m_curABWave]->m_TestResult.Refraction * m_pMapCurveData[maxWave]->m_TestResult.DataPointsNum);//wcq2011.11.25
	float fTemp=g_sorFileArray.sorFileList[m_nMaxIndex].pTestResultInfo->Length / g_sorFileArray.sorFileList[g_sorFileArray.curSelection].pTestResultInfo->Length;//wcq2011.11.25
	m_nPointA=fTemp * m_nPointA;
	m_nPointB=fTemp * m_nPointB;
}

////按下回车将曲线回到未缩放/平移状态 
void CSorDrawStatic::OnVKReturn()
{
	//水平缩放还原
	m_iHorizonZoomNum = 0;
	HorizonZooms();//wcq2011.09.13

	//竖直方向还原
	for(int nCount = 0; nCount < g_sorFileArray.totalCurveNumber; nCount++)
	{
		TestResultInfo* pCurve = g_sorFileArray.sorFileList[nCount].pTestResultInfo;
		if (pCurve)//wcq2012.01.12
		{
			pCurve->YOffset = 0;
		}//eif
	}//efor

	//竖直方向缩放还原
	m_iVerticalZoomNum = 0;
	VerticalZoom();

	//重绘
	InvalidateAll();
}

/*
***************************************************************
函数名称:             OnLButtonDown                                                                              
函数说明:             按下鼠标，判断AB线是否被选中，是，m_bSelecedAB=TRUE；否
保存鼠标按下的点，做为拉框放大矩形框的一个顶点                                                                          
入口参数:             UINT nFlags, CPoint point                                                                                      
出口参数:             无                                                                                        
建立时间:             2012-06-28                                                                 
Write By:             XJF                                                                                          
***************************************************************
*/
void CSorDrawStatic::OnLButtonDown(UINT nFlags, CPoint point) 
{
	//没有数据时直接返回
	if (g_sorFileArray.totalCurveNumber == 0)
		return;

	if (!PtInRect(&m_rectCoord, point))
	{
		return;
	}

	if(IsLineABSeleced( point))
	{
		m_bZoomOK = FALSE;
		InvalidateRect(m_rectCurve);
		//InitialReturnInfo();  //如果点中了A杆或B杆要更新信息
		EventIsSelected(point.x); //选中标杆后，事件表中的对应事件被选中
	}
	else
	{
		//m_nSelectCursor = CURSOR_SELECT::CURSOR_NONE;
		m_bZoomOK = TRUE;
		m_PLMouseDown = point;
	}
	CButton::OnLButtonDown(nFlags, point);
}

/*
***************************************************************
函数名称:             OnLButtonUp                                                                              
函数说明:            鼠标弹起，对鼠标拖动过程中产生的矩形框选定的点进行放大，对鼠标扑捉进行释放                                                  
入口参数:             UINT nFlags, CPoint point                                                                                      
出口参数:             无                                                                                        
建立时间:             2012-06-28                                                                 
Write By:             XJF                                                                                          
***************************************************************
*/
void CSorDrawStatic::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if (g_sorFileArray.totalCurveNumber == 0) //没有曲线时
	{
		m_RectSelected.SetRectEmpty();
		InvalidateRect(&m_rectCurve);
		return;
	}

	if (m_RectSelected.Width() > 15 && m_bZoomOK)
	{
		if (g_sorFileArray.totalCurveNumber == 0)
			return;
		TestResultInfo* pData = g_sorFileArray.sorFileList[g_sorFileArray.curSelection].pTestResultInfo;

		int iStartPoint;//保存放大前原始的纵轴横轴位置
		int iMaxPoint;
		float fTemptDB;
		float fStartDB,fMaxDB;//保存放大前原始的纵轴横轴位置		
		fStartDB = m_fStartDB;
		fMaxDB = m_fMaxDB;
		fTemptDB = m_fStartDB;
		iStartPoint = m_nStartPoint;
		iMaxPoint = m_nMaxPoint;

		m_nStartPoint=m_nStartPoint+(m_nMaxPoint-m_nStartPoint)*(m_RectSelected.left-m_rectCoord.left)/m_rectCoord.Width();
		m_nMaxPoint=iStartPoint+(m_nMaxPoint-iStartPoint)*(m_RectSelected.right-m_rectCoord.left)/m_rectCoord.Width();

		m_nStartPoint=m_nStartPoint<m_nMaxPoint?m_nStartPoint:m_nMaxPoint;
		if (m_nStartPoint==m_nMaxPoint) 
		{		
			m_nStartPoint=m_nMaxPoint-1;	

		}
		m_nMaxPoint=m_nMaxPoint>(unsigned int)iMaxPoint?(unsigned int)iMaxPoint:m_nMaxPoint;

		m_fStartDB=m_fStartDB+(m_fMaxDB-m_fStartDB)*(m_rectCoord.bottom-m_RectSelected.bottom)/m_rectCoord.Height();
		m_fMaxDB=fTemptDB+(m_fMaxDB-fTemptDB)*(m_rectCoord.bottom-m_RectSelected.top)/m_rectCoord.Height();

		if (int(m_fMaxDB/(m_fMaxDB-m_fStartDB))>20)
		{
			m_fMaxDB=fMaxDB;
			m_fStartDB=fStartDB;
		}
		m_iHorizonZoomNum=2;
		m_iVerticalZoomNum=2;

		//重新设置绘图区域大小
		if (m_iHorizonZoomNum == 0) //无缩放
		{
			m_nDrawWidth = m_rectCurve.Width();
		}
		else
		{
			m_nDrawWidth = m_rectCurve.Width() * m_iHorizonZoomNum;
		}
		//设置水平滚动条
		SetHorScroll();

		//重新设置绘图区域大小
		if (m_iVerticalZoomNum == 0) //无缩放
		{
			m_fCoeff = 1.0;
			m_nDrawHeight = m_rectCurve.Height();
		}
		else
		{
			m_fCoeff = (m_fMaxDB - m_fStartDB) / (M_FMAXDB0 - M_FSTARTDB0);
			m_nDrawHeight = m_rectCurve.Height()/m_fCoeff;
		}
		SetVerScoll();
	}
	
	InvalidateRect(&m_rectCurve);
	MAIN->m_pThumbFormView->Invalidate();//重绘缩略图
	m_RectSelected.SetRectEmpty();
	//m_nSelectCursor = CURSOR_SELECT::CURSOR_NONE;//作用和m_bIsLineSelected重合，为了方便理解，故此没有删除
	m_bZoomOK = FALSE;	
	m_PLMouseDown = NULL;
	CButton::OnLButtonUp(nFlags, point);
}

/*
***************************************************************
函数名称:             IsLineABSeleced                                                                              
函数说明:             按下鼠标，如果在A或B线附近,AB线被选中并返回TRUE，否则返回FALSE                                                                       
入口参数:             UINT nFlags, CPoint point                                                                                      
出口参数:             无                                                                                        
建立时间:             2012-06-28                                                                 
Write By:             XJF                                                                                          
***************************************************************
*/
BOOL CSorDrawStatic::IsLineABSeleced(CPoint point)
{
	if (!PtInRect(&m_rectCoord, point))
	{
		return FALSE;
	}

	TestResultInfo* pData =  g_sorFileArray.sorFileList[m_nMaxIndex].pTestResultInfo;
	//确保m_nPointA 和m_nPoint不越界
	if (m_nPointA > pData->DataPointsNum-1)
	{
		m_nPointA = pData->DataPointsNum-1;
	}
	if (m_nPointB > pData->DataPointsNum-1)
	{
		m_nPointB = pData->DataPointsNum-1;
	}
	int SpaceAR = pData->ptDrawData[m_nPointA].x+10;
	int SpaceAL = pData->ptDrawData[m_nPointA].x-10;
	int SpaceBR = pData->ptDrawData[m_nPointB].x+10;
	int SpaceBL = pData->ptDrawData[m_nPointB].x-10;

	/*选中A*/ 

	if ((point.x < SpaceAR && point.x > SpaceAL) &&
		!(point.x < SpaceBR && point.x > SpaceBL))
	{
		//m_nSelectCursor = CURSOR_SELECT::CURSOR_A; //表示A线被选中
		return TRUE;
	}
	/*选中B*/ 
	else if (!(point.x < SpaceAR && point.x > SpaceAL)&&
		(point.x < SpaceBR && point.x > SpaceBL))
	{		
		//m_nSelectCursor = CURSOR_SELECT::CURSOR_B;//表示B线被选中
		return TRUE;
	}
	/*同时选中AB*/ 
	else if((point.x < SpaceAR && point.x > SpaceAL)&&
		(point.x < SpaceBR && point.x > SpaceBL))
	{
		/*移动过程中AB交汇的时候，如果先前选的是A*/ 
		//m_nSelectCursor = CURSOR_SELECT::CURSOR_AB;//表示AB线被同时选中
		return TRUE;
	}
	return FALSE;
}

/*
***************************************************************
函数名称:             OnMouseMove                                                                              
函数说明:             拖动鼠标，如果有AB线被选中，则移动AB线，否则会出现一个矩形框，鼠标弹起左键
弹起时，对矩形框中的点进行放大                                                             
入口参数:             UINT nFlags, CPoint point                                                                                      
出口参数:             无                                                                                        
建立时间:             2012-06-28                                                                 
Write By:             XJF                                                                                          
***************************************************************
*/
void CSorDrawStatic::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (g_sorFileArray.totalCurveNumber == 0) //没有曲线打开
		return;

	if (nFlags == MK_LBUTTON && PtInRect(&m_rectCoord,point))
	{
		if (!m_bZoomOK)  //如果AB线可以拖动
		{
			if (m_nSelectCursor == CURSOR_SELECT::CURSOR_A) //如果选中A线
			{
				m_nPointA = m_nStartPoint+(point.x - m_rectCoord.left)*(m_nMaxPoint - m_nStartPoint) / m_rectCoord.Width();
				EventIsSelected(m_nPointA);
				InvalidateRect(m_rectCoord);
				m_point_minus=(m_nPointB-m_nPointA);//AB线之间的相对距离，zll 2013.1.6
			}
			else if (m_nSelectCursor == CURSOR_SELECT::CURSOR_B)/*如果选中B线*/ 
			{
				m_nPointB = m_nStartPoint + (point.x - m_rectCoord.left)*(m_nMaxPoint - m_nStartPoint) / m_rectCoord.Width();

				EventIsSelected(m_nPointA);
				InvalidateRect(m_rectCoord);
				m_point_minus=(m_nPointB-m_nPointA);//AB线之间的相对距离，zll 2013.1.6
			}
			else if (m_nSelectCursor == CURSOR_SELECT::CURSOR_AB)/*如果选中AB线*/ 
			{
				m_nPointA = m_nStartPoint+(point.x - m_rectCoord.left)*(m_nMaxPoint - m_nStartPoint) / m_rectCoord.Width();
				//m_nPointB值需加上AB线之间的相对距离m_point_minus，zll 2013.1.6
				m_nPointB = m_nStartPoint + m_point_minus + (point.x - m_rectCoord.left)*(m_nMaxPoint - m_nStartPoint) / m_rectCoord.Width();
				InvalidateRect(m_rectCoord);
			}
		}

		else //AB不能拖动，局部放大
		{
			//SetCapture();
			long iTempMx = m_PLMouseDown.x;
			long iTempPx = point.x;
			long iTempMy = m_PLMouseDown.y;
			long iTempPy = point.y;
			point.x = point.x < m_rectCoord.right ? point.x : m_rectCoord.right;
			point.x = point.x > m_rectCoord.left ? point.x : m_rectCoord.left;
			point.y = point.y < m_rectCoord.bottom ? point.y : m_rectCoord.bottom;
			point.y = point.y > m_rectCoord.top ? point.y : m_rectCoord.top;


			if (iTempMx < iTempPx)//鼠标向右移动
			{
				if (iTempMy < iTempPy)//to right and down
				{			      
					CRect RectTemp(m_PLMouseDown, point);
					m_RectSelected = RectTemp;

				}
				else//to right and up
				{	
					CRect RectTemp(m_PLMouseDown.x, point.y, point.x, m_PLMouseDown.y);
					m_RectSelected = RectTemp;
				}
			}
			else//鼠标向左移动
			{
				if (iTempMy < iTempPy)//左下
				{
					CRect RectTemp(point.x, m_PLMouseDown.y, m_PLMouseDown.x, point.y);
					m_RectSelected = RectTemp;
				}
				else//left and up
				{
					CRect RectTemp(point, m_PLMouseDown);
					m_RectSelected = RectTemp;
				}

			} 
			InvalidateRect(m_rectCoord);
		}	
	}

	CButton::OnMouseMove(nFlags, point);
}

TestResultInfo* CSorDrawStatic::GetMaxLengthCurveData()//获取具有最长距离的曲线
{
	if (g_sorFileArray.totalCurveNumber == 0)//没有曲线
		return NULL;

	return g_sorFileArray.sorFileList[m_nMaxIndex].pTestResultInfo;
}

/*
***************************************************************
入口参数:maxDistanceDataCount:最长距离曲线的数据点数
出口参数:缩放次数
建立时间:2011.09.20
Write By:wcq 
函数说明:得到曲线缩放次数,1~19:1倍；20~29:2倍；30~39:3倍；40~49:4倍
***************************************************************
*/
int CSorDrawStatic::GetZoomCount(int maxDistanceDataCount)
{
	//int pointCountPerTime=m_rectCurve.Width();//缩放点数因子,686
	int pointCountPerTime=m_rectCoord.Width();//缩放点数因子,686//wcq2011.11.22
	int zoomCount=0;//缩放次数
	int count;
	if (pointCountPerTime * (20 + ZOOM_LAST_COEFF) >= maxDistanceDataCount)//1~19
	{
		zoomCount=0;
		for(int i=1;i<20;i++)
		{
			zoomCount++;
			if(pointCountPerTime * (i + ZOOM_LAST_COEFF) >= maxDistanceDataCount)
			{
				break;
			}
		}
	}
	else if (pointCountPerTime * 40 + pointCountPerTime * 2 * ZOOM_LAST_COEFF>= maxDistanceDataCount)//20~29
	{
		zoomCount=19;
		count=pointCountPerTime * 20;
		for(int i=1;i<11;i++)
		{
			zoomCount++;
			if(count + pointCountPerTime * 2 * (i + ZOOM_LAST_COEFF) >= maxDistanceDataCount)
			{
				break;
			}
		}
	}
	else if (pointCountPerTime * 70 + pointCountPerTime * 3 * ZOOM_LAST_COEFF>= maxDistanceDataCount)//30~39
	{
		zoomCount=29;
		count=pointCountPerTime * 40;
		for(int i=1;i<11;i++)
		{
			zoomCount++;
			if(count + pointCountPerTime * 3 * (i + ZOOM_LAST_COEFF) >= maxDistanceDataCount)
			{
				break;
			}
		}
	}
	else if (pointCountPerTime * 110 >= maxDistanceDataCount)//40~49 49:个数达到96040
	{
		zoomCount=39;
		count=pointCountPerTime * 70;
		for(int i=1;i<11;i++)
		{
			zoomCount++;
			if(count + pointCountPerTime * 4 * (i + ZOOM_LAST_COEFF) >= maxDistanceDataCount)
			{
				break;
			}
		}
	}
	else if (pointCountPerTime * 160 >= maxDistanceDataCount)//50~59 59:个数达到109760 wcq2012.03.07
	{
		zoomCount=49;
		count=pointCountPerTime * 110;
		for(int i=1;i<11;i++)
		{
			zoomCount++;
			if(count + pointCountPerTime * 5 * (i + ZOOM_LAST_COEFF) >= maxDistanceDataCount)
			{
				break;
			}
		}
	}
	else if (pointCountPerTime * 220 >= maxDistanceDataCount)//60~69 69:个数达到150920 wcq2012.03.07
	{
		zoomCount=59;
		count=pointCountPerTime * 160;
		for(int i=1;i<11;i++)
		{
			zoomCount++;
			if(count + pointCountPerTime * 6 * (i + ZOOM_LAST_COEFF) >= maxDistanceDataCount)
			{
				break;
			}
		}
	}
	return zoomCount;
}
/*
***************************************************************
入口参数:
出口参数:
建立时间:2011.09.23
Write By:wcq 
函数说明:得到所有曲线缩放最后一次缩放的数据点个数
***************************************************************
*/
void CSorDrawStatic::GetLastZoomCounts()
{
	//
	for(int nCount = 0; nCount < g_sorFileArray.totalCurveNumber; nCount++)
	{
		TestResultInfo* pCurve = g_sorFileArray.sorFileList[nCount].pTestResultInfo;
		if (pCurve && pCurve->DataPointsNum>NUM_VALIDE_LEAST)
		{
			pCurve->LastZoomPointCount = GetLastZoomCount(pCurve->FullPointCountMin,pCurve->DataPointsNum,m_nZoomCount);
			//DEBUGMSG(true,(_T("lastzoompointcount:%d wave:%d\n"),pCurve->m_TestResult.LastZoomPointCount,it->first));
		}
	}
}
/*
***************************************************************
入口参数:countPerTime:正常情况下每次缩放点数;dataCount:数据点数总数;zoomCount:缩放次数
出口参数:缩放次数
建立时间:2011.09.21
Write By:wcq 
函数说明:得到曲线缩放最后一次缩放的数据点个数wcq 
***************************************************************
*/
int CSorDrawStatic::GetLastZoomCount(int countPerTime,int dataCount,int zoomCount)
{
	int count = 0;
	int nTemp;
	if (zoomCount < 20)
	{
		count = dataCount % countPerTime;
		nTemp = 1;
	}
	else if (zoomCount < 30)
	{
		count = (dataCount - countPerTime * 20) % (countPerTime * 2);
		nTemp = 2;
	}
	else if (zoomCount < 40)
	{
		count = (dataCount - countPerTime * 40) % (countPerTime * 3);
		nTemp = 3;
	}
	else if (zoomCount < 50)//40~49
	{
		count = (dataCount - countPerTime * 70) % (countPerTime * 4);
		nTemp = 4;
	}
	else if (zoomCount < 60)//50~59  wcq2012.03.07
	{
		count = (dataCount - countPerTime * 110) % (countPerTime * 5);
		nTemp = 5;
	}
	else//60~69  wcq2012.03.07
	{
		count = (dataCount - countPerTime * 160) % (countPerTime * 6);
		nTemp = 6;
	}
	if (0 == count)//刚好整除
	{
		count += countPerTime * nTemp;
	}
	else if (count < countPerTime * nTemp * ZOOM_LAST_COEFF)//小于一定比例则当作上一次的
	{
		count += countPerTime * nTemp;
	}
	return count;
}
//计算所有曲线未缩放时绘满绘图区域的点数 wcq 2011.09.21
void CSorDrawStatic::CalFullPointCount()
{
	for(int nCount = 0; nCount < g_sorFileArray.totalCurveNumber; nCount++)
	{
		TestResultInfo* pData = g_sorFileArray.sorFileList[nCount].pTestResultInfo;
		if (pData)
		{
			if (pData->IsMaxLength)
			{
				//pData->FullPointCountMin = m_rectCurve.Width();
				pData->FullPointCountMin = m_rectCoord.Width();
				//pData->FullPointCount = maxDistanceCount;
			}
			else{
				//不是最长距离的曲线要乘一个比例,m_curABWave表示最长距离曲线所在波长
				//float fTemp=m_pMapCurveData[m_curABWave]->m_TestResult.Frequency * m_pMapCurveData[m_curABWave]->m_TestResult.Refraction * pData->DataPointsNum;//wcq2011.11.25
				//fTemp=fTemp/(pData->Frequency * pData->Refraction * m_pMapCurveData[m_curABWave]->m_TestResult.DataPointsNum);//wcq2011.11.25
				//pData->FullPointCountMin = m_rectCoord.Width() * fTemp;//wcq2011.11.25
				//根据同一屏(m_rectCoord.Width())上,两条曲线长度相等//wcq2011.11.29
				pData->FullPointCountMin = m_rectCoord.Width() * (g_sorFileArray.sorFileList[m_nMaxIndex].pTestResultInfo->DataSpacing  /  pData->DataSpacing);//wcq2011.11.29
			}
		}
	}
}
//第一次或有更长距离曲线数据到来，重新计算缩放相关参数。在里面调用GetZoomCount,CalFullPointCount,GetLastZoomCount.wcq2011.09.23
void CSorDrawStatic::ReCalZoomParas()
{
	CalMaxDistance();//先计算所有曲线中最长的距离并赋给所有曲线结构相应变量
	m_nZoomCount=GetZoomCount(g_sorFileArray.sorFileList[m_nMaxIndex].pTestResultInfo->DataPointsNum);//得到最大放大倍数
	CalFullPointCount();//计算所有曲线未缩放时绘满绘图区域的点数
	GetLastZoomCounts();//得到所有曲线缩放最后一次缩放的数据点个数
}
/*
***************************************************************
入口参数:pData:当前数据结构体
出口参数:
建立时间:2011.09.26
Write By:wcq 
函数说明:设置CCurveData中的IsLowerStandard变量，以方便曲线竖直方向缩放。为节约时间，每一水平铅象素只取一点
***************************************************************
*/
void CSorDrawStatic::SetLowerStandard(TestResultInfo* pData)
{
	int len=pData->DataPointsNum;
	int nStep=len / m_rectCoord.Width();
	pData->IsLowerStandard = FALSE;//所有都 >ZOOM_VERTICAL_STANDARD
	for (int i=0;i<len; i+=nStep)
	{
		if(pData->pDataPoints[i] > ZOOM_VERTICAL_STANDARD)//原始数据要/-1000
		{
			pData->IsLowerStandard = TRUE;//只要有一个小于就行
			break;
		}
	}
}
/*
***************************************************************
入口参数:
出口参数:
建立时间:2011.09.26
Write By:wcq 
函数说明:遍布曲线所有数据，看是否至少有IsLowerStandard为真
***************************************************************
*/
BOOL CSorDrawStatic::IsLowerStandardAll()
{
	BOOL re=FALSE;
	//
	for(int nCount = 0; nCount < g_sorFileArray.totalCurveNumber; nCount++)
	{
		TestResultInfo* pCurve = g_sorFileArray.sorFileList[nCount].pTestResultInfo;
		if (pCurve && pCurve->IsLowerStandard)
		{
			re = TRUE;
			break;
		}
	}
	return re;
}
/*
***************************************************************
入口参数:zoomNum:竖直方向缩放次数,fStandPosDb:中心点Db值
出口参数:
建立时间:2011.09.26
Write By:wcq
函数说明:计算竖直方向缩放时最大值最小值.赋给m_fStartDB,m_fMaxDB.放大缩小次数一样
1~19:0.8dB；20~29:1.6dB；30~34:2dB；35~42:3dB；43~50:4dB,最大98dB；最小0.8dB
***************************************************************
*/
void CSorDrawStatic::CalVerticalRange(int zoomNum,float fStandPosDb)
{
	float fRange=0;
	float fCoeff=1.0;

	//未放大，原始（默认）大小,根据是否都>某一标准dB(-30,ZOOM_VERTICAL_STANDARD)
	if (0 == zoomNum)//
	{
		if (IsLowerStandardAll())//
		{
			m_fStartDB = M_FSTARTDB0;
			m_fMaxDB = M_FMAXDB0;
		}
		else
		{
			m_fStartDB = M_FSTARTDB1;
			m_fMaxDB = M_FMAXDB1;
		}
		return;
	}
	int zoom = ZOOM_VERTICAL_COUNT - zoomNum;

	//进行过放大
	if (0 == zoom)//区间最小
	{
		fRange = ZOOM_VERTICAL_LEAST_RANGE;
	}
	else if (zoom <20)//1~19
	{
		fRange = zoom * ZOOM_VERTICAL_PER_TIME + ZOOM_VERTICAL_LEAST_RANGE;
	}
	else if (zoom <30)//20~29
	{
		fRange = 19 * ZOOM_VERTICAL_PER_TIME + ZOOM_VERTICAL_LEAST_RANGE + (zoom - 19) * 2 * ZOOM_VERTICAL_PER_TIME;
	}
	else if (zoom <40)//30~39
	{
		fRange = 19 * ZOOM_VERTICAL_PER_TIME + ZOOM_VERTICAL_LEAST_RANGE + 10 * 2 * ZOOM_VERTICAL_PER_TIME + (zoom - 29) * 3 * ZOOM_VERTICAL_PER_TIME;
	}
	else//40+
	{
		fRange = 19 * ZOOM_VERTICAL_PER_TIME + ZOOM_VERTICAL_LEAST_RANGE + 10 * 2 * ZOOM_VERTICAL_PER_TIME + 10 * 3 * ZOOM_VERTICAL_PER_TIME + (zoom - 39) * 4 * ZOOM_VERTICAL_PER_TIME;
	}

	//
	if (IsLowerStandardAll())//较低区间
	{
		fCoeff = (fStandPosDb - M_FSTARTDB0) / (M_FMAXDB0 - M_FSTARTDB0);//
	}
	else
	{
		fCoeff = (fStandPosDb - M_FSTARTDB1)/ (M_FMAXDB1 - M_FSTARTDB1);//
	}
	//;//上下比例
	float fTemp = fRange * fCoeff;//
	m_fStartDB = fStandPosDb - fTemp;
	m_fMaxDB = fStandPosDb + (fRange - fTemp);

	m_fCoeff = (m_fMaxDB - m_fStartDB) / (M_FMAXDB0 - M_FSTARTDB0);
}

void CSorDrawStatic::EventSelectUp()
{
	POSITION pos = MAIN->m_pEventFormView->m_Eventlist.GetFirstSelectedItemPosition();
	int nIndex = MAIN->m_pEventFormView->m_Eventlist.GetNextSelectedItem(pos);
	if (nIndex > 0)
	{
		nIndex--;
		MoveEventList(nIndex);
		//重置选中项
		MAIN->m_pEventFormView->m_Eventlist.SetItemState(nIndex, LVIS_SELECTED, LVIS_SELECTED);
	}
}

void CSorDrawStatic::EventSelectDown()
{
	POSITION pos = MAIN->m_pEventFormView->m_Eventlist.GetFirstSelectedItemPosition();
	int nIndex =  MAIN->m_pEventFormView->m_Eventlist.GetNextSelectedItem(pos);
	if (nIndex >= 0 && nIndex < MAIN->m_pEventFormView->m_Eventlist.GetItemCount()-1)
	{
		nIndex++;
		MoveEventList(nIndex);
		//重置选中项
		MAIN->m_pEventFormView->m_Eventlist.SetItemState(nIndex, LVIS_SELECTED, LVIS_SELECTED);
	}
	else
	{
		//置第一个为选中项
		if (nIndex > 0)
			MAIN->m_pEventFormView->m_Eventlist.SetItemState(0, LVIS_SELECTED, LVIS_SELECTED);
	}
}
//选择了事件列表后，重新绘制曲线;wcq2011-11.15
//nIndex:从0开始相应曲线第几个事件点
void CSorDrawStatic::MoveEventList(INT nIndex)
{
	TestResultInfo* pCurveData = g_sorFileArray.sorFileList[g_sorFileArray.curSelection].pTestResultInfo;
	if (1 == m_nSelectCursor) //选中A
	{
		m_nPointA = pCurveData->EventList.vEventInfo[nIndex].index;
	}	
	else if (2 == m_nSelectCursor)////选中B
	{
		m_nPointB = pCurveData->EventList.vEventInfo[nIndex].index;
	}
	else//AB
	{
		m_nPointA = pCurveData->EventList.vEventInfo[nIndex].index;
		m_nPointB = m_nPointA;
	}//eif
	HorizonZooms();//
	InvalidateAll();//wcq2011.11.16
}

void CSorDrawStatic::OnSize(UINT nType, int cx, int cy)
{
	CButton::OnSize(nType, cx, cy);

	m_nCurWidth = cx;
	m_nCurHeight = cy;
}

void CSorDrawStatic::SetHorScroll() //设置水平滚动条
{
	//水平滚动条
	SCROLLINFO siHor;
	siHor.cbSize = sizeof(SCROLLINFO);
	siHor.fMask = SIF_ALL; 
	siHor.nMin = 0;
	siHor.nMax = m_nDrawWidth-1;//这么做后windows会把最大的滚动条位置限制为si.nmax-si.npage+1,而不是si.nmax
	siHor.nPage = m_rectCurve.Width();
	siHor.nPos = 0;

	SetScrollInfo(SB_HORZ, &siHor, TRUE);
}

void CSorDrawStatic::SetVerScoll() //设置竖直滚动条
{
	SCROLLINFO siVer;
	siVer.cbSize = sizeof(SCROLLINFO);
	siVer.fMask = SIF_ALL; 
	siVer.nMin = 0;
	siVer.nMax = m_nDrawHeight;
	siVer.nPage = m_rectCurve.Height();
	siVer.nPos = 0;
	SetScrollInfo(SB_VERT, &siVer, TRUE);
}

void CSorDrawStatic::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	int nDelta;
	int nMaxPos = m_nDrawHeight - m_nCurHeight;

	switch (nSBCode)
	{
	case SB_LINEDOWN:
		if (m_nScrollPosVer >= nMaxPos)
			return;

		nDelta = min(max(nMaxPos/20,5),nMaxPos-m_nScrollPosVer);
		break;

	case SB_LINEUP:
		if (m_nScrollPosVer <= 0)
			return;
		nDelta = -min(max(nMaxPos/20,5),m_nScrollPosVer);
		break;
	case SB_PAGEDOWN:
		if (m_nScrollPosVer >= nMaxPos)
			return;
		nDelta = min(max(nMaxPos/10,5),nMaxPos-m_nScrollPosVer);
		break;
	case SB_THUMBTRACK:
	case SB_THUMBPOSITION:
		nDelta = (int)nPos - m_nScrollPosVer;
		break;

	case SB_PAGEUP:
		if (m_nScrollPosVer <= 0)
			return;
		nDelta = -min(max(nMaxPos/10,5),m_nScrollPosVer);
		break;

	default:
		return;
	}
	m_nScrollPosVer += nDelta;
	SetScrollPos(SB_VERT,m_nScrollPosVer,TRUE);
	//上线移动曲线
	SCROLLINFO siVer;
	siVer.cbSize = sizeof(SCROLLINFO);

	GetScrollInfo(SB_VERT,&siVer);
	int nRange = siVer.nMax - siVer.nMin;
	float fVerBalFactor = siVer.nPos * 1.0 / siVer.nPage; //比例系数；
	nDelta = nRange * fVerBalFactor;
	CurvesVerMove(nDelta);
	CButton::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CSorDrawStatic::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	int nDelta;
	int nMaxPos = m_nDrawWidth - m_nCurWidth;

	switch (nSBCode)
	{
	case SB_LINELEFT:
		if (m_nScrollPosHor <= 0)
			return;

		nDelta = -min(max(nMaxPos/20,5),m_nScrollPosHor);
		break;

	case SB_LINERIGHT:
		if (m_nScrollPosHor >= nMaxPos)
			return;

		nDelta = min(max(nMaxPos/20,5),nMaxPos-m_nScrollPosHor);
		break;
	case SB_PAGEDOWN:
		if (m_nScrollPosHor <= 0)
			return;
		nDelta = -min(max(nMaxPos/10,5),nMaxPos-m_nScrollPosHor);
		break;
	case SB_THUMBTRACK:
	case SB_THUMBPOSITION:
		nDelta = (int)nPos - m_nScrollPosHor;
		TRACE(_T("Delta:%d \n"),nDelta);
		break;

	case SB_PAGEUP:
		if (m_nScrollPosHor >= nMaxPos)
			return;
		nDelta = min(max(nMaxPos/10,5),m_nScrollPosHor);
		break;

	default:
		return;
	}
	m_nScrollPosHor += nDelta;
	TRACE(_T("Delta:%d \n"),m_nScrollPosHor);
	SetScrollPos(SB_HORZ,m_nScrollPosHor,TRUE);

	SCROLLINFO siHor;
	siHor.cbSize = sizeof(SCROLLINFO);

	GetScrollInfo(SB_HORZ,&siHor);
	int nRange = siHor.nMax - siHor.nMin;
	//float fHorBalFactor = siHor.nPos * 1.0 / (siHor.nMax-siHor.nMin); //比例系数；
	float fHorBalFactor = abs(m_nScrollPosHor) * 1.0 / (m_nCurWidth * m_iHorizonZoomNum); //比例系数 2013.3.14 席工拿回去后修改的
	TRACE(_T("nMin:%d \n"),siHor.nMin);
	TRACE(_T("nMax:%d \n"),siHor.nMax);
	TRACE(_T("nPos:%d \n"),siHor.nPos);
	TRACE(_T("nPage:%d \n"),siHor.nPage);
	TRACE(_T("fHorBalFactor:%f \n"),fHorBalFactor);
	CurvesHorMove(nRange, fHorBalFactor);
	CButton::OnHScroll(nSBCode, nPos, pScrollBar);
}