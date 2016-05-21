#pragma once
#include "atltypes.h"
#include "ReportFileSelect.h"
#include "PropStep4.h"
// PrintView.h : header file
//
#define MARGIN_LEFT_COUNT 18  //左右留空的宽度占总宽度的几分之几lzy2014.8.30

/////////////////////////////////////////////////////////////////////////////
// CPrintView view

const float FONTSIZE_RATIO[5] = {1.0, 0.8, 0.6, 0.45, 0.4};  //报表输出时用的字体的缩放比例，[0]表示单报表，[1]表示2图/页，[2]表示4图/页，[3]表示6图/页，[4]表示8图每页


class CPrintView : public CView
{
public:
	CPrintView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CPrintView)

// Attributes
public:
	BOOL	m_bPrint;
	BOOL	m_bPrintCurve;
	CRect m_rectDraw;//绘制矩形区域大小
// Operations
public:
	void OnMyPrint();
	void OnMyPrintPreview();
	void PrintPageHeader(CDC* pDC, UINT nPageNumber, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);  
	virtual void OnEndPrintPreview(CDC* pDC, CPrintInfo* pInfo, POINT point, CPreviewView* pView);
	void DrawGridLine(CDC* pDC, CRect& rect);//绘制栅格 wcq2012.10.31
	void DrawCursorAB(CDC* pDC, CRect& rect);//绘制光标 xjf2014.3.3
	CString right_MAX(float m_float_zll,float flo_Length);//计算横坐标右边最大长度函数
	CString branch(float m_float_zll,float flo_Length);//计算横坐标单位长度函数
	void GetPrintInfo(int nCurSel); //获取打印需要的参数lzy 2014.8.12
	void DrawCurCurveNew(CDC* pDC, CRect rectDraw,int nCurSel = 0, BOOL bSingle = TRUE); //当前选中波形绘图lzy2014.8.11
	void DrawScale(CDC *pDC, CRect &rect, float fYmax = 0, float fXmax = 0);//绘制刻度lzy2014.7.17
	void DrawCurve(CDC* pDC, CRect& rect, int nCurveIndex);	
	void DrawTrace(CDC *pDC, CRect rc, int nCurSel, float fontRatio = 1.0); //绘制带坐标曲线，带有AB标线 lzy2014.8.26	
	void DrawTraceBmp(CDC *pDC, CRect rc, int nCurSel, float fontRatio); //绘制带坐标曲线，不带有AB标线，导出excel专用 lzy2014.9.3

private:
	void DrawReport(CDC* pDC);
	void Draw1Curve(CDC* pDC,int m_nCurPage); //每页1个波形
	void Draw2Curve(CDC* pDC,int m_nCurPage); //每页2个波形
	void Draw4Curve(CDC* pDC,int m_nCurPage); //每页4个波形
	void Draw6Curve(CDC* pDC,int m_nCurPage); //每页6个波形
	void Draw8Curve(CDC* pDC,int m_nCurPage); //每页8个波形
	void Draw1CurveNew(CDC* pDC,int m_nCurPage); //每页1个波形lzy2014.8.28
	void Draw2CurveNew(CDC* pDC,int m_nCurPage); //每页2个波形lzy2014.8.28
	void Draw4CurveNew(CDC* pDC,int m_nCurPage); //每页4个波形lzy2014.8.28
	void Draw6CurveNew(CDC* pDC,int m_nCurPage); //每页6个波形lzy2014.8.28
	void Draw8CurveNew(CDC* pDC,int m_nCurPage); //每页8个波形lzy2014.8.28
	void DrawCurCurve(CDC* pDC); //当前选中波形
	
	void InnerDraw(CDC* pDC,int m_nCurPage); //内部绘图只用
	void DrawReportItem(CDC* pDC, CRect& rect, int nItemIndex); //绘制报表项目
	void DrawCurveItem(CDC* pDC, CRect& rect, int nItemIndex,int m_CurveNum,CString m_sorName); //绘制每条曲线项目
	void DrawCurveByReport(CDC* pDC, CRect& rect, int nCurveIndex);//zll 2013.6.4	
	
	CString GetSorName(int m_CurveNum);// zll 2013.6.5 设置.sor曲线文件名 
	
	void DrawHorCoordUnit(CDC* pDC, CRect& rect, CString strText);//绘制横坐标单位文本
	void DrawVerCoordUnit(CDC* pDC, CRect& rect); //绘制纵坐标文本

	void DrawEventHead(CDC* pDC, int nLeft, int nTop, CRect rectDraw);//绘制事件列表表头lzy2014.8.28
	void DrawEventBody(CDC* pDC, int nLeft, int nTop, int nVspace, CRect rectDraw, int nCurSel);//绘制事件列表lzy2014.8.28
	void DrawEventBody1(CDC* pDC, int nLeft, int nTop,int m_nCurPage);//绘制事件列表
	void DrawCurveParam(CDC* pDC, int nLeft, int nTop); //绘制曲线参数
	void DrawReportContent(CDC* pDC, int nLeft, int nTop); //绘制报表内容
	
	void InitFont(CDC* pDC); //初始化字体 lzy2014.9.3
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPrintView)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPrintView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
public:
	//{{AFX_MSG(CPrintView)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	// Print 
	BOOL m_bPrintData;
	BOOL m_bPrintImage;
	BOOL m_bPrintHead;	
	UINT m_cxWidth;
	UINT m_cxOffset;
	UINT m_nLinesPerPage;
	UINT m_PageCount;
	UINT m_cyPrinter;
	CFont m_ListFont;
	CFont m_fontPrinter;
	CFont m_dataFont;
	CFont m_FontVR;
	CFont m_fontGroup;   //组标题的字体 lzy 2014.8.11

	//lzy 2014.8.12
	BOOL m_bPrintLogo; //是否打印光维版权信息
	BOOL m_bPrintJob;  //是否打印Job info
	BOOL m_bPrintConfig;
	BOOL m_bPrintComment;
	BOOL m_bPrintTrace;
	BOOL m_bPrintEvent;
	
};