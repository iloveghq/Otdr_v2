// PropStep5.cpp : 实现文件
//

#include "stdafx.h"
#include "OTDR.h"
#include "PropStep5.h"
#include "MainFrm.h"

// CPropStep5 对话框

IMPLEMENT_DYNAMIC(CPropStep5, CPropertyPage)

CPropStep5::CPropStep5()
	: CPropertyPage(CPropStep5::IDD)
{

}

CPropStep5::~CPropStep5()
{
}

void CPropStep5::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPropStep5, CPropertyPage)
END_MESSAGE_MAP()


// CPropStep5 消息处理程序
BOOL CPropStep5::OnSetActive() 
{
	// TODO: Add your specialized code here and/or call the base class
	//((CPropertySheet*)GetParent())->SetWizardButtons(PSWIZB_NEXT);
	return CPropertyPage::OnSetActive();
}

LRESULT CPropStep5::OnWizardNext() 
{
	// TODO: Add your specialized code here and/or call the base class
	UpdateData();

	return CPropertyPage::OnWizardNext();
}

BOOL CPropStep5::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}