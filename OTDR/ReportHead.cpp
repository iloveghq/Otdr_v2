// ReportHead.cpp : 实现文件
//

#include "stdafx.h"
#include "OTDR.h"
#include "ReportHead.h"

extern SorFileArray g_sorFileArray;

// CReportHead 对话框

IMPLEMENT_DYNAMIC(CReportHead, CDialog)

CReportHead::CReportHead(CWnd* pParent /*=NULL*/)
	: CDialog(CReportHead::IDD, pParent)
{

}

CReportHead::~CReportHead()
{
}

void CReportHead::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTN_BACK, m_btnBack);
	DDX_Control(pDX, IDC_BTN_NEXT, m_btnNext);
	DDX_Control(pDX, IDC_BTN_STEP1, m_btnStep1);
	DDX_Control(pDX, IDC_BTN_STEP2, m_btnStep2);
	DDX_Control(pDX, IDC_BTN_STEP3, m_btnStep3);
	DDX_Control(pDX, IDC_BTN_STEP4, m_btnStep4);
	DDX_Control(pDX, IDC_BTN_STEP5, m_btnStep5);
	DDX_Control(pDX, IDC_BTN_STEP6, m_btnStep6);
}


BEGIN_MESSAGE_MAP(CReportHead, CDialog)
	ON_BN_CLICKED(IDC_BTN_BACK, &CReportHead::OnBnClickedBtnBack)
	ON_BN_CLICKED(IDC_BTN_NEXT, &CReportHead::OnBnClickedBtnNext)
	ON_BN_CLICKED(IDC_BTN_STEP1, &CReportHead::OnBnClickedBtnStep1)
	ON_BN_CLICKED(IDC_BTN_STEP2, &CReportHead::OnBnClickedBtnStep2)
	ON_BN_CLICKED(IDC_BTN_STEP3, &CReportHead::OnBnClickedBtnStep3)
	ON_BN_CLICKED(IDC_BTN_STEP4, &CReportHead::OnBnClickedBtnStep4)
	ON_BN_CLICKED(IDC_BTN_STEP5, &CReportHead::OnBnClickedBtnStep5)
	ON_BN_CLICKED(IDC_BTN_STEP6, &CReportHead::OnBnClickedBtnStep6)
END_MESSAGE_MAP()


// CReportHead 消息处理程序

BOOL CReportHead::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_btnBack.SetIcon(IDI_WIZARD_PREV,32,32,IDI_WIZARD_PREV,32,32);
	m_btnNext.SetIcon(IDI_WIZARD_NEXT,32,32,IDI_WIZARD_NEXT,32,32);
	m_btnStep1.SetIcon(IDI_WIZARD_STEP1U,32,32,IDI_WIZARD_STEP1D,32,32);
	m_btnStep2.SetIcon(IDI_WIZARD_STEP2U,32,32,IDI_WIZARD_STEP2D,32,32);
	m_btnStep3.SetIcon(IDI_WIZARD_STEP3U,32,32,IDI_WIZARD_STEP3D,32,32);
	m_btnStep4.SetIcon(IDI_WIZARD_STEP4U,32,32,IDI_WIZARD_STEP4D,32,32);
	m_btnStep5.SetIcon(IDI_WIZARD_STEP5U,32,32,IDI_WIZARD_STEP5D,32,32);
	m_btnStep6.SetIcon(IDI_WIZARD_STEP6U,32,32,IDI_WIZARD_STEP6D,32,32);

	//SetLocaleString();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
void CReportHead::OnBnClickedBtnBack()
{
	MAIN->m_pReportFormView->m_ReportWizardDlg.m_pPropSheet->WizardBack();
}

void CReportHead::OnBnClickedBtnNext()
{
	MAIN->m_pReportFormView->m_ReportWizardDlg.m_pPropSheet->WizardNext();
}

void CReportHead::OnBnClickedBtnStep1()
{
	MAIN->m_pReportFormView->m_ReportWizardDlg.m_pPropSheet->SetActivePage(REPORT_PAGE::PROP_SETTING);
}

void CReportHead::OnBnClickedBtnStep2()
{
	//MAIN->m_pReportFormView->m_ReportWizardDlg.m_pPropSheet->SetActivePage(REPORT_PAGE::PROP_WAVE_ITEM);
}

void CReportHead::OnBnClickedBtnStep3()
{
	int nPrint = _wtoi(g_sorFileArray.reportConfig.Print);
	if (nPrint == 1) //波形
		MAIN->m_pReportFormView->m_ReportWizardDlg.m_pPropSheet->SetActivePage(REPORT_PAGE::PROP_WAVE_SETTING);
	else //总表
		MAIN->m_pReportFormView->m_ReportWizardDlg.m_pPropSheet->SetActivePage(REPORT_PAGE::PROP_RPT_SETTING);
}

void CReportHead::OnBnClickedBtnStep4()
{
	MAIN->ShowMainSetting(FALSE);
	MAIN->m_pReportFormView->m_ReportWizardDlg.m_pPropSheet->SetActivePage(REPORT_PAGE::PROP_FILE_SELECT);
}

void CReportHead::OnBnClickedBtnStep5()
{
	MAIN->ShowMainSetting(TRUE);
}

void CReportHead::OnBnClickedBtnStep6()
{
	MAIN->ShowMainSetting(FALSE);
	MAIN->m_pReportFormView->m_ReportWizardDlg.m_pPropSheet->SetActivePage(REPORT_PAGE::PROP_OUTPUT);
}

void CReportHead::SetReportHeadBtn(int nAlreadyPageIndex)//设置报表头按钮显示
{
	if (nAlreadyPageIndex == 1)
	{
		//GetDlgItem(IDC_STATIC_12STEP)->ShowWindow(SW_HIDE);
		//GetDlgItem(IDC_BTN_STEP2)->ShowWindow(SW_HIDE);
		//GetDlgItem(IDC_STATIC_23STEP)->ShowWindow(SW_HIDE);
		//GetDlgItem(IDC_BTN_STEP3)->ShowWindow(SW_HIDE);
		//GetDlgItem(IDC_STATIC_34STEP)->ShowWindow(SW_HIDE);
		//GetDlgItem(IDC_BTN_STEP4)->ShowWindow(SW_HIDE);
		//GetDlgItem(IDC_STATIC_45STEP)->ShowWindow(SW_HIDE);
		//GetDlgItem(IDC_BTN_STEP5)->ShowWindow(SW_HIDE);
		//GetDlgItem(IDC_STATIC_56STEP)->ShowWindow(SW_HIDE);
		//GetDlgItem(IDC_BTN_STEP6)->ShowWindow(SW_HIDE);
	}
	else if (nAlreadyPageIndex == 2)
	{
		GetDlgItem(IDC_STATIC_12STEP)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BTN_STEP2)->ShowWindow(SW_SHOW);
	}
	else if (nAlreadyPageIndex == 3)
	{
		GetDlgItem(IDC_STATIC_23STEP)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BTN_STEP3)->ShowWindow(SW_SHOW);
	}
	else if (nAlreadyPageIndex == 4)
	{
		GetDlgItem(IDC_STATIC_34STEP)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BTN_STEP4)->ShowWindow(SW_SHOW);
	}
	else if (nAlreadyPageIndex == 6)
	{
		GetDlgItem(IDC_STATIC_45STEP)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BTN_STEP5)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_STATIC_56STEP)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BTN_STEP6)->ShowWindow(SW_SHOW);
	}
}