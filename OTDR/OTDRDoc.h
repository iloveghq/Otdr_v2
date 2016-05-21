// OTDRDoc.h : interface of the COTDRDoc class
//


#pragma once


class COTDRDoc : public CDocument
{
protected: // create from serialization only
	COTDRDoc();
	DECLARE_DYNCREATE(COTDRDoc)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// Implementation
public:
	virtual ~COTDRDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};


