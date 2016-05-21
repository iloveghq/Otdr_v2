// MainFrm.h : interface of the CMainFrame class
//
#pragma once
#include "OTDRView.h"
#include "MyToolBarEx.h"
#include "Waveform.h"
#include "Eventlist.h"
#include "Thumbnail.h"
#include "Report.h"
#include "MainSetting.h"
#include "PrintView.h"
#include "ChildFrm.h"
#include "TrueColorToolBar.h"
#include "SorFile.h"
#include "Waveform.h"
#include "RemoteConnDlg.h"
#include "RemotePageSheet.h"
#include "SorFileInfo.h"
#include "SorAssist.h"
#include "SORDATA.h"
#include "SorDrawStatic.h"
#include "OtdrTrace.h"//zll2013.4.23
#include "MySmooth.h"
#include "WaveFormLeftDlg.h"
#include "PropStep2A.h"
#include "MyPropertySheet.h"

class CMainFrame : public CMDIFrameWnd
{
	DECLARE_DYNAMIC(CMainFrame)

public:
	CMainFrame();
	virtual ~CMainFrame();
	void SetLocaleString();
	void PrintPageHeader(CDC* pDC, UINT nPageNumber, CPrintInfo* pInfo);
	void ShowMainSetting(BOOL bShow); //是否显示主设置窗口
	void ShowReport(BOOL bShow); //是否显示报表
	void DecodeNetData(CString data); //收到上位机通过网络传过来的配置信息后，更新对应的配置信息 wcq2012.05.18
	CString GetNetData(CString setType);//上位机要求同步本地数据,把本地的一些配置信息按格式组合一下 wcq2012.05.18

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);


	// Generated message map functions
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	//afx_msg void OnUsbLink();
	afx_msg void OnClose();

	//File
	afx_msg void OnOpen();
	afx_msg void OnUpdateMenu(CCmdUI *pCmdUI);
	afx_msg void OnSave();
	afx_msg void OnUpdateSave(CCmdUI *pCmdUI);
	afx_msg void OnPageSetup();
	afx_msg void OnPrint();
	afx_msg void OnUpdatePrint(CCmdUI *pCmdUI);
	afx_msg void OnPrintPreview();
	afx_msg	void OnUpdatePrintPreview(CCmdUI *pCmdUI);

	//Tool
	afx_msg void OnRemoteConnect();
	afx_msg void OnRemoteSet();
	afx_msg void OnRemoteControl();
	afx_msg void OnSwitchChs();
	afx_msg void OnSwitchEng();
	afx_msg void OnSwitchPort();  //葡萄牙语切换

	//Event Analy Con
	afx_msg void OnEventEditMode();
	afx_msg void OnUpdateEventEditMode(CCmdUI *pCmdUI);
	afx_msg void OnEventAutoSearch();
	afx_msg void OnUpdateEventAutoSearch(CCmdUI *pCmdUI);
	afx_msg void OnEventAnalCon();
	afx_msg void OnUpdateEventAnalCon(CCmdUI *pCmdUI);
	//View
	afx_msg void OnViewWave();
	afx_msg void OnViewThumbnail();
	afx_msg void OnViewEvent();
	afx_msg void OnSceenSet();
	afx_msg void OnOriginSetting();

	//Report Wizard
	afx_msg void OnReportWizard();
	afx_msg void OnSize(UINT nType, int cx, int cy);

	//Help
	afx_msg void OnHelpContent();

	//Print
	LRESULT OnBeginPrinting(WPARAM wParam,LPARAM lParam);
	LRESULT OnEndPrinting(WPARAM wParam,LPARAM lParam);
	LRESULT OnMyPrint(WPARAM wParam,LPARAM lParam);


private:
	void CreateRemoteConnDlg(); //创建网络连接窗口
	void CreateWaveFormFrame();//创建WaveForm窗口
	void CreateEventListFrame();//创建EventList窗口
	void CreateThumbnailFrame();//创建Thumbnail窗口
	void CreateReportFrame(); //创建报表窗口
	void CreateMainSettingFrame(); //创建主设置窗口
	BOOL CreateToolBar();//创建工具栏
	BOOL CreateStatusBar();//创建状态栏
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	void TileViewFrame();
	void GetMDIClientSIZE(SIZE *fclient);
	COTDRView* GetSpecifyFrameView(LPCTSTR title,int viewID = -1,int type = 0);
	void OnWaveform();
	void OnEventlist();
	void InitWaveConfig();
	void InitReportConfig();    //初始化中文报表文字
	void InitReportConfig_EN();  //初始化英语报表文字
	void InitReportConfig_PT();  //初始化葡萄牙语报表文字
	void InitRepLayListTitle();//初始化step2中文信息
	void InitRepLayListTitle_EN();//初始化step2英文信息
	void InitRepLayListTitle_PT();//初始化step2葡文信息lzy2014.6.17
	void InitOTDRConfig();
	void InitPrintConfig();
	void InitEventAnalConfig();

	void RecordConfig();
	void RecordConfig_EN();	
	void RecordConfig_PT();//葡语配置文件保存lzy2014.6.15 	
	void LoadOneCurve(CString strFilePath,  //文件的全路径 
					  CString strFileName,	 //文件名
					  int index);//当前的索引号， 本函数加载一条曲线， lzy 2015.2.10

protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);

public:
	CWaveform *m_pWaveFormView;//波形视图
	CEventlist *m_pEventFormView;//事件列表视图
	CThumbnail *m_pThumbFormView;//缩略图
	CReport *m_pReportFormView;//报表视图
	CMainSetting *m_pMainSetting;//主设置窗口视图
	CPrintView *m_pPrintView; //打印机视图

	CChildFrame *m_cfWaveForm;
	BOOL m_bShowWaveForm;
	CChildFrame *m_cfReport;
	CChildFrame *m_cfEventList;
	BOOL m_bShowEventList;
	CChildFrame *m_cfThumbnail;
	BOOL m_bShowThumbnail;
	CChildFrame *m_cfMainSetting;
	BOOL m_bMainSetting;

	CStatusBar m_wndStatusBar;
	CMyToolBarEx m_wndToolBar;

	BOOL m_bTilelayout;
	CMenu m_menuChs; //中文菜单
	CMenu m_menuEng; //英文
	CMenu m_menuPort; //葡萄牙文菜单

	//子Frame位置
	CRect m_frameRect;
	CRect m_reportRect;
	CRect m_eventlistRect; //事件列表
	CRect m_thumbnailRect; //缩略图
	CRect m_mainSettingRect;//主设置窗口
	CRect m_formviewRect; //绘图区
	//远程连接到OTDR
	CRemoteConnDlg* m_pRemoteConnDlg;
	CRemotePageSheet* m_pRemotePageSheet;

	BOOL m_bFileMenuEnable;
	BOOL m_bReportMode; //是否为报表模式

	SORDATA m_sorDATA;
	CSorAssist m_sorAssist;
	CSorDrawStatic m_sorDrawStatic;

	CGrandwayReg  m_reg;//注册表操作类
	CUtil util;
public:
    INT printType;
	INT printFrame;//zll 2012.10.29 未完成
public:
	CMySmooth m_smooth;
	WORD* pDataSmooth;
	COLORREF colorLine;
	CWaveFormLeftDlg m_leftDlg;
	afx_msg void OnSetting();
};