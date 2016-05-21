// WaveInfo.cpp : 实现文件
//

#include "stdafx.h"
#include "OTDR.h"
#include "WaveInfo.h"


// CWaveInfo 对话框

IMPLEMENT_DYNAMIC(CWaveInfo, CDialog)

CWaveInfo::CWaveInfo(CWnd* pParent /*=NULL*/)
	: CDialog(CWaveInfo::IDD, pParent)
{

}

CWaveInfo::~CWaveInfo()
{
}

void CWaveInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CWaveInfo, CDialog)
END_MESSAGE_MAP()


// CWaveInfo 消息处理程序

BOOL CWaveInfo::OnInitDialog()
{
	CDialog::OnInitDialog();

	//设置对话框标题
	CString strTitle = IniUtil::ReadResString(_T("IDS_WAVEINFO"));
	SetWindowText(strTitle);
	//供应商
	CString strManufacture = IniUtil::ReadResString(_T("IDS_WAVEINFO_MANUFACTURE"));
	GetDlgItem(IDC_STATIC_MANUFACTURE)->SetWindowText(strManufacture);
	//操作者
	CString strOperator = IniUtil::ReadResString(_T("IDS_WAVINFO_OPERATOR"));
	GetDlgItem(IDC_STATIC_OPERATOR)->SetWindowText(strOperator);
	//缆号
	CString strLineNum = IniUtil::ReadResString(_T("IDS_WAVEINFO_LINE_NUM"));
	GetDlgItem(IDC_STATIC_NUM)->SetWindowText(strLineNum);
	//纤号
	CString strFiberNum = IniUtil::ReadResString(_T("IDS_WAVINFO_FIBER_NUM"));
	GetDlgItem(IDC_STATIC_FIBERNUM)->SetWindowText(strFiberNum);
	//起始位置
	CString strStartPos = IniUtil::ReadResString(_T("IDS_WAVEINFO_SRARTPOS"));
	GetDlgItem(IDC_STATIC_STARTPOS)->SetWindowText(strStartPos);
	//终止位置
	CString strEndPos = IniUtil::ReadResString(_T("IDS_WAVEINFO_ENDPOS"));
	GetDlgItem(IDC_STATIC_ENDPOS)->SetWindowText(strEndPos);
	//确定按钮
	CString strOk = IniUtil::ReadResString(_T("IDS_WAVEINFO_OK"));
	GetDlgItem(IDOK)->SetWindowText(strOk);
	//取消按钮
	CString strCancel = IniUtil::ReadResString(_T("IDS_WAVEINFO_CANCEL"));
	GetDlgItem(IDCANCEL)->SetWindowText(strCancel);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
