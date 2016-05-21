// OTDRView.cpp : implementation of the COTDRView class
//

#include "stdafx.h"
#include "OTDR.h"

#include "OTDRDoc.h"
#include "OTDRView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// COTDRView

IMPLEMENT_DYNCREATE(COTDRView, CView)

BEGIN_MESSAGE_MAP(COTDRView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_CREATE()
END_MESSAGE_MAP()

// COTDRView construction/destruction

COTDRView::COTDRView()
{
	// TODO: add construction code here
	m_viewID=-1;
}

COTDRView::~COTDRView()
{
}

BOOL COTDRView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// COTDRView drawing

void COTDRView::OnDraw(CDC* /*pDC*/)
{
	COTDRDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}

void COTDRView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	InitViewInfos();//初始化视图信息


}

void COTDRView::InitViewInfos()
{
	COTDRDoc* pDoc=GetDocument();

	switch(theApp.Doc_Index)
	{
	case 0:
		pDoc->SetTitle(_T("WaveForm"));
		break;
	case 1:
		pDoc->SetTitle(_T("EventList"));
		break;
	case 2:
		pDoc->SetTitle(_T("Thumbnail"));
		break;
	default: break;
	}
}


// COTDRView printing

BOOL COTDRView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void COTDRView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void COTDRView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// COTDRView diagnostics

#ifdef _DEBUG
void COTDRView::AssertValid() const
{
	CView::AssertValid();
}

void COTDRView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

COTDRDoc* COTDRView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(COTDRDoc)));
	return (COTDRDoc*)m_pDocument;
}
#endif //_DEBUG


// COTDRView message handlers

int COTDRView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here

	return 0;
}
