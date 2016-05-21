// PropStep2A.cpp : 实现文件
//

#include "stdafx.h"
#include "OTDR.h"
#include "PropStep2A.h"


// CPropStep2A 对话框

IMPLEMENT_DYNAMIC(CPropStep2A, CPropertyPage)

CPropStep2A::CPropStep2A()
	: CPropertyPage(CPropStep2A::IDD)
{
	
}

CPropStep2A::~CPropStep2A()
{
}

void CPropStep2A::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPropStep2A, CPropertyPage)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDOK, &CPropStep2A::OnBnClickedOk)
END_MESSAGE_MAP()


// CPropStep2A 消息处理程序

BOOL CPropStep2A::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_prop.AddPage(&m_propContent);
	m_prop.AddPage(&m_propConfig);
	/*m_prop.m_psh.dwFlags &= ~PSH_HASHELP;	
	m_prop.m_psh.dwFlags |= PSH_NOAPPLYNOW;
	m_propContent.m_psp.dwFlags &=  ~PSH_HASHELP;
	m_propConfig.m_psp.dwFlags &=  ~PSH_HASHELP;*/
	
	//修改尺寸
	CRect rc;
	GetWindowRect(rc);
	m_prop.Create(this, WS_CHILD | WS_VISIBLE, WS_EX_STATICEDGE);

	m_prop.SetWindowPos(NULL, 0, 0, rc.Width() * 11 / 12, rc.Height() * 11 / 12, SWP_SHOWWINDOW);
	//m_prop.DoModal();
	
	//修改属性页tab的文字
	TC_ITEM item;
	item.mask = TCIF_TEXT;
	item.pszText = _T("内容");
	m_prop.GetTabControl()->SetItem(0, &item);
	item.pszText = _T("配置");
	m_prop.GetTabControl()->SetItem(1, &item);

	//m_prop
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

//调整窗口位置
void CPropStep2A::AdjustWin()
{
	MoveWindow(m_rcLocation);
}

HBRUSH CPropStep2A::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何属性

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}

void CPropStep2A::OnBnClickedOk()
{
	OnOK();
}
