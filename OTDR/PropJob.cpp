// PropJob.cpp : 实现文件
//

#include "stdafx.h"
#include "OTDR.h"
#include "PropJob.h"


// CPropJob 对话框

IMPLEMENT_DYNAMIC(CPropJob, CPropertyPage)

CPropJob::CPropJob()
	: CPropertyPage(CPropJob::IDD)
	, m_strJobID(_T(""))
	, m_strContractor(_T(""))
	, m_strCustomer(_T(""))
	, m_strOperator(_T(""))
	, m_strFile(_T(""))
	, m_strTestDate(_T(""))
	, m_strComment(_T(""))
{
	SetLocalString();
}

CPropJob::~CPropJob()
{
}

void CPropJob::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_PROP_JOB_ID, m_strJobID);
	DDX_Text(pDX, IDC_EDIT_PROP_JOB_CONTRACTOR, m_strContractor);
	DDX_Text(pDX, IDC_EDIT_JOB_CUSTOMER, m_strCustomer);
	DDX_Text(pDX, IDC_EDIT_JOB_OPERATOR, m_strOperator);
	DDX_Text(pDX, IDC_EDIT_JOB_FILE, m_strFile);
	DDX_Text(pDX, IDC_EDIT_JOB_TESTDATE, m_strTestDate);
	DDX_Text(pDX, IDC_EDIT_JOB_COMMENT, m_strComment);
}


BEGIN_MESSAGE_MAP(CPropJob, CPropertyPage)
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_BTN_JOB_CLEAR, &CPropJob::OnBnClickedBtnJobClear)
END_MESSAGE_MAP()


// CPropJob 消息处理程序

BOOL CPropJob::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	return TRUE;
	//return CPropertyPage::OnEraseBkgnd(pDC);
}

//清空所有内容
void CPropJob::OnBnClickedBtnJobClear()
{
	m_strComment.Empty();
	m_strContractor.Empty();
	m_strCustomer.Empty();
	//m_strFile.Empty();
	m_strJobID.Empty();
	m_strOperator.Empty();
	//m_strTestDate.Empty();
	
	UpdateData(FALSE);
}

//根据当前选定的语言设置静态文本
void CPropJob::SetLocalString()
{
	m_strStInfo = IniUtil::ReadResString(_T("IDS_JOB_INFO"));
	m_strStID = IniUtil::ReadResString(_T("IDS_JOB_ID"));
	m_strStContractor = IniUtil::ReadResString(_T("IDS_JOB_CONTRACTOR"));
	m_strStCustomer = IniUtil::ReadResString(_T("IDS_JOB_CUSTOMER"));
	m_strStOperator = IniUtil::ReadResString(_T("IDS_JOB_OPERATOR"));
	m_strStFile = IniUtil::ReadResString(_T("IDS_JOB_FILE"));
	m_strStTime = IniUtil::ReadResString(_T("IDS_JOB_TIME"));
	m_strStComment = IniUtil::ReadResString(_T("IDS_JOB_COMMENT"));
	m_strBtnClear = IniUtil::ReadResString(_T("IDC_BTN_JOB_CLEAR"));
}

BOOL CPropJob::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  在此添加额外的初始化
	GetDlgItem(IDS_JOB_INFO)->SetWindowText(m_strStInfo);
	GetDlgItem(IDS_JOB_ID)->SetWindowText(m_strStID);
	GetDlgItem(IDS_JOB_CONTRACTOR)->SetWindowText(m_strStContractor);
	GetDlgItem(IDS_JOB_CUSTOMER)->SetWindowText(m_strStCustomer);
	GetDlgItem(IDS_JOB_OPERATOR)->SetWindowText(m_strStOperator);
	GetDlgItem(IDS_JOB_FILE)->SetWindowText(m_strStFile);
	GetDlgItem(IDS_JOB_TIME)->SetWindowText(m_strStTime);
	GetDlgItem(IDS_JOB_COMMENT)->SetWindowText(m_strStComment);
	GetDlgItem(IDC_BTN_JOB_CLEAR)->SetWindowText(m_strBtnClear);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
