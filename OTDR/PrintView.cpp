// PrintView.cpp : implementation file
//

#include "stdafx.h"

#include "PrintView.h"
#include "MainFrm.h"
#include <math.h>
#include "CursorInfo.h"

extern LANG g_currentLang;
extern SorFileArray g_sorFileArray;
extern CursorInfo g_cursorInfo;

#define PRINTMARGIN 2
/////////////////////////////////////////////////////////////////////////////
// CPrintView

IMPLEMENT_DYNCREATE(CPrintView, CView)

CPrintView::CPrintView()
{
	m_bPrint = TRUE;
	m_bPrintCurve = TRUE;
	m_bPrintHead = TRUE;
}

CPrintView::~CPrintView()
{
}


BEGIN_MESSAGE_MAP(CPrintView, CView)
	//ON_COMMAND(ID_FILE_PRINT, CPrintView::OnFilePrint) 
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPrintView drawing



void CPrintView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CPrintView diagnostics

#ifdef _DEBUG
void CPrintView::AssertValid() const
{
	CView::AssertValid();
}

void CPrintView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CPrintView message handlers
void CPrintView::OnMyPrint()
{
	//GetParent()->ShowWindow(SW_SHOWMINIMIZED);
	m_bPrint = TRUE;
	{
		CView::OnFilePrint();
	}

}
void CPrintView::OnMyPrintPreview()
{
	//GetParent()->ShowWindow(SW_SHOWMAXIMIZED);
	m_bPrint=FALSE;
	CView::OnFilePrintPreview();
}

BOOL CPrintView::OnPreparePrinting(CPrintInfo* pInfo) 
{
	//打印单个报表用竖打，打印多个报表用横打 lzy2014.8.28
	int nCurveNumber = _wtoi(g_sorFileArray.reportConfig.WaveList);	
	LPDEVMODE lpDevMode;
	PRINTDLG pd = pInfo->m_pPD->m_pd;
	if(AfxGetApp()->GetPrinterDeviceDefaults(&pd))
	{
		lpDevMode = (LPDEVMODE)GlobalLock(pd.hDevMode);
		if(lpDevMode)
		{
			lpDevMode->dmPaperSize = DMPAPER_A4;
			if((!nCurveNumber ) || (2 == nCurveNumber) || m_bPrintCurve ) //单个报表,4个报表
			{
				lpDevMode->dmOrientation = DMORIENT_PORTRAIT; //纵向打印
			}
			else  //多个报表
			{
				lpDevMode->dmOrientation = DMORIENT_LANDSCAPE; //横向打印
			}			
		}
		GlobalUnlock(pd.hDevMode);
	}	
	
	if(DoPreparePrinting(pInfo))
		return TRUE;
	else
	{
		return FALSE;
	}
}

void CPrintView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo) 
{
	CView::OnBeginPrinting(pDC, pInfo);

	InitFont(pDC);

	TEXTMETRIC tm;
	CFont* pOldFont = pDC->SelectObject (&m_fontPrinter);
	pDC->GetTextMetrics (&tm);
	m_cyPrinter = tm.tmHeight + tm.tmExternalLeading;
	CSize size = pDC->GetTextExtent (_T ("---------1---------2---------3---------4---------5---------6---------7---------8---"), 81);
	pDC->SelectObject (pOldFont);
	m_cxWidth = size.cx;

	m_nLinesPerPage = (pDC->GetDeviceCaps (VERTRES) -
		(m_cyPrinter * (3 + (2 * PRINTMARGIN)))) / m_cyPrinter;

	TRACE(_T("PrintType=%d\n"), MAIN->printType);
	if (1==MAIN->printType)  //zll 2012.10.29 printType=1时，打印报表向导中所有曲线文件
	{
		int nCurveNumberPerPage=0;//打印每页显示几个波形
		int nCurveNumber = _wtoi(g_sorFileArray.reportConfig.WaveList);
		if (nCurveNumber == 0)
			nCurveNumberPerPage=1;
		else if (nCurveNumber == 1)
			nCurveNumberPerPage=2;
		else if (nCurveNumber == 2)
			nCurveNumberPerPage=4;
		else if (nCurveNumber == 3)
			nCurveNumberPerPage=6;
		else if (nCurveNumber == 4)
			nCurveNumberPerPage=8;
		int maxPage=g_sorFileArray.totalCurveNumber/nCurveNumberPerPage;//配置页数
		if (0!=g_sorFileArray.totalCurveNumber%nCurveNumberPerPage)//非整数倍数页数则加一页
		{
			maxPage+=1;
		}
		pInfo->SetMaxPage (maxPage);
	}
	else if (0==MAIN->printType) //zll 2012.10.29
	{
        pInfo->SetMaxPage(1);
	}

	m_cxOffset = (pDC->GetDeviceCaps (HORZRES) - size.cx) / 2;
	//设置打印区域
	m_rectDraw.SetRect(0,0,pDC->GetDeviceCaps (HORZRES), pDC->GetDeviceCaps (VERTRES));
}

void CPrintView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo) 
{
	CView::OnEndPrinting(pDC, pInfo);

	if(m_fontPrinter.m_hObject!=NULL)
		m_fontPrinter.DeleteObject ();

	if(m_dataFont.m_hObject!=NULL)
		m_dataFont.DeleteObject();

	if(m_FontVR.m_hObject!=NULL)
		m_FontVR.DeleteObject();
}

void CPrintView::OnPrint(CDC* pDC, CPrintInfo* pInfo) 
{
	TRACE(_T("%d\n"),pInfo->m_nCurPage);
	if(m_bPrintHead)
		PrintPageHeader(pDC,pInfo->m_nCurPage,  pInfo);
	InnerDraw(pDC,pInfo->m_nCurPage);
}

void CPrintView::OnEndPrintPreview(CDC* pDC, CPrintInfo* pInfo, POINT point, CPreviewView* pView) 
{
	CView::OnEndPrintPreview(pDC, pInfo, point, pView);
	//恢复窗口最大化
	//MAIN->ShowReport(TRUE);
	//打印
	//OnMyPrint();//zll 2013.6.26

}

void CPrintView::PrintPageHeader(CDC* pDC, UINT nPageNumber, CPrintInfo* pInfo)
{
	CFont *pOldFont;
	CFont fontHeader;
	fontHeader.CreatePointFont(100,_T("FixedSys"),pDC);//
	//fontHeader.CreatePointFont(100,_T("宋体"),pDC);//
	pOldFont=(CFont *)(pDC->SelectObject(&fontHeader));
	int nPosY,nPosX;
	nPosY=m_cyPrinter;
	nPosX=m_cxOffset;
	pDC->TextOut(nPosX,nPosY,CString(_T("")));
	//CString str;
	//str.Format(_T("第 %d 页 / 共 %d 页"),nPageNumber,pInfo->GetMaxPage());
	//CSize size=pDC->GetTextExtent(str);
	//nPosX=m_cxOffset+m_cxWidth-size.cx;
	//pDC->TextOut(nPosX,nPosY,str);
	//pDC->MoveTo(m_cxOffset,1.5*m_cyPrinter);
	//pDC->LineTo(m_cxOffset+m_cxWidth,1.5*m_cyPrinter);
	pDC->SelectObject(pOldFont);
	fontHeader.DeleteObject();
}

