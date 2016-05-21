// Prop2.cpp : implementation file
//

#include "stdafx.h"
#include "PropStep2.h"
#include "MainFrm.h"
#include "ItemSettingDlg.h"


extern SorFileArray g_sorFileArray;
extern LANG g_currentLang;

/////////////////////////////////////////////////////////////////////////////
// CPropStep2 property page

IMPLEMENT_DYNCREATE(CPropStep2, CPropertyPage)

CPropStep2::CPropStep2() : CPropertyPage(CPropStep2::IDD)
{
}

CPropStep2::~CPropStep2()
{
}

void CPropStep2::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_STEP2_ITEM1, m_combox1);
	DDX_Control(pDX, IDC_COMBO_STEP2_ITEM2, m_combox2);
	DDX_Control(pDX, IDC_COMBO_STEP2_ITEM3, m_combox3);
	DDX_Control(pDX, IDC_COMBO_STEP2_ITEM4, m_combox4);
	DDX_Control(pDX, IDC_COMBO_STEP2_ITEM5, m_combox5);
	DDX_Control(pDX, IDC_COMBO_STEP2_ITEM6, m_combox6);
	DDX_Control(pDX, IDC_COMBO_STEP2_ITEM7, m_combox7);
	DDX_Control(pDX, IDC_COMBO_STEP2_ITEM8, m_combox8);
	DDX_Control(pDX, IDC_COMBO_STEP2_ITEM9, m_combox9);
	DDX_Control(pDX, IDC_COMBO_STEP2_ITEM10, m_combox10);
	DDX_Control(pDX, IDC_COMBO_STEP2_ITEM11, m_combox11);
	DDX_Control(pDX, IDC_COMBO_STEP2_ITEM12, m_combox12);
}


BEGIN_MESSAGE_MAP(CPropStep2, CPropertyPage)

	ON_BN_CLICKED(IDC_BTN_STEP2_EDIT, &CPropStep2::OnBnClickedBtnStep2Edit)
	ON_BN_CLICKED(IDC_BTN_STEP2_COPY, &CPropStep2::OnBnClickedBtnStep2Copy)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropStep2 message handlers

BOOL CPropStep2::OnSetActive() 
{
	// TODO: Add your specialized code here and/or call the base class
	//((CPropertySheet*)GetParent())->SetWizardButtons(PSWIZB_NEXT|PSWIZB_BACK);
	((CPropSheet*)GetParent())->SetReportHeadBtn(2);
	//重绘图形区域
	MAIN->m_pReportFormView->m_ReportViewDlg.m_DrawButton.ReDrawRect(2);
	((CPropSheet*)GetParent())->EnableNextBtn(TRUE);
	((CPropSheet*)GetParent())->EnablePrevBtn(TRUE);
	return CPropertyPage::OnSetActive();
}

LRESULT CPropStep2::OnWizardNext() 
{
	// TODO: Add your specialized code here and/or call the base class
	UpdateData();
	RecordSetting();
	//return CPropertyPage::OnWizardNext();
	return 0;
}

