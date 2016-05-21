#pragma once

#include "resource.h"
#include "afxcmn.h"
#include "EndGabor.h"


// CEventlist form view

class CEventlist : public CFormView
{
	DECLARE_DYNCREATE(CEventlist)

protected:
	CEventlist();           // protected constructor used by dynamic creation
	virtual ~CEventlist();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate();

public:
	void resizeControl();
	void SetLocalString();
	void FreshEventList();
	//void GetEventList();
	TestResultInfo* GetEventList();

	enum { IDD = IDD_EVENTLIST };

#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:	
	DECLARE_MESSAGE_MAP()
	afx_msg void OnSize(UINT nType, int cx, int cy);

private:
	void SetListViewText();
	void SetListColName(int nColIndex, const CString& colName);

public:
	CListCtrl m_Eventlist;
	CImageList* m_pImageList;
	afx_msg void OnLvnItemchangedListEvent(NMHDR *pNMHDR, LRESULT *pResult);
public:
	CEndGabor m_endgabor;//zll 2013.8.19
};