void CPrintView::DrawReport(CDC* pDC)
{
	CFont* pOldFont = pDC->SelectObject (&m_dataFont);
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
	CRect titleRect;
	titleRect.SetRect(m_rectDraw.Width() / 3, nCurHeight-nVspace, m_rectDraw.Width() * 2/3, nCurHeight);
	pDC->DrawText(g_sorFileArray.reportConfig.Title, titleRect, DT_CENTER);
	pDC->SelectObject (&m_dataFont);
	//内容
	for (int i=1; i<=6; i++)
	{
		nCurHeight = nCurHeight + nVspace;
		pDC->MoveTo(nHleftspace, nCurHeight);
		pDC->LineTo((m_rectDraw.Width() - nHmidspace)/2 ,nCurHeight);
		
		CRect itemRect;
		itemRect.SetRect(nHleftspace, nCurHeight - nVspace,(m_rectDraw.Width() - nHmidspace)/2, nCurHeight);
		DrawReportItem(pDC, itemRect, i*2-1);

		pDC->MoveTo((m_rectDraw.Width() + nHmidspace)/2, nCurHeight);
		pDC->LineTo((m_rectDraw.Width() - nHleftspace), nCurHeight);
		itemRect.SetRect((m_rectDraw.Width() + nHmidspace)/2, nCurHeight - nVspace,(m_rectDraw.Width() - nHleftspace), nCurHeight);
		DrawReportItem(pDC, itemRect, i*2);
	}
	//大表格
	{
		nCurHeight = nCurHeight + nVspace * 1.5;
		int nCurColCoord = nCurHeight + nVspace * 1.5;//当前表格纵坐标
		//绘制报表内容
		DrawReportContent(pDC, nHleftspace, nCurHeight);
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
			if (col > 1 &&
				col <= 9)
			{
				//int nListFormat = _wtoi(g_sorFileArray.reportConfig.ListFormat);
				//if (nListFormat == 1 ||
				//	nListFormat == 2)
				{
					pDC->MoveTo(nCurRowCoord, nCurColCoord + nVspace*1.5);
					pDC->LineTo(nCurRowCoord, nCurHeight);
				}
				//else
				//{
				//	pDC->MoveTo(nCurRowCoord, nCurColCoord);
				//	pDC->LineTo(nCurRowCoord, nCurHeight);
				//}
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
	pDC->SelectObject (pOldFont);
}
/**********************************************************************************/
/* 绘制报表项                                                                     */
/* param nItemIndex 报表项索引号 1-12                                            */
/*********************************************************************************/
void CPrintView::DrawReportItem(CDC* pDC, CRect& rect, int nItemIndex) //绘制报表项目
{
	CFont* pOldFont = pDC->SelectObject (&m_dataFont);
	CString strItemText;
	switch (nItemIndex)
	{
	case 1:
		{
			strItemText.Format(_T("%s:    %s"), g_sorFileArray.reportConfig.Item1, g_sorFileArray.reportConfig.Item1Val);
			break;
		}
	case 2:
		{
			strItemText.Format(_T("%s:    %s"), g_sorFileArray.reportConfig.Item2, g_sorFileArray.reportConfig.Item2Val);
			break;
		}
	case 3:
		{
			strItemText.Format(_T("%s:    %s"), g_sorFileArray.reportConfig.Item3, g_sorFileArray.reportConfig.Item3Val);
			break;
		}
	case 4:
		{
			strItemText.Format(_T("%s:    %s"), g_sorFileArray.reportConfig.Item4, g_sorFileArray.reportConfig.Item4Val);
			break;
		}
	case 5:
		{
			strItemText.Format(_T("%s:    %s"), g_sorFileArray.reportConfig.Item5, g_sorFileArray.reportConfig.Item5Val);
			break;
		}
	case 6:
		{
			strItemText.Format(_T("%s:    %s"), g_sorFileArray.reportConfig.Item6, g_sorFileArray.reportConfig.Item6Val);
			break;
		}
	case 7:
		{
			strItemText.Format(_T("%s:    %s"), g_sorFileArray.reportConfig.Item7, g_sorFileArray.reportConfig.Item7Val);
			break;
		}
	case 8:
		{
			strItemText.Format(_T("%s:    %s"), g_sorFileArray.reportConfig.Item8, g_sorFileArray.reportConfig.Item8Val);
			break;
		}
	case 9:
		{
			strItemText.Format(_T("%s:    %s"), g_sorFileArray.reportConfig.Item9, g_sorFileArray.reportConfig.Item9Val);
			break;
		}
	case 10:
		{
			strItemText.Format(_T("%s:    %s"), g_sorFileArray.reportConfig.Item10, g_sorFileArray.reportConfig.Item10Val);
			break;
		}
	case 11:
		{
			strItemText.Format(_T("%s:    %s"), g_sorFileArray.reportConfig.Item11, g_sorFileArray.reportConfig.Item11Val);
			break;
		}
	case 12:
		{
			strItemText.Format(_T("%s:    %s"), g_sorFileArray.reportConfig.Item12, g_sorFileArray.reportConfig.Item12Val);
			break;
		}
	}
	pDC->DrawText(strItemText, rect, DT_LEFT|DT_BOTTOM|DT_SINGLELINE);
	//pDC->ExtTextOut(rect.left,rect.top + 80,ETO_CLIPPED,rect,strItemText,NULL);
	pDC->SelectObject (pOldFont);
}

/**********************************************************************************/
/* 绘制每条曲线项目                                                                     */
/* param nItemIndex 曲线项目索引号 1-3                                            */
/*********************************************************************************/
void CPrintView::DrawCurveItem(CDC* pDC, CRect& rect, int nItemIndex,int m_CurveNum,CString m_sorName) //绘制曲线项目
{
	CFont* pOldFont = pDC->SelectObject (&m_dataFont);
	CString strItemText;
	CString str_m_CurveNum=_T("");
	str_m_CurveNum.Format(_T("%d"),m_CurveNum);
	switch (nItemIndex)
	{
	/*case 1:
		{
			strItemText.Format(_T("%s    %s"), g_sorFileArray.reportConfig.Step3Item1, g_sorFileArray.reportConfig.Step3Edit1);
			break;
		}*/ //zll 2012.9.27注释
	case 2:
		{
			strItemText.Format(_T("%s:%s  %s"), g_sorFileArray.reportConfig.Step3Item2, str_m_CurveNum, m_sorName);
			break;
		}
	/*case 3:
		{
			strItemText.Format(_T("%s    %s"), g_sorFileArray.reportConfig.Step3Item3, g_sorFileArray.reportConfig.Step3Edit3);
			break;
		}*/ //zll 2012.9.27注释
	}
	pDC->DrawText(strItemText, rect, DT_LEFT|DT_BOTTOM|DT_SINGLELINE);
	//pDC->ExtTextOut(rect.left,rect.top + 80,ETO_CLIPPED,rect,strItemText,NULL);
	pDC->SelectObject (pOldFont);
}

/**********************************************************************************/
/* 绘制报表内容                                                                     */
/* param                                            */
/*********************************************************************************/
void CPrintView::DrawReportContent(CDC* pDC, int nLeft, int nTop) //绘制报表内容
{
	CFont* pOldFont = pDC->SelectObject (&m_dataFont);
	int nVspace = m_rectDraw.Height() / 42;
	int nHspace = m_rectDraw.Width() / 10;
	int nStartLine = 1;

	CString strText;
	CRect textRect;

	// 报表标题
	{
		textRect.left = nLeft + 3 * nHspace;
		textRect.top = nTop + nStartLine * nVspace;
		textRect.right = nLeft + 8 * nHspace;
		textRect.bottom = nTop + (nStartLine+1) * nVspace;
		int nListFormat = _wtoi(g_sorFileArray.reportConfig.ListFormat);

		switch (nListFormat)
		{
		case 0:
			strText = _T("损耗");
			break;
		case 1:
			strText = _T("损耗及回波损耗");
			break;
		case 2:
			strText = _T("损耗及反射");
			break;
		case 3:
			strText = _T("");
			break;
		}
		pDC->ExtTextOut(textRect.left,textRect.top + 80,ETO_CLIPPED,textRect,strText,NULL);
		//pDC->DrawText(strText, &textRect,DT_CENTER|DT_BOTTOM);//文字
	}
	
	pDC->SelectObject (pOldFont);
}

/**********************************************************************************/
/* 绘制波形横坐标文本                                                             */
/* param rect 绘制矩形区域                                                       */
/*********************************************************************************/
void CPrintView::DrawHorCoordUnit(CDC* pDC, CRect& rect, CString strText) //绘制波形横坐标文本
{
	CFont* pOldFont = pDC->SelectObject (&m_dataFont);
	rect.OffsetRect(0, 50);
	pDC->DrawText(strText, rect, DT_CENTER | DT_BOTTOM);
	pDC->SelectObject (pOldFont);
}

/**********************************************************************************/
/* 绘制波形纵坐标文本                                                             */
/* param rect 绘制矩形区域                                                       */
/*********************************************************************************/
void CPrintView::DrawVerCoordUnit(CDC* pDC, CRect& rect) //绘制波纵横坐标文本
{
	CFont* pOldFont = pDC->SelectObject (&m_dataFont);
	CString strUnitText = MAIN->m_pWaveFormView->m_strUnitDiv;
	rect.OffsetRect(0, 50);
	pDC->DrawText(strUnitText, rect, DT_CENTER | DT_BOTTOM);
	pDC->SelectObject (pOldFont);
}


void CPrintView::Draw1Curve(CDC* pDC,int m_nCurPage)
{
	int m_CurveNum=m_nCurPage-1;
	//画线
	int nVspace = m_rectDraw.Height() / 64;
	int nCurHeight = nVspace * 3;
	//int nHleftspace = m_rectDraw.Width() / 10;
	int nHleftspace = m_rectDraw.Width() / 18; //增加显示宽度lzy2014.7.23
	int nHmidspace = m_rectDraw.Width() / 20;
	//标题
	pDC->MoveTo(m_rectDraw.Width() / 3, nCurHeight);
	pDC->LineTo(m_rectDraw.Width() * 2/3, nCurHeight);
	CRect titleRect;
	titleRect.SetRect(m_rectDraw.Width() / 3, nCurHeight-nVspace, m_rectDraw.Width() * 2/3, nCurHeight);
	pDC->DrawText(g_sorFileArray.reportConfig.Title, titleRect, DT_CENTER);
	//内容
	for (int i=1; i<=6; i++)
	{
		nCurHeight = nCurHeight + nVspace;
		pDC->MoveTo(nHleftspace, nCurHeight);
		pDC->LineTo((m_rectDraw.Width() - nHmidspace)/2 ,nCurHeight);

		CRect itemRect;
		itemRect.SetRect(nHleftspace, nCurHeight - nVspace,(m_rectDraw.Width() - nHmidspace)/2, nCurHeight);
		DrawReportItem(pDC, itemRect, i*2-1);

		pDC->MoveTo((m_rectDraw.Width() + nHmidspace)/2, nCurHeight);
		pDC->LineTo((m_rectDraw.Width() - nHleftspace), nCurHeight);
		itemRect.SetRect((m_rectDraw.Width() + nHmidspace)/2, nCurHeight - nVspace,(m_rectDraw.Width() - nHleftspace), nCurHeight);
		DrawReportItem(pDC, itemRect, i*2);
	}
	//分割线：曲线标题
	{
		nCurHeight = nCurHeight + nVspace * 3;  //Item1
		pDC->MoveTo(nHleftspace, nCurHeight);
		pDC->LineTo((m_rectDraw.Width() - nHmidspace)/2 ,nCurHeight);
		CRect itemRect;
		itemRect.SetRect(nHleftspace, nCurHeight - nVspace,(m_rectDraw.Width() - nHmidspace)/2, nCurHeight);
		DrawCurveItem(pDC, itemRect, 2,m_nCurPage,GetSorName(m_nCurPage));//  zll  2012.9.27 修改 只显示芯号

        //SetSorName(m_nCurPage,m_CurveNum+1,sorName);//设置.sor曲线文件名

		//  zll  2012.9.27 注释
		//pDC->MoveTo((m_rectDraw.Width()+ nHmidspace)/3, nCurHeight);  //Item2
		//pDC->LineTo((m_rectDraw.Width() - nHmidspace) *2/3 , nCurHeight);
		//itemRect.SetRect((m_rectDraw.Width()+ nHmidspace)/3, nCurHeight - nVspace,(m_rectDraw.Width() - nHmidspace) *2/3, nCurHeight);
		//DrawCurveItem(pDC, itemRect, 2);

		//pDC->MoveTo(m_rectDraw.Width()* 2/3, nCurHeight);  //Item3
		//pDC->LineTo((m_rectDraw.Width() - nHleftspace), nCurHeight);
		//itemRect.SetRect(m_rectDraw.Width()* 2/3, nCurHeight - nVspace,m_rectDraw.Width() - nHleftspace, nCurHeight);
		//DrawCurveItem(pDC, itemRect, 3);
	}
	//曲线
	{
		//大矩形
		nCurHeight = nCurHeight + nVspace * 2;
		CRect curveRect;
		curveRect.SetRect(nHleftspace*2 + nHmidspace/2, nCurHeight, m_rectDraw.Width() - nHleftspace, nCurHeight + nVspace * 20);
		pDC->Rectangle(&curveRect);
		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		DrawGridLine(pDC, curveRect);//绘制栅格 wcq2012.10.31
		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		DrawCurve(pDC, curveRect,m_nCurPage-1);

		DrawCursorAB(pDC, curveRect);  //lzy2014.7.23

		//DrawScale(pDC, curveRect);  //lzy2014.7.23

		//左侧小矩形
		/*CRect leftRect;
		leftRect.SetRect(nHleftspace, curveRect.top + curveRect.Height()/2-10*nVspace, curveRect.left - nHmidspace/2, curveRect.top + curveRect.Height()/2-8*nVspace);
		pDC->Rectangle(&leftRect);
		DrawVerCoordUnit(pDC, leftRect);lzy2014.7.23*/
		//底侧小矩形
		CRect bottomRect;		
		//bottomRect.SetRect(curveRect.left, curveRect.bottom +nVspace, curveRect.left + curveRect.Width()/2+3*nHleftspace, curveRect.bottom +nVspace*3);
		bottomRect.SetRect(curveRect.left, curveRect.bottom +nVspace, curveRect.left + curveRect.Width(), curveRect.bottom +nVspace*3);
        //pDC->Rectangle(&bottomRect);
		CRect leftUnitRect;
		leftUnitRect.SetRect(curveRect.left, bottomRect.top, curveRect.left + 400,  bottomRect.top + nVspace);
		//DrawHorCoordUnit(pDC, leftUnitRect, MAIN->m_pWaveFormView->m_strStartDis); //绘制横坐标文本左侧

		CRect mideUnitRect;
		mideUnitRect.SetRect(bottomRect.left-nHleftspace, bottomRect.top, bottomRect.right,  bottomRect.bottom);
		//DrawHorCoordUnit(pDC, mideUnitRect, MAIN->m_pWaveFormView->m_strUnitDis); //绘制横坐标文本单位长度
		//DrawHorCoordUnit(pDC, mideUnitRect, MAIN->m_pWaveFormView->m_strUnitDis); //绘制横坐标文本单位长度//wcq2012.11.02
		CString strDivUnit;
		float m_float_zll=0.0f;
		float flo_Length=g_sorFileArray.sorFileList[m_CurveNum].pTestResultInfo->Length;
		
		//DrawHorCoordUnit(pDC, mideUnitRect, branch(m_float_zll,flo_Length)); //绘制横坐标文本单位长度//wcq2012.11.02

		CRect rightUnitRect;
		rightUnitRect.SetRect(curveRect.right -400-nHleftspace, bottomRect.top, curveRect.right,  bottomRect.top + nVspace);
		///DrawHorCoordUnit(pDC, rightUnitRect, MAIN->m_pWaveFormView->m_strEndDis); //绘制横坐标文本
		//DrawHorCoordUnit(pDC, rightUnitRect, MAIN->m_pWaveFormView->m_strEndDis); //绘制横坐标文本//wcq2012.11.02
		
		//DrawHorCoordUnit(pDC, rightUnitRect, right_MAX(m_float_zll,flo_Length));//绘制横坐标文本 //横坐标右边最大长度 cwcq2012.11.02

		//画刻度lzy2014.7.18
		DrawScale(pDC, curveRect, 0, flo_Length);
	}
	//事件列表
	{
		nCurHeight = nCurHeight + nVspace * 27;
		int nCurLeft = 0;
	   /*for (int nLine=0; nLine<=12;nLine++)
		{
			nCurLeft = nHleftspace;
			for (int nRow=0; nRow<8;nRow++)
			{
				pDC->MoveTo(nCurLeft, nCurHeight);
				pDC->LineTo(nCurLeft+nHleftspace/1.1, nCurHeight);
				nCurLeft = nCurLeft + nHleftspace;
			}
			nCurHeight = nCurHeight + nVspace * 1.5;
		}*/
		//绘制事件列表表头
		DrawEventHead(pDC, nHleftspace, nCurHeight, m_rectDraw);
		//绘制事件体
		DrawEventBody1(pDC, nHleftspace, nCurHeight,m_nCurPage);
	}
}

void CPrintView::Draw2Curve(CDC* pDC,int m_nCurPage)
{
	//画线
	int nVspace = m_rectDraw.Height() / 64;
	int nCurHeight = nVspace * 3;
	//int nHleftspace = m_rectDraw.Width() / 10;
	int nHleftspace = m_rectDraw.Width() / 18;  //lzy2014.7.23
	int nHmidspace = m_rectDraw.Width() / 20;
	//标题
	pDC->MoveTo(m_rectDraw.Width() / 3, nCurHeight);
	pDC->LineTo(m_rectDraw.Width() * 2/3, nCurHeight);
	CRect titleRect;
	titleRect.SetRect(m_rectDraw.Width() / 3, nCurHeight-nVspace, m_rectDraw.Width() * 2/3, nCurHeight);
	pDC->DrawText(g_sorFileArray.reportConfig.Title, titleRect, DT_CENTER);
	//内容
	for (int i=1; i<=6; i++)
	{
		nCurHeight = nCurHeight + nVspace;
		pDC->MoveTo(nHleftspace, nCurHeight);
		pDC->LineTo((m_rectDraw.Width() - nHmidspace)/2 ,nCurHeight);

		CRect itemRect;
		itemRect.SetRect(nHleftspace, nCurHeight - nVspace,(m_rectDraw.Width() - nHmidspace)/2, nCurHeight);
		DrawReportItem(pDC, itemRect, i*2-1);

		pDC->MoveTo((m_rectDraw.Width() + nHmidspace)/2, nCurHeight);
		pDC->LineTo((m_rectDraw.Width() - nHleftspace), nCurHeight);
		itemRect.SetRect((m_rectDraw.Width() + nHmidspace)/2, nCurHeight - nVspace,m_rectDraw.Width() - nHleftspace, nCurHeight);
		DrawReportItem(pDC, itemRect, i*2);
	}
	//分割线:曲线标题
	int m_CurveNum=2*(m_nCurPage-1);//第几条曲线
	if (g_sorFileArray.totalCurveNumber>m_CurveNum)
	{
		nCurHeight = nCurHeight + nVspace * 3;
		pDC->MoveTo(nHleftspace, nCurHeight);
		pDC->LineTo((m_rectDraw.Width() - nHmidspace)/2 ,nCurHeight);
		CRect itemRect;
		itemRect.SetRect(nHleftspace, nCurHeight - nVspace,(m_rectDraw.Width() - nHmidspace)/2, nCurHeight);
		DrawCurveItem(pDC, itemRect, 2,m_CurveNum+1,GetSorName(m_CurveNum+1));//  zll  2012.9.27 修改 只显示芯号

		//  zll  2012.9.27 注释
		/*pDC->MoveTo((m_rectDraw.Width()+ nHmidspace)/3, nCurHeight);
		pDC->LineTo((m_rectDraw.Width() - nHmidspace) *2/3 , nCurHeight);
		itemRect.SetRect((m_rectDraw.Width()+ nHmidspace)/3, nCurHeight - nVspace,(m_rectDraw.Width() - nHmidspace) *2/3, nCurHeight);
		DrawCurveItem(pDC, itemRect, 2);

		pDC->MoveTo(m_rectDraw.Width()* 2/3, nCurHeight);
		pDC->LineTo((m_rectDraw.Width() - nHleftspace), nCurHeight);
		itemRect.SetRect(m_rectDraw.Width()* 2/3, nCurHeight - nVspace,m_rectDraw.Width() - nHleftspace, nCurHeight);
		DrawCurveItem(pDC, itemRect, 3);*/
	}
	//曲线1
	m_CurveNum=2*(m_nCurPage-1);//第几条曲线
	if (g_sorFileArray.totalCurveNumber>m_CurveNum)
	{
		//大矩形
		nCurHeight = nCurHeight + nVspace * 2;
		CRect curveRect;
		curveRect.SetRect(nHleftspace*2 + nHmidspace/2, nCurHeight - nVspace, m_rectDraw.Width() - nHleftspace, nCurHeight + nVspace * 20);
		pDC->Rectangle(&curveRect);
		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		DrawGridLine(pDC, curveRect);//绘制栅格 wcq2012.10.31
		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		DrawCurveByReport(pDC, curveRect, m_CurveNum);//绘制曲线1
		//左侧小矩形
		/*CRect leftRect;
		leftRect.SetRect(nHleftspace, nCurHeight - nVspace, curveRect.left - nHmidspace/2, curveRect.top + curveRect.Height()/2-nVspace*17/2);
		pDC->Rectangle(&leftRect);
		DrawVerCoordUnit(pDC, leftRect);lzy2014.7.23*/
		//底侧小矩形
		CRect bottomRect;
		bottomRect.SetRect(curveRect.left, curveRect.bottom +nVspace, curveRect.left + curveRect.Width()/2+3*nHleftspace, curveRect.bottom +nVspace*3);
		//pDC->Rectangle(&bottomRect);
		CRect leftUnitRect;
		leftUnitRect.SetRect(curveRect.left, bottomRect.top, curveRect.left + 400,  bottomRect.top + nVspace);
		//DrawHorCoordUnit(pDC, leftUnitRect, MAIN->m_pWaveFormView->m_strStartDis); //绘制横坐标文本左侧

		CRect mideUnitRect;
		mideUnitRect.SetRect(bottomRect.left-nHleftspace, bottomRect.top, bottomRect.right,  bottomRect.bottom);
		//DrawHorCoordUnit(pDC, mideUnitRect, MAIN->m_pWaveFormView->m_strUnitDis); //绘制横坐标文本单位长度//wcq2012.11.02
		CString strDivUnit;
		float m_float_zll=0.0f;
		float flo_Length=g_sorFileArray.sorFileList[m_CurveNum].pTestResultInfo->Length;
		
		//DrawHorCoordUnit(pDC, mideUnitRect, branch(m_float_zll,flo_Length)); //绘制横坐标文本单位长度//wcq2012.11.02
		
		CRect rightUnitRect;
		rightUnitRect.SetRect(curveRect.right -400-nHleftspace, bottomRect.top, curveRect.right,  bottomRect.top + nVspace);
		//DrawHorCoordUnit(pDC, rightUnitRect, MAIN->m_pWaveFormView->m_strEndDis); //绘制横坐标文本//wcq2012.11.02
		
		//DrawHorCoordUnit(pDC, rightUnitRect, right_MAX(m_float_zll,flo_Length));//绘制横坐标文本 //横坐标右边最大长度 cwcq2012.11.02
		DrawScale(pDC, curveRect, 0, flo_Length);
	}
	//曲线2分割线
	m_CurveNum=2*(m_nCurPage-1)+1;//第几条曲线
	if (g_sorFileArray.totalCurveNumber>m_CurveNum)
	{
		nCurHeight = nCurHeight + nVspace * 23;
		pDC->MoveTo(nHleftspace, nCurHeight+ nVspace);
		pDC->LineTo((m_rectDraw.Width() - nHmidspace)/2 ,nCurHeight+ nVspace);
		CRect itemRect;
		itemRect.SetRect(nHleftspace, nCurHeight,(m_rectDraw.Width() - nHmidspace)/2, nCurHeight+ nVspace);
		DrawCurveItem(pDC, itemRect, 2,m_CurveNum+1,GetSorName(m_CurveNum+1));//  zll  2012.9.27 修改 只显示芯号

		//  zll  2012.9.27 注释
		/*pDC->MoveTo((m_rectDraw.Width()+ nHmidspace)/3, nCurHeight);
		pDC->LineTo((m_rectDraw.Width() - nHmidspace) *2/3 , nCurHeight);
		itemRect.SetRect((m_rectDraw.Width()+ nHmidspace)/3, nCurHeight - nVspace,(m_rectDraw.Width() - nHmidspace) *2/3, nCurHeight);
		DrawCurveItem(pDC, itemRect, 2);

		pDC->MoveTo(m_rectDraw.Width()* 2/3, nCurHeight);
		pDC->LineTo((m_rectDraw.Width() - nHleftspace), nCurHeight);
		itemRect.SetRect(m_rectDraw.Width()* 2/3, nCurHeight - nVspace,m_rectDraw.Width() - nHleftspace, nCurHeight);
		DrawCurveItem(pDC, itemRect, 3);*/
	}
	//曲线2
	m_CurveNum=2*(m_nCurPage-1)+1;//第几条曲线
	if (g_sorFileArray.totalCurveNumber>m_CurveNum)
	{
		//曲线2大矩形
		nCurHeight = nCurHeight + nVspace * 2;
		CRect curveRect;
		curveRect.SetRect(nHleftspace*2 + nHmidspace/2, nCurHeight, m_rectDraw.Width() - nHleftspace, nCurHeight + nVspace * 20);
		pDC->Rectangle(&curveRect);
		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		DrawGridLine(pDC, curveRect);//绘制栅格 wcq2012.10.31
		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		DrawCurveByReport(pDC, curveRect, m_CurveNum);//绘制曲线2
		//曲线2左侧小矩形
		/*CRect leftRect;
		leftRect.SetRect(nHleftspace, nCurHeight, curveRect.left - nHmidspace/2, curveRect.top + curveRect.Height()/2-nVspace*8);
		pDC->Rectangle(&leftRect);
		DrawVerCoordUnit(pDC, leftRect);*/
		//曲线2底侧小矩形
		CRect bottomRect;
		bottomRect.SetRect(curveRect.left, curveRect.bottom +nVspace, curveRect.left + curveRect.Width()/2+3*nHleftspace, curveRect.bottom +nVspace*3);
		//pDC->Rectangle(&bottomRect);
		CRect leftUnitRect;
		leftUnitRect.SetRect(curveRect.left, bottomRect.top, curveRect.left + 400,  bottomRect.top + nVspace);
		//DrawHorCoordUnit(pDC, leftUnitRect, MAIN->m_pWaveFormView->m_strStartDis); //绘制横坐标文本左侧

		CRect mideUnitRect;
		mideUnitRect.SetRect(bottomRect.left-nHleftspace, bottomRect.top, bottomRect.right,  bottomRect.bottom);
		//DrawHorCoordUnit(pDC, mideUnitRect, MAIN->m_pWaveFormView->m_strUnitDis); //绘制横坐标文本单位长度//wcq2012.11.02
		CString strDivUnit;
		float m_float_zll=0.0f;
		float flo_Length=g_sorFileArray.sorFileList[m_CurveNum].pTestResultInfo->Length;
		
		//DrawHorCoordUnit(pDC, mideUnitRect, branch(m_float_zll,flo_Length)); //绘制横坐标文本单位长度//wcq2012.11.02

		CRect rightUnitRect;
		rightUnitRect.SetRect(curveRect.right -400-nHleftspace, bottomRect.top, curveRect.right,  bottomRect.top + nVspace);
		//DrawHorCoordUnit(pDC, rightUnitRect, MAIN->m_pWaveFormView->m_strEndDis); //绘制横坐标文本//wcq2012.11.02
		
		//DrawHorCoordUnit(pDC, rightUnitRect, right_MAX(m_float_zll,flo_Length));//绘制横坐标文本 //横坐标右边最大长度 cwcq2012.11.02
		DrawScale(pDC, curveRect, 0, flo_Length);
	}
}


void CPrintView::Draw4Curve(CDC* pDC,int m_nCurPage)
{
	//画线
	int nVspace = m_rectDraw.Height() / 64;
	int nCurHeight = nVspace * 3;
	int nHleftspace = m_rectDraw.Width() / 10;
	//int nHleftspace = m_rectDraw.Width() / 18;
	int nHmidspace = m_rectDraw.Width() / 20;
	int nSeparateSpace = (m_rectDraw.Width() - 2 * (nHleftspace + nHmidspace)) / 6;
	int nCurSeparate = nHleftspace;
	//标题
	pDC->MoveTo(m_rectDraw.Width() / 3, nCurHeight);
	pDC->LineTo(m_rectDraw.Width() * 2/3, nCurHeight);
	CRect titleRect;
	titleRect.SetRect(m_rectDraw.Width() / 3, nCurHeight-nVspace, m_rectDraw.Width() * 2/3, nCurHeight);
	pDC->DrawText(g_sorFileArray.reportConfig.Title, titleRect, DT_CENTER);
	//内容
	for (int i=1; i<=6; i++)
	{
		nCurHeight = nCurHeight + nVspace;
		pDC->MoveTo(nHleftspace, nCurHeight);
		pDC->LineTo((m_rectDraw.Width() - nHmidspace)/2 ,nCurHeight);
		CRect itemRect;
		itemRect.SetRect(nHleftspace, nCurHeight - nVspace,(m_rectDraw.Width() - nHmidspace)/2, nCurHeight);
		DrawReportItem(pDC, itemRect, i*2-1);

		pDC->MoveTo((m_rectDraw.Width() + nHmidspace)/2, nCurHeight);
		pDC->LineTo((m_rectDraw.Width() - nHleftspace), nCurHeight);
		itemRect.SetRect((m_rectDraw.Width() + nHmidspace)/2, nCurHeight - nVspace,m_rectDraw.Width() - nHleftspace, nCurHeight);
		DrawReportItem(pDC, itemRect, i*2);
	}
	//曲线12分割线 zll 2012.9.20
	int m_CurveNum=4*(m_nCurPage-1)+1;//第几条曲线
	if (g_sorFileArray.totalCurveNumber>m_CurveNum)
	{
		nCurHeight = nCurHeight + nVspace * 3;
		for (int i=1; i<=6; i++)
		{
			if (2==i)
			{
				pDC->MoveTo(nCurSeparate, nCurHeight);
				pDC->LineTo(nCurSeparate + nSeparateSpace*5/2, nCurHeight);
				CRect itemRect;
				itemRect.SetRect(nCurSeparate, nCurHeight- nVspace, nCurSeparate + 3*nSeparateSpace, nCurHeight);
				int nIndex = i;
				if (nIndex > 3)
					nIndex = nIndex - 3;
				DrawCurveItem(pDC, itemRect, nIndex,m_CurveNum,GetSorName(m_CurveNum));
				nCurSeparate = nCurSeparate + nSeparateSpace + nHmidspace / 3;
			}
			else if (5==i)
			{
				pDC->MoveTo((m_rectDraw.Width() + nHmidspace)/2, nCurHeight);
				pDC->LineTo((m_rectDraw.Width() + nHmidspace)/2 + nSeparateSpace*5/2, nCurHeight);
				CRect itemRect;
				itemRect.SetRect((m_rectDraw.Width() + nHmidspace)/2, nCurHeight- nVspace, nCurSeparate*3 + 3*nSeparateSpace, nCurHeight);
				int nIndex = i;
				if (nIndex > 3)
					nIndex = nIndex - 3;
				DrawCurveItem(pDC, itemRect, nIndex,m_CurveNum+1,GetSorName(m_CurveNum+1));
				nCurSeparate = nCurSeparate + nSeparateSpace + nHmidspace / 3;
			}
		}
	}
	else if (g_sorFileArray.totalCurveNumber==m_CurveNum)//曲线1分割线  zll 2012.9.20
	{
		nCurHeight = nCurHeight + nVspace * 3;
		for (int i=1; i<=3; i++)
		{
			if (2==i)
			{
				pDC->MoveTo(nCurSeparate, nCurHeight);
				pDC->LineTo(nCurSeparate + nSeparateSpace*5/2, nCurHeight);
				CRect itemRect;
				itemRect.SetRect(nCurSeparate, nCurHeight- nVspace, nCurSeparate + 3*nSeparateSpace, nCurHeight);
				int nIndex = i;
				if (nIndex > 3)
					nIndex = nIndex - 3;
				DrawCurveItem(pDC, itemRect, nIndex,m_CurveNum,GetSorName(m_CurveNum));
				nCurSeparate = nCurSeparate + nSeparateSpace + nHmidspace / 3;
			}
		}
	}

	//曲线1
	m_CurveNum=4*(m_nCurPage-1);//第几条曲线
	if (g_sorFileArray.totalCurveNumber>m_CurveNum)
	{
		//曲线1大矩形
		nCurHeight = nCurHeight + nVspace * 2;
		CRect curveRect;
		curveRect.SetRect(nHleftspace*2, nCurHeight, (m_rectDraw.Width() - nHleftspace)/2, nCurHeight + nVspace * 10);
		pDC->Rectangle(&curveRect);
		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		DrawGridLine(pDC, curveRect);//绘制栅格 wcq2012.10.31
		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		DrawCurveByReport(pDC, curveRect, m_CurveNum);//绘制曲线1
		//左侧小矩形
		CRect leftRect;
		leftRect.SetRect(nHleftspace, curveRect.top + curveRect.Height()/2-5*nVspace, curveRect.left - nHmidspace/2, curveRect.top + curveRect.Height()/2-3*nVspace);
		pDC->Rectangle(&leftRect);
		DrawVerCoordUnit(pDC, leftRect);
		//底侧小矩形
		CRect bottomRect;
		bottomRect.SetRect(curveRect.left-nHmidspace/3, curveRect.bottom +nVspace, curveRect.left + curveRect.Width()/2+nHleftspace*5/4, curveRect.bottom +nVspace*3);
		pDC->Rectangle(&bottomRect);
		CRect leftUnitRect;
		leftUnitRect.SetRect(curveRect.left-nHleftspace, bottomRect.top, curveRect.left + 400,  bottomRect.top + nVspace);
		DrawHorCoordUnit(pDC, leftUnitRect, MAIN->m_pWaveFormView->m_strStartDis); //绘制横坐标文本左侧

		CRect mideUnitRect;
		mideUnitRect.SetRect(curveRect.left-nHmidspace/2, bottomRect.top, curveRect.right,  bottomRect.bottom);
		CString strDivUnit;
		float m_float_zll=0.0f;
		float flo_Length=g_sorFileArray.sorFileList[m_CurveNum].pTestResultInfo->Length;
		
		DrawHorCoordUnit(pDC, mideUnitRect, branch(m_float_zll,flo_Length)); //绘制横坐标文本单位长度//wcq2012.11.02

		CRect rightUnitRect;
		rightUnitRect.SetRect(curveRect.right -nHleftspace, bottomRect.top, curveRect.right,  bottomRect.top + nVspace);
		
		DrawHorCoordUnit(pDC, rightUnitRect, right_MAX(m_float_zll,flo_Length));//绘制横坐标文本 //横坐标右边最大长度 cwcq2012.11.02

		//float flo_Length=g_sorFileArray.sorFileList[m_CurveNum].pTestResultInfo->Length;
		//DrawScale(pDC, curveRect, 0, flo_Length);


	}
	//曲线2
	m_CurveNum=4*(m_nCurPage-1)+1;//第几条曲线
	if (g_sorFileArray.totalCurveNumber>m_CurveNum)
	{
		//曲线2大矩形
		CRect curveRect;
		curveRect.SetRect((m_rectDraw.Width() + nHmidspace)/2 + nHleftspace, nCurHeight, m_rectDraw.Width() - nHleftspace*6/5, nCurHeight + nVspace * 10);
		pDC->Rectangle(&curveRect);
		//左侧小矩形
		CRect leftRect;
		leftRect.SetRect((m_rectDraw.Width() + nHmidspace)/2, curveRect.top + curveRect.Height()/2-5*nVspace, curveRect.left - nHmidspace/2, curveRect.top + curveRect.Height()/2-3*nVspace);
		pDC->Rectangle(&leftRect);
		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		DrawGridLine(pDC, curveRect);//绘制栅格 wcq2012.10.31
		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		DrawCurveByReport(pDC, curveRect, m_CurveNum);//绘制曲线2
		DrawVerCoordUnit(pDC, leftRect);
		//底侧小矩形
		CRect bottomRect;
		bottomRect.SetRect(curveRect.left-nHmidspace/3, curveRect.bottom +nVspace, curveRect.left + curveRect.Width()/2+nHleftspace*5/4, curveRect.bottom +nVspace*3);
		//bottomRect.SetRect(curveRect.left + curveRect.Width()/2-nHmidspace, curveRect.bottom +nVspace, curveRect.left + curveRect.Width()/2+nHmidspace, curveRect.bottom +nVspace*3);
		pDC->Rectangle(&bottomRect);
		CRect leftUnitRect;
		leftUnitRect.SetRect(curveRect.left-nHleftspace, bottomRect.top, curveRect.left + 400,  bottomRect.top + nVspace);
		//leftUnitRect.SetRect(curveRect.left, bottomRect.top, curveRect.left + 400,  bottomRect.top + nVspace);
		DrawHorCoordUnit(pDC, leftUnitRect, MAIN->m_pWaveFormView->m_strStartDis); //绘制横坐标文本左侧

		CRect mideUnitRect;
		mideUnitRect.SetRect(curveRect.left-nHmidspace/2, bottomRect.top, curveRect.right,  bottomRect.bottom);
		//mideUnitRect.SetRect(bottomRect.left, bottomRect.top, bottomRect.right,  bottomRect.bottom);
		//DrawHorCoordUnit(pDC, mideUnitRect, MAIN->m_pWaveFormView->m_strUnitDis); //绘制横坐标文本单位长度//wcq2012.11.02
		CString strDivUnit;
		float m_float_zll=0.0f;
		float flo_Length=g_sorFileArray.sorFileList[m_CurveNum].pTestResultInfo->Length;
		
		DrawHorCoordUnit(pDC, mideUnitRect, branch(m_float_zll,flo_Length)); //绘制横坐标文本单位长度//wcq2012.11.02

		CRect rightUnitRect;
		rightUnitRect.SetRect(curveRect.right -nHleftspace, bottomRect.top, curveRect.right,  bottomRect.top + nVspace);
		//rightUnitRect.SetRect(curveRect.right -400, bottomRect.top, curveRect.right,  bottomRect.top + nVspace);
		//DrawHorCoordUnit(pDC, rightUnitRect, MAIN->m_pWaveFormView->m_strEndDis); //绘制横坐标文本//wcq2012.11.02
		
		DrawHorCoordUnit(pDC, rightUnitRect, right_MAX(m_float_zll,flo_Length));//绘制横坐标文本 //横坐标右边最大长度 cwcq2012.11.02
		

	}
	//曲线34分割线
	m_CurveNum=4*(m_nCurPage-1)+3;//第几条曲线
	if (g_sorFileArray.totalCurveNumber>m_CurveNum)
	{
		nCurHeight = nCurHeight + nVspace * 27;
		nCurSeparate = nHleftspace;
		for (int i=1; i<=6; i++)
		{
			if (2==i)
			{
				pDC->MoveTo(nCurSeparate, nCurHeight-4*nVspace);
				pDC->LineTo(nCurSeparate + nSeparateSpace*5/2, nCurHeight-4*nVspace);
				CRect itemRect;
				itemRect.SetRect(nCurSeparate, nCurHeight- 5*nVspace, nCurSeparate + 3*nSeparateSpace, nCurHeight-4*nVspace);
				int nIndex = i;
				if (nIndex > 3)
					nIndex = nIndex - 3;
				DrawCurveItem(pDC, itemRect, nIndex,m_CurveNum,GetSorName(m_CurveNum));
				nCurSeparate = nCurSeparate + nSeparateSpace + nHmidspace / 3;
			}
			else if (5==i)
			{
				pDC->MoveTo((m_rectDraw.Width() + nHmidspace)/2, nCurHeight-4*nVspace);
				pDC->LineTo((m_rectDraw.Width() + nHmidspace)/2 + nSeparateSpace*5/2, nCurHeight-4*nVspace);
				CRect itemRect;
				itemRect.SetRect((m_rectDraw.Width() + nHmidspace)/2, nCurHeight- 5*nVspace, nCurSeparate*3 + 3*nSeparateSpace, nCurHeight-4*nVspace);
				int nIndex = i;
				if (nIndex > 3)
					nIndex = nIndex - 3;
				DrawCurveItem(pDC, itemRect, nIndex,m_CurveNum+1,GetSorName(m_CurveNum+1));
				nCurSeparate = nCurSeparate + nSeparateSpace + nHmidspace / 3;
			}
		}
	}
	else if (g_sorFileArray.totalCurveNumber==m_CurveNum)
	{
		nCurHeight = nCurHeight + nVspace * 27;
		nCurSeparate = nHleftspace;
		for (int i=1; i<=3; i++)
		{
			if (2==i)
			{
				pDC->MoveTo(nCurSeparate, nCurHeight-4*nVspace);
				pDC->LineTo(nCurSeparate + nSeparateSpace*5/2, nCurHeight-4*nVspace);
				CRect itemRect;
				itemRect.SetRect(nCurSeparate, nCurHeight- 5*nVspace, nCurSeparate + 3*nSeparateSpace, nCurHeight-4*nVspace);
				int nIndex = i;
				if (nIndex > 3)
					nIndex = nIndex - 3;
				DrawCurveItem(pDC, itemRect, nIndex,m_CurveNum,GetSorName(m_CurveNum));
				nCurSeparate = nCurSeparate + nSeparateSpace + nHmidspace / 3;
			}
		}
	}
	//曲线3
	m_CurveNum=4*(m_nCurPage-1)+2;//第几条曲线
	if (g_sorFileArray.totalCurveNumber>m_CurveNum)
	{
		//曲线3大矩形
		nCurHeight = nCurHeight + nVspace * 2;
		CRect curveRect;
		curveRect.SetRect(nHleftspace*2, nCurHeight-4*nVspace, (m_rectDraw.Width() - nHleftspace)/2, nCurHeight + nVspace * 6);
		//curveRect.SetRect(nHleftspace*2 + nHmidspace/2, nCurHeight, (m_rectDraw.Width() - nHleftspace)/2, nCurHeight + nVspace * 10);
		pDC->Rectangle(&curveRect);
		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		DrawGridLine(pDC, curveRect);//绘制栅格 wcq2012.10.31
		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		DrawCurveByReport(pDC, curveRect, m_CurveNum);//绘制曲线3
		//左侧小矩形
		CRect leftRect;
		leftRect.SetRect(nHleftspace, curveRect.top + curveRect.Height()/2-5*nVspace, curveRect.left - nHmidspace/2, curveRect.top + curveRect.Height()/2-3*nVspace);
		//leftRect.SetRect(nHleftspace, curveRect.top + curveRect.Height()/2-nVspace, curveRect.left - nHmidspace/2, curveRect.top + curveRect.Height()/2+nVspace);
		pDC->Rectangle(&leftRect);
		DrawVerCoordUnit(pDC, leftRect);
		//底侧小矩形
		CRect bottomRect;
		bottomRect.SetRect(curveRect.left-nHmidspace/3, curveRect.bottom +nVspace, curveRect.left + curveRect.Width()/2+nHleftspace*5/4, curveRect.bottom +nVspace*3);
		//bottomRect.SetRect(curveRect.left + curveRect.Width()/2-nHmidspace, curveRect.bottom +nVspace, curveRect.left + curveRect.Width()/2+nHmidspace, curveRect.bottom +nVspace*3);
		pDC->Rectangle(&bottomRect);
		CRect leftUnitRect;
		leftUnitRect.SetRect(curveRect.left-nHleftspace, bottomRect.top, curveRect.left + 400,  bottomRect.top + nVspace);
		//leftUnitRect.SetRect(curveRect.left, bottomRect.top, curveRect.left + 400,  bottomRect.top + nVspace);
		DrawHorCoordUnit(pDC, leftUnitRect, MAIN->m_pWaveFormView->m_strStartDis); //绘制横坐标文本左侧

		CRect mideUnitRect;
		mideUnitRect.SetRect(curveRect.left-nHmidspace/2, bottomRect.top, curveRect.right,  bottomRect.bottom);
		//mideUnitRect.SetRect(bottomRect.left, bottomRect.top, bottomRect.right,  bottomRect.bottom);
		//DrawHorCoordUnit(pDC, mideUnitRect, MAIN->m_pWaveFormView->m_strUnitDis); //绘制横坐标文本单位长度//wcq2012.11.02
		CString strDivUnit;
		float m_float_zll=0.0f;
		float flo_Length=g_sorFileArray.sorFileList[m_CurveNum].pTestResultInfo->Length;
		
		DrawHorCoordUnit(pDC, mideUnitRect, branch(m_float_zll,flo_Length)); //绘制横坐标文本单位长度//wcq2012.11.02

		CRect rightUnitRect;
		rightUnitRect.SetRect(curveRect.right -nHleftspace, bottomRect.top, curveRect.right,  bottomRect.top + nVspace);
		//rightUnitRect.SetRect(curveRect.right -400, bottomRect.top, curveRect.right,  bottomRect.top + nVspace);
		//DrawHorCoordUnit(pDC, rightUnitRect, MAIN->m_pWaveFormView->m_strEndDis); //绘制横坐标文本//wcq2012.11.02
		
		DrawHorCoordUnit(pDC, rightUnitRect, right_MAX(m_float_zll,flo_Length));//绘制横坐标文本 //横坐标右边最大长度 cwcq2012.11.02

	}
	//曲线4
	m_CurveNum=4*(m_nCurPage-1)+3;//第几条曲线
	if (g_sorFileArray.totalCurveNumber>m_CurveNum)
	{
		//曲线4大矩形
		CRect curveRect;
		curveRect.SetRect((m_rectDraw.Width() + nHmidspace)/2 + nHleftspace, nCurHeight-4*nVspace, m_rectDraw.Width() - nHleftspace*6/5, nCurHeight + nVspace * 6);
		pDC->Rectangle(&curveRect);
		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		DrawGridLine(pDC, curveRect);//绘制栅格 wcq2012.10.31
		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		DrawCurveByReport(pDC, curveRect, m_CurveNum);//绘制曲线4
		//左侧小矩形
		CRect leftRect;
		leftRect.SetRect((m_rectDraw.Width() + nHmidspace)/2, curveRect.top + curveRect.Height()/2-5*nVspace, curveRect.left - nHmidspace/2, curveRect.top + curveRect.Height()/2-3*nVspace);
		pDC->Rectangle(&leftRect);
		DrawVerCoordUnit(pDC, leftRect);
		//底侧小矩形
		CRect bottomRect;
		bottomRect.SetRect(curveRect.left-nHmidspace/3, curveRect.bottom +nVspace, curveRect.left + curveRect.Width()/2+nHleftspace*5/4, curveRect.bottom +nVspace*3);
		//bottomRect.SetRect(curveRect.left + curveRect.Width()/2-nHmidspace, curveRect.bottom +nVspace, curveRect.left + curveRect.Width()/2+nHmidspace, curveRect.bottom +nVspace*3);
		pDC->Rectangle(&bottomRect);
		CRect leftUnitRect;
		leftUnitRect.SetRect(curveRect.left-nHleftspace, bottomRect.top, curveRect.left + 400,  bottomRect.top + nVspace);
		//leftUnitRect.SetRect(curveRect.left, bottomRect.top, curveRect.left + 400,  bottomRect.top + nVspace);
		DrawHorCoordUnit(pDC, leftUnitRect, MAIN->m_pWaveFormView->m_strStartDis); //绘制横坐标文本左侧

		CRect mideUnitRect;
		mideUnitRect.SetRect(curveRect.left-nHmidspace/2, bottomRect.top, curveRect.right,  bottomRect.bottom);
		//mideUnitRect.SetRect(bottomRect.left, bottomRect.top, bottomRect.right,  bottomRect.bottom);
		//DrawHorCoordUnit(pDC, mideUnitRect, MAIN->m_pWaveFormView->m_strUnitDis); //绘制横坐标文本单位长度//wcq2012.11.02
		CString strDivUnit;
		float m_float_zll=0.0f;
		float flo_Length=g_sorFileArray.sorFileList[m_CurveNum].pTestResultInfo->Length;
		
		DrawHorCoordUnit(pDC, mideUnitRect, branch(m_float_zll,flo_Length)); //绘制横坐标文本单位长度//wcq2012.11.02

		CRect rightUnitRect;
		rightUnitRect.SetRect(curveRect.right -nHleftspace, bottomRect.top, curveRect.right,  bottomRect.top + nVspace);
		//rightUnitRect.SetRect(curveRect.right -400, bottomRect.top, curveRect.right,  bottomRect.top + nVspace);
		//DrawHorCoordUnit(pDC, rightUnitRect, MAIN->m_pWaveFormView->m_strEndDis); //绘制横坐标文本//wcq2012.11.02
		
		DrawHorCoordUnit(pDC, rightUnitRect, right_MAX(m_float_zll,flo_Length));//绘制横坐标文本 //横坐标右边最大长度 cwcq2012.11.02

	}

}

void CPrintView::Draw6Curve(CDC* pDC,int m_nCurPage)
{
	//画线
	int nVspace = m_rectDraw.Height() / 64;
	int nCurHeight = nVspace * 3;
	int nHleftspace = m_rectDraw.Width() / 10;
	int nHmidspace = m_rectDraw.Width() / 20;
	int nSeparateSpace = (m_rectDraw.Width() - 2 * (nHleftspace + nHmidspace)) / 6;
	int nCurSeparate = nHleftspace;
	//标题
	pDC->MoveTo(m_rectDraw.Width() / 3, nCurHeight);
	pDC->LineTo(m_rectDraw.Width() * 2/3, nCurHeight);
	CRect titleRect;
	titleRect.SetRect(m_rectDraw.Width() / 3, nCurHeight-nVspace, m_rectDraw.Width() * 2/3, nCurHeight);
	pDC->DrawText(g_sorFileArray.reportConfig.Title, titleRect, DT_CENTER);
	//内容
	for (int i=1; i<=6; i++)
	{
		nCurHeight = nCurHeight + nVspace;
		pDC->MoveTo(nHleftspace, nCurHeight);
		pDC->LineTo((m_rectDraw.Width() - nHmidspace)/2 ,nCurHeight);

		CRect itemRect;
		itemRect.SetRect(nHleftspace, nCurHeight - nVspace,(m_rectDraw.Width() - nHmidspace)/2, nCurHeight);
		DrawReportItem(pDC, itemRect, i*2-1);

		pDC->MoveTo((m_rectDraw.Width() + nHmidspace)/2, nCurHeight);
		pDC->LineTo((m_rectDraw.Width() - nHleftspace), nCurHeight);
		itemRect.SetRect((m_rectDraw.Width() + nHmidspace)/2, nCurHeight - nVspace,m_rectDraw.Width() - nHleftspace, nCurHeight);
		DrawReportItem(pDC, itemRect, i*2);
	}
	//曲线12分割线
	int m_CurveNum=6*(m_nCurPage-1)+1;//第几条曲线
	if (g_sorFileArray.totalCurveNumber>m_CurveNum)
	{
		nCurHeight = nCurHeight + nVspace * 3;
		for (int i=1; i<=6; i++)
		{
			if (2==i)
			{
				pDC->MoveTo(nCurSeparate, nCurHeight);
				pDC->LineTo(nCurSeparate + nSeparateSpace*5/2, nCurHeight);
				CRect itemRect;
				itemRect.SetRect(nCurSeparate, nCurHeight- nVspace, nCurSeparate + 3*nSeparateSpace, nCurHeight);
				int nIndex = i;
				if (nIndex > 3)
					nIndex = nIndex - 3;
				DrawCurveItem(pDC, itemRect, nIndex,m_CurveNum,GetSorName(m_CurveNum));
				nCurSeparate = nCurSeparate + nSeparateSpace + nHmidspace / 3;
			}
			else if (5==i)
			{
				pDC->MoveTo((m_rectDraw.Width() + nHmidspace)/2, nCurHeight);
				pDC->LineTo((m_rectDraw.Width() + nHmidspace)/2 + nSeparateSpace*5/2, nCurHeight);
				CRect itemRect;
				itemRect.SetRect((m_rectDraw.Width() + nHmidspace)/2, nCurHeight- nVspace, nCurSeparate*3 + 3*nSeparateSpace, nCurHeight);
				int nIndex = i;
				if (nIndex > 3)
					nIndex = nIndex - 3;
				DrawCurveItem(pDC, itemRect, nIndex,m_CurveNum+1,GetSorName(m_CurveNum+1));
				nCurSeparate = nCurSeparate + nSeparateSpace + nHmidspace / 3;
			}
		}
	}
	else if (g_sorFileArray.totalCurveNumber==m_CurveNum)
	{
		nCurHeight = nCurHeight + nVspace * 3;
		for (int i=1; i<=6; i++)
		{
			if (2==i)
			{
				pDC->MoveTo(nCurSeparate, nCurHeight);
				pDC->LineTo(nCurSeparate + nSeparateSpace*5/2, nCurHeight);
				CRect itemRect;
				itemRect.SetRect(nCurSeparate, nCurHeight- nVspace, nCurSeparate + 3*nSeparateSpace, nCurHeight);
				int nIndex = i;
				if (nIndex > 3)
					nIndex = nIndex - 3;
				DrawCurveItem(pDC, itemRect, nIndex,m_CurveNum,GetSorName(m_CurveNum));
				nCurSeparate = nCurSeparate + nSeparateSpace + nHmidspace / 3;
			}
		}
	}
	//曲线1
	m_CurveNum=6*(m_nCurPage-1);//第几条曲线
	if (g_sorFileArray.totalCurveNumber>m_CurveNum)
	{
		//曲线1大矩形
		nCurHeight = nCurHeight + nVspace * 2;
		CRect curveRect;
		curveRect.SetRect(nHleftspace*2, nCurHeight-nVspace, (m_rectDraw.Width() - nHleftspace)/2, nCurHeight + nVspace * 9);
		pDC->Rectangle(&curveRect);
		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		DrawGridLine(pDC, curveRect);//绘制栅格 wcq2012.10.31
		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		DrawCurveByReport(pDC, curveRect, m_CurveNum);//绘制曲线1
		//左侧小矩形
		CRect leftRect;
		leftRect.SetRect(nHleftspace, curveRect.top + curveRect.Height()/2-5*nVspace, curveRect.left - nHmidspace/2, curveRect.top + curveRect.Height()/2-3*nVspace);
		//leftRect.SetRect(nHleftspace, curveRect.top + curveRect.Height()/2-nVspace, curveRect.left - nHmidspace/2, curveRect.top + curveRect.Height()/2+nVspace);
		pDC->Rectangle(&leftRect);
		DrawVerCoordUnit(pDC, leftRect);
		//底侧小矩形
		CRect bottomRect;
		bottomRect.SetRect(curveRect.left-nHmidspace/3, curveRect.bottom +nVspace, curveRect.left + curveRect.Width()/2+nHleftspace*5/4, curveRect.bottom +nVspace*3);
		//bottomRect.SetRect(curveRect.left + curveRect.Width()/2-nHmidspace, curveRect.bottom +nVspace/2, curveRect.left + curveRect.Width()/2+nHmidspace, curveRect.bottom +nVspace*3/2);
		pDC->Rectangle(&bottomRect);
		CRect leftUnitRect;leftUnitRect.SetRect(curveRect.left-nHleftspace, bottomRect.top, curveRect.left + 400,  bottomRect.top + nVspace);
		//leftUnitRect.SetRect(curveRect.left, bottomRect.top, curveRect.left + 400,  bottomRect.top + nVspace);
		DrawHorCoordUnit(pDC, leftUnitRect, MAIN->m_pWaveFormView->m_strStartDis); //绘制横坐标文本左侧

		CRect mideUnitRect;
		mideUnitRect.SetRect(curveRect.left-nHmidspace/2, bottomRect.top, curveRect.right,  bottomRect.bottom);
		//mideUnitRect.SetRect(bottomRect.left, bottomRect.top, bottomRect.right,  bottomRect.bottom);
		//DrawHorCoordUnit(pDC, mideUnitRect, MAIN->m_pWaveFormView->m_strUnitDis); //绘制横坐标文本单位长度//wcq2012.11.02
		CString strDivUnit;
		float m_float_zll=0.0f;
		float flo_Length=g_sorFileArray.sorFileList[m_CurveNum].pTestResultInfo->Length;
		
		DrawHorCoordUnit(pDC, mideUnitRect, branch(m_float_zll,flo_Length)); //绘制横坐标文本单位长度//wcq2012.11.02

		CRect rightUnitRect;
		rightUnitRect.SetRect(curveRect.right -nHleftspace, bottomRect.top, curveRect.right,  bottomRect.top + nVspace);
		//rightUnitRect.SetRect(curveRect.right -400, bottomRect.top, curveRect.right,  bottomRect.top + nVspace);
		//DrawHorCoordUnit(pDC, rightUnitRect, MAIN->m_pWaveFormView->m_strEndDis); //绘制横坐标文本//wcq2012.11.02
		
		DrawHorCoordUnit(pDC, rightUnitRect, right_MAX(m_float_zll,flo_Length));//绘制横坐标文本 //横坐标右边最大长度 cwcq2012.11.02

	}
	//曲线2
	m_CurveNum=6*(m_nCurPage-1)+1;//第几条曲线
	if (g_sorFileArray.totalCurveNumber>m_CurveNum)
	{
		//曲线2大矩形
		CRect curveRect;
		curveRect.SetRect((m_rectDraw.Width() + nHmidspace)/2 + nHleftspace, nCurHeight-nVspace, m_rectDraw.Width() - nHleftspace*6/5, nCurHeight + nVspace * 9);
		pDC->Rectangle(&curveRect);
		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		DrawGridLine(pDC, curveRect);//绘制栅格 wcq2012.10.31
		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		DrawCurveByReport(pDC, curveRect, m_CurveNum);//绘制曲线2
		//左侧小矩形
		CRect leftRect;
		leftRect.SetRect((m_rectDraw.Width() + nHmidspace)/2, curveRect.top + curveRect.Height()/2-5*nVspace, curveRect.left - nHmidspace/2, curveRect.top + curveRect.Height()/2-3*nVspace);
		pDC->Rectangle(&leftRect);
		DrawVerCoordUnit(pDC, leftRect);
		//底侧小矩形
		CRect bottomRect;
		bottomRect.SetRect(curveRect.left-nHmidspace/3, curveRect.bottom +nVspace, curveRect.left + curveRect.Width()/2+nHleftspace*5/4, curveRect.bottom +nVspace*3);
		//bottomRect.SetRect(curveRect.left + curveRect.Width()/2-nHmidspace, curveRect.bottom +nVspace/2, curveRect.left + curveRect.Width()/2+nHmidspace, curveRect.bottom +nVspace*3/2);
		pDC->Rectangle(&bottomRect);
		CRect leftUnitRect;
		leftUnitRect.SetRect(curveRect.left-nHleftspace, bottomRect.top, curveRect.left + 400,  bottomRect.top + nVspace);
		//leftUnitRect.SetRect(curveRect.left, bottomRect.top, curveRect.left + 400,  bottomRect.top + nVspace);
		DrawHorCoordUnit(pDC, leftUnitRect, MAIN->m_pWaveFormView->m_strStartDis); //绘制横坐标文本左侧

		CRect mideUnitRect;
		mideUnitRect.SetRect(curveRect.left-nHmidspace/2, bottomRect.top, curveRect.right,  bottomRect.bottom);
		//mideUnitRect.SetRect(bottomRect.left, bottomRect.top, bottomRect.right,  bottomRect.bottom);
		//DrawHorCoordUnit(pDC, mideUnitRect, MAIN->m_pWaveFormView->m_strUnitDis); //绘制横坐标文本单位长度//wcq2012.11.02
		CString strDivUnit;
		float m_float_zll=0.0f;
		float flo_Length=g_sorFileArray.sorFileList[m_CurveNum].pTestResultInfo->Length;
		
		DrawHorCoordUnit(pDC, mideUnitRect, branch(m_float_zll,flo_Length)); //绘制横坐标文本单位长度//wcq2012.11.02

		CRect rightUnitRect;
		rightUnitRect.SetRect(curveRect.right -nHleftspace, bottomRect.top, curveRect.right,  bottomRect.top + nVspace);
		//rightUnitRect.SetRect(curveRect.right -400, bottomRect.top, curveRect.right,  bottomRect.top + nVspace);
		//DrawHorCoordUnit(pDC, rightUnitRect, MAIN->m_pWaveFormView->m_strEndDis); //绘制横坐标文本//wcq2012.11.02
		
		DrawHorCoordUnit(pDC, rightUnitRect, right_MAX(m_float_zll,flo_Length));//绘制横坐标文本 //横坐标右边最大长度 cwcq2012.11.02

	}
	//曲线34分割线
	m_CurveNum=6*(m_nCurPage-1)+3;//第几条曲线
	if (g_sorFileArray.totalCurveNumber>m_CurveNum)
	{
		nCurHeight = nCurHeight + nVspace * 17;
		nCurSeparate = nHleftspace;
		for (int i=1; i<=6; i++)
		{
			if (2==i)
			{
				pDC->MoveTo(nCurSeparate, nCurHeight-2*nVspace);
				pDC->LineTo(nCurSeparate + nSeparateSpace*5/2, nCurHeight-2*nVspace);
				CRect itemRect;
				itemRect.SetRect(nCurSeparate, nCurHeight- 3*nVspace, nCurSeparate + 3*nSeparateSpace, nCurHeight-2*nVspace);
				int nIndex = i;
				if (nIndex > 3)
					nIndex = nIndex - 3;
				DrawCurveItem(pDC, itemRect, nIndex,m_CurveNum,GetSorName(m_CurveNum));
				nCurSeparate = nCurSeparate + nSeparateSpace + nHmidspace / 3;
			}
			else if (5==i)
			{
				pDC->MoveTo((m_rectDraw.Width() + nHmidspace)/2, nCurHeight-2*nVspace);
				pDC->LineTo((m_rectDraw.Width() + nHmidspace)/2 + nSeparateSpace*5/2, nCurHeight-2*nVspace);
				CRect itemRect;
				itemRect.SetRect((m_rectDraw.Width() + nHmidspace)/2, nCurHeight- 3*nVspace, nCurSeparate*3 + 3*nSeparateSpace, nCurHeight-2*nVspace);
				int nIndex = i;
				if (nIndex > 3)
					nIndex = nIndex - 3;
				DrawCurveItem(pDC, itemRect, nIndex,m_CurveNum+1,GetSorName(m_CurveNum+1));
				nCurSeparate = nCurSeparate + nSeparateSpace + nHmidspace / 3;
			}
		}
	}
	else if (g_sorFileArray.totalCurveNumber==m_CurveNum)
	{
		nCurHeight = nCurHeight + nVspace * 17;
		nCurSeparate = nHleftspace;
		for (int i=1; i<=3; i++)
		{
			if (2==i)
			{
				pDC->MoveTo(nCurSeparate, nCurHeight-2*nVspace);
				pDC->LineTo(nCurSeparate + nSeparateSpace*5/2, nCurHeight-2*nVspace);
				CRect itemRect;
				itemRect.SetRect(nCurSeparate, nCurHeight- 3*nVspace, nCurSeparate + 3*nSeparateSpace, nCurHeight-2*nVspace);
				int nIndex = i;
				if (nIndex > 3)
					nIndex = nIndex - 3;
				DrawCurveItem(pDC, itemRect, nIndex,m_CurveNum,GetSorName(m_CurveNum));
				nCurSeparate = nCurSeparate + nSeparateSpace + nHmidspace / 3;
			}
		}
	}
	//曲线3
	m_CurveNum=6*(m_nCurPage-1)+2;//第几条曲线
	if (g_sorFileArray.totalCurveNumber>m_CurveNum)
	{
		//曲线3大矩形
		nCurHeight = nCurHeight + nVspace * 2;
		CRect curveRect;
		curveRect.SetRect(nHleftspace*2, nCurHeight-3*nVspace, (m_rectDraw.Width() - nHleftspace)/2, nCurHeight + nVspace * 7);
		//curveRect.SetRect(nHleftspace*2 + nHmidspace/2, nCurHeight, (m_rectDraw.Width() - nHleftspace)/2, nCurHeight + nVspace * 10);
		pDC->Rectangle(&curveRect);
		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		DrawGridLine(pDC, curveRect);//绘制栅格 wcq2012.10.31
		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		DrawCurveByReport(pDC, curveRect, m_CurveNum); //绘制曲线3
		//左侧小矩形
		CRect leftRect;
		leftRect.SetRect(nHleftspace, curveRect.top + curveRect.Height()/2-5*nVspace, curveRect.left - nHmidspace/2, curveRect.top + curveRect.Height()/2-3*nVspace);
		//leftRect.SetRect(nHleftspace, curveRect.top + curveRect.Height()/2-nVspace, curveRect.left - nHmidspace/2, curveRect.top + curveRect.Height()/2+nVspace);
		pDC->Rectangle(&leftRect);
		DrawVerCoordUnit(pDC, leftRect);
		//底侧小矩形
		CRect bottomRect;
		bottomRect.SetRect(curveRect.left-nHmidspace/3, curveRect.bottom +nVspace, curveRect.left + curveRect.Width()/2+nHleftspace*5/4, curveRect.bottom +nVspace*3);
		//bottomRect.SetRect(curveRect.left + curveRect.Width()/2-nHmidspace, curveRect.bottom +nVspace/2, curveRect.left + curveRect.Width()/2+nHmidspace, curveRect.bottom +nVspace*3/2);
		pDC->Rectangle(&bottomRect);
		CRect leftUnitRect;
		leftUnitRect.SetRect(curveRect.left-nHleftspace, bottomRect.top, curveRect.left + 400,  bottomRect.top + nVspace);
		//leftUnitRect.SetRect(curveRect.left, bottomRect.top, curveRect.left + 400,  bottomRect.top + nVspace);
		DrawHorCoordUnit(pDC, leftUnitRect, MAIN->m_pWaveFormView->m_strStartDis); //绘制横坐标文本左侧

		CRect mideUnitRect;
		mideUnitRect.SetRect(curveRect.left-nHmidspace/2, bottomRect.top, curveRect.right,  bottomRect.bottom);
		//mideUnitRect.SetRect(bottomRect.left, bottomRect.top, bottomRect.right,  bottomRect.bottom);
		//DrawHorCoordUnit(pDC, mideUnitRect, MAIN->m_pWaveFormView->m_strUnitDis); //绘制横坐标文本单位长度//wcq2012.11.02
		CString strDivUnit;
		float m_float_zll=0.0f;
		float flo_Length=g_sorFileArray.sorFileList[m_CurveNum].pTestResultInfo->Length;
		
		DrawHorCoordUnit(pDC, mideUnitRect, branch(m_float_zll,flo_Length)); //绘制横坐标文本单位长度//wcq2012.11.02

		CRect rightUnitRect;
		rightUnitRect.SetRect(curveRect.right -nHleftspace, bottomRect.top, curveRect.right,  bottomRect.top + nVspace);
		//rightUnitRect.SetRect(curveRect.right -400, bottomRect.top, curveRect.right,  bottomRect.top + nVspace);
		//DrawHorCoordUnit(pDC, rightUnitRect, MAIN->m_pWaveFormView->m_strEndDis); //绘制横坐标文本//wcq2012.11.02
		
		DrawHorCoordUnit(pDC, rightUnitRect, right_MAX(m_float_zll,flo_Length));//绘制横坐标文本 //横坐标右边最大长度 cwcq2012.11.02

	}
	//曲线4
	m_CurveNum=6*(m_nCurPage-1)+3;//第几条曲线
	if (g_sorFileArray.totalCurveNumber>m_CurveNum)
	{
		//曲线4大矩形
		CRect curveRect;
		curveRect.SetRect((m_rectDraw.Width() + nHmidspace)/2 + nHleftspace, nCurHeight-3*nVspace, m_rectDraw.Width() - nHleftspace*6/5, nCurHeight + nVspace * 7);
		pDC->Rectangle(&curveRect);
		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		DrawGridLine(pDC, curveRect);//绘制栅格 wcq2012.10.31
		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		DrawCurveByReport(pDC, curveRect, m_CurveNum); //绘制曲线4
		//左侧小矩形
		CRect leftRect;
		leftRect.SetRect((m_rectDraw.Width() + nHmidspace)/2, curveRect.top + curveRect.Height()/2-5*nVspace, curveRect.left - nHmidspace/2, curveRect.top + curveRect.Height()/2-3*nVspace);
		pDC->Rectangle(&leftRect);
		DrawVerCoordUnit(pDC, leftRect);
		//底侧小矩形
		CRect bottomRect;
		bottomRect.SetRect(curveRect.left-nHmidspace/3, curveRect.bottom +nVspace, curveRect.left + curveRect.Width()/2+nHleftspace*5/4, curveRect.bottom +nVspace*3);
		//bottomRect.SetRect(curveRect.left + curveRect.Width()/2-nHmidspace, curveRect.bottom +nVspace/2, curveRect.left + curveRect.Width()/2+nHmidspace, curveRect.bottom +nVspace*3/2);
		pDC->Rectangle(&bottomRect);
		CRect leftUnitRect;
		leftUnitRect.SetRect(curveRect.left-nHleftspace, bottomRect.top, curveRect.left + 400,  bottomRect.top + nVspace);
		//leftUnitRect.SetRect(curveRect.left, bottomRect.top, curveRect.left + 400,  bottomRect.top + nVspace);
		DrawHorCoordUnit(pDC, leftUnitRect, MAIN->m_pWaveFormView->m_strStartDis); //绘制横坐标文本左侧

		CRect mideUnitRect;
		mideUnitRect.SetRect(curveRect.left-nHmidspace/2, bottomRect.top, curveRect.right,  bottomRect.bottom);
		//mideUnitRect.SetRect(bottomRect.left, bottomRect.top, bottomRect.right,  bottomRect.bottom);
		//DrawHorCoordUnit(pDC, mideUnitRect, MAIN->m_pWaveFormView->m_strUnitDis); //绘制横坐标文本单位长度//wcq2012.11.02
		CString strDivUnit;
		float m_float_zll=0.0f;
		float flo_Length=g_sorFileArray.sorFileList[m_CurveNum].pTestResultInfo->Length;
		
		DrawHorCoordUnit(pDC, mideUnitRect, branch(m_float_zll,flo_Length)); //绘制横坐标文本单位长度//wcq2012.11.02

		CRect rightUnitRect;
		rightUnitRect.SetRect(curveRect.right -nHleftspace, bottomRect.top, curveRect.right,  bottomRect.top + nVspace);
		//rightUnitRect.SetRect(curveRect.right -400, bottomRect.top, curveRect.right,  bottomRect.top + nVspace);
		//DrawHorCoordUnit(pDC, rightUnitRect, MAIN->m_pWaveFormView->m_strEndDis); //绘制横坐标文本//wcq2012.11.02
		
		DrawHorCoordUnit(pDC, rightUnitRect, right_MAX(m_float_zll,flo_Length));//绘制横坐标文本 //横坐标右边最大长度 cwcq2012.11.02

	}
	//曲线56分割线
	m_CurveNum=6*(m_nCurPage-1)+5;//第几条曲线
	if (g_sorFileArray.totalCurveNumber>m_CurveNum)
	{
		nCurHeight = nCurHeight + nVspace * 17;
		nCurSeparate = nHleftspace;
		for (int i=1; i<=6; i++)
		{
			if (2==i)
			{
				pDC->MoveTo(nCurSeparate, nCurHeight-4*nVspace);
				pDC->LineTo(nCurSeparate + nSeparateSpace*5/2, nCurHeight-4*nVspace);
				CRect itemRect;
				itemRect.SetRect(nCurSeparate, nCurHeight- 5*nVspace, nCurSeparate + 3*nSeparateSpace, nCurHeight-4*nVspace);
				int nIndex = i;
				if (nIndex > 3)
					nIndex = nIndex - 3;
				DrawCurveItem(pDC, itemRect, nIndex,m_CurveNum,GetSorName(m_CurveNum));
				nCurSeparate = nCurSeparate + nSeparateSpace + nHmidspace / 3;
			}
			else if (5==i)
			{
				pDC->MoveTo((m_rectDraw.Width() + nHmidspace)/2, nCurHeight-4*nVspace);
				pDC->LineTo((m_rectDraw.Width() + nHmidspace)/2 + nSeparateSpace*5/2, nCurHeight-4*nVspace);
				CRect itemRect;
				itemRect.SetRect((m_rectDraw.Width() + nHmidspace)/2, nCurHeight- 5*nVspace, nCurSeparate*3 + 3*nSeparateSpace, nCurHeight-4*nVspace);
				int nIndex = i;
				if (nIndex > 3)
					nIndex = nIndex - 3;
				DrawCurveItem(pDC, itemRect, nIndex,m_CurveNum+1,GetSorName(m_CurveNum+1));
				nCurSeparate = nCurSeparate + nSeparateSpace + nHmidspace / 3;
			}
		}
	}
	else if (g_sorFileArray.totalCurveNumber==m_CurveNum)
	{
		nCurHeight = nCurHeight + nVspace * 17;
		nCurSeparate = nHleftspace;
		for (int i=1; i<=3; i++)
		{
			if (2==i)
			{
				pDC->MoveTo(nCurSeparate, nCurHeight-4*nVspace);
				pDC->LineTo(nCurSeparate + nSeparateSpace*5/2, nCurHeight-4*nVspace);
				CRect itemRect;
				itemRect.SetRect(nCurSeparate, nCurHeight- 5*nVspace, nCurSeparate + 3*nSeparateSpace, nCurHeight-4*nVspace);
				int nIndex = i;
				if (nIndex > 3)
					nIndex = nIndex - 3;
				DrawCurveItem(pDC, itemRect, nIndex,m_CurveNum,GetSorName(m_CurveNum));
				nCurSeparate = nCurSeparate + nSeparateSpace + nHmidspace / 3;
			}
		}
	}
	//曲线5
	m_CurveNum=6*(m_nCurPage-1)+4;//第几条曲线
	if (g_sorFileArray.totalCurveNumber>m_CurveNum)
	{
		//曲线5大矩形
		nCurHeight = nCurHeight + nVspace * 2;
		CRect curveRect;
		curveRect.SetRect(nHleftspace*2, nCurHeight-5*nVspace, (m_rectDraw.Width() - nHleftspace)/2, nCurHeight + nVspace * 5);
		//curveRect.SetRect(nHleftspace*2 + nHmidspace/2, nCurHeight, (m_rectDraw.Width() - nHleftspace)/2, nCurHeight + nVspace * 10);
		pDC->Rectangle(&curveRect);
		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		DrawGridLine(pDC, curveRect);//绘制栅格 wcq2012.10.31
		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		DrawCurveByReport(pDC, curveRect, m_CurveNum);//绘制曲线5
		//左侧小矩形
		CRect leftRect;
		leftRect.SetRect(nHleftspace, curveRect.top + curveRect.Height()/2-5*nVspace, curveRect.left - nHmidspace/2, curveRect.top + curveRect.Height()/2-3*nVspace);
		//leftRect.SetRect(nHleftspace, curveRect.top + curveRect.Height()/2-nVspace, curveRect.left - nHmidspace/2, curveRect.top + curveRect.Height()/2+nVspace);
		pDC->Rectangle(&leftRect);
		DrawVerCoordUnit(pDC, leftRect);
		//底侧小矩形
		CRect bottomRect;
		bottomRect.SetRect(curveRect.left-nHmidspace/3, curveRect.bottom +nVspace, curveRect.left + curveRect.Width()/2+nHleftspace*5/4, curveRect.bottom +nVspace*3);
		//bottomRect.SetRect(curveRect.left + curveRect.Width()/2-nHmidspace, curveRect.bottom +nVspace/2, curveRect.left + curveRect.Width()/2+nHmidspace, curveRect.bottom +nVspace*3/2);
		pDC->Rectangle(&bottomRect);
		CRect leftUnitRect;
		leftUnitRect.SetRect(curveRect.left-nHleftspace, bottomRect.top, curveRect.left + 400,  bottomRect.top + nVspace);
		//leftUnitRect.SetRect(curveRect.left, bottomRect.top, curveRect.left + 400,  bottomRect.top + nVspace);
		DrawHorCoordUnit(pDC, leftUnitRect, MAIN->m_pWaveFormView->m_strStartDis); //绘制横坐标文本左侧

		CRect mideUnitRect;
		mideUnitRect.SetRect(curveRect.left-nHmidspace/2, bottomRect.top, curveRect.right,  bottomRect.bottom);
		//mideUnitRect.SetRect(bottomRect.left, bottomRect.top, bottomRect.right,  bottomRect.bottom);
		//DrawHorCoordUnit(pDC, mideUnitRect, MAIN->m_pWaveFormView->m_strUnitDis); //绘制横坐标文本单位长度//wcq2012.11.02
		CString strDivUnit;
		float m_float_zll=0.0f;
		float flo_Length=g_sorFileArray.sorFileList[m_CurveNum].pTestResultInfo->Length;
		
		DrawHorCoordUnit(pDC, mideUnitRect, branch(m_float_zll,flo_Length)); //绘制横坐标文本单位长度//wcq2012.11.02

		CRect rightUnitRect;
		rightUnitRect.SetRect(curveRect.right -nHleftspace, bottomRect.top, curveRect.right,  bottomRect.top + nVspace);
		//rightUnitRect.SetRect(curveRect.right -400, bottomRect.top, curveRect.right,  bottomRect.top + nVspace);
		//DrawHorCoordUnit(pDC, rightUnitRect, MAIN->m_pWaveFormView->m_strEndDis); //绘制横坐标文本//wcq2012.11.02
		
		DrawHorCoordUnit(pDC, rightUnitRect, right_MAX(m_float_zll,flo_Length));//绘制横坐标文本 //横坐标右边最大长度 cwcq2012.11.02

	}
	//曲线6
	m_CurveNum=6*(m_nCurPage-1)+5;//第几条曲线
	if (g_sorFileArray.totalCurveNumber>m_CurveNum)
	{
		//曲线6大矩形
		CRect curveRect;
		curveRect.SetRect((m_rectDraw.Width() + nHmidspace)/2 + nHleftspace, nCurHeight-5*nVspace, m_rectDraw.Width() - nHleftspace*6/5, nCurHeight + nVspace * 5);
		pDC->Rectangle(&curveRect);
		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		DrawGridLine(pDC, curveRect);//绘制栅格 wcq2012.10.31
		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		DrawCurveByReport(pDC, curveRect, m_CurveNum);//绘制曲线6
		//左侧小矩形
		CRect leftRect;
		leftRect.SetRect((m_rectDraw.Width() + nHmidspace)/2, curveRect.top + curveRect.Height()/2-5*nVspace, curveRect.left - nHmidspace/2, curveRect.top + curveRect.Height()/2-3*nVspace);
		pDC->Rectangle(&leftRect);
		DrawVerCoordUnit(pDC, leftRect);
		//底侧小矩形
		CRect bottomRect;
		bottomRect.SetRect(curveRect.left-nHmidspace/3, curveRect.bottom +nVspace, curveRect.left + curveRect.Width()/2+nHleftspace*5/4, curveRect.bottom +nVspace*3);
		//bottomRect.SetRect(curveRect.left + curveRect.Width()/2-nHmidspace, curveRect.bottom +nVspace/2, curveRect.left + curveRect.Width()/2+nHmidspace, curveRect.bottom +nVspace*3/2);
		pDC->Rectangle(&bottomRect);
		CRect leftUnitRect;
		leftUnitRect.SetRect(curveRect.left-nHleftspace, bottomRect.top, curveRect.left + 400,  bottomRect.top + nVspace);
		//leftUnitRect.SetRect(curveRect.left, bottomRect.top, curveRect.left + 400,  bottomRect.top + nVspace);
		DrawHorCoordUnit(pDC, leftUnitRect, MAIN->m_pWaveFormView->m_strStartDis); //绘制横坐标文本左侧

		CRect mideUnitRect;
		mideUnitRect.SetRect(curveRect.left-nHmidspace/2, bottomRect.top, curveRect.right,  bottomRect.bottom);
		//mideUnitRect.SetRect(bottomRect.left, bottomRect.top, bottomRect.right,  bottomRect.bottom);
		//DrawHorCoordUnit(pDC, mideUnitRect, MAIN->m_pWaveFormView->m_strUnitDis); //绘制横坐标文本单位长度//wcq2012.11.02
		CString strDivUnit;
		float m_float_zll=0.0f;
		float flo_Length=g_sorFileArray.sorFileList[m_CurveNum].pTestResultInfo->Length;
		
		DrawHorCoordUnit(pDC, mideUnitRect, branch(m_float_zll,flo_Length)); //绘制横坐标文本单位长度//wcq2012.11.02

		CRect rightUnitRect;
		rightUnitRect.SetRect(curveRect.right -nHleftspace, bottomRect.top, curveRect.right,  bottomRect.top + nVspace);
		//rightUnitRect.SetRect(curveRect.right -400, bottomRect.top, curveRect.right,  bottomRect.top + nVspace);
		//DrawHorCoordUnit(pDC, rightUnitRect, MAIN->m_pWaveFormView->m_strEndDis); //绘制横坐标文本//wcq2012.11.02
		
		DrawHorCoordUnit(pDC, rightUnitRect, right_MAX(m_float_zll,flo_Length));//绘制横坐标文本 //横坐标右边最大长度 cwcq2012.11.02

	}
}

void CPrintView::Draw8Curve(CDC* pDC,int m_nCurPage)
{
	//画线
	int nVspace = m_rectDraw.Height() / 64;
	int nCurHeight = nVspace * 3;
	int nHleftspace = m_rectDraw.Width() / 10;
	int nHmidspace = m_rectDraw.Width() / 20;
	int nSeparateSpace = (m_rectDraw.Width() - 2 * (nHleftspace + nHmidspace)) / 6;
	int nCurSeparate = nHleftspace;

	//标题
	pDC->MoveTo(m_rectDraw.Width() / 3, nCurHeight);
	pDC->LineTo(m_rectDraw.Width() * 2/3, nCurHeight);
	CRect titleRect;
	titleRect.SetRect(m_rectDraw.Width() / 3, nCurHeight-nVspace, m_rectDraw.Width() * 2/3, nCurHeight);
	pDC->DrawText(g_sorFileArray.reportConfig.Title, titleRect, DT_CENTER);
	//曲线12分割线
	int m_CurveNum=8*(m_nCurPage-1)+1;//第几条曲线
	if (g_sorFileArray.totalCurveNumber>m_CurveNum)
	{
		nCurHeight = nCurHeight + nVspace * 2;
		nCurSeparate = nHleftspace;
		for (int i=1; i<=6; i++)
		{
			if (2==i)
			{
				pDC->MoveTo(nCurSeparate, nCurHeight);
				pDC->LineTo(nCurSeparate + nSeparateSpace*5/2, nCurHeight);
				CRect itemRect;
				itemRect.SetRect(nCurSeparate, nCurHeight- nVspace, nCurSeparate + 3*nSeparateSpace, nCurHeight);
				int nIndex = i;
				if (nIndex > 3)
					nIndex = nIndex - 3;
				DrawCurveItem(pDC, itemRect, nIndex,m_CurveNum,GetSorName(m_CurveNum));
				nCurSeparate = nCurSeparate + nSeparateSpace + nHmidspace / 3;
			}
			else if (5==i)
			{
				pDC->MoveTo((m_rectDraw.Width() + nHmidspace)/2, nCurHeight);
				pDC->LineTo((m_rectDraw.Width() + nHmidspace)/2 + nSeparateSpace*5/2, nCurHeight);
				CRect itemRect;
				itemRect.SetRect((m_rectDraw.Width() + nHmidspace)/2, nCurHeight- nVspace, nCurSeparate*3 + 3*nSeparateSpace, nCurHeight);
				int nIndex = i;
				if (nIndex > 3)
					nIndex = nIndex - 3;
				DrawCurveItem(pDC, itemRect, nIndex,m_CurveNum+1,GetSorName(m_CurveNum+1));
				nCurSeparate = nCurSeparate + nSeparateSpace + nHmidspace / 3;
			}
		}
	}
	else if (g_sorFileArray.totalCurveNumber==m_CurveNum)
	{
		nCurHeight = nCurHeight + nVspace * 2;
		nCurSeparate = nHleftspace;
		for (int i=1; i<=3; i++)
		{
			if (2==i)
			{
				pDC->MoveTo(nCurSeparate, nCurHeight);
				pDC->LineTo(nCurSeparate + nSeparateSpace*5/2, nCurHeight);
				CRect itemRect;
				itemRect.SetRect(nCurSeparate, nCurHeight- nVspace, nCurSeparate + 3*nSeparateSpace, nCurHeight);
				int nIndex = i;
				if (nIndex > 3)
					nIndex = nIndex - 3;
				DrawCurveItem(pDC, itemRect, nIndex,m_CurveNum,GetSorName(m_CurveNum));
				nCurSeparate = nCurSeparate + nSeparateSpace + nHmidspace / 3;
			}
		}

	}
	//曲线1
	m_CurveNum=8*(m_nCurPage-1);//第几条曲线
	if (g_sorFileArray.totalCurveNumber>m_CurveNum)
	{
		//曲线1大矩形
		nCurHeight = nCurHeight + nVspace * 2;
		CRect curveRect;
		curveRect.SetRect(nHleftspace*2, nCurHeight-nVspace, (m_rectDraw.Width() - nHleftspace)/2, nCurHeight + nVspace * 10-nVspace);
		//curveRect.SetRect(nHleftspace*2 + nHmidspace/2, nCurHeight, (m_rectDraw.Width() - nHleftspace)/2, nCurHeight + nVspace * 10);
		pDC->Rectangle(&curveRect);
		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		DrawGridLine(pDC, curveRect);//绘制栅格 wcq2012.10.31
		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		DrawCurveByReport(pDC, curveRect, m_CurveNum);//绘制曲线1
		//左侧小矩形
		CRect leftRect;
		leftRect.SetRect(nHleftspace, curveRect.top + curveRect.Height()/2-5*nVspace, curveRect.left - nHmidspace/2, curveRect.top + curveRect.Height()/2-3*nVspace);
		//leftRect.SetRect(nHleftspace, curveRect.top + curveRect.Height()/2-nVspace, curveRect.left - nHmidspace/2, curveRect.top + curveRect.Height()/2+nVspace);
		pDC->Rectangle(&leftRect);
		DrawVerCoordUnit(pDC, leftRect);
		//底侧小矩形
		CRect bottomRect;
		bottomRect.SetRect(curveRect.left-nHmidspace/3, curveRect.bottom +nVspace, curveRect.left + curveRect.Width()/2+nHleftspace*5/4, curveRect.bottom +nVspace*3);
		//bottomRect.SetRect(curveRect.left + curveRect.Width()/2-nHmidspace, curveRect.bottom +nVspace/2, curveRect.left + curveRect.Width()/2+nHmidspace, curveRect.bottom +nVspace*3/2);
		pDC->Rectangle(&bottomRect);
		CRect leftUnitRect;
		leftUnitRect.SetRect(curveRect.left-nHleftspace, bottomRect.top, curveRect.left + 400,  bottomRect.top + nVspace);
		//leftUnitRect.SetRect(curveRect.left, bottomRect.top, curveRect.left + 400,  bottomRect.top + nVspace);
		DrawHorCoordUnit(pDC, leftUnitRect, MAIN->m_pWaveFormView->m_strStartDis); //绘制横坐标文本左侧

		CRect mideUnitRect;
		mideUnitRect.SetRect(curveRect.left-nHmidspace/2, bottomRect.top, curveRect.right,  bottomRect.bottom);
		//mideUnitRect.SetRect(bottomRect.left, bottomRect.top, bottomRect.right,  bottomRect.bottom);
		//DrawHorCoordUnit(pDC, mideUnitRect, MAIN->m_pWaveFormView->m_strUnitDis); //绘制横坐标文本单位长度//wcq2012.11.02
		CString strDivUnit;
		float m_float_zll=0.0f;
		float flo_Length=g_sorFileArray.sorFileList[m_CurveNum].pTestResultInfo->Length;
		
		DrawHorCoordUnit(pDC, mideUnitRect, branch(m_float_zll,flo_Length)); //绘制横坐标文本单位长度//wcq2012.11.02

		CRect rightUnitRect;
		rightUnitRect.SetRect(curveRect.right -nHleftspace, bottomRect.top, curveRect.right,  bottomRect.top + nVspace);
		//rightUnitRect.SetRect(curveRect.right -400, bottomRect.top, curveRect.right,  bottomRect.top + nVspace);
		//DrawHorCoordUnit(pDC, rightUnitRect, MAIN->m_pWaveFormView->m_strEndDis); //绘制横坐标文本//wcq2012.11.02
		
		DrawHorCoordUnit(pDC, rightUnitRect, right_MAX(m_float_zll,flo_Length));//绘制横坐标文本 //横坐标右边最大长度 cwcq2012.11.02

	}
	//曲线2
	m_CurveNum=8*(m_nCurPage-1)+1;//第几条曲线
	if (g_sorFileArray.totalCurveNumber>m_CurveNum)
	{
		//曲线2大矩形
		CRect curveRect;
		curveRect.SetRect((m_rectDraw.Width() + nHmidspace)/2 + nHleftspace, nCurHeight-nVspace, m_rectDraw.Width() - nHleftspace*6/5, nCurHeight + nVspace * 10-nVspace);
		pDC->Rectangle(&curveRect);
		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		DrawGridLine(pDC, curveRect);//绘制栅格 wcq2012.10.31
		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		DrawCurveByReport(pDC, curveRect, m_CurveNum);//绘制曲线2
		//左侧小矩形
		CRect leftRect;
		leftRect.SetRect((m_rectDraw.Width() + nHmidspace)/2, curveRect.top + curveRect.Height()/2-5*nVspace, curveRect.left - nHmidspace/2, curveRect.top + curveRect.Height()/2-3*nVspace);
		pDC->Rectangle(&leftRect);
		DrawVerCoordUnit(pDC, leftRect);
		//底侧小矩形
		CRect bottomRect;
		bottomRect.SetRect(curveRect.left-nHmidspace/3, curveRect.bottom +nVspace, curveRect.left + curveRect.Width()/2+nHleftspace*5/4, curveRect.bottom +nVspace*3);
		//bottomRect.SetRect(curveRect.left + curveRect.Width()/2-nHmidspace, curveRect.bottom +nVspace/2, curveRect.left + curveRect.Width()/2+nHmidspace, curveRect.bottom +nVspace*3/2);
		pDC->Rectangle(&bottomRect);
		CRect leftUnitRect;
		leftUnitRect.SetRect(curveRect.left-nHleftspace, bottomRect.top, curveRect.left + 400,  bottomRect.top + nVspace);
		//leftUnitRect.SetRect(curveRect.left, bottomRect.top, curveRect.left + 400,  bottomRect.top + nVspace);
		DrawHorCoordUnit(pDC, leftUnitRect, MAIN->m_pWaveFormView->m_strStartDis); //绘制横坐标文本左侧

		CRect mideUnitRect;
		mideUnitRect.SetRect(curveRect.left-nHmidspace/2, bottomRect.top, curveRect.right,  bottomRect.bottom);
		//mideUnitRect.SetRect(bottomRect.left, bottomRect.top, bottomRect.right,  bottomRect.bottom);
		//DrawHorCoordUnit(pDC, mideUnitRect, MAIN->m_pWaveFormView->m_strUnitDis); //绘制横坐标文本单位长度//wcq2012.11.02
		CString strDivUnit;
		float m_float_zll=0.0f;
		float flo_Length=g_sorFileArray.sorFileList[m_CurveNum].pTestResultInfo->Length;
		
		DrawHorCoordUnit(pDC, mideUnitRect, branch(m_float_zll,flo_Length)); //绘制横坐标文本单位长度//wcq2012.11.02

		CRect rightUnitRect;
		rightUnitRect.SetRect(curveRect.right -nHleftspace, bottomRect.top, curveRect.right,  bottomRect.top + nVspace);
		//rightUnitRect.SetRect(curveRect.right -400, bottomRect.top, curveRect.right,  bottomRect.top + nVspace);
		//DrawHorCoordUnit(pDC, rightUnitRect, MAIN->m_pWaveFormView->m_strEndDis); //绘制横坐标文本//wcq2012.11.02
		
		DrawHorCoordUnit(pDC, rightUnitRect, right_MAX(m_float_zll,flo_Length));//绘制横坐标文本 //横坐标右边最大长度 cwcq2012.11.02

	}
	//曲线34分割线
	m_CurveNum=8*(m_nCurPage-1)+3;//第几条曲线
	if (g_sorFileArray.totalCurveNumber>m_CurveNum)
	{
		nCurHeight = nCurHeight + nVspace * 13;
		nCurSeparate = nHleftspace;
		for (int i=1; i<=6; i++)
		{
			if (2==i)
			{
				pDC->MoveTo(nCurSeparate, nCurHeight);
				pDC->LineTo(nCurSeparate + nSeparateSpace*5/2, nCurHeight);
				CRect itemRect;
				itemRect.SetRect(nCurSeparate, nCurHeight- nVspace, nCurSeparate + 3*nSeparateSpace, nCurHeight);
				int nIndex = i;
				if (nIndex > 3)
					nIndex = nIndex - 3;
				DrawCurveItem(pDC, itemRect, nIndex,m_CurveNum,GetSorName(m_CurveNum));
				nCurSeparate = nCurSeparate + nSeparateSpace + nHmidspace / 3;
			}
			else if (5==i)
			{
				pDC->MoveTo((m_rectDraw.Width() + nHmidspace)/2, nCurHeight);
				pDC->LineTo((m_rectDraw.Width() + nHmidspace)/2 + nSeparateSpace*5/2, nCurHeight);
				CRect itemRect;
				itemRect.SetRect((m_rectDraw.Width() + nHmidspace)/2, nCurHeight- nVspace, nCurSeparate*3 + 3*nSeparateSpace, nCurHeight);
				int nIndex = i;
				if (nIndex > 3)
					nIndex = nIndex - 3;
				DrawCurveItem(pDC, itemRect, nIndex,m_CurveNum+1,GetSorName(m_CurveNum+1));
				nCurSeparate = nCurSeparate + nSeparateSpace + nHmidspace / 3;
			}
		}
	}
	else if (g_sorFileArray.totalCurveNumber==m_CurveNum)
	{
		nCurHeight = nCurHeight + nVspace * 13;
		nCurSeparate = nHleftspace;
		for (int i=1; i<=3; i++)
		{
			if (2==i)
			{
				pDC->MoveTo(nCurSeparate, nCurHeight);
				pDC->LineTo(nCurSeparate + nSeparateSpace*5/2, nCurHeight);
				CRect itemRect;
				itemRect.SetRect(nCurSeparate, nCurHeight- nVspace, nCurSeparate + 3*nSeparateSpace, nCurHeight);
				int nIndex = i;
				if (nIndex > 3)
					nIndex = nIndex - 3;
				DrawCurveItem(pDC, itemRect, nIndex,m_CurveNum,GetSorName(m_CurveNum));
				nCurSeparate = nCurSeparate + nSeparateSpace + nHmidspace / 3;
			}
		}
	}
	//曲线3
	m_CurveNum=8*(m_nCurPage-1)+2;//第几条曲线
	if (g_sorFileArray.totalCurveNumber>m_CurveNum)
	{
		//曲线3大矩形
		nCurHeight = nCurHeight + nVspace * 2;
		CRect curveRect;
		curveRect.SetRect(nHleftspace*2, nCurHeight-nVspace, (m_rectDraw.Width() - nHleftspace)/2, nCurHeight + nVspace * 10-nVspace);
		//curveRect.SetRect(nHleftspace*2 + nHmidspace/2, nCurHeight, (m_rectDraw.Width() - nHleftspace)/2, nCurHeight + nVspace * 10);
		pDC->Rectangle(&curveRect);
		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		DrawGridLine(pDC, curveRect);//绘制栅格 wcq2012.10.31
		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		DrawCurveByReport(pDC, curveRect, m_CurveNum); //绘制曲线3
		//左侧小矩形
		CRect leftRect;
		leftRect.SetRect(nHleftspace, curveRect.top + curveRect.Height()/2-5*nVspace, curveRect.left - nHmidspace/2, curveRect.top + curveRect.Height()/2-3*nVspace);
		//leftRect.SetRect(nHleftspace, curveRect.top + curveRect.Height()/2-nVspace, curveRect.left - nHmidspace/2, curveRect.top + curveRect.Height()/2+nVspace);
		pDC->Rectangle(&leftRect);
		DrawVerCoordUnit(pDC, leftRect);
		//底侧小矩形
		CRect bottomRect;
		bottomRect.SetRect(curveRect.left-nHmidspace/3, curveRect.bottom +nVspace, curveRect.left + curveRect.Width()/2+nHleftspace*5/4, curveRect.bottom +nVspace*3);
		//bottomRect.SetRect(curveRect.left + curveRect.Width()/2-nHmidspace, curveRect.bottom +nVspace/2, curveRect.left + curveRect.Width()/2+nHmidspace, curveRect.bottom +nVspace*3/2);
		pDC->Rectangle(&bottomRect);
		CRect leftUnitRect;
		leftUnitRect.SetRect(curveRect.left-nHleftspace, bottomRect.top, curveRect.left + 400,  bottomRect.top + nVspace);
		//leftUnitRect.SetRect(curveRect.left, bottomRect.top, curveRect.left + 400,  bottomRect.top + nVspace);
		DrawHorCoordUnit(pDC, leftUnitRect, MAIN->m_pWaveFormView->m_strStartDis); //绘制横坐标文本左侧

		CRect mideUnitRect;
		mideUnitRect.SetRect(curveRect.left-nHmidspace/2, bottomRect.top, curveRect.right,  bottomRect.bottom);
		//mideUnitRect.SetRect(bottomRect.left, bottomRect.top, bottomRect.right,  bottomRect.bottom);
		//DrawHorCoordUnit(pDC, mideUnitRect, MAIN->m_pWaveFormView->m_strUnitDis); //绘制横坐标文本单位长度//wcq2012.11.02
		CString strDivUnit;
		float m_float_zll=0.0f;
		float flo_Length=g_sorFileArray.sorFileList[m_CurveNum].pTestResultInfo->Length;
		
		DrawHorCoordUnit(pDC, mideUnitRect, branch(m_float_zll,flo_Length)); //绘制横坐标文本单位长度//wcq2012.11.02

		CRect rightUnitRect;
		rightUnitRect.SetRect(curveRect.right -nHleftspace, bottomRect.top, curveRect.right,  bottomRect.top + nVspace);
		//rightUnitRect.SetRect(curveRect.right -400, bottomRect.top, curveRect.right,  bottomRect.top + nVspace);
		//DrawHorCoordUnit(pDC, rightUnitRect, MAIN->m_pWaveFormView->m_strEndDis); //绘制横坐标文本//wcq2012.11.02
		
		DrawHorCoordUnit(pDC, rightUnitRect, right_MAX(m_float_zll,flo_Length));//绘制横坐标文本 //横坐标右边最大长度 cwcq2012.11.02

	}
	//曲线4
	m_CurveNum=8*(m_nCurPage-1)+3;//第几条曲线
	if (g_sorFileArray.totalCurveNumber>m_CurveNum)
	{
		//曲线4大矩形
		CRect curveRect;
		curveRect.SetRect((m_rectDraw.Width() + nHmidspace)/2 + nHleftspace, nCurHeight-nVspace, m_rectDraw.Width() - nHleftspace*6/5, nCurHeight + nVspace * 10-nVspace);
		pDC->Rectangle(&curveRect);
		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		DrawGridLine(pDC, curveRect);//绘制栅格 wcq2012.10.31
		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		DrawCurveByReport(pDC, curveRect, m_CurveNum); //绘制曲线4
		//左侧小矩形
		CRect leftRect;
		leftRect.SetRect((m_rectDraw.Width() + nHmidspace)/2, curveRect.top + curveRect.Height()/2-5*nVspace, curveRect.left - nHmidspace/2, curveRect.top + curveRect.Height()/2-3*nVspace);
		pDC->Rectangle(&leftRect);
		DrawVerCoordUnit(pDC, leftRect);
		//底侧小矩形
		CRect bottomRect;
		bottomRect.SetRect(curveRect.left-nHmidspace/3, curveRect.bottom +nVspace, curveRect.left + curveRect.Width()/2+nHleftspace*5/4, curveRect.bottom +nVspace*3);
		//bottomRect.SetRect(curveRect.left + curveRect.Width()/2-nHmidspace, curveRect.bottom +nVspace/2, curveRect.left + curveRect.Width()/2+nHmidspace, curveRect.bottom +nVspace*3/2);
		pDC->Rectangle(&bottomRect);
		CRect leftUnitRect;
		leftUnitRect.SetRect(curveRect.left-nHleftspace, bottomRect.top, curveRect.left + 400,  bottomRect.top + nVspace);
		//leftUnitRect.SetRect(curveRect.left, bottomRect.top, curveRect.left + 400,  bottomRect.top + nVspace);
		DrawHorCoordUnit(pDC, leftUnitRect, MAIN->m_pWaveFormView->m_strStartDis); //绘制横坐标文本左侧

		CRect mideUnitRect;
		mideUnitRect.SetRect(curveRect.left-nHmidspace/2, bottomRect.top, curveRect.right,  bottomRect.bottom);
		//mideUnitRect.SetRect(bottomRect.left, bottomRect.top, bottomRect.right,  bottomRect.bottom);
		//DrawHorCoordUnit(pDC, mideUnitRect, MAIN->m_pWaveFormView->m_strUnitDis); //绘制横坐标文本单位长度//wcq2012.11.02
		CString strDivUnit;
		float m_float_zll=0.0f;
		float flo_Length=g_sorFileArray.sorFileList[m_CurveNum].pTestResultInfo->Length;
		
		DrawHorCoordUnit(pDC, mideUnitRect, branch(m_float_zll,flo_Length)); //绘制横坐标文本单位长度//wcq2012.11.02

		CRect rightUnitRect;
		rightUnitRect.SetRect(curveRect.right -nHleftspace, bottomRect.top, curveRect.right,  bottomRect.top + nVspace);
		//rightUnitRect.SetRect(curveRect.right -400, bottomRect.top, curveRect.right,  bottomRect.top + nVspace);
		//DrawHorCoordUnit(pDC, rightUnitRect, MAIN->m_pWaveFormView->m_strEndDis); //绘制横坐标文本//wcq2012.11.02
		
		DrawHorCoordUnit(pDC, rightUnitRect, right_MAX(m_float_zll,flo_Length));//绘制横坐标文本 //横坐标右边最大长度 cwcq2012.11.02

	}
	//曲线56分割线
	m_CurveNum=8*(m_nCurPage-1)+5;//第几条曲线
	if (g_sorFileArray.totalCurveNumber>m_CurveNum)
	{
		nCurHeight = nCurHeight + nVspace * 13;
		nCurSeparate = nHleftspace;
		for (int i=1; i<=6; i++)
		{
			if (2==i)
			{
				pDC->MoveTo(nCurSeparate, nCurHeight);
				pDC->LineTo(nCurSeparate + nSeparateSpace*5/2, nCurHeight);
				CRect itemRect;
				itemRect.SetRect(nCurSeparate, nCurHeight- nVspace, nCurSeparate + 3*nSeparateSpace, nCurHeight);
				int nIndex = i;
				if (nIndex > 3)
					nIndex = nIndex - 3;
				DrawCurveItem(pDC, itemRect, nIndex,m_CurveNum,GetSorName(m_CurveNum));
				nCurSeparate = nCurSeparate + nSeparateSpace + nHmidspace / 3;
			}
			else if (5==i)
			{
				pDC->MoveTo((m_rectDraw.Width() + nHmidspace)/2, nCurHeight);
				pDC->LineTo((m_rectDraw.Width() + nHmidspace)/2 + nSeparateSpace*5/2, nCurHeight);
				CRect itemRect;
				itemRect.SetRect((m_rectDraw.Width() + nHmidspace)/2, nCurHeight- nVspace, nCurSeparate*3 + 3*nSeparateSpace, nCurHeight);
				int nIndex = i;
				if (nIndex > 3)
					nIndex = nIndex - 3;
				DrawCurveItem(pDC, itemRect, nIndex,m_CurveNum+1,GetSorName(m_CurveNum+1));
				nCurSeparate = nCurSeparate + nSeparateSpace + nHmidspace / 3;
			}
		}
	}
	else if (g_sorFileArray.totalCurveNumber==m_CurveNum)
	{
		nCurHeight = nCurHeight + nVspace * 13;
		nCurSeparate = nHleftspace;
		for (int i=1; i<=3; i++)
		{
			if (2==i)
			{
				pDC->MoveTo(nCurSeparate, nCurHeight);
				pDC->LineTo(nCurSeparate + nSeparateSpace*5/2, nCurHeight);
				CRect itemRect;
				itemRect.SetRect(nCurSeparate, nCurHeight- nVspace, nCurSeparate + 3*nSeparateSpace, nCurHeight);
				int nIndex = i;
				if (nIndex > 3)
					nIndex = nIndex - 3;
				DrawCurveItem(pDC, itemRect, nIndex,m_CurveNum,GetSorName(m_CurveNum));
				nCurSeparate = nCurSeparate + nSeparateSpace + nHmidspace / 3;
			}
		}
	}
	//曲线5
	m_CurveNum=8*(m_nCurPage-1)+4;//第几条曲线
	if (g_sorFileArray.totalCurveNumber>m_CurveNum)
	{
		//曲线5大矩形
		nCurHeight = nCurHeight + nVspace * 2;
		CRect curveRect;
		curveRect.SetRect(nHleftspace*2, nCurHeight-nVspace, (m_rectDraw.Width() - nHleftspace)/2, nCurHeight + nVspace * 10-nVspace);
		//curveRect.SetRect(nHleftspace*2 + nHmidspace/2, nCurHeight, (m_rectDraw.Width() - nHleftspace)/2, nCurHeight + nVspace * 10);
		pDC->Rectangle(&curveRect);
		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		DrawGridLine(pDC, curveRect);//绘制栅格 wcq2012.10.31
		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		DrawCurveByReport(pDC, curveRect, m_CurveNum);//绘制曲线5
		//左侧小矩形
		CRect leftRect;
		leftRect.SetRect(nHleftspace, curveRect.top + curveRect.Height()/2-5*nVspace, curveRect.left - nHmidspace/2, curveRect.top + curveRect.Height()/2-3*nVspace);
		//leftRect.SetRect(nHleftspace, curveRect.top + curveRect.Height()/2-nVspace, curveRect.left - nHmidspace/2, curveRect.top + curveRect.Height()/2+nVspace);
		pDC->Rectangle(&leftRect);
		DrawVerCoordUnit(pDC, leftRect);
		//底侧小矩形
		CRect bottomRect;
		bottomRect.SetRect(curveRect.left-nHmidspace/3, curveRect.bottom +nVspace, curveRect.left + curveRect.Width()/2+nHleftspace*5/4, curveRect.bottom +nVspace*3);
		//bottomRect.SetRect(curveRect.left + curveRect.Width()/2-nHmidspace, curveRect.bottom +nVspace/2, curveRect.left + curveRect.Width()/2+nHmidspace, curveRect.bottom +nVspace*3/2);
		pDC->Rectangle(&bottomRect);
		CRect leftUnitRect;
		leftUnitRect.SetRect(curveRect.left-nHleftspace, bottomRect.top, curveRect.left + 400,  bottomRect.top + nVspace);
		//leftUnitRect.SetRect(curveRect.left, bottomRect.top, curveRect.left + 400,  bottomRect.top + nVspace);
		DrawHorCoordUnit(pDC, leftUnitRect, MAIN->m_pWaveFormView->m_strStartDis); //绘制横坐标文本左侧

		CRect mideUnitRect;
		mideUnitRect.SetRect(curveRect.left-nHmidspace/2, bottomRect.top, curveRect.right,  bottomRect.bottom);
		//mideUnitRect.SetRect(bottomRect.left, bottomRect.top, bottomRect.right,  bottomRect.bottom);
		//DrawHorCoordUnit(pDC, mideUnitRect, MAIN->m_pWaveFormView->m_strUnitDis); //绘制横坐标文本单位长度//wcq2012.11.02
		CString strDivUnit;
		float m_float_zll=0.0f;
		float flo_Length=g_sorFileArray.sorFileList[m_CurveNum].pTestResultInfo->Length;
		
		DrawHorCoordUnit(pDC, mideUnitRect, branch(m_float_zll,flo_Length)); //绘制横坐标文本单位长度//wcq2012.11.02

		CRect rightUnitRect;
		rightUnitRect.SetRect(curveRect.right -nHleftspace, bottomRect.top, curveRect.right,  bottomRect.top + nVspace);
		//rightUnitRect.SetRect(curveRect.right -400, bottomRect.top, curveRect.right,  bottomRect.top + nVspace);
		//DrawHorCoordUnit(pDC, rightUnitRect, MAIN->m_pWaveFormView->m_strEndDis); //绘制横坐标文本//wcq2012.11.02
		
		DrawHorCoordUnit(pDC, rightUnitRect, right_MAX(m_float_zll,flo_Length));//绘制横坐标文本 //横坐标右边最大长度 cwcq2012.11.02

	}
	//曲线6
	m_CurveNum=8*(m_nCurPage-1)+5;//第几条曲线
	if (g_sorFileArray.totalCurveNumber>m_CurveNum)
	{
		//曲线6大矩形
		CRect curveRect;
		curveRect.SetRect((m_rectDraw.Width() + nHmidspace)/2 + nHleftspace, nCurHeight-nVspace, m_rectDraw.Width() - nHleftspace*6/5, nCurHeight + nVspace * 10-nVspace);
		pDC->Rectangle(&curveRect);
		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		DrawGridLine(pDC, curveRect);//绘制栅格 wcq2012.10.31
		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		DrawCurveByReport(pDC, curveRect, m_CurveNum);//绘制曲线6
		//左侧小矩形
		CRect leftRect;
		leftRect.SetRect((m_rectDraw.Width() + nHmidspace)/2, curveRect.top + curveRect.Height()/2-5*nVspace, curveRect.left - nHmidspace/2, curveRect.top + curveRect.Height()/2-3*nVspace);
		pDC->Rectangle(&leftRect);
		DrawVerCoordUnit(pDC, leftRect);
		//底侧小矩形
		CRect bottomRect;
		bottomRect.SetRect(curveRect.left-nHmidspace/3, curveRect.bottom +nVspace, curveRect.left + curveRect.Width()/2+nHleftspace*5/4, curveRect.bottom +nVspace*3);
		//bottomRect.SetRect(curveRect.left + curveRect.Width()/2-nHmidspace, curveRect.bottom +nVspace/2, curveRect.left + curveRect.Width()/2+nHmidspace, curveRect.bottom +nVspace*3/2);
		pDC->Rectangle(&bottomRect);
		CRect leftUnitRect;
		leftUnitRect.SetRect(curveRect.left-nHleftspace, bottomRect.top, curveRect.left + 400,  bottomRect.top + nVspace);
		//leftUnitRect.SetRect(curveRect.left, bottomRect.top, curveRect.left + 400,  bottomRect.top + nVspace);
		DrawHorCoordUnit(pDC, leftUnitRect, MAIN->m_pWaveFormView->m_strStartDis); //绘制横坐标文本左侧

		CRect mideUnitRect;
		mideUnitRect.SetRect(curveRect.left-nHmidspace/2, bottomRect.top, curveRect.right,  bottomRect.bottom);
		//mideUnitRect.SetRect(bottomRect.left, bottomRect.top, bottomRect.right,  bottomRect.bottom);
		//DrawHorCoordUnit(pDC, mideUnitRect, MAIN->m_pWaveFormView->m_strUnitDis); //绘制横坐标文本单位长度//wcq2012.11.02
		CString strDivUnit;
		float m_float_zll=0.0f;
		float flo_Length=g_sorFileArray.sorFileList[m_CurveNum].pTestResultInfo->Length;
		
		DrawHorCoordUnit(pDC, mideUnitRect, branch(m_float_zll,flo_Length)); //绘制横坐标文本单位长度//wcq2012.11.02

		CRect rightUnitRect;
		rightUnitRect.SetRect(curveRect.right -nHleftspace, bottomRect.top, curveRect.right,  bottomRect.top + nVspace);
		//rightUnitRect.SetRect(curveRect.right -400, bottomRect.top, curveRect.right,  bottomRect.top + nVspace);
		//DrawHorCoordUnit(pDC, rightUnitRect, MAIN->m_pWaveFormView->m_strEndDis); //绘制横坐标文本//wcq2012.11.02
		
		DrawHorCoordUnit(pDC, rightUnitRect, right_MAX(m_float_zll,flo_Length));//绘制横坐标文本 //横坐标右边最大长度 cwcq2012.11.02

	}
	//曲线78分割线
	m_CurveNum=8*(m_nCurPage-1)+7;//第几条曲线
	if (g_sorFileArray.totalCurveNumber>m_CurveNum)
	{
		nCurHeight = nCurHeight + nVspace * 13;
		nCurSeparate = nHleftspace;
		for (int i=1; i<=6; i++)
		{
			if (2==i)
			{
				pDC->MoveTo(nCurSeparate, nCurHeight);
				pDC->LineTo(nCurSeparate + nSeparateSpace*5/2, nCurHeight);
				CRect itemRect;
				itemRect.SetRect(nCurSeparate, nCurHeight- nVspace, nCurSeparate + 3*nSeparateSpace, nCurHeight);
				int nIndex = i;
				if (nIndex > 3)
					nIndex = nIndex - 3;
				DrawCurveItem(pDC, itemRect, nIndex,m_CurveNum,GetSorName(m_CurveNum));
				nCurSeparate = nCurSeparate + nSeparateSpace + nHmidspace / 3;
			}
			else if (5==i)
			{
				pDC->MoveTo((m_rectDraw.Width() + nHmidspace)/2, nCurHeight);
				pDC->LineTo((m_rectDraw.Width() + nHmidspace)/2 + nSeparateSpace*5/2, nCurHeight);
				CRect itemRect;
				itemRect.SetRect((m_rectDraw.Width() + nHmidspace)/2, nCurHeight- nVspace, nCurSeparate*3 + 3*nSeparateSpace, nCurHeight);
				int nIndex = i;
				if (nIndex > 3)
					nIndex = nIndex - 3;
				DrawCurveItem(pDC, itemRect, nIndex,m_CurveNum+1,GetSorName(m_CurveNum+1));
				nCurSeparate = nCurSeparate + nSeparateSpace + nHmidspace / 3;
			}
		}
	}
	else if (g_sorFileArray.totalCurveNumber==m_CurveNum)
	{
		nCurHeight = nCurHeight + nVspace * 13;
		nCurSeparate = nHleftspace;
		for (int i=1; i<=3; i++)
		{
			if (2==i)
			{
				pDC->MoveTo(nCurSeparate, nCurHeight);
				pDC->LineTo(nCurSeparate + nSeparateSpace*5/2, nCurHeight);
				CRect itemRect;
				itemRect.SetRect(nCurSeparate, nCurHeight- nVspace, nCurSeparate + 3*nSeparateSpace, nCurHeight);
				int nIndex = i;
				if (nIndex > 3)
					nIndex = nIndex - 3;
				DrawCurveItem(pDC, itemRect, nIndex,m_CurveNum,GetSorName(m_CurveNum));
				nCurSeparate = nCurSeparate + nSeparateSpace + nHmidspace / 3;
			}
		}
	}
	//曲线7
	m_CurveNum=8*(m_nCurPage-1)+6;//第几条曲线
	if (g_sorFileArray.totalCurveNumber>m_CurveNum)
	{
		//曲线7大矩形
		nCurHeight = nCurHeight + nVspace * 2;
		CRect curveRect;
		curveRect.SetRect(nHleftspace*2, nCurHeight-nVspace, (m_rectDraw.Width() - nHleftspace)/2, nCurHeight + nVspace * 10-nVspace);
		//curveRect.SetRect(nHleftspace*2 + nHmidspace/2, nCurHeight, (m_rectDraw.Width() - nHleftspace)/2, nCurHeight + nVspace * 10);
		pDC->Rectangle(&curveRect);
		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		DrawGridLine(pDC, curveRect);//绘制栅格 wcq2012.10.31
		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		DrawCurveByReport(pDC, curveRect, m_CurveNum);//绘制曲线7
		//左侧小矩形
		CRect leftRect;
		leftRect.SetRect(nHleftspace, curveRect.top + curveRect.Height()/2-5*nVspace, curveRect.left - nHmidspace/2, curveRect.top + curveRect.Height()/2-3*nVspace);
		//leftRect.SetRect(nHleftspace, curveRect.top + curveRect.Height()/2-nVspace, curveRect.left - nHmidspace/2, curveRect.top + curveRect.Height()/2+nVspace);
		pDC->Rectangle(&leftRect);
		DrawVerCoordUnit(pDC, leftRect);
		//底侧小矩形
		CRect bottomRect;
		bottomRect.SetRect(curveRect.left-nHmidspace/3, curveRect.bottom +nVspace, curveRect.left + curveRect.Width()/2+nHleftspace*5/4, curveRect.bottom +nVspace*3);
		//bottomRect.SetRect(curveRect.left + curveRect.Width()/2-nHmidspace, curveRect.bottom +nVspace/2, curveRect.left + curveRect.Width()/2+nHmidspace, curveRect.bottom +nVspace*3/2);
		pDC->Rectangle(&bottomRect);
		CRect leftUnitRect;
		leftUnitRect.SetRect(curveRect.left-nHleftspace, bottomRect.top, curveRect.left + 400,  bottomRect.top + nVspace);
		//leftUnitRect.SetRect(curveRect.left, bottomRect.top, curveRect.left + 400,  bottomRect.top + nVspace);
		DrawHorCoordUnit(pDC, leftUnitRect, MAIN->m_pWaveFormView->m_strStartDis); //绘制横坐标文本左侧

		CRect mideUnitRect;
		mideUnitRect.SetRect(curveRect.left-nHmidspace/2, bottomRect.top, curveRect.right,  bottomRect.bottom);
		//mideUnitRect.SetRect(bottomRect.left, bottomRect.top, bottomRect.right,  bottomRect.bottom);
		//DrawHorCoordUnit(pDC, mideUnitRect, MAIN->m_pWaveFormView->m_strUnitDis); //绘制横坐标文本单位长度//wcq2012.11.02
		CString strDivUnit;
		float m_float_zll=0.0f;
		float flo_Length=g_sorFileArray.sorFileList[m_CurveNum].pTestResultInfo->Length;
		
		DrawHorCoordUnit(pDC, mideUnitRect, branch(m_float_zll,flo_Length)); //绘制横坐标文本单位长度//wcq2012.11.02

		CRect rightUnitRect;
		rightUnitRect.SetRect(curveRect.right -nHleftspace, bottomRect.top, curveRect.right,  bottomRect.top + nVspace);
		//rightUnitRect.SetRect(curveRect.right -400, bottomRect.top, curveRect.right,  bottomRect.top + nVspace);
		//DrawHorCoordUnit(pDC, rightUnitRect, MAIN->m_pWaveFormView->m_strEndDis); //绘制横坐标文本//wcq2012.11.02
		
		DrawHorCoordUnit(pDC, rightUnitRect, right_MAX(m_float_zll,flo_Length));//绘制横坐标文本 //横坐标右边最大长度 cwcq2012.11.02

	}
	//曲线8
	m_CurveNum=8*(m_nCurPage-1)+7;//第几条曲线
	if (g_sorFileArray.totalCurveNumber>m_CurveNum)
	{
		//曲线8大矩形
		CRect curveRect;
		curveRect.SetRect((m_rectDraw.Width() + nHmidspace)/2 + nHleftspace, nCurHeight-nVspace, m_rectDraw.Width() - nHleftspace*6/5, nCurHeight + nVspace * 10-nVspace);
		pDC->Rectangle(&curveRect);
		//左侧小矩形
		CRect leftRect;
		leftRect.SetRect((m_rectDraw.Width() + nHmidspace)/2, curveRect.top + curveRect.Height()/2-5*nVspace, curveRect.left - nHmidspace/2, curveRect.top + curveRect.Height()/2-3*nVspace);
		pDC->Rectangle(&leftRect);
		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		DrawGridLine(pDC, curveRect);//绘制栅格 wcq2012.10.31
		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		DrawCurveByReport(pDC, curveRect, m_CurveNum);//绘制曲线8
		DrawVerCoordUnit(pDC, leftRect);
		//底侧小矩形
		CRect bottomRect;
		bottomRect.SetRect(curveRect.left-nHmidspace/3, curveRect.bottom +nVspace, curveRect.left + curveRect.Width()/2+nHleftspace*5/4, curveRect.bottom +nVspace*3);
		//bottomRect.SetRect(curveRect.left + curveRect.Width()/2-nHmidspace, curveRect.bottom +nVspace/2, curveRect.left + curveRect.Width()/2+nHmidspace, curveRect.bottom +nVspace*3/2);
		pDC->Rectangle(&bottomRect);
		CRect leftUnitRect;
		leftUnitRect.SetRect(curveRect.left-nHleftspace, bottomRect.top, curveRect.left + 400,  bottomRect.top + nVspace);
		//leftUnitRect.SetRect(curveRect.left, bottomRect.top, curveRect.left + 400,  bottomRect.top + nVspace);
		DrawHorCoordUnit(pDC, leftUnitRect, MAIN->m_pWaveFormView->m_strStartDis); //绘制横坐标文本左侧

		CRect mideUnitRect;
		mideUnitRect.SetRect(curveRect.left-nHmidspace/2, bottomRect.top, curveRect.right,  bottomRect.bottom);
		//mideUnitRect.SetRect(bottomRect.left, bottomRect.top, bottomRect.right,  bottomRect.bottom);
		//DrawHorCoordUnit(pDC, mideUnitRect, MAIN->m_pWaveFormView->m_strUnitDis); //绘制横坐标文本单位长度//wcq2012.11.02
		CString strDivUnit;
		float m_float_zll=0.0f;
		float flo_Length=g_sorFileArray.sorFileList[m_CurveNum].pTestResultInfo->Length;
		
		DrawHorCoordUnit(pDC, mideUnitRect, branch(m_float_zll,flo_Length)); //绘制横坐标文本单位长度//wcq2012.11.02

		CRect rightUnitRect;
		rightUnitRect.SetRect(curveRect.right -nHleftspace, bottomRect.top, curveRect.right,  bottomRect.top + nVspace);
		//rightUnitRect.SetRect(curveRect.right -400, bottomRect.top, curveRect.right,  bottomRect.top + nVspace);
		//DrawHorCoordUnit(pDC, rightUnitRect, MAIN->m_pWaveFormView->m_strEndDis); //绘制横坐标文本//wcq2012.11.02
		
		DrawHorCoordUnit(pDC, rightUnitRect, right_MAX(m_float_zll,flo_Length));//绘制横坐标文本 //横坐标右边最大长度 cwcq2012.11.02

	}
}

void CPrintView::DrawCurCurve(CDC* pDC)
{
	//画线
	int nVspace = m_rectDraw.Height() / 64;
	int nCurHeight = nVspace * 3;
	int nHleftspace = m_rectDraw.Width() / 18;
	int nHmidspace = m_rectDraw.Width() / 20;
	//标题
	pDC->MoveTo(m_rectDraw.Width() / 3, nCurHeight);
	pDC->LineTo(m_rectDraw.Width() * 2/3, nCurHeight);
	CRect titleRect;
	CFont* pOldFont = pDC->SelectObject (&m_fontPrinter);
	titleRect.SetRect(m_rectDraw.Width() / 3, nCurHeight-nVspace, m_rectDraw.Width() * 2/3, nCurHeight);
	pDC->DrawText(g_sorFileArray.reportConfig.Title, titleRect, DT_CENTER);
	 pDC->SelectObject (&m_dataFont);
	//内容
	for (int i=1; i<=6; i++)
	{
		nCurHeight = nCurHeight + nVspace;
		pDC->MoveTo(nHleftspace, nCurHeight);
		pDC->LineTo((m_rectDraw.Width() - nHmidspace)/2 ,nCurHeight);

		CRect itemRect;
		itemRect.SetRect(nHleftspace, nCurHeight - nVspace,(m_rectDraw.Width() - nHmidspace)/2, nCurHeight);
		DrawReportItem(pDC, itemRect, i*2-1);

		pDC->MoveTo((m_rectDraw.Width() + nHmidspace)/2, nCurHeight);
		pDC->LineTo((m_rectDraw.Width() - nHleftspace), nCurHeight);
		itemRect.SetRect((m_rectDraw.Width() + nHmidspace)/2, nCurHeight - nVspace,(m_rectDraw.Width() - nHleftspace), nCurHeight);
		DrawReportItem(pDC, itemRect, i*2);
	}
	//分割线：曲线标题 // zll 2012.9.27注释
	//{
	//	nCurHeight = nCurHeight + nVspace * 3;  //Item1
	//	pDC->MoveTo(nHleftspace, nCurHeight);
	//	pDC->LineTo((m_rectDraw.Width() - nHmidspace)/3 ,nCurHeight);
	//	CRect itemRect;
	//	itemRect.SetRect(nHleftspace, nCurHeight - nVspace,(m_rectDraw.Width() - nHmidspace)/3, nCurHeight);
	//	DrawCurveItem(pDC, itemRect, 1);

	//	pDC->MoveTo((m_rectDraw.Width()+ nHmidspace)/3, nCurHeight);  //Item2
	//	pDC->LineTo((m_rectDraw.Width() - nHmidspace) *2/3 , nCurHeight);
	//	itemRect.SetRect((m_rectDraw.Width()+ nHmidspace)/3, nCurHeight - nVspace,(m_rectDraw.Width() - nHmidspace) *2/3, nCurHeight);
	//	DrawCurveItem(pDC, itemRect, 2);

	//	pDC->MoveTo(m_rectDraw.Width()* 2/3, nCurHeight);  //Item3
	//	pDC->LineTo((m_rectDraw.Width() - nHleftspace), nCurHeight);
	//	itemRect.SetRect(m_rectDraw.Width()* 2/3, nCurHeight - nVspace,m_rectDraw.Width() - nHleftspace, nCurHeight);
	//	DrawCurveItem(pDC, itemRect, 3);
	//}
	//曲线
	{
		//大矩形
		nCurHeight = nCurHeight + nVspace * 2;
		CRect curveRect;
		curveRect.SetRect(nHleftspace*2 + nHmidspace/2, nCurHeight, m_rectDraw.Width() - nHleftspace, nCurHeight + nVspace * 20);
		pDC->Rectangle(&curveRect);
		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		DrawGridLine(pDC, curveRect);//绘制栅格 wcq2012.10.31
		//////////////////////////////////////////////////////////////////////////////////////////////////////////
		DrawCurve(pDC, curveRect, g_sorFileArray.curSelection);
		DrawCursorAB(pDC, curveRect); //绘制AB光标 xjf2014.03.03
		//左侧小矩形,因为需要画刻度，所以将其舍弃lzy2014.7.18
		//CRect leftRect;  
		//leftRect.SetRect(nHleftspace, curveRect.top + curveRect.Height()/2-10*nVspace, curveRect.left - nHmidspace/2, curveRect.top + curveRect.Height()/2-8*nVspace);
		//pDC->Rectangle(&leftRect);
		//DrawVerCoordUnit(pDC, leftRect); //绘制纵坐标文本
		
		

		//底侧小矩形
		CRect bottomRect;
		//bottomRect.SetRect(curveRect.left, curveRect.bottom +nVspace, curveRect.left + curveRect.Width()/2+3*nHleftspace, curveRect.bottom +nVspace*3);//cwcq
		bottomRect.SetRect(curveRect.left, curveRect.bottom +nVspace, curveRect.left + curveRect.Width(), curveRect.bottom +nVspace*3);//wcq
		//pDC->Rectangle(&bottomRect);
		
		CRect leftUnitRect;
		leftUnitRect.SetRect(curveRect.left, bottomRect.top, curveRect.left + 400,  bottomRect.top + nVspace);
		//DrawHorCoordUnit(pDC, leftUnitRect, MAIN->m_pWaveFormView->m_strStartDis); //绘制横坐标文本左侧
		
		CRect mideUnitRect;
		mideUnitRect.SetRect(bottomRect.left-nHleftspace, bottomRect.top, bottomRect.right,  bottomRect.bottom);
		//DrawHorCoordUnit(pDC, mideUnitRect, MAIN->m_pWaveFormView->m_strUnitDis); //绘制横坐标文本单位长度//wcq2012.11.02
		CString strDivUnit;
		float m_float_zll=0.0f;
		float flo_Length=g_sorFileArray.sorFileList[g_sorFileArray.curSelection].pTestResultInfo->Length;
		
		//DrawHorCoordUnit(pDC, mideUnitRect, branch(m_float_zll,flo_Length)); //绘制横坐标文本单位长度//wcq2012.11.02

		CRect rightUnitRect;
		rightUnitRect.SetRect(curveRect.right -400-nHleftspace, bottomRect.top, curveRect.right,  bottomRect.top + nVspace);
		//DrawHorCoordUnit(pDC, rightUnitRect, MAIN->m_pWaveFormView->m_strEndDis); //绘制横坐标文本
		//DrawHorCoordUnit(pDC, rightUnitRect, MAIN->m_pWaveFormView->m_strEndDis); //绘制横坐标文本 //横坐标右边最大长度 cwcq2012.11.02
		
		//DrawHorCoordUnit(pDC, rightUnitRect, right_MAX(m_float_zll,flo_Length));//绘制横坐标文本 //横坐标右边最大长度 cwcq2012.11.02

		//画刻度lzy2014.7.18
		DrawScale(pDC, curveRect, 0, flo_Length);
	}

	//曲线参数
	{
		nCurHeight = nCurHeight + nVspace * 22;
		//绘制曲线参数
		DrawCurveParam(pDC, nHleftspace, nCurHeight);
	}
	//事件列表
	{
		nCurHeight = nCurHeight + nVspace * 10;
		//绘制事件列表表头
		DrawEventHead(pDC, nHleftspace, nCurHeight, m_rectDraw);
		//绘制事件体
		DrawEventBody(pDC, nHleftspace, nCurHeight , nVspace, m_rectDraw, g_sorFileArray.curSelection);
	}	

	pDC->SelectObject (pOldFont);
}

//新报表用的画单张报表函数，lzy2014.8.28
void CPrintView::Draw1CurveNew(CDC* pDC,int nCurPage)
{
	int nCurCurve = nCurPage - 1;
	if(nCurCurve < g_sorFileArray.totalCurveNumber)
	{
	   DrawCurCurveNew(pDC, m_rectDraw, nCurCurve, FALSE);
	}
	//DrawCurCurveNew(pDC,CRect m_rectDraw, g_sorFileArray.curSelection, TRUE);
}
//新报表用的画2张报表函数，lzy2014.8.28
void CPrintView::Draw2CurveNew(CDC* pDC,int nCurPage)
{
	/*CRect rcLeft, rcRight;
	rcLeft.SetRect(m_rectDraw.left, m_rectDraw.top, m_rectDraw.left + m_rectDraw.Width()/2, m_rectDraw.bottom);
	rcRight.SetRect(m_rectDraw.left + m_rectDraw.Width()/2, m_rectDraw.top,m_rectDraw.right, m_rectDraw.bottom);

	DrawCurCurveNew(pDC, rcLeft, 0, FALSE);
	DrawCurCurveNew(pDC, rcRight, 1, FALSE);*/

	CRect rect[2];
	rect[0].SetRect(m_rectDraw.left, m_rectDraw.top, m_rectDraw.left + m_rectDraw.Width()/2, m_rectDraw.bottom);
	rect[1].SetRect(m_rectDraw.left + m_rectDraw.Width()/2, m_rectDraw.top,m_rectDraw.right, m_rectDraw.bottom);

	//曲线1
	int nCurCurve = 2 * (nCurPage - 1);
	if(nCurCurve < g_sorFileArray.totalCurveNumber)
	{
		DrawCurCurveNew(pDC, rect[0], nCurCurve, FALSE);
	}

	//曲线2
	nCurCurve = 2 * nCurPage - 1;
	if(nCurCurve < g_sorFileArray.totalCurveNumber)
	{
		DrawCurCurveNew(pDC, rect[1], nCurCurve, FALSE);
	}
	
	/*int nCount = g_sorFileArray.totalCurveNumber; 	
	for(int i=0; i<nCount; i++)
	{
		DrawCurCurveNew(pDC, rect[i], i, FALSE);
	}*/
}
//新报表用的画4张报表函数，lzy2014.8.28
void CPrintView::Draw4CurveNew(CDC* pDC,int nCurPage)
{
	/*CRect rcLeftUp, rcRightUp, rcLeftDown, rcRightDown;
	rcLeftUp.SetRect(m_rectDraw.left, m_rectDraw.top, m_rectDraw.left + m_rectDraw.Width()/2, m_rectDraw.top + m_rectDraw.Height()/2);
	rcRightUp.SetRect(m_rectDraw.left + m_rectDraw.Width()/2, m_rectDraw.top, m_rectDraw.right, m_rectDraw.top + m_rectDraw.Height()/2);
	rcLeftDown.SetRect(m_rectDraw.left, m_rectDraw.top + m_rectDraw.Height()/2, m_rectDraw.left + m_rectDraw.Width()/2, m_rectDraw.bottom);
	rcRightDown.SetRect(m_rectDraw.left + m_rectDraw.Width()/2, m_rectDraw.top + m_rectDraw.Height()/2, m_rectDraw.right, m_rectDraw.bottom);

	DrawCurCurveNew(pDC, rcLeftUp, 0, FALSE);
	DrawCurCurveNew(pDC, rcRightUp, 1, FALSE);
	DrawCurCurveNew(pDC, rcLeftDown, 2, FALSE);
	DrawCurCurveNew(pDC, rcRightDown, 3, FALSE);*/

	CRect rect[4];
	rect[0].SetRect(m_rectDraw.left, m_rectDraw.top, m_rectDraw.left + m_rectDraw.Width()/2, m_rectDraw.top + m_rectDraw.Height()/2);
	rect[1].SetRect(m_rectDraw.left + m_rectDraw.Width()/2, m_rectDraw.top, m_rectDraw.right, m_rectDraw.top + m_rectDraw.Height()/2);
	rect[2].SetRect(m_rectDraw.left, m_rectDraw.top + m_rectDraw.Height()/2, m_rectDraw.left + m_rectDraw.Width()/2, m_rectDraw.bottom);
	rect[3].SetRect(m_rectDraw.left + m_rectDraw.Width()/2, m_rectDraw.top + m_rectDraw.Height()/2, m_rectDraw.right, m_rectDraw.bottom);

	//曲线1
	int nCurCurve = 4 * (nCurPage - 1);
	if(nCurCurve < g_sorFileArray.totalCurveNumber)
	{
		DrawCurCurveNew(pDC, rect[0], nCurCurve, FALSE);
	}
	//曲线2
	nCurCurve = 4 * (nCurPage - 1) + 1;
	if(nCurCurve < g_sorFileArray.totalCurveNumber)
	{
		DrawCurCurveNew(pDC, rect[1], nCurCurve, FALSE);
	}
	//曲线3
	nCurCurve = 4 * (nCurPage - 1) + 2;
	if(nCurCurve < g_sorFileArray.totalCurveNumber)
	{
		DrawCurCurveNew(pDC, rect[2], nCurCurve, FALSE);
	}
	//曲线4
	nCurCurve = 4 * (nCurPage - 1) + 3;
	if(nCurCurve < g_sorFileArray.totalCurveNumber)
	{
		DrawCurCurveNew(pDC, rect[3], nCurCurve, FALSE);
	}



	
	/*int nCount = g_sorFileArray.totalCurveNumber;
	for(int i=0; i<nCount; i++)
	{
		DrawCurCurveNew(pDC, rect[i], i, FALSE);
	}*/
	
}
//新报表用的画6张报表函数，lzy2014.8.28
void CPrintView::Draw6CurveNew(CDC* pDC,int nCurPage)
{
	/*CRect rcLeftUp, rcCenterUp, rcRightUp, rcLeftDown, rcCenterDown, rcRightDown;
	rcLeftUp.SetRect(m_rectDraw.left, m_rectDraw.top, m_rectDraw.left + m_rectDraw.Width()/3, m_rectDraw.top + m_rectDraw.Height()/2);
	rcCenterUp.SetRect(m_rectDraw.left + m_rectDraw.Width()/3, m_rectDraw.top, m_rectDraw.left + m_rectDraw.Width()*2/3, m_rectDraw.top + m_rectDraw.Height()/2);
	rcRightUp.SetRect(m_rectDraw.left + m_rectDraw.Width()*2/3, m_rectDraw.top, m_rectDraw.right, m_rectDraw.top + m_rectDraw.Height()/2);
	rcLeftDown.SetRect(m_rectDraw.left, m_rectDraw.top + m_rectDraw.Height()/2, m_rectDraw.left + m_rectDraw.Width()/3, m_rectDraw.bottom);
	rcCenterDown.SetRect(m_rectDraw.left + m_rectDraw.Width()/3, m_rectDraw.top + m_rectDraw.Height()/2, m_rectDraw.left + m_rectDraw.Width()*2/3, m_rectDraw.bottom);
	rcRightDown.SetRect(m_rectDraw.left + m_rectDraw.Width()*2/3, m_rectDraw.top + m_rectDraw.Height()/2, m_rectDraw.right, m_rectDraw.bottom);

	DrawCurCurveNew(pDC, rcLeftUp, 0, TRUE);
	DrawCurCurveNew(pDC, rcCenterUp, 1, TRUE);
	DrawCurCurveNew(pDC, rcRightUp, 2, TRUE);
	DrawCurCurveNew(pDC, rcLeftDown, 3, TRUE);
	DrawCurCurveNew(pDC, rcCenterDown, 4, TRUE);
	DrawCurCurveNew(pDC, rcRightDown, 5, TRUE);*/

	CRect rect[6];
	rect[0].SetRect(m_rectDraw.left, m_rectDraw.top, m_rectDraw.left + m_rectDraw.Width()/3, m_rectDraw.top + m_rectDraw.Height()/2);
	rect[1].SetRect(m_rectDraw.left + m_rectDraw.Width()/3, m_rectDraw.top, m_rectDraw.left + m_rectDraw.Width()*2/3, m_rectDraw.top + m_rectDraw.Height()/2);
	rect[2].SetRect(m_rectDraw.left + m_rectDraw.Width()*2/3, m_rectDraw.top, m_rectDraw.right, m_rectDraw.top + m_rectDraw.Height()/2);
	rect[3].SetRect(m_rectDraw.left, m_rectDraw.top + m_rectDraw.Height()/2, m_rectDraw.left + m_rectDraw.Width()/3, m_rectDraw.bottom);
	rect[4].SetRect(m_rectDraw.left + m_rectDraw.Width()/3, m_rectDraw.top + m_rectDraw.Height()/2, m_rectDraw.left + m_rectDraw.Width()*2/3, m_rectDraw.bottom);
	rect[5].SetRect(m_rectDraw.left + m_rectDraw.Width()*2/3, m_rectDraw.top + m_rectDraw.Height()/2, m_rectDraw.right, m_rectDraw.bottom);

	//曲线1
	int nCurCurve = 6 * (nCurPage - 1);
	if(nCurCurve < g_sorFileArray.totalCurveNumber)
	{
		DrawCurCurveNew(pDC, rect[0], nCurCurve, FALSE);
	}

	//曲线2
	nCurCurve = 6 * (nCurPage - 1) + 1;
	if(nCurCurve < g_sorFileArray.totalCurveNumber)
	{
		DrawCurCurveNew(pDC, rect[1], nCurCurve, FALSE);
	}

	//曲线3
	nCurCurve = 6 * (nCurPage - 1) + 2;
	if(nCurCurve < g_sorFileArray.totalCurveNumber)
	{
		DrawCurCurveNew(pDC, rect[2], nCurCurve, FALSE);
	}

	//曲线4
	nCurCurve = 6 * (nCurPage - 1) + 3;
	if(nCurCurve < g_sorFileArray.totalCurveNumber)
	{
		DrawCurCurveNew(pDC, rect[3], nCurCurve, FALSE);
	}

	//曲线5
    nCurCurve = 6 * (nCurPage - 1) + 4;
	if(nCurCurve < g_sorFileArray.totalCurveNumber)
	{
		DrawCurCurveNew(pDC, rect[4], nCurCurve, FALSE);
	}

	//曲线6
	nCurCurve = 6 * (nCurPage - 1) + 5;
	if(nCurCurve < g_sorFileArray.totalCurveNumber)
	{
		DrawCurCurveNew(pDC, rect[5], nCurCurve, FALSE);
	}
	
	/*int nCount = g_sorFileArray.totalCurveNumber;
	for(int i=0; i<nCount; i++)
	{
		DrawCurCurveNew(pDC, rect[i], i, FALSE);
	}
*/
}
//新报表用的画8张报表函数，lzy2014.8.28
void CPrintView::Draw8CurveNew(CDC* pDC,int nCurPage)
{  
	CRect rect[8];
	rect[0].SetRect(m_rectDraw.left, m_rectDraw.top, m_rectDraw.left + m_rectDraw.Width()/4, m_rectDraw.top + m_rectDraw.Height()/2);
	rect[1].SetRect(m_rectDraw.left + m_rectDraw.Width()/4 , m_rectDraw.top, m_rectDraw.left + m_rectDraw.Width()*2/4, m_rectDraw.top + m_rectDraw.Height()/2);
	rect[2].SetRect(m_rectDraw.left + m_rectDraw.Width()*2/4 , m_rectDraw.top, m_rectDraw.left + m_rectDraw.Width()*3/4, m_rectDraw.top + m_rectDraw.Height()/2);
	rect[3].SetRect(m_rectDraw.left + m_rectDraw.Width()*3/4 , m_rectDraw.top, m_rectDraw.right, m_rectDraw.top + m_rectDraw.Height()/2);
	rect[4].SetRect(m_rectDraw.left, m_rectDraw.top + m_rectDraw.Height()/2, m_rectDraw.left + m_rectDraw.Width()/4, m_rectDraw.bottom);
	rect[5].SetRect(m_rectDraw.left + m_rectDraw.Width()/4 , m_rectDraw.top + m_rectDraw.Height()/2, m_rectDraw.left + m_rectDraw.Width()*2/4, m_rectDraw.bottom);
	rect[6].SetRect(m_rectDraw.left + m_rectDraw.Width()*2/4 , m_rectDraw.top + m_rectDraw.Height()/2, m_rectDraw.left + m_rectDraw.Width()*3/4, m_rectDraw.bottom);
	rect[7].SetRect(m_rectDraw.left + m_rectDraw.Width()*3/4 , m_rectDraw.top + m_rectDraw.Height()/2, m_rectDraw.right, m_rectDraw.bottom);

	//曲线1
	int nCurCurve = 8 * (nCurPage - 1);
	if(nCurCurve < g_sorFileArray.totalCurveNumber)
	{
		DrawCurCurveNew(pDC, rect[0], nCurCurve, FALSE);
	}

	//曲线2
	nCurCurve = 8 * (nCurPage - 1) + 1;
	if(nCurCurve < g_sorFileArray.totalCurveNumber)
	{
		DrawCurCurveNew(pDC, rect[1], nCurCurve, FALSE);
	} 

	//曲线3
	nCurCurve = 8 * (nCurPage - 1) + 2;
	if(nCurCurve < g_sorFileArray.totalCurveNumber)
	{
		DrawCurCurveNew(pDC, rect[2], nCurCurve, FALSE);
	}

	//曲线4
	nCurCurve = 8 * (nCurPage - 1) + 3;
	if(nCurCurve < g_sorFileArray.totalCurveNumber)
	{
		DrawCurCurveNew(pDC, rect[3], nCurCurve, FALSE);
	}

	//曲线5 
	nCurCurve = 8 * (nCurPage - 1) + 4;
	if(nCurCurve < g_sorFileArray.totalCurveNumber)
	{
		DrawCurCurveNew(pDC, rect[4], nCurCurve, FALSE);
	}

	//曲线6
	nCurCurve = 8 * (nCurPage - 1) + 5;
	if(nCurCurve < g_sorFileArray.totalCurveNumber)
	{
		DrawCurCurveNew(pDC, rect[5], nCurCurve, FALSE);
	} 

	//曲线7
	nCurCurve = 8 * (nCurPage - 1) + 6;
	if(nCurCurve < g_sorFileArray.totalCurveNumber)
	{
		DrawCurCurveNew(pDC, rect[6], nCurCurve, FALSE);
	}

	//曲线8
	nCurCurve = 8 * (nCurPage - 1) + 7;
	if(nCurCurve < g_sorFileArray.totalCurveNumber)
	{
		DrawCurCurveNew(pDC, rect[7], nCurCurve, FALSE);
	}
	
	
	/*int nCount = g_sorFileArray.totalCurveNumber;
	for(int i=0; i<nCount; i++)
	{
		DrawCurCurveNew(pDC, rect[i], i, FALSE);
	}*/

}

//新报表用的函数，画报表内容。lzy 2014.8.11
void CPrintView::DrawCurCurveNew(CDC* pDC, CRect rectDraw, int nCurSel, BOOL bSingle)
{
	//GetPrintInfo(g_sorFileArray.curSelection);
	GetPrintInfo(nCurSel);  //为适用多曲线打印而改，lzy2014.8.30
	
	//画线
	int nXoffset = rectDraw.left;
	int nYoffset = rectDraw.top;
	int nVspace = rectDraw.Height() / 64;
	int nCurHeight = nYoffset+ nVspace * 3;
	int nHleftspace = rectDraw.Width() / MARGIN_LEFT_COUNT;
	int nHmidspace = rectDraw.Width() / 20;
	int nHinterval = nVspace / 4;   //矩形框垂直间隔
	int nHMargin = nHleftspace / 2;   //矩形框内文字与边缘的距离
	int nHSubMargin = nHleftspace / 2;  //子项目的缩进量
	int n1 = nXoffset + nHleftspace + nHMargin;  //左边第一项的起始X坐标
	int n2 = n1 + (rectDraw.Width() - 2 * (n1 - nXoffset)) / 3;   //中间项的起始X坐标
	int n3 = n2 + (rectDraw.Width() - 2 * (n1 - nXoffset)) / 3;   //最右边项的起始X坐标
	int nBottom = 0;   //保存矩形框的下边纵坐标
	CRect textRect;
	CString strText , strTemp;	

	//标题	
	CFont* pOldFont = pDC->SelectObject (&m_fontPrinter);
	CSize sizeText = pDC->GetTextExtent(g_sorFileArray.reportConfig.Title);
	textRect.SetRect(nXoffset + rectDraw.Width() / 2 - sizeText.cx / 2, nCurHeight-nVspace ,nXoffset + rectDraw.Width() / 2 + sizeText.cx / 2 ,  nCurHeight-nVspace + sizeText.cy );
	pDC->DrawText(g_sorFileArray.reportConfig.Title, textRect, DT_CENTER);  //写报表抬头
	nBottom = textRect.bottom;
	
	//以下写第一行	
	{
		pDC->SelectObject (&m_dataFont);
		nCurHeight = nBottom + nVspace; 		
		sizeText = pDC->GetTextExtent(g_sorFileArray.reportConfig.GrandwayLogo);
		nBottom = nCurHeight + sizeText.cy;
		textRect.SetRect(nXoffset + nHleftspace, nCurHeight, nXoffset + nHleftspace + sizeText.cx, nCurHeight + sizeText.cy);
		if(m_bPrintLogo)
			pDC->DrawText(g_sorFileArray.reportConfig.GrandwayLogo, textRect, DT_LEFT);  //写软件版本及光维版权
		CTime tm = CTime::GetCurrentTime();
		CString strCurTime = tm.Format(_T("%Y-%m-%d %H:%M:%S"));
		sizeText = pDC->GetTextExtent(strCurTime);
		textRect.SetRect(nXoffset + rectDraw.Width() - nHleftspace - sizeText.cx ,nCurHeight, nXoffset + rectDraw.Width() - nHleftspace, nCurHeight + sizeText.cy);
		pDC->DrawText(strCurTime, textRect, DT_LEFT);  //写当前打印的时刻

	}
	
	TEXTMETRIC  tm;
	pDC->GetTextMetrics(&tm);
	int nCharWidth = tm.tmMaxCharWidth;  //最大字符宽度
	strText = _T("Test char height 测试字体");
	sizeText = pDC->GetTextExtent(strText);
	int nCharHeight = sizeText.cy;    //内容字体的高度
	int nLineInterval = nCharHeight / 2 ;  //0.5倍行距
    //以下写Job info的内容
	if(m_bPrintJob)
	{
		nCurHeight = textRect.bottom + nHinterval;
		//strText = _T("Job ID");
		//sizeText = pDC->GetTextExtent(strText);
		nBottom = nCurHeight + nLineInterval * 4 + nCharHeight * 3;
		pDC->Rectangle(nXoffset + nHleftspace, nCurHeight, nXoffset + rectDraw.Width() - nHleftspace, nBottom);	
		pDC->SelectObject(&m_fontGroup);
		//strText = _T(" Job info ");
		strText = _T(" ") + g_sorFileArray.reportConfig.Job.strJobTitle + _T(" ");
		sizeText = pDC->GetTextExtent(strText);
		textRect.SetRect(nXoffset + (rectDraw.Width() - sizeText.cx) / 2, nCurHeight - sizeText.cy / 2, nXoffset + (rectDraw.Width() + sizeText.cx) / 2, nCurHeight + sizeText.cy / 2);
		pDC->DrawText(strText, textRect, DT_CENTER);  //写job info字样	
		pDC->SelectObject(&m_dataFont);
		nCurHeight += nLineInterval;
		//strText = _T("Job ID: ") + g_sorFileArray.reportConfig.Job.JobID;  //写任务编号
		strText = g_sorFileArray.reportConfig.Job.strJobID + _T(": ") + g_sorFileArray.reportConfig.Job.JobID;  //写任务编号
		sizeText = pDC->GetTextExtent(strText);
		textRect.SetRect(nXoffset + nHleftspace + nHMargin, nCurHeight, nXoffset + nHleftspace + sizeText.cx + nHMargin, nCurHeight + sizeText.cy);
		pDC->DrawText(strText, textRect, DT_LEFT);		
		//strText = _T("File: ") + g_sorFileArray.reportConfig.Job.SorFileName;  //写文件名
		strText = g_sorFileArray.reportConfig.Job.strSorFileName + _T(": ") + g_sorFileArray.reportConfig.Job.SorFileName;  //写文件名
		sizeText = pDC->GetTextExtent(strText);
		//textRect.SetRect(m_rectDraw.Width() / 2  - nHMargin, nCurHeight, m_rectDraw.Width() / 2 + sizeText.cx - nHMargin, nCurHeight + sizeText.cy);
		textRect.SetRect(nXoffset + rectDraw.Width() / 2  - nHMargin, nCurHeight, 2*nXoffset + rectDraw.Width() - n1, nCurHeight + sizeText.cy);
		pDC->DrawText(strText, textRect, DT_LEFT);
		nCurHeight = textRect.bottom + nLineInterval;
		//strText = _T("Contractor: ") + g_sorFileArray.reportConfig.Job.Contractor;  //写承包商
		strText = g_sorFileArray.reportConfig.Job.strContractor + _T(": ") + g_sorFileArray.reportConfig.Job.Contractor;  //写承包商
		sizeText = pDC->GetTextExtent(strText);
		textRect.SetRect(nXoffset + nHleftspace + nHMargin, nCurHeight, nXoffset + nHleftspace + sizeText.cx + nHMargin, nCurHeight + sizeText.cy);
		pDC->DrawText(strText, textRect, DT_LEFT);
		//strText = _T("Test date: ") + g_sorFileArray.reportConfig.Job.TestDate;  //写测试时间
		strText = g_sorFileArray.reportConfig.Job.strTestDate + _T(": ") + g_sorFileArray.reportConfig.Job.TestDate;  //写测试时间
		sizeText = pDC->GetTextExtent(strText);
		textRect.SetRect(nXoffset + rectDraw.Width() / 2 - nHMargin, nCurHeight, nXoffset + rectDraw.Width() / 2 + sizeText.cx - nHMargin, nCurHeight + sizeText.cy);
		pDC->DrawText(strText, textRect, DT_LEFT);
		nCurHeight = textRect.bottom + nLineInterval;
		strText = g_sorFileArray.reportConfig.Job.strCustomer + _T(": ") + g_sorFileArray.reportConfig.Job.Customer;  //客户
		sizeText = pDC->GetTextExtent(strText);
		textRect.SetRect(nXoffset + nHleftspace + nHMargin, nCurHeight, nXoffset + nHleftspace + sizeText.cx + nHMargin, nCurHeight + sizeText.cy);
		pDC->DrawText(strText, textRect, DT_LEFT);
		//strText = _T("Operator: ") + g_sorFileArray.reportConfig.Job.Operator;  //写操作员
		strText = g_sorFileArray.reportConfig.Job.strOperator + _T(": ") + g_sorFileArray.reportConfig.Job.Operator;  //写操作员
		sizeText = pDC->GetTextExtent(strText);
		textRect.SetRect(nXoffset + rectDraw.Width() / 2 - nHMargin, nCurHeight, nXoffset + rectDraw.Width() / 2 + sizeText.cx - nHMargin, nCurHeight + sizeText.cy);
		pDC->DrawText(strText, textRect, DT_LEFT);
	}
	
    
    //以下画配置信息
	if(m_bPrintConfig)
	{
		nCurHeight = nBottom + nVspace / 2;	
		nBottom = nCurHeight + nCharHeight * 7 + nLineInterval * 8;
		pDC->Rectangle(nXoffset + nHleftspace, nCurHeight, nXoffset + rectDraw.Width() - nHleftspace, nBottom);
		pDC->SelectObject(&m_fontGroup);
		//strText = _T(" Configuration info ");
		strText = _T(" ") + g_sorFileArray.reportConfig.Configuration.strTitle + _T(" ");
		sizeText = pDC->GetTextExtent(strText);
		textRect.SetRect(nXoffset + (rectDraw.Width() - sizeText.cx) / 2, nCurHeight - sizeText.cy / 2, nXoffset +(rectDraw.Width() + sizeText.cx) / 2, nCurHeight + sizeText.cy / 2);
		pDC->DrawText(strText, textRect, DT_LEFT);
		pDC->SelectObject(&m_dataFont);	
		nCurHeight += nLineInterval;
		//strText = _T("Wavelength(nm): ") + g_sorFileArray.reportConfig.Configuration.Wavelength;  //写wavelength
		strText = g_sorFileArray.reportConfig.Configuration.strWavelength + _T("(nm): ") + g_sorFileArray.reportConfig.Configuration.Wavelength;  //写wavelength
		sizeText = pDC->GetTextExtent(strText);
		textRect.SetRect(n1, nCurHeight, n1 + sizeText.cx, nCurHeight + sizeText.cy);
		pDC->DrawText(strText, textRect, DT_LEFT);		
		//strText = _T("Range(m): ") + g_sorFileArray.reportConfig.Configuration.Range;  //写Range
		strText = g_sorFileArray.reportConfig.Configuration.strRange + _T("(m): ") + g_sorFileArray.reportConfig.Configuration.Range;  //写Range
		sizeText = pDC->GetTextExtent(strText);
		textRect.SetRect(n2 , nCurHeight, n2 + sizeText.cx, nCurHeight + sizeText.cy);
		pDC->DrawText(strText, textRect, DT_LEFT);
		//strText = _T("Backscatter coeff(dB): ") + g_sorFileArray.reportConfig.Configuration.BackScatter;  //写Backscatter coeff 3
		strText = g_sorFileArray.reportConfig.Configuration.strBackScatter + _T("(dB): ") + g_sorFileArray.reportConfig.Configuration.BackScatter;  //写Backscatter coeff 3
		sizeText = pDC->GetTextExtent(strText);
		textRect.SetRect(n3 , nCurHeight, n3 + sizeText.cx, nCurHeight + sizeText.cy);
		pDC->DrawText(strText, textRect, DT_LEFT);

		nCurHeight = textRect.bottom + nLineInterval;
		//strText = _T("Pulse(ns): ") + g_sorFileArray.reportConfig.Configuration.Pulse;  //写Pulse
		strText = g_sorFileArray.reportConfig.Configuration.strPulse + _T("(ns): ") + g_sorFileArray.reportConfig.Configuration.Pulse;//写PULSE
		sizeText = pDC->GetTextExtent(strText);
		textRect.SetRect(n1, nCurHeight, n1 + sizeText.cx , nCurHeight + sizeText.cy);
		pDC->DrawText(strText, textRect, DT_LEFT);
		//strText = _T("Average: ") + g_sorFileArray.reportConfig.Configuration.Average;  //写Average counts
		strText = g_sorFileArray.reportConfig.Configuration.strAverage + _T(": ") + g_sorFileArray.reportConfig.Configuration.Average;  //写Average counts  
		sizeText = pDC->GetTextExtent(strText);
		textRect.SetRect(n2 , nCurHeight, n2 + sizeText.cx , nCurHeight + sizeText.cy);
		pDC->DrawText(strText, textRect, DT_LEFT);
		//strText = _T("Loss threshold(dB): ") + g_sorFileArray.reportConfig.Configuration.LossThreshold;  //写LossThreshold 3
		strText = g_sorFileArray.reportConfig.Configuration.strLossThreshold + _T("(dB): ") + g_sorFileArray.reportConfig.Configuration.LossThreshold;  //写LossThreshold 3
		sizeText = pDC->GetTextExtent(strText);
		textRect.SetRect(n3 , nCurHeight, n3 + sizeText.cx, nCurHeight + sizeText.cy);
		pDC->DrawText(strText, textRect, DT_LEFT);
		
		nCurHeight = textRect.bottom + nLineInterval;
		//strText = _T("ORIGIN");  //写ORIGIN 
		strText = g_sorFileArray.reportConfig.Configuration.strOrigin;//写起始点
		sizeText = pDC->GetTextExtent(strText);
		textRect.SetRect(n1, nCurHeight, n1 + sizeText.cx, nCurHeight + sizeText.cy);
		pDC->DrawText(strText, textRect, DT_LEFT);
        //strText = _T("END");  //写END 2
		strText = g_sorFileArray.reportConfig.Configuration.strEnd;  //写结束点
		sizeText = pDC->GetTextExtent(strText);
		textRect.SetRect(n2, nCurHeight, n2 + sizeText.cx , nCurHeight + sizeText.cy);
		pDC->DrawText(strText, textRect, DT_LEFT);
		//strText = _T("Reflection threshold(dB): ") + g_sorFileArray.reportConfig.Configuration.RefThreshold;  //写Reflection threshold
		strText = g_sorFileArray.reportConfig.Configuration.strRefThreshold + _T("(dB): ") + g_sorFileArray.reportConfig.Configuration.RefThreshold;  //写Reflection threshold
		sizeText = pDC->GetTextExtent(strText);
		textRect.SetRect(n3 , nCurHeight, n3 + sizeText.cx, nCurHeight + sizeText.cy);
		pDC->DrawText(strText, textRect, DT_LEFT);

		nCurHeight = textRect.bottom + nLineInterval;
		//strText = _T("Origin location: ") + g_sorFileArray.reportConfig.Configuration.OriLocation;  //写Origin location 1
		strText = g_sorFileArray.reportConfig.Configuration.strOriLocation + _T(": ") + g_sorFileArray.reportConfig.Configuration.OriLocation;  //写Origin location 1
		sizeText = pDC->GetTextExtent(strText);
		textRect.SetRect(n1 + nHSubMargin, nCurHeight, n1 + nHSubMargin + sizeText.cx, nCurHeight + sizeText.cy);
		pDC->DrawText(strText, textRect, DT_LEFT);
		//strText = _T("End location: ") + g_sorFileArray.reportConfig.Configuration.EndLocation;  //写 End location 2
		strText = g_sorFileArray.reportConfig.Configuration.strEndLocation + _T(": ") + g_sorFileArray.reportConfig.Configuration.EndLocation;  //写 End location 2
		sizeText = pDC->GetTextExtent(strText);
		textRect.SetRect(n2 + nHSubMargin , nCurHeight, n2 + nHSubMargin + sizeText.cx , nCurHeight + sizeText.cy);
		pDC->DrawText(strText, textRect, DT_LEFT);
		//strText = _T("End threshold(dB): ") + g_sorFileArray.reportConfig.Configuration.EndThreshold;  //写Fiber end threshold
		strText = g_sorFileArray.reportConfig.Configuration.strEndThreshold + _T("(dB): ") + g_sorFileArray.reportConfig.Configuration.EndThreshold;  //写Fiber end threshold
		sizeText = pDC->GetTextExtent(strText);
		textRect.SetRect(n3 , nCurHeight, n3 + sizeText.cx, nCurHeight + sizeText.cy);
		pDC->DrawText(strText, textRect, DT_LEFT);

		nCurHeight = textRect.bottom + nLineInterval;
		//strText = _T("Origin cable: ") + g_sorFileArray.reportConfig.Configuration.OriCable;  //写Origin Cable
		strText = g_sorFileArray.reportConfig.Configuration.strOriCable + _T(": ") + g_sorFileArray.reportConfig.Configuration.OriCable;  //写Origin Cable
		sizeText = pDC->GetTextExtent(strText);
		textRect.SetRect(n1 + nHSubMargin, nCurHeight, n1 + nHSubMargin + sizeText.cx, nCurHeight + sizeText.cy);
		pDC->DrawText(strText, textRect, DT_LEFT);
		//strText = _T("End cable: ") + g_sorFileArray.reportConfig.Configuration.EndCable;  //写End Cable
		strText = g_sorFileArray.reportConfig.Configuration.strEndCable + _T(": ") + g_sorFileArray.reportConfig.Configuration.EndCable;  //写End Cable
		sizeText = pDC->GetTextExtent(strText);
		textRect.SetRect(n2 + nHSubMargin , nCurHeight, n2 + nHSubMargin + sizeText.cx , nCurHeight + sizeText.cy);
		pDC->DrawText(strText, textRect, DT_LEFT);
		//strText = _T("Index: ") + g_sorFileArray.reportConfig.Configuration.Index;  //写 折射率
		strText = g_sorFileArray.reportConfig.Configuration.strIndex + _T(": ") + g_sorFileArray.reportConfig.Configuration.Index;  //写 折射率
		sizeText = pDC->GetTextExtent(strText);
		textRect.SetRect(n3, nCurHeight, n3 + sizeText.cx , nCurHeight + sizeText.cy);
		pDC->DrawText(strText, textRect, DT_LEFT);

		nCurHeight = textRect.bottom + nLineInterval;
		//strText = _T("Origin fiber: ") + g_sorFileArray.reportConfig.Configuration.OriFiber;  //写Origin Fiber
		strText = g_sorFileArray.reportConfig.Configuration.strOriFiber + _T(": ") + g_sorFileArray.reportConfig.Configuration.OriFiber;  //写Origin Fiber
		sizeText = pDC->GetTextExtent(strText);
		textRect.SetRect(n1 + nHSubMargin, nCurHeight, n1 + nHSubMargin + sizeText.cx, nCurHeight + sizeText.cy);
		pDC->DrawText(strText, textRect, DT_LEFT);
		//strText = _T("End fiber: ") + g_sorFileArray.reportConfig.Configuration.EndFiber;  //写 End Fiber
		strText = g_sorFileArray.reportConfig.Configuration.strEndFiber + _T(": ") + g_sorFileArray.reportConfig.Configuration.EndFiber;  //写 End Fiber
		sizeText = pDC->GetTextExtent(strText);
		textRect.SetRect(n2 + nHSubMargin, nCurHeight, n2 + nHSubMargin + sizeText.cx, nCurHeight + sizeText.cy);
		pDC->DrawText(strText, textRect, DT_LEFT);

		nCurHeight = textRect.bottom + nLineInterval;
		//strText = _T("Origin color: ") + g_sorFileArray.reportConfig.Configuration.OriColor;  //写Origin Color
		strText = g_sorFileArray.reportConfig.Configuration.strOriColor + _T(": ") + g_sorFileArray.reportConfig.Configuration.OriColor;  //写Origin Color		
		sizeText = pDC->GetTextExtent(strText);
		textRect.SetRect(n1 + nHSubMargin, nCurHeight, n1 + nHSubMargin + sizeText.cx, nCurHeight +sizeText.cy);
		pDC->DrawText(strText, textRect, DT_LEFT);
		//strText = _T("End color: ") + g_sorFileArray.reportConfig.Configuration.EndColor;  //写 End color
		strText = g_sorFileArray.reportConfig.Configuration.strEndColor + _T(": ") + g_sorFileArray.reportConfig.Configuration.EndColor;  //写 End color		
		sizeText = pDC->GetTextExtent(strText);
		textRect.SetRect(n2 + nHSubMargin, nCurHeight, n2 + nHSubMargin + sizeText.cx, nCurHeight + sizeText.cy);
		pDC->DrawText(strText, textRect, DT_LEFT);		
		
	}

	//以下画Comment
	if(m_bPrintComment && bSingle)
	{
		//TEXTMETRIC  tm;
		//pDC->GetTextMetrics(&tm);
		int cxChar = tm.tmAveCharWidth * 1.5;
		int cyChar = tm.tmHeight;
		int nOneLine = (rectDraw.Width() - nHleftspace * 2 - nHMargin * 2) / cxChar;  //每行多少字符
		int nchars = g_sorFileArray.reportConfig.Comment.GetLength();
		sizeText = pDC->GetTextExtent( g_sorFileArray.reportConfig.Comment);
		int nLines = sizeText.cx / (rectDraw.Width() - n1 * 2 );  //共有多少行
		int nRemain = sizeText.cx % (rectDraw.Width() - n1 * 2 ); //余数
		nLines = nRemain > 0 ? ++nLines : nLines;
		
		nCurHeight = nBottom + nVspace / 2;
		nBottom = nCurHeight + nCharHeight * nLines + nLineInterval * (nLines + 1);
		pDC->Rectangle(nXoffset + nHleftspace, nCurHeight, nXoffset + rectDraw.Width() - nHleftspace, nBottom);
		pDC->SelectObject(&m_fontGroup);
		//strText = _T(" Comment ");
		strText = _T(" ") + g_sorFileArray.reportConfig.Job.strComment + _T(" ");
		sizeText = pDC->GetTextExtent(strText);
		textRect.SetRect(nXoffset + (rectDraw.Width() - sizeText.cx) / 2, nCurHeight - sizeText.cy / 2, nXoffset + (rectDraw.Width() + sizeText.cx) / 2, nCurHeight + sizeText.cy / 2);
		pDC->DrawText(strText, textRect, DT_LEFT);
		pDC->SelectObject(&m_dataFont);
		strTemp = g_sorFileArray.reportConfig.Comment;
		int nCharCount = 0; //显示的字符的个数
		strText.Empty();
		for(int i=0; i<nchars; i++)
		{
			strText += strTemp.GetAt(i);
			sizeText = pDC->GetTextExtent(strText);
			if((sizeText.cx >= rectDraw.Width() - (n1-nXoffset) * 2 ) || i == nchars - 1)  //写一行
			{
				nCurHeight += nLineInterval;
                textRect.SetRect(n1, nCurHeight, n1 + sizeText.cx, nCurHeight  + sizeText.cy);
                pDC->DrawText(strText, textRect, DT_LEFT); 
                nCurHeight += nCharHeight;
				strText.Empty();
			}
		}
		/*for(int i=0; i<nLines; i++)
		{
			nCurHeight += nLineInterval;
			if( nOneLine < strTemp.GetLength())
			{
				strText = strTemp.Left( i * nOneLine );
				strTemp = strTemp.Right(strTemp.GetLength() - nOneLine);
			}
			else
				strText = strTemp;
			
			sizeText = pDC->GetTextExtent(strText);			
			textRect.SetRect(n1, nCurHeight, n1 + sizeText.cx, nCurHeight  + sizeText.cy);
			pDC->DrawText(strText, textRect, DT_LEFT); 
			nCurHeight += nCharHeight;
		}*/
		//if(strTemp.GetLength() > 0)  //最后一行
		//{
		//	nCurHeight += nLineInterval + nCharHeight;
		//	sizeText = pDC->GetTextExtent(strTemp);
		//	textRect.SetRect(n1, nCurHeight, n1 + sizeText.cx, nCurHeight + sizeText.cy);
		//	pDC->DrawText(strTemp, textRect, DT_LEFT);  
		//}	
		
	}
	
		
	//曲线
	if(m_bPrintTrace)
	{
		nCurHeight = nBottom + nVspace / 2;	
		nBottom = nCurHeight + nVspace * 24;
		pDC->SelectObject(&m_fontGroup);
		//strText = _T(" Trace ");
		strText = _T(" ") + g_sorFileArray.reportConfig.Trace.strTitle + _T(" ");
		sizeText = pDC->GetTextExtent(strText);
		pDC->Rectangle(nXoffset + nHleftspace, nCurHeight , nXoffset + rectDraw.Width() - nHleftspace, nBottom);
		textRect.SetRect(nXoffset + (rectDraw.Width() - sizeText.cx) / 2, nCurHeight - sizeText.cy /2, nXoffset + (rectDraw.Width() + sizeText.cx) / 2, nCurHeight + sizeText.cy /2);
		pDC->DrawText(strText, textRect, DT_LEFT);
		pDC->SelectObject(&m_dataFont);
		
		//大矩形
		nCurHeight += nHMargin + nLineInterval;
		CRect curveRect;
		curveRect.SetRect(nXoffset + nHleftspace*1.5 + nHmidspace/2, nCurHeight, nXoffset + rectDraw.Width() - 1.5 * nHleftspace - nHmidspace/2, nBottom - nVspace * 1.5);
	    DrawTrace(pDC, curveRect, nCurSel);
		//pDC->Rectangle(&curveRect);
		////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//DrawGridLine(pDC, curveRect);//绘制栅格 wcq2012.10.31
		////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//DrawCurve(pDC, curveRect, g_sorFileArray.curSelection);
		//DrawCursorAB(pDC, curveRect); //绘制AB光标 xjf2014.03.03		
		//float flo_Length=g_sorFileArray.sorFileList[g_sorFileArray.curSelection].pTestResultInfo->Length;	
		//
		////画刻度lzy2014.7.18
		//DrawScale(pDC, curveRect, 0, flo_Length);
	}

	//曲线参数	
	if(m_bPrintTrace && bSingle)
	{
		pDC->SelectObject(&m_dataFont);
		strText = _T("A: ") + g_sorFileArray.reportConfig.Trace.LocationA; // + _T(" m");  //写A标位置
		sizeText = pDC->GetTextExtent(strText);
		nCurHeight = nBottom + nVspace / 4;
		nBottom = nCurHeight + nLineInterval * 4  + nCharHeight * 3 ;		
		pDC->Rectangle(nXoffset + nHleftspace, nCurHeight, nXoffset + rectDraw.Width() - nHleftspace, nBottom);        
		
		nCurHeight += nLineInterval;
		textRect.SetRect(n1, nCurHeight, n1 + sizeText.cx, nCurHeight + sizeText.cy);
		pDC->DrawText(strText, textRect, DT_LEFT);
		strText = _T("B: ") + g_sorFileArray.reportConfig.Trace.LocationB; //+ _T(" m");;  //写B标位置
		sizeText = pDC->GetTextExtent(strText);
		textRect.SetRect(n2 , nCurHeight, n2 + sizeText.cx, nCurHeight + sizeText.cy);
		pDC->DrawText(strText, textRect, DT_LEFT);
		strText = _T("A-B: ") + g_sorFileArray.reportConfig.Trace.DistanceAB + _T("km");;  //写AB标距离
		sizeText = pDC->GetTextExtent(strText);
		textRect.SetRect(n3 , nCurHeight, n3 + sizeText.cx, nCurHeight + sizeText.cy);
		pDC->DrawText(strText, textRect, DT_LEFT);
		nCurHeight = textRect.bottom + nLineInterval;
		strText = _T("    ") + g_sorFileArray.reportConfig.Trace.LossA; //+ _T(" dB");;  //写A标损耗
		sizeText = pDC->GetTextExtent(strText);
		textRect.SetRect(n1 , nCurHeight, n1 + sizeText.cx, nCurHeight + sizeText.cy);
		pDC->DrawText(strText, textRect, DT_LEFT);
		strText = _T("    ") + g_sorFileArray.reportConfig.Trace.LossB; //+ _T(" dB");;  //写B标损耗
		sizeText = pDC->GetTextExtent(strText);
		textRect.SetRect(n2 , nCurHeight, n2 + sizeText.cx , nCurHeight + sizeText.cy);
		pDC->DrawText(strText, textRect, DT_LEFT);
		strText = _T("        ") + g_sorFileArray.reportConfig.Trace.LossAB + _T("dB");;  //写AB标损耗差
		sizeText = pDC->GetTextExtent(strText);
		textRect.SetRect(n3 , nCurHeight, n3 + sizeText.cx, nCurHeight + sizeText.cy);
		pDC->DrawText(strText, textRect, DT_LEFT);
		nCurHeight = textRect.bottom + nLineInterval;		
		strText = _T("        ") + g_sorFileArray.reportConfig.Trace.SlopeAB + _T("dB/km");;  //写AB段损耗斜率
		sizeText = pDC->GetTextExtent(strText);
		textRect.SetRect(n3 , nCurHeight, n3 + sizeText.cx, nCurHeight + sizeText.cy);
		pDC->DrawText(strText, textRect, DT_LEFT);

		
		//nCurHeight = nCurHeight + nVspace * 22;
		////绘制曲线参数
		//DrawCurveParam(pDC, nHleftspace, nCurHeight);
	}
	//事件列表
	if(m_bPrintEvent)
	{
		nCurHeight = nBottom + nVspace / 2;
		//绘制事件列表表头
		DrawEventHead(pDC, nXoffset + nHleftspace, nCurHeight + nLineInterval, rectDraw);
		//绘制事件体
		if(bSingle) //为了区分单曲线与多曲线，此处改为根据自定义曲线编号来显示事件。lzy2014.8.30
			DrawEventBody(pDC, nXoffset + nHleftspace * 1.2, nCurHeight + nLineInterval, nLineInterval , rectDraw, g_sorFileArray.curSelection);
		else
			DrawEventBody(pDC, nXoffset + nHleftspace * 1.2, nCurHeight + nLineInterval, nLineInterval , rectDraw, nCurSel);

        
		//画矩形框
		pDC->SelectObject(&m_dataFont);
		//strText = _T(" Event info ");
		strText = _T(" ") + g_sorFileArray.reportConfig.Job.strEventTitle + _T(" ");
		sizeText = pDC->GetTextExtent(strText);
		nBottom =  nCurHeight + (g_sorFileArray.reportConfig.EventCount + 1) * (nLineInterval + nCharHeight) + nLineInterval;
		textRect.SetRect(nXoffset + nHleftspace , nCurHeight,nXoffset + rectDraw.Width() - nHleftspace, nBottom);
		CBrush *pBrush = CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));
		CBrush *pOldBrush = pDC->SelectObject(pBrush);
		pDC->Rectangle(textRect);  //画矩形框
		pDC->SelectObject(pOldBrush);
        
		//写title
		pDC->SelectObject(&m_fontGroup);
		sizeText = pDC->GetTextExtent(strText);
		textRect.SetRect(nXoffset + (rectDraw.Width() - sizeText.cx) / 2, nCurHeight - sizeText.cy /2, nXoffset+(rectDraw.Width() + sizeText.cx) / 2, nCurHeight + sizeText.cy /2);
		pDC->DrawText(strText, textRect, DT_LEFT);

		//画横线
		for(int i = 1; i<g_sorFileArray.reportConfig.EventCount + 1; i++)
		{		
			pDC->MoveTo(nXoffset + nHleftspace , nCurHeight + nLineInterval / 2 + i * (nLineInterval + nCharHeight));
			pDC->LineTo(rectDraw.right - nHleftspace, nCurHeight + nLineInterval / 2 + i * (nLineInterval + nCharHeight));
		}

	}	

	pDC->SelectObject (pOldFont);
}