void CPropStep2::RecordSetting() //保存配置信息
{
	GetDlgItem(IDC_EDIT_STEP2_TITLE)->GetWindowText(g_sorFileArray.reportConfig.Title);

	GetDlgItem(IDC_EDIT_STEP2_ITEM1)->GetWindowText(g_sorFileArray.reportConfig.Item1Val);
	m_combox1.GetWindowText(g_sorFileArray.reportConfig.Item1);

	GetDlgItem(IDC_EDIT_STEP2_ITEM2)->GetWindowText(g_sorFileArray.reportConfig.Item2Val);
	m_combox2.GetWindowText(g_sorFileArray.reportConfig.Item2);

	GetDlgItem(IDC_EDIT_STEP2_ITEM3)->GetWindowText(g_sorFileArray.reportConfig.Item3Val);
	m_combox3.GetWindowText(g_sorFileArray.reportConfig.Item3);

	GetDlgItem(IDC_EDIT_STEP2_ITEM4)->GetWindowText(g_sorFileArray.reportConfig.Item4Val);
	m_combox4.GetWindowText(g_sorFileArray.reportConfig.Item4);

	GetDlgItem(IDC_EDIT_STEP2_ITEM5)->GetWindowText(g_sorFileArray.reportConfig.Item5Val);
	m_combox5.GetWindowText(g_sorFileArray.reportConfig.Item5);

	GetDlgItem(IDC_EDIT_STEP2_ITEM6)->GetWindowText(g_sorFileArray.reportConfig.Item6Val);
	m_combox6.GetWindowText(g_sorFileArray.reportConfig.Item6);

	GetDlgItem(IDC_EDIT_STEP2_ITEM7)->GetWindowText(g_sorFileArray.reportConfig.Item7Val);
	m_combox7.GetWindowText(g_sorFileArray.reportConfig.Item7);

	GetDlgItem(IDC_EDIT_STEP2_ITEM8)->GetWindowText(g_sorFileArray.reportConfig.Item8Val);
	m_combox8.GetWindowText(g_sorFileArray.reportConfig.Item8);

	GetDlgItem(IDC_EDIT_STEP2_ITEM9)->GetWindowText(g_sorFileArray.reportConfig.Item9Val);
	m_combox9.GetWindowText(g_sorFileArray.reportConfig.Item9);

	GetDlgItem(IDC_EDIT_STEP2_ITEM10)->GetWindowText(g_sorFileArray.reportConfig.Item10Val);
	m_combox10.GetWindowText(g_sorFileArray.reportConfig.Item10);

	GetDlgItem(IDC_EDIT_STEP2_ITEM11)->GetWindowText(g_sorFileArray.reportConfig.Item11Val);
	m_combox11.GetWindowText(g_sorFileArray.reportConfig.Item11);

	GetDlgItem(IDC_EDIT_STEP2_ITEM12)->GetWindowText(g_sorFileArray.reportConfig.Item12Val);
	m_combox12.GetWindowText(g_sorFileArray.reportConfig.Item12);

	//以下将数据保存到config.ini文件lzy2014.7.14
	CString strSection;
	if(g_currentLang == LANG::CHS)   //中文
	{
		strSection = _T("RepLayListTitle");
	}
	else if(g_currentLang == LANG::ENG)  //ENGLISH
	{
		strSection = _T("RepLayListTitle_EN");
	}
	else if(g_currentLang == LANG::PORT)   //葡语
	{
		strSection = _T("RepLayListTitle_PT");
	}	
	IniUtil::WriteSingleConfigInfo(strSection, _T("Edit1"), g_sorFileArray.reportConfig.Item1Val);
	IniUtil::WriteSingleConfigInfo(strSection, _T("Edit2"), g_sorFileArray.reportConfig.Item2Val);
	IniUtil::WriteSingleConfigInfo(strSection, _T("Edit3"), g_sorFileArray.reportConfig.Item3Val);
	IniUtil::WriteSingleConfigInfo(strSection, _T("Edit4"), g_sorFileArray.reportConfig.Item4Val);
	IniUtil::WriteSingleConfigInfo(strSection, _T("Edit5"), g_sorFileArray.reportConfig.Item5Val);
	IniUtil::WriteSingleConfigInfo(strSection, _T("Edit6"), g_sorFileArray.reportConfig.Item6Val);
	IniUtil::WriteSingleConfigInfo(strSection, _T("Edit7"), g_sorFileArray.reportConfig.Item7Val);
	IniUtil::WriteSingleConfigInfo(strSection, _T("Edit8"), g_sorFileArray.reportConfig.Item8Val);
	IniUtil::WriteSingleConfigInfo(strSection, _T("Edit9"), g_sorFileArray.reportConfig.Item9Val);
	IniUtil::WriteSingleConfigInfo(strSection, _T("Edit10"), g_sorFileArray.reportConfig.Item10Val);
	IniUtil::WriteSingleConfigInfo(strSection, _T("Edit11"), g_sorFileArray.reportConfig.Item11Val);
	IniUtil::WriteSingleConfigInfo(strSection, _T("Edit12"), g_sorFileArray.reportConfig.Item12Val);
}

