// OTDR.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "OTDR.h"
#include "MainFrm.h"

#include "ChildFrm.h"
#include "OTDRDoc.h"
#include "OTDRView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// COTDRApp

BEGIN_MESSAGE_MAP(COTDRApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &COTDRApp::OnAppAbout)
	// Standard file based document commands
	//ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
	//ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PAGE_SETUP, &CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()


// COTDRApp construction

COTDRApp::COTDRApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only COTDRApp object

COTDRApp theApp;


// COTDRApp initialization

BOOL COTDRApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));
	LoadStdProfileSettings(4);  // Load standard INI file options (including MRU)

	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_OTDRTYPE))//LoadFrame(IDR_MAINFRAME))改为LoadFrame(IDR_OTDRTYPE)以改变初始"默认菜单"为"自定义菜单"
	{
		delete pMainFrame;
		return FALSE;
	}

	m_pMainWnd = pMainFrame;
	//m_pMainWnd->SetWindowText(_T("OTDR Viewer"));
	m_pMainWnd->SetWindowText(_T(""));
	// call DragAcceptFiles only if there's a suffix
	//  In an MDI app, this should occur immediately after setting m_pMainWnd

	pMainFrame->ShowWindow(SW_SHOWMAXIMIZED);//修改后,主窗口最大化显示wcq20110810
	pMainFrame->UpdateWindow();

	return TRUE;
}



// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// App command to run the dialog
void COTDRApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


// COTDRApp message handlers

