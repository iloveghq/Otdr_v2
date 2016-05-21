// EventGenerateCon.cpp : 实现文件
//

#include "stdafx.h"
#include "OTDR.h"
#include "EventGenerateCon.h"


// CEventGenerateCon 对话框

IMPLEMENT_DYNAMIC(CEventGenerateCon, CPropertyPage)

CEventGenerateCon::CEventGenerateCon()
	: CPropertyPage(CEventGenerateCon::IDD)
{

}

CEventGenerateCon::~CEventGenerateCon()
{
}

void CEventGenerateCon::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SPIN_PERMITRANGE, m_spinPermRange);
	DDX_Control(pDX, IDC_SPIN_LERATEFREQ, m_spinRelaDegree);
	DDX_Control(pDX, IDC_SPIN_BOTHPERMITRANGE, m_spinBothPermRange);
}


BEGIN_MESSAGE_MAP(CEventGenerateCon, CPropertyPage)
END_MESSAGE_MAP()


// CEventGenerateCon 消息处理程序

BOOL CEventGenerateCon::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
	/*                 主事件自动生成方法                                          */
	CString strMethod= IniUtil::ReadResString(_T("IDS_EVENT_CON_GENERATE_METHOD"));
	GetDlgItem(IDC_STATIC_GENERATE)->SetWindowText(strMethod);
	//从事件列表中生成
	CString strEventList= IniUtil::ReadResString(_T("IDS_EVENT_CON_EVENTLIST"));
	CButton* pBtnEventList = (CButton*)GetDlgItem(IDC_RDO_EVENTLIST);
	pBtnEventList->SetWindowText(strEventList);
	//从自动搜索中生成
	CString strAutoSearch = IniUtil::ReadResString(_T("IDS_EVENT_CON_AUTOSEARCH"));
	CButton* pBtnAutoSearch = (CButton*)GetDlgItem(IDC_RDO_AUTOSEARCH);
	pBtnAutoSearch->SetWindowText(strAutoSearch);
	CString strGenMethod = IniUtil::ReadSingleConfigInfo(_T("PageSetAutoGen"), _T("GenMethod"));
	if (strGenMethod.CompareNoCase(_T("1")) == 0)
	{
		pBtnEventList->SetCheck(TRUE);
		pBtnAutoSearch->SetCheck(FALSE);
	}
	else
	{
		pBtnEventList->SetCheck(FALSE);
		pBtnAutoSearch->SetCheck(TRUE);
	}
	/*                 生成条件                                          */
	CString strGenerateCon= IniUtil::ReadResString(_T("IDS_EVENT_CON_GENERATE_CON"));
	GetDlgItem(IDC_STATIC_CONDITION)->SetWindowText(strGenerateCon);
	CString strPermRangeVal = IniUtil::ReadSingleConfigInfo(_T("PageSetAutoGen"), _T("PermRange"));
	//区间距离容许范围
	CString strPermitRange=IniUtil::ReadResString(_T("IDS_EVENT_CON_RANGE_PERMIT"));
	GetDlgItem(IDC_STATIC_PERMITRANGE)->SetWindowText(strPermitRange);
	CString strPermRange = IniUtil::ReadSingleConfigInfo(_T("PageSetAutoGen"), _T("PermRange"));
	int nPermRange = _wtoi(strPermRange);
	m_spinPermRange.SetSpin(0,1,10,1);
	m_spinPermRange.SetPos(nPermRange);
	//相对度数
	CString strRelativeDegree=IniUtil::ReadResString(_T("IDS_EVENT_CON_RELATIVE_DEGREE"));
	GetDlgItem(IDC_STATIC_LERATEFREQ)->SetWindowText(strRelativeDegree);
	CString strLerateFreq = IniUtil::ReadSingleConfigInfo(_T("PageSetAutoGen"), _T("LerateFreq"));
	int nLerateFreq = _wtoi(strLerateFreq);
	m_spinRelaDegree.SetSpin(0,1,100,1);
	m_spinRelaDegree.SetPos(nLerateFreq);
	//区间距离容限(双向)
	CString strBothPermitRange= IniUtil::ReadResString(_T("IDS_EVENT_CON_BOTHRANGE_PERMIT"));
	GetDlgItem(IDC_STATIC_BOTHPERMITRANGE)->SetWindowText(strBothPermitRange);
	CString strBothPermRange = IniUtil::ReadSingleConfigInfo(_T("PageSetAutoGen"), _T("BothPermRange"));
	int nBothPermRange = _wtoi(strBothPermRange);
	m_spinBothPermRange.SetSpin(0,1,10,1);
	m_spinBothPermRange.SetPos(nLerateFreq);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

BOOL CEventGenerateCon::OnApply()
{
	/*                 主事件自动生成方法                                          */
	CButton* pBtnEventList = (CButton*)GetDlgItem(IDC_RDO_EVENTLIST);//从事件列表中生成
	CButton* pBtnAutoSearch = (CButton*)GetDlgItem(IDC_RDO_AUTOSEARCH);//从自动搜索中生成
	CString strAutoSearch;
	if (pBtnEventList->GetCheck())
	{
		strAutoSearch = _T("0");
	}
	else if (pBtnAutoSearch->GetCheck())
	{
		strAutoSearch = _T("1");
	}
	IniUtil::WriteSingleConfigInfo(_T("PageSetAutoGen"), _T("GenMethod"), strAutoSearch);
	/*                 生成条件                                          */
	//区间距离容许范围
	CString strPermitRange;
	int nPermitRange = m_spinPermRange.GetPos();
	strPermitRange.Format(_T("%d"), nPermitRange);
	IniUtil::WriteSingleConfigInfo(_T("PageSetAutoGen"), _T("PermRange"), strPermitRange);
	//相对度数
	CString strRelativeDegree;
	int nRelativeDegree = m_spinRelaDegree.GetPos();
	strRelativeDegree.Format(_T("%d"), nRelativeDegree); //相对度数
	IniUtil::WriteSingleConfigInfo(_T("PageSetAutoGen"), _T("LerateFreq"), strRelativeDegree);
	//区间距离容限(双向)
	CString strBothPermitRange;
	int nBothPermitRange = m_spinBothPermRange.GetPos();
	strBothPermitRange.Format(_T("%d"), strBothPermitRange);
	IniUtil::WriteSingleConfigInfo(_T("PageSetAutoGen"), _T("BothPermRange"), strBothPermitRange);

	//生成事件信息


	return CPropertyPage::OnApply();
}