BOOL CPropStep2::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	//设置本地化
	SetLocalString();
	//标题
	//InitCombox();

	/*GetDlgItem(IDC_EDIT_STEP2_ITEM1)->SetWindowText(g_sorFileArray.reportConfig.Item1Val);
	GetDlgItem(IDC_EDIT_STEP2_ITEM2)->SetWindowText(g_sorFileArray.reportConfig.Item2Val);
	GetDlgItem(IDC_EDIT_STEP2_ITEM3)->SetWindowText(g_sorFileArray.reportConfig.Item3Val);
	GetDlgItem(IDC_EDIT_STEP2_ITEM4)->SetWindowText(g_sorFileArray.reportConfig.Item4Val);
	GetDlgItem(IDC_EDIT_STEP2_ITEM5)->SetWindowText(g_sorFileArray.reportConfig.Item5Val);
	GetDlgItem(IDC_EDIT_STEP2_ITEM6)->SetWindowText(g_sorFileArray.reportConfig.Item6Val);
	GetDlgItem(IDC_EDIT_STEP2_ITEM7)->SetWindowText(g_sorFileArray.reportConfig.Item7Val);
	GetDlgItem(IDC_EDIT_STEP2_ITEM8)->SetWindowText(g_sorFileArray.reportConfig.Item8Val);
	GetDlgItem(IDC_EDIT_STEP2_ITEM9)->SetWindowText(g_sorFileArray.reportConfig.Item9Val);
	GetDlgItem(IDC_EDIT_STEP2_ITEM10)->SetWindowText(g_sorFileArray.reportConfig.Item10Val);
	GetDlgItem(IDC_EDIT_STEP2_ITEM11)->SetWindowText(g_sorFileArray.reportConfig.Item11Val);
	GetDlgItem(IDC_EDIT_STEP2_ITEM12)->SetWindowText(g_sorFileArray.reportConfig.Item12Val);*/
	InitEdit();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CPropStep2::SetLocalString()
{
	/*                          标题                                          */
	CString strTitle = IniUtil::ReadResString(_T("IDS_REPORT_STEP2_TITLE"));
	GetDlgItem(IDC_STATIC_STEP2_TITLE)->SetWindowText(strTitle);
	//项目
	CString strItem = IniUtil::ReadResString(_T("IDS_REPORT_STEP2_ITEM"));
	GetDlgItem(IDC_STATIC_STEP2_ITEM)->SetWindowText(strItem);
	//从总表拷贝
	CString strCopy = IniUtil::ReadResString(_T("IDS_REPORT_STEP2_COPYSUMMARY"));
	GetDlgItem(IDC_BTN_STEP2_COPY)->SetWindowText(strCopy);
	GetDlgItem(IDC_BTN_STEP2_COPY)->EnableWindow(FALSE);
	//编辑项
	CString strEditItem = IniUtil::ReadResString(_T("IDS_REPROT_STEP2_EDIT_ITEM"));
	GetDlgItem(IDC_BTN_STEP2_EDIT)->SetWindowText(strEditItem);
	//标题
	GetDlgItem(IDC_EDIT_STEP2_TITLE)->SetWindowText(g_sorFileArray.reportConfig.Title);

	//标题
	InitCombox();

	//初始化EDIT控件内容lzy2014.7.14
	InitEdit();
}

void CPropStep2::InitCombox()
{
	InitCombox(m_combox1, g_sorFileArray.reportConfig.Item1);
	InitCombox(m_combox2, g_sorFileArray.reportConfig.Item2);
	InitCombox(m_combox3, g_sorFileArray.reportConfig.Item3);
	InitCombox(m_combox4, g_sorFileArray.reportConfig.Item4);
	InitCombox(m_combox5, g_sorFileArray.reportConfig.Item5);
	InitCombox(m_combox6, g_sorFileArray.reportConfig.Item6);
	InitCombox(m_combox7, g_sorFileArray.reportConfig.Item7);
	InitCombox(m_combox8, g_sorFileArray.reportConfig.Item8);
	InitCombox(m_combox9, g_sorFileArray.reportConfig.Item9);
	InitCombox(m_combox10, g_sorFileArray.reportConfig.Item10);
	InitCombox(m_combox11, g_sorFileArray.reportConfig.Item11);
	InitCombox(m_combox12, g_sorFileArray.reportConfig.Item12);
}

