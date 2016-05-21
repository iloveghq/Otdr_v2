#ifndef CONST_HEAD_FILE
#define CONST_HEAD_FILE
#endif

#ifdef CONST_HEAD_FILE
//开始定义///////////////////////////////////////////////////////////////////////////////
#include "mainfrm.h"
#include "OTDR.h"
extern COTDRApp theApp;
// 定义常标 //////////////////////////////////////////////////////////////////////

#define CONST_INITEVERYBODY // 初始化时加入固定数据到list控件
#define CONST_ID_LENGTH 5 //id号最大长度
#define LPMAIN ((CMainFrame *)::AfxGetApp ()->m_pMainWnd) //获得main指针
#define LISTBOX_MAX 1024 //列表框的最大行数
#define CONST_TEST //定义测试标志
#define LEFT_TOOLBAR WM_USER + 100	//左边工具栏事件消息







// 常函数 //////////////////////////////////////////////////////////////////////

//功能:显示警告消息以及一些日志
#define CONST_WARNINGLOG(str,type) \
{	\
	(LPMAIN->m_list[1].AddString ((CString)str)); \
	(LPMAIN->m_list[1].SendMessage(WM_VSCROLL,SB_BOTTOM,0)); \
	(((int)type == 1)?(NULL):(theApp.logcount++)); \
	(LPMAIN->WatchListFull(&LPMAIN->m_list[1])); \
}







// 控件ID //////////////////////////////////////////////////////////////////

#define ID_DOCKWND1 WM_USER + 150
#define ID_DOCKWND2 WM_USER + 151
#define ID_TABCTRL1 WM_USER + 152
#define ID_TABCTRL2 WM_USER + 153
#define ID_LISTBOX1 WM_USER + 154
#define ID_LISTBOX2 WM_USER + 155
#define ID_LISTBOX3 WM_USER + 156



//结束///////////////////////////////////////////////////////////////////////////////////
#endif