//绘制带坐标曲线，带有AB标线 lzy2014.8.26
void CPrintView::DrawTrace(CDC *pDC, CRect rc, int nCurSel, float fontRatio)
{	
	InitFont(pDC);  //lzy2014.9.3
	
	pDC->SelectObject(&m_dataFont);
	//float flo_Length=g_sorFileArray.sorFileList[g_sorFileArray.curSelection].pTestResultInfo->Length;	
	float flo_Length=g_sorFileArray.sorFileList[nCurSel].pTestResultInfo->Length;	
	DrawScale(pDC, rc, 0, flo_Length);//画刻度lzy2014.7.18	
	pDC->Rectangle(&rc);//大矩形
	DrawGridLine(pDC, rc);//绘制栅格 wcq2012.10.31
	//DrawCurve(pDC, rc, g_sorFileArray.curSelection);
	DrawCurve(pDC, rc, nCurSel);
	int nCurveNumber = _wtoi(g_sorFileArray.reportConfig.WaveList);  //当前选择每页打印的曲线数
	if(!nCurveNumber || m_bPrintCurve)  //只有单个报表时才画AB标
		DrawCursorAB(pDC, rc); //绘制AB光标 xjf2014.03.03
    //TRACE(_T("nCurveNumber = %d\n"), nCurveNumber);
}