void CPropStep2::InitCombox(CComboBox& combox, CString strText)
{
	if (NULL == combox)
		return;

	combox.ResetContent();
	if (!g_sorFileArray.reportConfig.Combox1.IsEmpty())
		combox.AddString(g_sorFileArray.reportConfig.Combox1);
	if (!g_sorFileArray.reportConfig.Combox2.IsEmpty())
		combox.AddString(g_sorFileArray.reportConfig.Combox2);
	if (!g_sorFileArray.reportConfig.Combox3.IsEmpty())
		combox.AddString(g_sorFileArray.reportConfig.Combox3);
	if (!g_sorFileArray.reportConfig.Combox4.IsEmpty())
		combox.AddString(g_sorFileArray.reportConfig.Combox4);
	if (!g_sorFileArray.reportConfig.Combox5.IsEmpty())
		combox.AddString(g_sorFileArray.reportConfig.Combox5);
	if (!g_sorFileArray.reportConfig.Combox6.IsEmpty())
		combox.AddString(g_sorFileArray.reportConfig.Combox6);
	if (!g_sorFileArray.reportConfig.Combox7.IsEmpty())
		combox.AddString(g_sorFileArray.reportConfig.Combox7);
	if (!g_sorFileArray.reportConfig.Combox8.IsEmpty())
		combox.AddString(g_sorFileArray.reportConfig.Combox8);
	if (!g_sorFileArray.reportConfig.Combox9.IsEmpty())
		combox.AddString(g_sorFileArray.reportConfig.Combox9);
	if (!g_sorFileArray.reportConfig.Combox10.IsEmpty())
		combox.AddString(g_sorFileArray.reportConfig.Combox10);
	if (!g_sorFileArray.reportConfig.Combox11.IsEmpty())
		combox.AddString(g_sorFileArray.reportConfig.Combox11);
	if (!g_sorFileArray.reportConfig.Combox12.IsEmpty())
		combox.AddString(g_sorFileArray.reportConfig.Combox12);
	if (!g_sorFileArray.reportConfig.Combox13.IsEmpty())
		combox.AddString(g_sorFileArray.reportConfig.Combox13);
	if (!g_sorFileArray.reportConfig.Combox14.IsEmpty())
		combox.AddString(g_sorFileArray.reportConfig.Combox14);
	if (!g_sorFileArray.reportConfig.Combox15.IsEmpty())
		combox.AddString(g_sorFileArray.reportConfig.Combox15);
	int nPos = combox.FindString(0, strText);
	if (nPos != LB_ERR)
	{
		combox.SetCurSel(nPos);
	}
}

//初始化所有EDIT的值lzy2014.7.14
void CPropStep2::InitEdit()
{
	GetDlgItem(IDC_EDIT_STEP2_ITEM1)->SetWindowText(g_sorFileArray.reportConfig.Item1Val);
	GetDlgItem(IDC_EDIT_STEP2_ITEM2)->SetWindowText(g_sorFileArray.reportConfig.Item2Val);
	GetDlgItem(IDC_EDIT_STEP2_ITEM3)->SetWindowText(g_sorFileArray.reportConfig.Item3Val);
	GetDlgItem(IDC_EDIT_STEP2_ITEM4)->SetWindowText(g_sorFileArray.reportConfig.Item4Val);
	GetDlgItem(IDC_EDIT_STEP2_ITEM5)->SetWindowText(g_sorFileArray.reportConfig.Item5Val);
	GetDlgItem(IDC_EDIT_STEP2_ITEM6)->SetWindowText(g_sorFileArray.reportConfig.Item6Val);
	GetDlgItem(IDC_EDIT_STEP2_ITEM7)->SetWindowText(g_sorFileArray.reportConfig.Item7Val);
	GetDlgItem(IDC_EDIT_STEP2_ITEM8)->SetWindowText(g_sorFileArray.reportConfig.Item8Val);
	GetDlgItem(IDC_EDIT_STEP2_ITEM9)->SetWindowText(g_sorFileArray.reportConfig.Item9Val);
	GetDlgItem(IDC_EDIT_STEP2_ITEM10)->SetWindowText(g_sorFileArray.reportConfig.Item10Val);
	GetDlgItem(IDC_EDIT_STEP2_ITEM11)->SetWindowText(g_sorFileArray.reportConfig.Item11Val);
	GetDlgItem(IDC_EDIT_STEP2_ITEM12)->SetWindowText(g_sorFileArray.reportConfig.Item12Val);

}

void CPropStep2::OnBnClickedBtnStep2Edit()
{
	CItemSettingDlg dlg(this);
	INT_PTR nRes = dlg.DoModal();
	if (nRes == IDOK)
	{
		InitCombox();
	}
}

void CPropStep2::OnBnClickedBtnStep2Copy()
{
	// TODO: 在此添加控件通知处理程序代码
	InitCombox();
}


