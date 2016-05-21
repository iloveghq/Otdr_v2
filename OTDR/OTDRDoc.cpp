// OTDRDoc.cpp : implementation of the COTDRDoc class
//

#include "stdafx.h"
#include "OTDR.h"

#include "OTDRDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// COTDRDoc

IMPLEMENT_DYNCREATE(COTDRDoc, CDocument)

BEGIN_MESSAGE_MAP(COTDRDoc, CDocument)
END_MESSAGE_MAP()


// COTDRDoc construction/destruction

COTDRDoc::COTDRDoc()
{
	// TODO: add one-time construction code here

}

COTDRDoc::~COTDRDoc()
{
}

BOOL COTDRDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// COTDRDoc serialization

void COTDRDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}


// COTDRDoc diagnostics

#ifdef _DEBUG
void COTDRDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void COTDRDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// COTDRDoc commands