//初始化字体 lzy2014.9.3
void CPrintView::InitFont(CDC *pDC)
{
	m_fontPrinter.DeleteObject();
	m_dataFont.DeleteObject();
	m_fontGroup.DeleteObject();
	m_FontVR.DeleteObject();


	//获取打印逻辑尺寸 lzy2014.8.15
	int nLogicX = pDC->GetDeviceCaps(LOGPIXELSX);
	int nLogicY = pDC->GetDeviceCaps(LOGPIXELSY);
	//获取物理打印尺寸
	int nPhysicalX = pDC->GetDeviceCaps(PHYSICALWIDTH);
	int nPhysicalY = pDC->GetDeviceCaps(PHYSICALHEIGHT);
	//CString str;
	//str.Format(L"lx = %d;ly=%d;px=%d;py=%d", nLogicX, nLogicY, nPhysicalX, nPhysicalY);
	//	AfxMessageBox(str);
	float fScale = nLogicX / 300.0;
	int nCurveNumber = _wtoi(g_sorFileArray.reportConfig.WaveList);  //当前选择每页打印的曲线数
	if(nCurveNumber > 4 || nCurveNumber < 0 || m_bPrintCurve)
		nCurveNumber = 0;

	if(m_fontPrinter.m_hObject==NULL)
	{
		m_fontPrinter.CreateFont(54 * fScale * FONTSIZE_RATIO[nCurveNumber], 0, 0, 0, FW_BOLD,//从上往下2700
			0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
			CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
			DEFAULT_PITCH,_T("Arial")); 
	}
	
	if(m_dataFont.m_hObject==NULL)
	{
		m_dataFont.CreateFont(36 * fScale * FONTSIZE_RATIO[nCurveNumber], 0, 0, 0, FW_NORMAL,//从上往下2700
			0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
			CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
			DEFAULT_PITCH,_T("Arial")); 
	}
	//m_dataFont.CreatePointFont(80,_T("宋体"),pDC);
	if(m_fontGroup.m_hObject == NULL)
	{
		m_fontGroup.CreateFont(36 * fScale * FONTSIZE_RATIO[nCurveNumber], 0, 0, 0, FW_BOLD,//从上往下2700
			0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
			CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
			DEFAULT_PITCH,_T("Arial")); 
	}
	//m_fontGroup.CreatePointFont(70,_T("宋体"),pDC);
	if(m_FontVR.m_hObject==NULL)
		m_FontVR.CreateFont(32 * fScale * FONTSIZE_RATIO[nCurveNumber], 0, 900, 900, FW_NORMAL,//从上往下2700
		0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH,_T("Arial"));	
}


