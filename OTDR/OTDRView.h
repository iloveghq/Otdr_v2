// OTDRView.h : interface of the COTDRView class
//


#pragma once
#include "OTDRDoc.h"


class COTDRView : public CView
{
protected: // create from serialization only
	COTDRView();
	DECLARE_DYNCREATE(COTDRView)

// Attributes
public:
	COTDRDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	int GetMyID();
	virtual ~COTDRView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
private:
	int m_viewID;
	void InitViewInfos();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};

#ifndef _DEBUG  // debug version in OTDRView.cpp
inline COTDRDoc* COTDRView::GetDocument() const
   { return reinterpret_cast<COTDRDoc*>(m_pDocument); }
#endif

