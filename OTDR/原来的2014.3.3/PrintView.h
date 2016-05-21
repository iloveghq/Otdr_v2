#pragma once
#include "atltypes.h"
#include "ReportFileSelect.h"
#include "PropStep4.h"
// PrintView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPrintView view

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
	CString right_MAX(float m_float_zll,float flo_Length);//计算横坐标右边最大长度函数
	CString branch(float m_float_zll,float flo_Length);//计算横坐标单位长度函数

private:
	void DrawReport(CDC* pDC);
	void Draw1Curve(CDC* pDC,int m_nCurPage); //每页1个波形
	void Draw2Curve(CDC* pDC,int m_nCurPage); //每页2个波形
	void Draw4Curve(CDC* pDC,int m_nCurPage); //每页4个波形
	void Draw6Curve(CDC* pDC,int m_nCurPage); //每页6个波形
	void Draw8Curve(CDC* pDC,int m_nCurPage); //每页8个波形
	void DrawCurCurve(CDC* pDC); //当前选中波形
	void InnerDraw(CDC* pDC,int m_nCurPage); //内部绘图只用
	void DrawReportItem(CDC* pDC, CRect& rect, int nItemIndex); //绘制报表项目
	void DrawCurveItem(CDC* pDC, CRect& rect, int nItemIndex,int m_CurveNum,CString m_sorName); //绘制每条曲线项目
	void DrawCurve(CDC* pDC, CRect& rect, int nCurveIndex);
	void DrawCurveByReport(CDC* pDC, CRect& rect, int nCurveIndex);//zll 2013.6.4
	CString GetSorName(int m_CurveNum);// zll 2013.6.5 设置.sor曲线文件名 
	
	void DrawHorCoordUnit(CDC* pDC, CRect& rect, CString strText);//绘制横坐标单位文本
	void DrawVerCoordUnit(CDC* pDC, CRect& rect); //绘制纵坐标文本

	void DrawEventHead(CDC* pDC, int nLeft, int nTop);//绘制事件列表表头
	void DrawEventBody(CDC* pDC, int nLeft, int nTop);//绘制事件列表
	void DrawEventBody1(CDC* pDC, int nLeft, int nTop,int m_nCurPage);//绘制事件列表
	void DrawCurveParam(CDC* pDC, int nLeft, int nTop); //绘制曲线参数
	void DrawReportContent(CDC* pDC, int nLeft, int nTop); //绘制报表内容
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
};