//绘制带坐标曲线，不带有AB标线，导出excel专用 lzy2014.8.26
void CPrintView::DrawTraceBmp(CDC *pDC, CRect rc, int nCurSel, float fontRatio)
{	
	//InitFont(pDC);  //lzy2014.9.3
	
	m_dataFont.DeleteObject();
	m_fontGroup.DeleteObject();
	m_FontVR.DeleteObject();

	float fScale = 1.0 ;
	int nCurveNumber = _wtoi(g_sorFileArray.reportConfig.WaveList);  //当前选择每页打印的曲线数
	//if(nCurveNumber > 4 || nCurveNumber < 0)
		//nCurveNumber = 0;
	//nCurveNumber = 0;

	if(m_dataFont.m_hObject==NULL)
	{
		m_dataFont.CreateFont(36 * fScale * fontRatio * FONTSIZE_RATIO[0], 0, 0, 0, FW_NORMAL,//从上往下2700
			0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
			CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
			DEFAULT_PITCH,_T("Arial")); 
	}
	//m_dataFont.CreatePointFont(80,_T("宋体"),pDC);
	if(m_fontGroup.m_hObject == NULL)
	{
		m_fontGroup.CreateFont(36 * fScale * fontRatio * FONTSIZE_RATIO[0], 0, 0, 0, FW_BOLD,//从上往下2700
			0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
			CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
			DEFAULT_PITCH,_T("Arial")); 
	}
	//m_fontGroup.CreatePointFont(70,_T("宋体"),pDC);
	if(m_FontVR.m_hObject==NULL)
		m_FontVR.CreateFont(32 * fScale * fontRatio * FONTSIZE_RATIO[0], 0, 900, 900, FW_NORMAL,//从上往下2700
		0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH,_T("Arial"));	

	pDC->SelectObject(&m_dataFont);
	//float flo_Length=g_sorFileArray.sorFileList[g_sorFileArray.curSelection].pTestResultInfo->Length;	
	float flo_Length=g_sorFileArray.sorFileList[nCurSel].pTestResultInfo->Length;	
	DrawScale(pDC, rc, 0, flo_Length);//画刻度lzy2014.7.18	
	pDC->Rectangle(&rc);//大矩形
	DrawGridLine(pDC, rc);//绘制栅格 wcq2012.10.31
	//DrawCurve(pDC, rc, g_sorFileArray.curSelection);
	DrawCurve(pDC, rc, nCurSel);
	if(!nCurveNumber)  //单报表就绘制AB光标
	    DrawCursorAB(pDC, rc); //绘制AB光标 xjf2014.03.03

}




