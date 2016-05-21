// PrintMargin.cpp : 实现文件
//

#include "stdafx.h"
#include "OTDR.h"
#include "PrintMargin.h"

extern SorFileArray g_sorFileArray;
// CPrintMargin 对话框

IMPLEMENT_DYNAMIC(CPrintMargin, CPropertyPage)

CPrintMargin::CPrintMargin()
	: CPropertyPage(CPrintMargin::IDD)
{

}

CPrintMargin::~CPrintMargin()
{
}

void CPrintMargin::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SPIN_LEFT, m_spinLeft);
	DDX_Control(pDX, IDC_SPIN_RIGHT, m_spinRight);
	DDX_Control(pDX, IDC_SPIN_TOP, m_spinTop);
	DDX_Control(pDX, IDC_SPIN_BOTTOM, m_spinBottom);
}


BEGIN_MESSAGE_MAP(CPrintMargin, CPropertyPage)
END_MESSAGE_MAP()


// CPrintMargin 消息处理程序

BOOL CPrintMargin::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	/*               页边                                                  */
	CString strMargin = IniUtil::ReadResString(_T("IDS_PRINT_MARGIN"));
	GetDlgItem(IDC_STATIC_PAGEMARGIN)->SetWindowText(strMargin);
	//左边
	CString strLeft = IniUtil::ReadResString(_T("IDS_PRINT_LEFT"));
	GetDlgItem(IDC_STATIC_LEFT)->SetWindowText(strLeft);
	float fLeft = _wtof(g_sorFileArray.printConfig.Left);
	m_spinLeft.SetSpin(1,0.0,20.0,0.1);
	m_spinLeft.SetPos(fLeft);
	//右边
	CString strRight = IniUtil::ReadResString(_T("IDS_PRINT_RIGHT"));
	GetDlgItem(IDC_STATIC_RIGHT)->SetWindowText(strRight);
	float fRight = _wtof(g_sorFileArray.printConfig.Right);
	m_spinRight.SetSpin(1,0.0,20.0,0.1);
	m_spinRight.SetPos(fRight);
	//顶部
	CString strTop= IniUtil::ReadResString(_T("IDS_PRINT_TOP"));
	GetDlgItem(IDC_STATIC_TOP)->SetWindowText(strTop);
	float fTop = _wtof(g_sorFileArray.printConfig.Top);
	m_spinTop.SetSpin(1,0.0,20.0,0.1);
	m_spinTop.SetPos(fTop);
	//底部
	CString strBottom = IniUtil::ReadResString(_T("IDS_PRINT_BOTTOM"));
	GetDlgItem(IDC_STATIC_BOTTOM)->SetWindowText(strBottom);
	float fBottom = _wtof(g_sorFileArray.printConfig.Bottom);
	m_spinBottom.SetSpin(1,0.0,20.0,0.1);
	m_spinBottom.SetPos(fBottom);
	//单位
	CString strUnit=IniUtil::ReadResString(_T("IDS_PRINT_UNIT"));
	GetDlgItem(IDC_STATIC_UNIT)->SetWindowText(strUnit);
	GetDlgItem(IDC_EDIT_UNIT)->SetWindowText(_T("mm"));

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

BOOL CPrintMargin::OnApply()
{
	// TODO: 在此添加专用代码和/或调用基类

	return CPropertyPage::OnApply();
}
