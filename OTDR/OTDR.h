// OTDR.h : main header file for the OTDR application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// COTDRApp:
// See OTDR.cpp for the implementation of this class
//

class COTDRApp : public CWinApp
{
public:
	int Doc_Index;
	COTDRApp();


// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
protected:
	HMENU m_hMDIMenu;
	HMENU m_hMenu;
	HACCEL m_hMDIAccel;
};

extern COTDRApp theApp;