//绘制刻度lzy2014.7.17
void CPrintView::DrawScale(CDC *pDC, CRect &rect, float fYmax, float fXmax)
{
	if (g_sorFileArray.waveConfig.ShowGrid)
	{
		//float flo_Length = fXmax;
		int ndx = rect.Width () / COOR_SPLIT_COUNT_H;
		int ndy = rect.Height () / COOR_SPLIT_COUNT_V;
		CString strText;
		CSize sizeText;
		CRect rectText;

		int iYmin = 0;//M_FMAXDB0;
		int iYmax = M_FMAXDB0 - M_FSTARTDB0;
		int itemp;
		int bottom = rect.bottom;
		int left = rect.left;
		int iOffset = min(10, rect.Height() / 20);
				
		//横坐标刻度
		rectText.top = rect.bottom + iOffset;		
		for ( int i = 0; i< COOR_SPLIT_COUNT_H; i++)
		{			
			itemp = left + ndx * i;
			strText.Format(_T("%.1f"), 0.001 * fXmax * i / COOR_SPLIT_COUNT_H);
			sizeText = pDC->GetTextExtent(strText);//得到文本长宽度
			rectText.left = itemp - sizeText.cx / 2;
			rectText.right = rectText.left + sizeText.cx;
			rectText.bottom = rectText.top + sizeText.cy;			
			pDC->DrawText(strText, &rectText, DT_LEFT | DT_SINGLELINE);			
		}
		strText = _T("km");
		sizeText = pDC->GetTextExtent(strText);//得到文本长宽度
		rectText.right = rect.right - 10;
		rectText.left = rectText.right - sizeText.cx;		
		pDC->DrawText(strText, &rectText, DT_LEFT | DT_SINGLELINE);


		//纵坐标刻度
		//pDC->SetTextAlign(TA_LEFT | TA_TOP);
		strText = _T("0000");
		sizeText = pDC->GetTextExtent(strText);//得到文本长宽度
		int iText = sizeText.cx;  //文字宽度
		for (int i=1; i<COOR_SPLIT_COUNT_V; i++)
		{
			//画Y轴刻度
			if(iYmax >= 0 && iYmax <= 100)
			{
				rectText.left = rect.left - iText - 5;		

				for(int i = 0; i < COOR_SPLIT_COUNT_V; i++)
				{
					itemp = bottom - ndy * i;
					strText.Format(_T("%.1f"), (M_FMAXDB0 - M_FSTARTDB0) * i / COUNT_GRIDLINE_VERTICAL);
					sizeText = pDC->GetTextExtent(strText);//得到文本长宽度
					rectText.top = itemp - sizeText.cy / 2;
					rectText.bottom = rectText.top + sizeText.cy;
					rectText.right = rectText.left + sizeText.cx;
					pDC->DrawText(strText, &rectText, DT_LEFT | DT_SINGLELINE);
				}

				strText = _T("dB");
				sizeText = pDC->GetTextExtent(strText);//得到文本长宽度
				rectText.top = rect.top + 5;
				rectText.bottom = rectText.top + sizeText.cy;
				rectText.right = rectText.left + sizeText.cx;
				pDC->DrawText(strText, &rectText, DT_LEFT | DT_SINGLELINE);
			}
		}
	}
}
//
////绘制刻度lzy2014.7.17
//void COtdrTrace::DrawScale(CDC *pDC)
//{
//	//如果不需要重绘则退出
//	if (!m_bDrawGrid)
//	{
//		return;
//	}
//
//	int iYmax = m_fRangeY[1];
//	int iXmax = m_fRangeX[1];
//	float ndx = m_rectCurve.Width ()/COUNT_GRIDLINE_HORIZONE;
//	float ndy = m_rectCurve.Height ()/ COUNT_GRIDLINE_VERTICAL;
//	int bottom = m_rectCurve.Height();
//	int left = 0;
//	int itemp;
//
//	CRect rectText;
//	CString strText;
//	pDC->SetBkMode(OPAQUE);
//	//pDC->SetBkMode(TRANSPARENT);
//	pDC->SetTextColor(g_COLORREF[9]);
//	pDC->SetBkColor(g_COLORREF[8]);
//	CSize sizeText;
//
//	CFont   font;   
//	CFont   *pOldFont;   
//	font.CreateFont(15, 0, 0, 0, FW_NORMAL,	0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
//		CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,DEFAULT_PITCH,_T("宋体")); 
//	//CreateMyFont(pDC, font, _T("Arial"));
//	pOldFont = pDC->SelectObject(&font);
//
//	//画Y轴刻度
//	if(iYmax >= 0 && iYmax <= 100)
//	{
//		rectText.left = m_rectCurve.left + 5;		
//		
//		for(int i = 1; i < COUNT_GRIDLINE_VERTICAL; i++)
//		{
//			itemp = bottom - ndy * i;
//			strText.Format(_T("%.1f"), m_fRangeY[0] + (m_fRangeY[1] - m_fRangeY[0]) * i / COUNT_GRIDLINE_VERTICAL);
//			sizeText = pDC->GetTextExtent(strText);//得到文本长宽度
//			rectText.top = itemp - sizeText.cy / 2;
//			rectText.bottom = rectText.top + sizeText.cy;
//			rectText.right = rectText.left + sizeText.cx;
//			pDC->DrawText(strText, &rectText, DT_LEFT | DT_SINGLELINE);
//		}
//
//		strText = _T("dB");
//		sizeText = pDC->GetTextExtent(strText);//得到文本长宽度
//		rectText.top = m_rectCurve.top + 5;
//		rectText.bottom = rectText.top + sizeText.cy;
//		rectText.right = rectText.left + sizeText.cx;
//		pDC->DrawText(strText, &rectText, DT_LEFT | DT_SINGLELINE);
//	}
//
//	//画X轴刻度
//	if(iXmax > 0)
//	{
//		rectText.bottom = m_rectCurve.bottom - 3;
//		for(int i = 1; i < COUNT_GRIDLINE_HORIZONE; i++)
//		{
//			itemp = left + ndx * i;
//			strText.Format(_T("%.1f"), 0.001 * m_fRangeX[0] + 0.001 * (m_fRangeX[1] - m_fRangeX[0]) * i / COUNT_GRIDLINE_HORIZONE);
//			sizeText = pDC->GetTextExtent(strText);//得到文本长宽度
//			rectText.left = itemp - sizeText.cx / 2;
//			rectText.right = rectText.left + sizeText.cx;
//			rectText.top = rectText.bottom - sizeText.cy;			
//			pDC->DrawText(strText, &rectText, DT_LEFT | DT_SINGLELINE);
//		}
//
//		strText = _T("km");
//		sizeText = pDC->GetTextExtent(strText);//得到文本长宽度
//		rectText.right = m_rectCurve.right - 10;
//		rectText.left = rectText.right - sizeText.cx;		
//		pDC->DrawText(strText, &rectText, DT_LEFT | DT_SINGLELINE);
//
//	}
//
//    pDC->SelectObject(pOldFont);
//}

/*
***************************************************************
函数名称:             DrawEventHead()                                                                               
函数说明:             画曲线事件列表头                                                                             
入口参数:             nLeft 左起点， nTop 上起点                                                                   
出口参数:             无                                                                                        
建立时间:             2011-07-24                                                                  
Write By:             XJF                                                                                          
***************************************************************
*/
void CPrintView::DrawEventHead(CDC* pDC, int nLeft, int nTop, CRect rectDraw)//绘制事件列表
{
	CFont* pOldFont = pDC->SelectObject (&m_dataFont);
	//绘制标题栏
	LV_COLUMN lvColumn;
	TCHAR uin[50];
	lvColumn.pszText=uin;
	lvColumn.cchTextMax=50;
	lvColumn.mask=LVCF_TEXT;
	CString header=_T("");
	int nLen = MAIN->m_pEventFormView->m_Eventlist.GetHeaderCtrl()->GetItemCount();//得到列数
	g_sorFileArray.reportConfig.EventColumn = nLen;   //保存列数
	int nVspace = rectDraw.Height() / 42;
	//int nHspace = rectDraw.Width() / 10;
	int nLeftSpace = rectDraw.Width() / MARGIN_LEFT_COUNT;
	int nHspace = ( rectDraw.Width() - 2 * nLeftSpace)  / nLen;  //lzy2014.8.12


	//表头内容
	if (nLen > 8) //最多9列
	{
		nLen = 8;
	}
	CRect textRect;
	for(int i=0;i<nLen;i++)
	{
		textRect.left = nLeft + i * nHspace;
		textRect.top = nTop;
		textRect.right = nLeft + (i+1) * nHspace;
		textRect.bottom = nTop + nVspace;
		if(MAIN->m_pEventFormView->m_Eventlist.GetColumn(i,&lvColumn))
		{
			header=lvColumn.pszText;
			pDC->DrawText(header, &textRect,DT_CENTER);//文字
			TRACE(_T("left=%d;right=%d;offset=%d\n"), textRect.left, textRect.right, nLeft);
		}
		//nStartLine++;
	}
	pDC->SelectObject (pOldFont);
}

/*
***************************************************************
函数名称:             DrawEventBody()                                                                               
函数说明:             画曲线事件列表体                                                                             
入口参数:             nLeft 左起点， nTop 上起点                                                                   
出口参数:             无                                                                                        
建立时间:             2011-07-24                                                                  
Write By:             XJF                                                                                          
***************************************************************
*/
void CPrintView::DrawEventBody(CDC* pDC, int nLeft, int nTop , int nVspace , CRect rectDraw, int nCurSel)//绘制事件列表
{
	CFont* pOldFont = pDC->SelectObject (&m_dataFont);
	//int nVspace = rectDraw.Height() / 42;
	//int nHspace = rectDraw.Width() / 10;
	int nLeftSpace = rectDraw.Width() / MARGIN_LEFT_COUNT;
	int nHspace = (rectDraw.Width() - 2 * nLeftSpace) / 7;  //lzy2014.8.12
	int nStartLine = 1;

	float preDistance=0;



	//TestResultInfo* pData = g_sorFileArray.sorFileList[g_sorFileArray.curSelection].pTestResultInfo;
	TestResultInfo* pData = g_sorFileArray.sorFileList[nCurSel].pTestResultInfo;  //lzy2014.8.30

	if (NULL == pData)
		return;

	CString strText=_T("Test Height");
	CSize sizeText = pDC->GetTextExtent(strText);
	int nCharHeight = sizeText.cy;  //获取字体高度
	CRect textRect;
	std::vector<EventInfo> ve = pData->EventList.vEventInfo;//速度??
	int nSize=ve.size();
	g_sorFileArray.reportConfig.EventCount = nSize;   //保存行数
	for (int i=0;i<nSize;i++)//
	{
		// 序号
		{
			textRect.left = nLeft;
			textRect.top = nTop + nStartLine * (nVspace + nCharHeight);
			textRect.right = nLeft + nHspace;
			textRect.bottom = textRect.top + nCharHeight; //nTop + (nStartLine+1) * nVspace; lzy 2014.8.15

			//类型
			strText.Format(_T("%d"),i+1);
			if (g_currentLang == LANG::CHS)
			{
				if (0 == ve[i].nEventType)
				{
					strText += _T(" 非反射");//0代表非反射事件符号
				}
				else if (1 == ve[i].nEventType)
				{
					strText += _T(" 反射");;//1代表反射事件符号
				}
			}
			else if (g_currentLang == LANG::ENG)
			{
				if (0 == ve[i].nEventType)
				{
					strText += _T(" NoReflect");//0代表非反射事件符号
				}
				else if (1 == ve[i].nEventType)
				{
					strText += _T(" Reflect");;//1代表反射事件符号
				}
			}

			if (_T('S')==ve[i].cEventType)
			{
				strText+=_T("(S)");
			}
			else if (_T('E')==ve[i].cEventType)
			{
				strText+=_T("(E)");
			}
			else if (_T('A')==ve[i].cEventType)
			{
				strText+=_T("(A)");
			}
			else if (_T('F')==ve[i].cEventType)
			{
				strText+=_T("(F)");
			}
			else if (_T('O')==ve[i].cEventType)
			{
				strText+=_T("(O)");
			}

			pDC->DrawText(strText, &textRect,DT_LEFT);//文字
		}
		// 事件类型
		//{
		//	textRect.left = nLeft + nHspace;
		//	textRect.top = nTop + nStartLine * nVspace;
		//	textRect.right = nLeft + 2 * nHspace;
		//	textRect.bottom = nTop + (nStartLine+1) * nVspace;

		//	//类型
		//	if (0 == ve[i].nEventType)
		//	{
		//		strText = _T("开始事件");
		//	}
		//	else if (1 == ve[i].nEventType)
		//	{
		//		strText = _T("反射事件");
		//	}
		//	else if (2 == ve[i].nEventType)
		//	{
		//		strText = _T("非反射事件");
		//	}
		//	else if (3 == ve[i].nEventType)
		//	{
		//		strText = _T("反射事件");
		//	}
		//	else if (4 == ve[i].nEventType)
		//	{
		//		strText = _T("结束事件");
		//	}
		//	else
		//	{
		//		strText = _T("--");
		//	}
		//	pDC->DrawText(strText, &textRect,DT_CENTER);//文字
		//}
		// 距离
		{
			textRect.left = nLeft + nHspace;
			textRect.top = nTop + nStartLine * (nVspace + nCharHeight);
			textRect.right = nLeft + 2 * nHspace;
			textRect.bottom = textRect.top + nCharHeight;//nTop + (nStartLine+1) * nVspace;
			float m_float_zll=0.0f;
			if (0==i)
			{
				strText.Format(_T("%.3f"),0.0f);//距离
			}
			else
			{
				if(_T("m")==g_sorFileArray.waveConfig.DistUnit)//横坐标单位为km
					strText.Format(_T("%.3f"),ve[i].fLocation/1000);//距离
				else if(_T("mi")==g_sorFileArray.waveConfig.DistUnit)//横坐标单位为miles
				{
					m_float_zll=KmeterToOther(ve[i].fLocation/1000,UNIT_SHOW_MILE);
					strText.Format(_T("%.3f"),m_float_zll);//距离
				}
				else if(_T("ft")==g_sorFileArray.waveConfig.DistUnit)//横坐标单位为kft
				{
					m_float_zll=KmeterToOther(ve[i].fLocation/1000,UNIT_SHOW_KFEET);
					strText.Format(_T("%.3f"),m_float_zll);//距离
				}
			}
			pDC->DrawText(strText, &textRect,DT_CENTER);//文字
		}
		// 段
		{
			textRect.left = nLeft + 2 * nHspace;
			textRect.top = nTop + nStartLine * (nVspace + nCharHeight);
			textRect.right = nLeft + 3 * nHspace;
			textRect.bottom = textRect.top + nCharHeight;//nTop + (nStartLine+1) * nVspace;
			float a,b=0.0f;
			if (0==i)
			{
				strText.Format(_T("%.3f"),0.0f);//段
			}
			else
			{
				if(_T("m")==g_sorFileArray.waveConfig.DistUnit)//横坐标单位为km
					strText.Format(_T("%.3f"),(ve[i].fLocation-preDistance)/1000);//段,两事件点间距离
				else if(_T("mi")==g_sorFileArray.waveConfig.DistUnit)//横坐标单位为miles
				{
					b=(ve[i].fLocation-preDistance)/1000;
					a=KmeterToOther(b,UNIT_SHOW_MILE);
					strText.Format(_T("%.3f"),a);//段,两事件点间距离
				}
				else if(_T("ft")==g_sorFileArray.waveConfig.DistUnit)//横坐标单位为kft
				{
					b=(ve[i].fLocation-preDistance)/1000;
					a=KmeterToOther(b,UNIT_SHOW_KFEET);
					strText.Format(_T("%.3f"),a);//段,两事件点间距离
				}
			}
			if (0==i)
			{
				preDistance = 0.0f;
			}
			else
			{
				preDistance = ve[i].fLocation;
			}
			pDC->DrawText(strText, &textRect,DT_CENTER);//文字
		}
		// 损耗
		{
			textRect.left = nLeft + 3 * nHspace;
			textRect.top = nTop + nStartLine * (nVspace + nCharHeight);
			textRect.right = nLeft + 4 * nHspace;
			textRect.bottom = textRect.top + nCharHeight;//nTop + (nStartLine+1) * nVspace;
			if (0 == ve[i].fEventloss)//为0时显示"-"wcq2012.07.09
			{
				strText.Format(_T("--"));//总损耗
			}
			else
			{
				strText.Format(_T("%.3f"), ve[i].fEventloss);//损耗
			}
			pDC->DrawText(strText, &textRect,DT_CENTER);//文字
		}
		// 总损耗
		{
			textRect.left = nLeft + 4 * nHspace;
			textRect.top = nTop + nStartLine * (nVspace + nCharHeight);
			textRect.right = nLeft + 5 * nHspace;
			textRect.bottom = textRect.top + nCharHeight;//nTop + (nStartLine+1) * nVspace;
			if (0 == ve[i].fTotalLoss)//为0时显示"-"wcq2012.07.09
			{
				strText.Format(_T("--"));//总损耗
			}
			else
			{
				strText.Format(_T("%.3f"),ve[i].fTotalLoss);//总损耗
			}
			pDC->DrawText(strText, &textRect,DT_CENTER);//文字
		}
		// 斜率
		{
			textRect.left = nLeft + 5 * nHspace;
			textRect.top = nTop + nStartLine * (nVspace + nCharHeight);
			textRect.right = nLeft + 6 * nHspace;
			textRect.bottom = textRect.top + nCharHeight; //nTop + (nStartLine+1) * nVspace;
			//斜率
			if (fabs(ve[i].fLineK)>0.5)//zll 2012.10.31
			{
				strText.Format(_T("--"));
			}
			else if (0 == ve[i].fLineK)//为0时显示"-" zll 2012.10.31
			{
				strText.Format(_T("--"));
			}
			else
			{
				strText.Format(_T("%.3f"),ve[i].fLineK);//斜率
			}
			pDC->DrawText(strText, &textRect,DT_CENTER);//文字
		}
		// 反射
		{
			textRect.left = nLeft + 6 * nHspace;
			textRect.top = nTop + nStartLine * (nVspace + nCharHeight);
			textRect.right = nLeft + 7 * nHspace;
			textRect.bottom = textRect.top + nCharHeight; //nTop + (nStartLine+1) * nVspace;
			if (0 == ve[i].nEventType)//反射
			{
				strText=_T("--");
			}
			else if (0 == ve[i].fEventHigh)//为0时显示"-"wcq2012.07.09
			{
				strText=_T("--");
			}
			else
			{
				strText.Format(_T("%.3f"),ve[i].fEventHigh);//反射
			}
			pDC->DrawText(strText, &textRect,DT_CENTER);//文字
		}

		nStartLine++; //行数加1
	}
	pDC->SelectObject (pOldFont);
}


/*
***************************************************************
函数名称:             DrawEventBody()                                                                               
函数说明:             画曲线事件列表体                                                                             
入口参数:             nLeft 左起点， nTop 上起点                                                                   
出口参数:             无                                                                                        
建立时间:             2011-07-24                                                                  
Write By:             XJF                                                                                          
***************************************************************
*/
void CPrintView::DrawEventBody1(CDC* pDC, int nLeft, int nTop,int m_nCurPage)//绘制事件列表
{
	CFont* pOldFont = pDC->SelectObject (&m_dataFont);
	int nVspace = m_rectDraw.Height() / 42;
	int nHspace = m_rectDraw.Width() / 10;
	int nStartLine = 1;

	float preDistance=0;

	TestResultInfo* pData = g_sorFileArray.sorFileList[m_nCurPage-1].pTestResultInfo;
	if (NULL == pData)
		return;

	CString strText=_T("");
	CRect textRect;
	std::vector<EventInfo> ve = pData->EventList.vEventInfo;//速度??
	int nSize=ve.size();
	for (int i=0;i<nSize;i++)//
	{
		// 序号
		{
			textRect.left = nLeft;
			textRect.top = nTop + nStartLine * nVspace;
			textRect.right = nLeft + nHspace;
			textRect.bottom = nTop + (nStartLine+1) * nVspace;

			//类型
			strText.Format(_T("%d"),i+1);
			if (g_currentLang == LANG::CHS)
			{
				if (0 == ve[i].nEventType)
				{
					strText += _T(" 非反射");//0代表非反射事件符号
				}
				else if (1 == ve[i].nEventType)
				{
					strText += _T(" 反射");;//1代表反射事件符号
				}
			}
			else if (g_currentLang == LANG::ENG)
			{
				if (0 == ve[i].nEventType)
				{
					strText += _T(" NoReflect");//0代表非反射事件符号
				}
				else if (1 == ve[i].nEventType)
				{
					strText += _T(" Reflect");;//1代表反射事件符号
				}
			}

			if (_T('S')==ve[i].cEventType)
			{
				strText+=_T("(S)");
			}
			else if (_T('E')==ve[i].cEventType)
			{
				strText+=_T("(E)");
			}
			else if (_T('A')==ve[i].cEventType)
			{
				strText+=_T("(A)");
			}
			else if (_T('F')==ve[i].cEventType)
			{
				strText+=_T("(F)");
			}
			else if (_T('O')==ve[i].cEventType)
			{
				strText+=_T("(O)");
			}

			pDC->DrawText(strText, &textRect,DT_LEFT);//文字
		}
		// 事件类型
		//{
		//	textRect.left = nLeft + nHspace;
		//	textRect.top = nTop + nStartLine * nVspace;
		//	textRect.right = nLeft + 2 * nHspace;
		//	textRect.bottom = nTop + (nStartLine+1) * nVspace;

		//	//类型
		//	if (0 == ve[i].nEventType)
		//	{
		//		strText = _T("开始事件");
		//	}
		//	else if (1 == ve[i].nEventType)
		//	{
		//		strText = _T("反射事件");
		//	}
		//	else if (2 == ve[i].nEventType)
		//	{
		//		strText = _T("非反射事件");
		//	}
		//	else if (3 == ve[i].nEventType)
		//	{
		//		strText = _T("反射事件");
		//	}
		//	else if (4 == ve[i].nEventType)
		//	{
		//		strText = _T("结束事件");
		//	}
		//	else
		//	{
		//		strText = _T("--");
		//	}
		//	pDC->DrawText(strText, &textRect,DT_CENTER);//文字
		//}
		// 距离
		{
			textRect.left = nLeft + nHspace;
			textRect.top = nTop + nStartLine * nVspace;
			textRect.right = nLeft + 2 * nHspace;
			textRect.bottom = nTop + (nStartLine+1) * nVspace;
			float m_float_zll=0.0f;
			if (0==i)
			{
				strText.Format(_T("%.3f"),0.0f);//距离
			}
			else
			{
				if(_T("m")==g_sorFileArray.waveConfig.DistUnit)//横坐标单位为km
					strText.Format(_T("%.3f"),ve[i].fLocation/1000);//距离
				else if(_T("mi")==g_sorFileArray.waveConfig.DistUnit)//横坐标单位为miles
				{
					m_float_zll=KmeterToOther(ve[i].fLocation/1000,UNIT_SHOW_MILE);
					strText.Format(_T("%.3f"),m_float_zll);//距离
				}
				else if(_T("ft")==g_sorFileArray.waveConfig.DistUnit)//横坐标单位为kft
				{
					m_float_zll=KmeterToOther(ve[i].fLocation/1000,UNIT_SHOW_KFEET);
					strText.Format(_T("%.3f"),m_float_zll);//距离
				}
			}
			pDC->DrawText(strText, &textRect,DT_CENTER);//文字
		}
		// 段
		{
			textRect.left = nLeft + 2 * nHspace;
			textRect.top = nTop + nStartLine * nVspace;
			textRect.right = nLeft + 3 * nHspace;
			textRect.bottom = nTop + (nStartLine+1) * nVspace;
			float a,b=0.0f;
			if (0==i)
			{
				strText.Format(_T("%.3f"),0.0f);//段
			}
			else
			{
				if(_T("m")==g_sorFileArray.waveConfig.DistUnit)//横坐标单位为km
					strText.Format(_T("%.3f"),(ve[i].fLocation-preDistance)/1000);//段,两事件点间距离
				else if(_T("mi")==g_sorFileArray.waveConfig.DistUnit)//横坐标单位为miles
				{
					b=(ve[i].fLocation-preDistance)/1000;
					a=KmeterToOther(b,UNIT_SHOW_MILE);
					strText.Format(_T("%.3f"),a);//段,两事件点间距离
				}
				else if(_T("ft")==g_sorFileArray.waveConfig.DistUnit)//横坐标单位为kft
				{
					b=(ve[i].fLocation-preDistance)/1000;
					a=KmeterToOther(b,UNIT_SHOW_KFEET);
					strText.Format(_T("%.3f"),a);//段,两事件点间距离
				}
			}
			if (0==i)
			{
				preDistance = 0.0f;
			}
			else
			{
				preDistance = ve[i].fLocation;
			}
			pDC->DrawText(strText, &textRect,DT_CENTER);//文字
		}
		// 损耗
		{
			textRect.left = nLeft + 3 * nHspace;
			textRect.top = nTop + nStartLine * nVspace;
			textRect.right = nLeft + 4 * nHspace;
			textRect.bottom = nTop + (nStartLine+1) * nVspace;
			if (0 == ve[i].fEventloss)//为0时显示"-"wcq2012.07.09
			{
				strText.Format(_T("--"));//总损耗
			}
			else
			{
				strText.Format(_T("%.3f"), ve[i].fEventloss);//损耗
			}
			pDC->DrawText(strText, &textRect,DT_CENTER);//文字
		}
		// 总损耗
		{
			textRect.left = nLeft + 4 * nHspace;
			textRect.top = nTop + nStartLine * nVspace;
			textRect.right = nLeft + 5 * nHspace;
			textRect.bottom = nTop + (nStartLine+1) * nVspace;
			if (0 == ve[i].fTotalLoss)//为0时显示"-"wcq2012.07.09
			{
				strText.Format(_T("--"));//总损耗
			}
			else
			{
				strText.Format(_T("%.3f"),ve[i].fTotalLoss);//总损耗
			}
			pDC->DrawText(strText, &textRect,DT_CENTER);//文字
		}
		// 斜率
		{
			textRect.left = nLeft + 5 * nHspace;
			textRect.top = nTop + nStartLine * nVspace;
			textRect.right = nLeft + 6 * nHspace;
			textRect.bottom = nTop + (nStartLine+1) * nVspace;
			//斜率
			if (fabs(ve[i].fLineK)>0.5)//zll 2012.10.31
			{
				strText.Format(_T("--"));
			}
			else if (0 == ve[i].fLineK)//为0时显示"-" zll 2012.10.31
			{
				strText.Format(_T("--"));
			}
			else
			{
				strText.Format(_T("%.3f"),ve[i].fLineK);//斜率
			}
			pDC->DrawText(strText, &textRect,DT_CENTER);//文字
		}
		// 反射
		{
			textRect.left = nLeft + 6 * nHspace;
			textRect.top = nTop + nStartLine * nVspace;
			textRect.right = nLeft + 7 * nHspace;
			textRect.bottom = nTop + (nStartLine+1) * nVspace;
			if (0 == ve[i].nEventType)//非反射
			{
				strText=_T("--");
			}
			else if (0 == ve[i].fEventHigh)//为0时显示"-"wcq2012.07.09
			{
				strText=_T("--");
			}
			else
			{
				strText.Format(_T("%.3f"),ve[i].fEventHigh);//反射
			}
			pDC->DrawText(strText, &textRect,DT_CENTER);//文字
		}
	
		nStartLine++; //行数加1
	}
	pDC->SelectObject (pOldFont);
}

/*
***************************************************************
函数名称:             DrawCurveParam()                                                                               
函数说明:             画曲线事件列表体                                                                             
入口参数:             nLeft 左起点， nTop 上起点                                                                   
出口参数:             无                                                                                        
建立时间:             2011-07-24                                                                  
Write By:             XJF                                                                                          
***************************************************************
*/
void CPrintView::DrawCurveParam(CDC* pDC, int nLeft, int nTop)//绘制曲线参数
{
	int nVspace = m_rectDraw.Height() / 42;
	int nHspace = m_rectDraw.Width() / 10;
	int nStartLine = 1;

	TestResultInfo* pData = g_sorFileArray.sorFileList[g_sorFileArray.curSelection].pTestResultInfo;
	if (NULL == pData)
		return;

	CFont* pOldFont = pDC->SelectObject (&m_dataFont);
	CRect textRect;
	CString strText;
	// 文件名
	{
		textRect.left = nLeft;
		textRect.top = nTop + nVspace;
		textRect.right = m_rectDraw.right;
		textRect.bottom = nTop + 2 * nVspace;
		CString strFileName = IniUtil::ReadResString(_T("IDS_FRM_VIEW_DLG_FILENAME"));
		strText.Format(strFileName+_T("：	%s"), g_sorFileArray.sorFileList[g_sorFileArray.curSelection].filePathName);
		pDC->DrawText(strText, &textRect,DT_LEFT);//文字
	}
	// 波长
	{
		textRect.left = m_rectDraw.Width()/2;
		textRect.top = nTop + 2 * nVspace;
		textRect.right = textRect.left + 4 * nHspace;
		textRect.bottom = nTop + 3 * nVspace;
		CString strWaveLength = IniUtil::ReadResString(_T("IDS_FRM_WAVE_DLG_WAVE_LENGTH"));
		strText.Format(strWaveLength+_T("：	%dnm"), pData->WaveLength);//pData->WaveLength / 10改为pData->WaveLength zll 2012.11.1
		pDC->DrawText(strText, &textRect,DT_LEFT);//文字
	}
	// 脉宽
	{
		textRect.left = nLeft;
		textRect.top = nTop + 2 * nVspace;
		textRect.right = nLeft + 4 * nHspace;
		textRect.bottom = nTop + 3 * nVspace;
		CString strPulseWidth = IniUtil::ReadResString(_T("IDS_FRM_WAVE_DLG_PULSE_WIDTH"));
		if (pData->PulseWidth>1000||pData->PulseWidth==1000)
		{
			strText.Format(strPulseWidth+_T("：	%dus"), pData->PulseWidth/1000);
		}
		else
		{
			strText.Format(strPulseWidth+_T("：	%dns"), pData->PulseWidth);
		}
		pDC->DrawText(strText, &textRect,DT_LEFT);//文字
	}

	// 测量范围
	{
		float m_Length=0.0f;
		textRect.left = m_rectDraw.Width()/2;
		textRect.top = nTop + 3 * nVspace;
		textRect.right = textRect.left + 4 * nHspace;
		textRect.bottom = nTop + 4 * nVspace;
		CString strRange=IniUtil::ReadResString(_T("IDS_FRM_WAVE_DLG_RANGE"));
		if (_T("m")==g_sorFileArray.waveConfig.DistUnit)
		{
			strText.Format(strRange+_T("：	%.2fkm"), pData->Length/1000.0);
		}
		else if (_T("mi")==g_sorFileArray.waveConfig.DistUnit)
		{
			m_Length=KmeterToOther(pData->Length/1000.0,UNIT_SHOW_MILE);
			strText.Format(strRange+_T("：	%.2fmiles"), m_Length);
		}
		else if (_T("ft")==g_sorFileArray.waveConfig.DistUnit)
		{
			m_Length=KmeterToOther(pData->Length/1000.0,UNIT_SHOW_KFEET);
			strText.Format(strRange+_T("：	%.2fkft"), m_Length);
		}
		pDC->DrawText(strText, &textRect,DT_LEFT);//文字
	}
	// 折射率
	{
		textRect.left = nLeft;
		textRect.top = nTop + 3 * nVspace;
		textRect.right = nLeft + 4 * nHspace;
		textRect.bottom = nTop + 4 * nVspace;
		CString strRefraction = IniUtil::ReadResString(_T("IDS_FRM_WAVE_DLG_REFRACTION"));
		strText.Format(strRefraction+_T("：	%.3f"), pData->Refraction);
		pDC->DrawText(strText, &textRect,DT_LEFT);//文字
	}

	// 衰减 zll 2012.11.1 暂时注释
	//{
	//	textRect.left = m_rectDraw.Width()/2;
	//	textRect.top = nTop + 4 * nVspace;
	//	textRect.right = textRect.left + 4 * nHspace;
	//	textRect.bottom = nTop + 5 * nVspace;
	//	CString strAttenu = IniUtil::ReadResString(_T("IDS_FRM_WAVE_DLG_ATTENU"));
	//	strText.Format(strAttenu+_T("：	%.2f"), pData->Atten);
	//	pDC->DrawText(strText, &textRect,DT_LEFT);//文字
	//}
	// 采样数
	{
		textRect.left = nLeft;
		textRect.top = nTop + 4 * nVspace;
		textRect.right = nLeft + 4 * nHspace;
		textRect.bottom = nTop + 5 * nVspace;
		CString strSample = IniUtil::ReadResString(_T("IDS_FRM_WAVE_DLG_SAMPLE"));
		strText.Format(strSample+_T("：	%d"), pData->DataPointsNum);
		pDC->DrawText(strText, &textRect,DT_LEFT);//文字
	}

	pDC->SelectObject (pOldFont);
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
void CPrintView::DrawCurve(CDC* pDC, CRect& rect, int nCurveIndex)
{
	TestResultInfo* pData = g_sorFileArray.sorFileList[nCurveIndex].pTestResultInfo;
	if (pData == NULL ||
		pData->DataPointsNum < NUM_VALIDE_LEAST ||
		CLEAR_DATAPOINTS_NUM == pData->DataPointsNum)//没有或数据很少
		return;

	unsigned int i;

	//转化为绘图像素坐标
	if (NULL==pData->ptDrawData)
	{
		pData->ptDrawData=new CPoint[pData->DataPointsNum];
	}
	pData->StartPoint=0;
    pData->MaxPoint=pData->DataPointsNum;
	for(i = pData->StartPoint; i < pData->MaxPoint; i++)
	{
		pData->ptDrawData[i].x = int(rect.left + rect.Width () * (i - pData->StartPoint)/(pData->MaxPoint - pData->StartPoint));
		//pData->ptDrawData[i].y = int(rect.bottom - (((-pData->pDataPoints[i]/1000.0) - M_FSTARTDB0)/(M_FMAXDB0 - M_FSTARTDB0)) * rect.Height());
#ifdef COMPILE_SHOW_SMOOTH_DATA////显示时用平滑过的数据，保存时用实际数据 2013.05.12
		pData->ptDrawData[i].y = int(rect.bottom - (((-pData->pDataPointsSmooth[i]/1000.0) - M_FSTARTDB0)/(M_FMAXDB0 - M_FSTARTDB0)) * rect.Height());
#else
		//pData->ptDrawData[i].y = int(rect.bottom - (((-pData->pDataPoints[i]/1000.0) - M_FSTARTDB0)/(M_FMAXDB0 - M_FSTARTDB0)) * rect.Height() - 150);//zll 2013.6.3 -150 使曲线沿y轴向上移动
		pData->ptDrawData[i].y = int(rect.bottom - (((-pData->pDataPoints[i]/1000.0) - M_FSTARTDB0)/(M_FMAXDB0 - M_FSTARTDB0)) * rect.Height());//lzy2014.7.18

#endif
		if(pData->ptDrawData[i].x > rect.right)
			pData->ptDrawData[i].x = rect.right;
		if(pData->ptDrawData[i].y < rect.top)
			pData->ptDrawData[i].y = rect.top;
		if(pData->ptDrawData[i].y > rect.bottom)
			pData->ptDrawData[i].y = rect.bottom; 
	}

	//画曲线
	pDC->Polyline(&pData->ptDrawData[pData->StartPoint], pData->MaxPoint - pData->StartPoint);

	//绘事件点竖线及下方矩形框 zll 2013.5.23 晚
	int fX,fY;
	int prevfX = 0;//上一节点的位置
	int offsetfx = 0; //两节点间偏距
	CRect rectTemp;
	rectTemp.SetRectEmpty();
	//TEXTMETRIC  tm;
	//pDC->GetTextMetrics(&tm);
	//int nCharWidth = tm.tmMaxCharWidth;  //最大字符宽度
	for (int i=0; i<pData->EventList.vEventInfo.size(); i++)
	{		
		CRect rectLineBottom;
		CString temp;
		fX=int(rect.left + pData->EventList.vEventInfo[i].fLocation*rect.Width()/pData->Length);
		//fY=int(rect.bottom - (65535-pData->pDataPoints[pData->EventList.vEventInfo[i].index])/1000.0*rect.Height()/(g_nTraceYmax-OTDR_TRACE_Y_MIN) - 110);//zll 2013.6.3 -110 使事件点标示沿y轴向上移动
		fY=int(rect.bottom - (65535-pData->pDataPoints[pData->EventList.vEventInfo[i].index])/1000.0*rect.Height()/(g_nTraceYmax-OTDR_TRACE_Y_MIN));//lzy2014.7.23 使事件点文字不沿y轴移动

		if(i == 0)
		{
			prevfX = fX;
		}
		else
		{
			offsetfx = fX - prevfX;
			prevfX = fX;
		}
		//画竖直线
		int nLineH = rect.Height() / 10; //竖线高度
		int nSquare = nLineH / 5 ;  //小正方形的边长
		int nLineTop = nLineH / 6;  //上起始点位置
		pDC->MoveTo(fX,fY- nLineTop);
		//pDC->LineTo(fX,fY+44+50);//zll 2013.6.3 +50 使事件点标示沿y轴向上移动
		pDC->LineTo(fX,fY+nLineH);//lzy2014.8.26 使事件点标示沿y轴向上移动
		//竖直线下面的正方形
		rectLineBottom.SetRect(fX - nSquare / 2,fY + nLineH,fX + nSquare / 2, fY + nLineH + nSquare);
		//rectLineBottom.SetRect(fX - 8-20,fY + 34+60,fX - 8 + 16+20, fY + 34 + 18+100);
		if (i>8)//两位数
		{
			rectLineBottom.right += 2;
		}
		//pDC->Rectangle(&rectLineBottom);
		//绘制衰减和相对距离、斜率
		CString strEventLosss;
		strEventLosss.Format(_T("%.3fdB"), pData->EventList.vEventInfo[i].fEventloss);
		CString strLocation;
		CString strLink;
		//距离文本
		float fValLocation = pData->EventList.vEventInfo[i].fLocation/Divisor_toKm;
		float fValLink = pData->EventList.vEventInfo[i].fLineK;
		if(_T("m")==g_sorFileArray.waveConfig.DistUnit)//横坐标单位为km
		{
			strLocation.Format(_T("%.3fkm"), fValLocation);
			if(0 != fValLink)
				strLink.Format(_T("%.3fdB/km"), fValLink);
		}
		else if(_T("mi")==g_sorFileArray.waveConfig.DistUnit)//横坐标单位为miles
		{
			fValLocation=KmeterToOther(fValLocation,UNIT_SHOW_MILE);
			strLocation.Format(_T("%.3fmiles"), fValLocation);
			if(0 != fValLink)
				strLink.Format(_T("%.3fdB/fmiles"), fValLink);
		}
		else if(_T("ft")==g_sorFileArray.waveConfig.DistUnit)//横坐标单位为kft
		{
			fValLocation=KmeterToOther(fValLocation,UNIT_SHOW_KFEET);
			strLocation.Format(_T("%.3fkft"), fValLocation);
			if(0 != fValLink)
				strLink.Format(_T("%.3fdB/fmiles"), fValLink);
		}
		
		CFont* pOldPen = pDC->SelectObject(&m_FontVR);

		//计算要显示的字符串高度
		int height = pDC->DrawText( strEventLosss, &rectLineBottom, DT_WORDBREAK | DT_CENTER | DT_CALCRECT );
		CSize sizeText = pDC->GetTextExtent(strEventLosss);
		//rectLineBottom.top += (rectLineBottom.Height() - height)/2;
		//损耗
		pDC->TextOut(fX - sizeText.cy, fY + nLineH +  rectLineBottom.Height() + sizeText.cx, strEventLosss);
		//pDC->TextOut(fX - 20, fY + rectLineBottom.Width() + 130, strEventLosss);
		//位置
		sizeText = pDC->GetTextExtent(strLocation);
		pDC->TextOut(fX , fY + nLineH + rectLineBottom.Height() + sizeText.cx, strLocation);
		//斜率
		//pDC->TextOut(fX + offsetfx / 2, fY + rectLineBottom.Width() + 300, strLink);//cwcq
		sizeText = pDC->GetTextExtent(strEventLosss);
		pDC->TextOut(fX - offsetfx / 2, fY + nLineH*2 + rectLineBottom.Height() + sizeText.cx, strLink);//lzy2014.8.26
		pDC->SelectObject(pOldPen);
	}
}


/*
***************************************************************
函数名称:             DrawCursorAB()                                                                               
函数说明:             画光标AB                                                                              
入口参数:             CDC                                                                                           
出口参数:             无                                                                                        
建立时间:             2014-03-03                                                                  
Write By:             XJF                                                                                          
***************************************************************
*/
void CPrintView::DrawCursorAB(CDC* pDC, CRect& rect)
{
	if(!g_cursorInfo.bDrawAB)
		return;
	//绘制AB线
	INT nMarginTop = rect.Height() / 10;//lzy2014.8.26
	CPen linePen;
	linePen.CreatePen(PS_SOLID, 1, RGB(0,0,0));
	CPen *oldPen = (CPen*)pDC->SelectObject(&linePen);
	CRect rectHead;  //画个方框，把AB圈起来lzy2014.7.18
	int nHead = 2;//lzy2014.7.18
	//A线
	if(g_cursorInfo.fRatioA > 0)
	{
		INT ndxA = rect.left + g_cursorInfo.fRatioA * rect.Width();
		pDC->MoveTo(ndxA, rect.top + nMarginTop);
		pDC->LineTo(ndxA, rect.bottom);
		CString strText;
		strText.Format(_T("%s"), g_cursorInfo.strA);//距离及损耗不在此处显示lzy2014.7.18, g_cursorInfo.strDistanceA,g_cursorInfo.strDBA);
		CSize sizeText = pDC->GetTextExtent(strText);//得到文本长宽度
		CRect rectText;
		rectText.top = rect.top + nMarginTop;
		rectText.bottom = rectText.top + sizeText.cy;		

		if (ndxA + sizeText.cx + 1 <= rect.right)//文本正常显示，在右边,1：文本和竖线间隔
		{
			rectText.left = ndxA + 1;
			rectText.right = ndxA + 1 + sizeText.cx;
		}
		else//移动到右边，若文本显示不下，则把文本显示到左边
		{
			//rectText.left = ndxA - (sizeText.cx + 1);
			//rectText.right = ndxA - 1;
			
		}//eif

		rectText.left = ndxA - sizeText.cx / 2 + 1;
		rectText.right = rectText.left + sizeText.cx + 1;

		rectHead.SetRect(rectText.left - nHead, rectText.top - nHead / 2, rectText.right + nHead, rectText.bottom + nHead / 2); //lzy2014.7.18
		pDC->Rectangle(&rectHead);//lzy2014.7.18
		pDC->DrawText(strText, &rectText, DT_LEFT | DT_SINGLELINE);
	}
	//B线
	if(g_cursorInfo.fRatioB > 0)
	{
		//nMarginTop += 80;
		INT ndxB = rect.left + g_cursorInfo.fRatioB * rect.Width();
		pDC->MoveTo(ndxB, rect.top + nMarginTop);
		pDC->LineTo(ndxB, rect.bottom);
		CString strText;
		strText.Format(_T("%s"), g_cursorInfo.strB);//距离及损耗不在此处显示lzy2014.7.18, g_cursorInfo.strDistanceB,g_cursorInfo.strDBB);
		CSize sizeText = pDC->GetTextExtent(strText);//得到文本长宽度
		CRect rectText;
		rectText.top = rect.top + nMarginTop;
		rectText.bottom = rectText.top + sizeText.cy;
		//if (ndxB + sizeText.cx + 1 <= rect.right)//文本正常显示，在右边,1：文本和竖线间隔
		//{
		//	rectText.left = ndxB + 1;
		//	rectText.right = ndxB + 1 + sizeText.cx;
		//}
		//else//移动到右边，若文本显示不下，则把文本显示到左边
		//{
		//	rectText.left = ndxB - (sizeText.cx + 1);
		//	rectText.right = ndxB - 1;
		//}//eif
		rectText.left = ndxB - sizeText.cx / 2 + 1;
		rectText.right = rectText.left + sizeText.cx + 1;

		rectHead.SetRect(rectText.left - nHead, rectText.top - nHead / 2, rectText.right + nHead, rectText.bottom + nHead / 2); //lzy2014.7.18
		pDC->Rectangle(&rectHead);//lzy2014.7.18
		pDC->DrawText(strText, &rectText, DT_LEFT | DT_SINGLELINE);
	}

	pDC->SelectObject(oldPen);

	//将AB线的坐标显示在右上方lzy2014.7.18
	/*CString strTextAB;  
	CSize sizeTextAB;
	CRect rcTextAB;
	rcTextAB.right = rect.right - 5;
	
	strTextAB.Format(_T("%s %s %s"), g_cursorInfo.strA, g_cursorInfo.strDistanceA,g_cursorInfo.strDBA);
	sizeTextAB = pDC->GetTextExtent(strTextAB);
	rcTextAB.left = rcTextAB.right - sizeTextAB.cx;
	rcTextAB.top = rect.top + 5;
	rcTextAB.bottom = rcTextAB.top + sizeTextAB.cy;
	pDC->DrawText(strTextAB, &rcTextAB, DT_LEFT | DT_SINGLELINE);

	strTextAB.Format(_T("%s %s %s"), g_cursorInfo.strB, g_cursorInfo.strDistanceB,g_cursorInfo.strDBB);
	rcTextAB.left = rcTextAB.right - sizeTextAB.cx;
	rcTextAB.top = rect.top + sizeTextAB.cy + 10;
	rcTextAB.bottom = rcTextAB.top + sizeTextAB.cy;
	pDC->DrawText(strTextAB, &rcTextAB, DT_LEFT | DT_SINGLELINE);*/
	
	//在右上方画A/B/A-B的信息
	//float fABdistance = _wtof(g_cursorInfo.strDistanceA.GetBuffer()) - _wtof(g_cursorInfo.strDistanceB.GetBuffer()); //AB距离 
	//g_cursorInfo.strDistanceA.ReleaseBuffer();
	//g_cursorInfo.strDistanceB.ReleaseBuffer();
	//float fABLoss = abs(_wtof(g_cursorInfo.strDBA.GetBuffer()) - _wtof(g_cursorInfo.strDBB.GetBuffer())); 
	//g_cursorInfo.strDistanceA.ReleaseBuffer();
	//g_cursorInfo.strDistanceB.ReleaseBuffer();
	//strTextAB.Format(_T("A-B %.3fkm %.2fdB"), fABdistance, fABLoss);
	//sizeTextAB = pDC->GetTextExtent(strTextAB);
	//rcTextAB.top = rect.top + sizeTextAB.cy * 2 + 15;
	//rcTextAB.bottom = rcTextAB.top + sizeTextAB.cy;
	//rcTextAB.left = rcTextAB.right - sizeTextAB.cx;
	//pDC->DrawText(strTextAB, &rcTextAB, DT_LEFT | DT_SINGLELINE);
	//g_sorFileArray.reportConfig.
	
	//获取AB标的显示信息
	g_sorFileArray.reportConfig.Trace.LocationA = g_cursorInfo.strDistanceA;   //A标位置
	g_sorFileArray.reportConfig.Trace.LossA = g_cursorInfo.strDBA;  //A标损耗
	g_sorFileArray.reportConfig.Trace.LocationB = g_cursorInfo.strDistanceB;   //B标位置
	g_sorFileArray.reportConfig.Trace.LossB = g_cursorInfo.strDBB;  //B标损耗
	float fABdistance = _wtof(g_cursorInfo.strDistanceA.GetBuffer()) - _wtof(g_cursorInfo.strDistanceB.GetBuffer()); //AB距离 
	g_cursorInfo.strDistanceA.ReleaseBuffer();
	g_cursorInfo.strDistanceB.ReleaseBuffer();
	float fABLoss = abs(_wtof(g_cursorInfo.strDBA.GetBuffer()) - _wtof(g_cursorInfo.strDBB.GetBuffer())); 
	g_cursorInfo.strDistanceA.ReleaseBuffer();
	g_cursorInfo.strDistanceB.ReleaseBuffer();
	float fABslope = fABdistance !=0 ? fABLoss / fABdistance : 0;
	g_sorFileArray.reportConfig.Trace.DistanceAB.Format(_T("%.3f"),fABdistance); //AB标的距离	
	g_sorFileArray.reportConfig.Trace.LossAB.Format(_T("%.2f"), fABLoss); //AB标间的损耗	
	g_sorFileArray.reportConfig.Trace.SlopeAB.Format(_T("%.3f"), fABslope); //AB标间的损耗斜率

}

/*
***************************************************************
函数名称:             DrawCurveByReport()                                                                               
函数说明:             画测量曲线                                                                              
入口参数:             CDC                                                                                           
出口参数:             无                                                                                        
建立时间:             2011-07-24                                                                  
Write By:             XJF                                                                                          
***************************************************************
*/
void CPrintView::DrawCurveByReport(CDC* pDC, CRect& rect, int nCurveIndex)//zll 2013.6.3 晚
{
	TestResultInfo* pData = g_sorFileArray.sorFileList[nCurveIndex].pTestResultInfo;
	if (pData == NULL ||
		pData->DataPointsNum < NUM_VALIDE_LEAST ||
		CLEAR_DATAPOINTS_NUM == pData->DataPointsNum)//没有或数据很少
		return;

	unsigned int i;

	//转化为绘图像素坐标
	if (NULL==pData->ptDrawData)
	{
		pData->ptDrawData=new CPoint[pData->DataPointsNum];
	}
	pData->StartPoint=0;
	pData->MaxPoint=pData->DataPointsNum;
	for(i = pData->StartPoint; i < pData->MaxPoint; i++)
	{
		pData->ptDrawData[i].x = int(rect.left + rect.Width () * (i - pData->StartPoint)/(pData->MaxPoint - pData->StartPoint));
		//pData->ptDrawData[i].y = int(rect.bottom - (((-pData->pDataPoints[i]/1000.0) - M_FSTARTDB0)/(M_FMAXDB0 - M_FSTARTDB0)) * rect.Height());
#ifdef COMPILE_SHOW_SMOOTH_DATA////显示时用平滑过的数据，保存时用实际数据 2013.05.12
		pData->ptDrawData[i].y = int(rect.bottom - (((-pData->pDataPointsSmooth[i]/1000.0) - M_FSTARTDB0)/(M_FMAXDB0 - M_FSTARTDB0)) * rect.Height());
#else
		//pData->ptDrawData[i].y = int(rect.bottom - (((-pData->pDataPoints[i]/1000.0) - M_FSTARTDB0)/(M_FMAXDB0 - M_FSTARTDB0)) * rect.Height() - 150);//zll 2013.6.3 -150 使曲线沿y轴向上移动
		pData->ptDrawData[i].y = int(rect.bottom - (((-pData->pDataPoints[i]/1000.0) - M_FSTARTDB0)/(M_FMAXDB0 - M_FSTARTDB0)) * rect.Height());//lzy2014.7.23使曲线从0开始，不移动

#endif
		if(pData->ptDrawData[i].x > rect.right)
			pData->ptDrawData[i].x = rect.right;
		if(pData->ptDrawData[i].y < rect.top)
			pData->ptDrawData[i].y = rect.top;
		if(pData->ptDrawData[i].y > rect.bottom)
			pData->ptDrawData[i].y = rect.bottom; 
	}

	//画曲线
	pDC->Polyline(&pData->ptDrawData[pData->StartPoint], pData->MaxPoint - pData->StartPoint);

	//绘事件点竖线及下方矩形框 zll 2013.5.23 晚
	int fX,fY;
	int iYoffset = 134;
	for (int i=0; i<pData->EventList.vEventInfo.size(); i++)
	{		
		CRect rectLineBottom;
		CString temp;
		fX=int(rect.left + pData->EventList.vEventInfo[i].fLocation*rect.Width()/pData->Length);
		//fY=int(rect.bottom - (65535-pData->pDataPoints[pData->EventList.vEventInfo[i].index])/1000.0*rect.Height()/(g_nTraceYmax-OTDR_TRACE_Y_MIN) - 150);//zll 2013.6.3 -110 使事件点标示沿y轴向上移动
		fY=int(rect.bottom - (65535-pData->pDataPoints[pData->EventList.vEventInfo[i].index])/1000.0*rect.Height()/(g_nTraceYmax-OTDR_TRACE_Y_MIN));//lzy2014.7.23

		//画竖直线
		pDC->MoveTo(fX,fY);
		pDC->LineTo(fX,fY + iYoffset);//zll 2013.6.3 +50 使事件点标示沿y轴向上移动 原为134
		//竖直线下面的正方形
		rectLineBottom.SetRect(fX - 8-20,fY + 34+100,fX - 8 + 16+20, fY + 34 + 18+140);
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

void CPrintView::InnerDraw(CDC* pDC,int nCurPage)
{
	if (m_bPrintCurve) //打印当前选中曲线
	{
		//DrawCurCurve(pDC);
		DrawCurCurveNew(pDC, m_rectDraw, g_sorFileArray.curSelection); //lzy2014.8.11
		return;
	}
	
	int nPrint = _wtoi(g_sorFileArray.reportConfig.Print);
	if (!nPrint) //总表
		DrawReport(pDC);
	else //波形
	{
		int nCurveNumber = _wtoi(g_sorFileArray.reportConfig.WaveList);
		if (nCurveNumber == 0)
			Draw1CurveNew(pDC, nCurPage);
			//DrawCurCurveNew(pDC, m_rectDraw, g_sorFileArray.curSelection); //lzy2014.8.14			
		else if (nCurveNumber == 1)
			Draw2CurveNew(pDC,nCurPage);
		else if (nCurveNumber == 2)
			Draw4CurveNew(pDC,nCurPage);
		else if (nCurveNumber == 3)
			Draw6CurveNew(pDC,nCurPage);
		else if (nCurveNumber == 4)
			Draw8CurveNew(pDC,nCurPage);
	}
}

//绘制栅格 wcq2012.10.31
void CPrintView::DrawGridLine(CDC* pDC, CRect& rect)
{
	if(!g_sorFileArray.waveConfig.ShowGrid)//不显示栅格
	{
		return;
	}//eif

	CPen penGridLine;
	//penGridLine.CreatePen (PS_DOT/*虚线*/,1, g_sorFileArray.waveConfig.ColorGrid);//改为下面的cwcq2012.10.31
	penGridLine.CreatePen (PS_DOT/*虚线*/, 1, RGB(0xcc,0xcc,0xcc) );//wcq2012.10.31
	CPen* pOldPen = pDC->SelectObject (&penGridLine);

	if (g_sorFileArray.waveConfig.ShowGrid)
	{
		//横坐标刻度
		int ndx = rect.Width () / COOR_SPLIT_COUNT_H;
		int ndy = rect.Height () / COOR_SPLIT_COUNT_V;
		pDC->SelectObject (&penGridLine);
		for ( int i = 1; i< COOR_SPLIT_COUNT_H; i++)
		{	
			//横坐标刻度
			pDC->MoveTo(rect.left + ndx * i,rect.bottom );
			pDC->LineTo(rect.left + ndx * i,rect.top );
		}

		//纵坐标刻度
		pDC->SetTextAlign(TA_LEFT | TA_TOP);
		for (int i=1; i<COOR_SPLIT_COUNT_V; i++)
		{
			pDC->MoveTo(rect.left , rect.bottom - ndy * i);
			pDC->LineTo(rect.right, rect.bottom - ndy * i);
		}
	}

	//恢复
	pDC->SelectObject(pOldPen);
	//释放资源
	DeleteObject(penGridLine.GetSafeHandle());
}

CString CPrintView::right_MAX(float m_float_zll,float flo_Length)
{
	CString temp=_T("");
	if(_T("m")==g_sorFileArray.waveConfig.DistUnit)//横坐标单位为km
	{
		if(3==g_sorFileArray.waveConfig.UnitDistFig)
			temp.Format(_T("%.3fkm"),flo_Length / 1000);//wcq2012.11.02
		else if(4==g_sorFileArray.waveConfig.UnitDistFig)
			temp.Format(_T("%.4fkm"),flo_Length / 1000);//wcq2012.11.02
		else if(5==g_sorFileArray.waveConfig.UnitDistFig)
			temp.Format(_T("%.5fkm"),flo_Length / 1000);//wcq2012.11.02
	}
	else if(_T("mi")==g_sorFileArray.waveConfig.DistUnit)//横坐标单位为miles
	{
		m_float_zll=KmeterToOther(flo_Length / 1000,UNIT_SHOW_MILE);
		if(3==g_sorFileArray.waveConfig.UnitDistFig)
			temp.Format(_T("%.3fmiles"),m_float_zll);//距离
		else if(4==g_sorFileArray.waveConfig.UnitDistFig)
			temp.Format(_T("%.4fmiles"),m_float_zll);//距离
		else if(5==g_sorFileArray.waveConfig.UnitDistFig)
			temp.Format(_T("%.5fmiles"),m_float_zll);//距离
	}
	else if(_T("ft")==g_sorFileArray.waveConfig.DistUnit)//横坐标单位为kft
	{
		m_float_zll=KmeterToOther(flo_Length / 1000,UNIT_SHOW_KFEET);
		if(3==g_sorFileArray.waveConfig.UnitDistFig)
			temp.Format(_T("%.3fkft"),m_float_zll);//距离
		else if(4==g_sorFileArray.waveConfig.UnitDistFig)
			temp.Format(_T("%.4fkft"),m_float_zll);//距离
		else if(5==g_sorFileArray.waveConfig.UnitDistFig)
			temp.Format(_T("%.5fkft"),m_float_zll);//距离
	}
	return temp;
}

CString CPrintView::branch(float m_float_zll,float flo_Length)
{
	CString strDivUnit=_T("");
	if(_T("m")==g_sorFileArray.waveConfig.DistUnit)//横坐标单位为km
	{
		if(3==g_sorFileArray.waveConfig.UnitDistFig)
			strDivUnit.Format(_T("%.3fkm"),(flo_Length / 1000)/ 10);//wcq2012.11.02
		else if(4==g_sorFileArray.waveConfig.UnitDistFig)
			strDivUnit.Format(_T("%.4fkm"),(flo_Length / 1000)/ 10);//wcq2012.11.02
		else if(5==g_sorFileArray.waveConfig.UnitDistFig)
			strDivUnit.Format(_T("%.5fkm"),(flo_Length / 1000)/ 10);//wcq2012.11.02
	}
	else if(_T("mi")==g_sorFileArray.waveConfig.DistUnit)//横坐标单位为miles
	{
		m_float_zll=KmeterToOther((flo_Length / 1000)/ 10,UNIT_SHOW_MILE);
		if(3==g_sorFileArray.waveConfig.UnitDistFig)
			strDivUnit.Format(_T("%.3fmiles"),m_float_zll);//距离
		else if(4==g_sorFileArray.waveConfig.UnitDistFig)
			strDivUnit.Format(_T("%.4fmiles"),m_float_zll);//距离
		else if(5==g_sorFileArray.waveConfig.UnitDistFig)
			strDivUnit.Format(_T("%.5fmiles"),m_float_zll);//距离
	}
	else if(_T("ft")==g_sorFileArray.waveConfig.DistUnit)//横坐标单位为kft
	{
		m_float_zll=KmeterToOther((flo_Length / 1000)/ 10,UNIT_SHOW_KFEET);
		if(3==g_sorFileArray.waveConfig.UnitDistFig)
			strDivUnit.Format(_T("%.3fkft"),m_float_zll);//距离
		else if(4==g_sorFileArray.waveConfig.UnitDistFig)
			strDivUnit.Format(_T("%.4fkft"),m_float_zll);//距离
		else if(5==g_sorFileArray.waveConfig.UnitDistFig)
			strDivUnit.Format(_T("%.5fkft"),m_float_zll);//距离
	}
	return strDivUnit;
}

CString CPrintView::GetSorName(int m_CurveNum)
{
	int filePathNameLength=g_sorFileArray.sorFileList[m_CurveNum-1].filePathName.GetLength();
	int fileNameLength=g_sorFileArray.sorFileList[m_CurveNum-1].fileName.GetLength();
	CString sorName=g_sorFileArray.sorFileList[m_CurveNum-1].fileName.Mid(filePathNameLength+1,fileNameLength-(filePathNameLength+1));
	return sorName;
}

//获取打印需要的参数lzy 2014.8.12
void CPrintView::GetPrintInfo(int nCurSel)
{
	CString strSection, strKey, strValue;

	//获取需要打印的内容
	{
		strSection = _T("PrintSetting");
		strKey = _T("SelectLogo");
		strValue = IniUtil::ReadSingleConfigInfo(strSection, strKey);
		m_bPrintLogo = strValue == _T("0") ? 0 : 1;

		strKey = _T("SelectJob");
		strValue = IniUtil::ReadSingleConfigInfo(strSection, strKey);
		m_bPrintJob = strValue == _T("0") ? 0 : 1;

		strKey = _T("SelectConfig");
		strValue = IniUtil::ReadSingleConfigInfo(strSection, strKey);
		m_bPrintConfig = strValue == _T("0") ? 0 : 1;

		strKey = _T("SelectComment");
		strValue = IniUtil::ReadSingleConfigInfo(strSection, strKey);
		m_bPrintComment = strValue == _T("0") ? 0 : 1;

		strKey = _T("SelectTrace");
		strValue = IniUtil::ReadSingleConfigInfo(strSection, strKey);
		m_bPrintTrace = strValue == _T("0") ? 0 : 1;

		strKey = _T("SelectEvent");
		strValue = IniUtil::ReadSingleConfigInfo(strSection, strKey);
		m_bPrintEvent = strValue == _T("0") ? 0 : 1;		
	}

	//获取job info
	{
		strKey = _T("Job");
		g_sorFileArray.reportConfig.Job.JobID = IniUtil::ReadSingleConfigInfo(strSection, strKey);
		strKey = _T("Contractor");
		g_sorFileArray.reportConfig.Job.Contractor = IniUtil::ReadSingleConfigInfo(strSection, strKey);
		strKey = _T("Customer");
		g_sorFileArray.reportConfig.Job.Customer = IniUtil::ReadSingleConfigInfo(strSection, strKey);
		strKey = _T("Operator");
		g_sorFileArray.reportConfig.Job.Operator = IniUtil::ReadSingleConfigInfo(strSection, strKey);
		if(g_sorFileArray.totalCurveNumber > 0)
		{			
			g_sorFileArray.reportConfig.Job.SorFileName = g_sorFileArray.sorFileList[nCurSel].fileName;//filePathName;
			CTime tm(g_sorFileArray.sorFileList[nCurSel].pTestResultInfo->MeasureTime);
			g_sorFileArray.reportConfig.Job.TestDate = tm.Format(_T("%Y-%m-%d %H:%M:%S"));
		}
		
	}

	//获取配置信息
	{
		strKey = _T("OriginLocation");
		g_sorFileArray.reportConfig.Configuration.OriLocation = IniUtil::ReadSingleConfigInfo(strSection, strKey);
		strKey = _T("OriginCable");
		g_sorFileArray.reportConfig.Configuration.OriCable = IniUtil::ReadSingleConfigInfo(strSection, strKey);
		strKey = _T("OriginFiber");
		g_sorFileArray.reportConfig.Configuration.OriFiber = IniUtil::ReadSingleConfigInfo(strSection, strKey);
		strKey = _T("OriginColor");
		g_sorFileArray.reportConfig.Configuration.OriColor = IniUtil::ReadSingleConfigInfo(strSection, strKey);
		strKey = _T("EndLocation");
		g_sorFileArray.reportConfig.Configuration.EndLocation = IniUtil::ReadSingleConfigInfo(strSection, strKey);
		strKey = _T("EndCable");
		g_sorFileArray.reportConfig.Configuration.EndCable = IniUtil::ReadSingleConfigInfo(strSection, strKey);
		strKey = _T("EndFiber");
		g_sorFileArray.reportConfig.Configuration.EndFiber = IniUtil::ReadSingleConfigInfo(strSection, strKey);
		strKey = _T("EndColor");
		g_sorFileArray.reportConfig.Configuration.EndColor = IniUtil::ReadSingleConfigInfo(strSection, strKey);
		
		strKey = _T("SelfDefine");
		strValue = IniUtil::ReadSingleConfigInfo(strSection, strKey);
		BOOL bSelfDefine = strValue == _T("0") ? 0 : 1;
		if(! bSelfDefine)
		{
		
			if(g_sorFileArray.totalCurveNumber > 0)
			{
				g_sorFileArray.reportConfig.Configuration.Wavelength.Format(_T("%d"), g_sorFileArray.sorFileList[nCurSel].pTestResultInfo->WaveLength);  //波长
				g_sorFileArray.reportConfig.Configuration.Pulse.Format(_T("%d"), g_sorFileArray.sorFileList[nCurSel].pTestResultInfo->PulseWidth); //脉宽
				g_sorFileArray.reportConfig.Configuration.Range.Format(_T("%d"), g_sorFileArray.sorFileList[nCurSel].pTestResultInfo->Range); //脉宽
				g_sorFileArray.reportConfig.Configuration.LossThreshold.Format(_T("%.3f"), g_sorFileArray.sorFileList[nCurSel].pTestResultInfo->LossThreshold); //衰减门限
				g_sorFileArray.reportConfig.Configuration.RefThreshold.Format(_T("%.3f"), g_sorFileArray.sorFileList[nCurSel].pTestResultInfo->ReflectanceThreshold); //反射门限
				g_sorFileArray.reportConfig.Configuration.EndThreshold.Format(_T("%.3f"), g_sorFileArray.sorFileList[nCurSel].pTestResultInfo->EndOfFiberThreshold); //光纤末端门限
				g_sorFileArray.reportConfig.Configuration.BackScatter.Format(_T("%.3f"), g_sorFileArray.sorFileList[nCurSel].pTestResultInfo->BackscatterCoefficient); //光纤末端门限
				g_sorFileArray.reportConfig.Configuration.Average.Format(_T("%.0f"), g_sorFileArray.sorFileList[nCurSel].pTestResultInfo->Average); //平均次数
				g_sorFileArray.reportConfig.Configuration.Index.Format(_T("%.3f"),  g_sorFileArray.sorFileList[nCurSel].pTestResultInfo->GroupIndex);//折射率

			}
		}
		else
		{
			strKey = _T("Wavelength");
			g_sorFileArray.reportConfig.Configuration.Wavelength = IniUtil::ReadSingleConfigInfo(strSection, strKey);
			strKey = _T("Pulse");
			g_sorFileArray.reportConfig.Configuration.Pulse = IniUtil::ReadSingleConfigInfo(strSection, strKey);
			strKey = _T("Range");
			g_sorFileArray.reportConfig.Configuration.Range = IniUtil::ReadSingleConfigInfo(strSection, strKey);
			strKey = _T("LossThreshold");
			g_sorFileArray.reportConfig.Configuration.LossThreshold = IniUtil::ReadSingleConfigInfo(strSection, strKey);
			strKey = _T("ReflectionThreshold");
			g_sorFileArray.reportConfig.Configuration.RefThreshold = IniUtil::ReadSingleConfigInfo(strSection, strKey);
			strKey = _T("EndThreshold");
			g_sorFileArray.reportConfig.Configuration.EndThreshold = IniUtil::ReadSingleConfigInfo(strSection, strKey);
			strKey = _T("BackscatterCoeff");
			g_sorFileArray.reportConfig.Configuration.BackScatter = IniUtil::ReadSingleConfigInfo(strSection, strKey);
			strKey = _T("Index");
			g_sorFileArray.reportConfig.Configuration.Index = IniUtil::ReadSingleConfigInfo(strSection, strKey);		
			strKey = _T("Average");
			g_sorFileArray.reportConfig.Configuration.Average = IniUtil::ReadSingleConfigInfo(strSection, strKey);

		}

		//g_sorFileArray.reportConfig.Configuration.Index = IniUtil::ReadResString(_T("IDS_FRM_WAVE_DLG_REFRACTION"));  //获取折射率
	}

	//获取注释信息
	strKey = _T("Comment");
	g_sorFileArray.reportConfig.Comment = IniUtil::ReadSingleConfigInfo(strSection, strKey);

	////初始化配置属性页
	//strKey = _T("OriginLocation");
	//m_propConfig.m_strStartLoc = IniUtil::ReadSingleConfigInfo(strSection, strKey);
	//strKey = _T("OriginCable");
	//m_propConfig.m_strStartCable = IniUtil::ReadSingleConfigInfo(strSection, strKey);
	//strKey = _T("OriginFiber");
	//m_propConfig.m_strStartFiber = IniUtil::ReadSingleConfigInfo(strSection, strKey);
	//strKey = _T("OriginColor");
	//m_propConfig.m_strStartColor = IniUtil::ReadSingleConfigInfo(strSection, strKey);
	//strKey = _T("EndLocation");
	//m_propConfig.m_strEndLoc = IniUtil::ReadSingleConfigInfo(strSection, strKey);
	//strKey = _T("EndCable");
	//m_propConfig.m_strEndCable = IniUtil::ReadSingleConfigInfo(strSection, strKey);
	//strKey = _T("EndFiber");
	//m_propConfig.m_strEndFiber = IniUtil::ReadSingleConfigInfo(strSection, strKey);
	//strKey = _T("EndColor");
	//m_propConfig.m_strEndColor = IniUtil::ReadSingleConfigInfo(strSection, strKey);
	//if(g_sorFileArray.totalCurveNumber > 0)
	//{
	//	m_propConfig.m_strWavelength.Format(_T("%d"), g_sorFileArray.sorFileList[g_sorFileArray.curSelection].pTestResultInfo->WaveLength);  //波长
	//	m_propConfig.m_strPulse.Format(_T("%d"), g_sorFileArray.sorFileList[g_sorFileArray.curSelection].pTestResultInfo->PulseWidth); //脉宽
	//	m_propConfig.m_strRange.Format(_T("%d"), g_sorFileArray.sorFileList[g_sorFileArray.curSelection].pTestResultInfo->Range); //脉宽
	//	m_propConfig.m_strLossThreshold.Format(_T("%.3f"), g_sorFileArray.sorFileList[g_sorFileArray.curSelection].pTestResultInfo->LossThreshold); //衰减门限
	//	m_propConfig.m_strRefThreshold.Format(_T("%.3f"), g_sorFileArray.sorFileList[g_sorFileArray.curSelection].pTestResultInfo->ReflectanceThreshold); //反射门限
	//	m_propConfig.m_strEndThreshold.Format(_T("%.3f"), g_sorFileArray.sorFileList[g_sorFileArray.curSelection].pTestResultInfo->EndOfFiberThreshold); //光纤末端门限
	//	m_propConfig.m_strRBS.Format(_T("%.3f"), g_sorFileArray.sorFileList[g_sorFileArray.curSelection].pTestResultInfo->BackscatterCoefficient); //光纤末端门限

	//}

	//
	////初始化任务属性页
	//strKey = _T("Job");
	//m_propJob.m_strJobID = IniUtil::ReadSingleConfigInfo(strSection, strKey);
	//strKey = _T("Contractor");
	//m_propJob.m_strContractor = IniUtil::ReadSingleConfigInfo(strSection, strKey);
	//strKey = _T("Customer");
	//m_propJob.m_strCustomer = IniUtil::ReadSingleConfigInfo(strSection, strKey);
	//strKey = _T("Operator");
	//m_propJob.m_strOperator = IniUtil::ReadSingleConfigInfo(strSection, strKey);
	//if(g_sorFileArray.totalCurveNumber > 0)
	//{
	//	//(CSorFileBase*)m_lpVoid
	//	m_propJob.m_strFile = g_sorFileArray.sorFileList[g_sorFileArray.curSelection].filePathName;
	//	CTime tm(g_sorFileArray.sorFileList[g_sorFileArray.curSelection].pTestResultInfo->MeasureTime);
	//	m_propJob.m_strTestDate = tm.Format(_T("%Y-%m-%d %H:%M:%S"));
	//}

}