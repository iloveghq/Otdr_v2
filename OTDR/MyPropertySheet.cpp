// MyPropertySheet.cpp : 实现文件
//

#include "stdafx.h"
#include "OTDR.h"
#include "MyPropertySheet.h"
//#include "MainFrm.h"
//#include "OTDR.h"

// CMyPropertySheet
extern SorFileArray g_sorFileArray;

IMPLEMENT_DYNAMIC(CMyPropertySheet, CPropertySheet)

CMyPropertySheet::CMyPropertySheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
	
}

CMyPropertySheet::CMyPropertySheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	AddPage(&m_propContent);
	AddPage(&m_propConfig);
	AddPage(&m_propJob);

	//m_pWnd = new CWnd;
	//m_pWnd = NULL;

	m_psh.dwFlags |= PSH_NOAPPLYNOW;   //隐藏应用按钮
}

CMyPropertySheet::~CMyPropertySheet()
{
}


BEGIN_MESSAGE_MAP(CMyPropertySheet, CPropertySheet)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CMyPropertySheet 消息处理程序

HBRUSH CMyPropertySheet::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CPropertySheet::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何属性
	/*if(CTLCOLOR_DLG == nCtlColor)
	{
       return m_bkBrush;
	}*/
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}

BOOL CMyPropertySheet::OnInitDialog()
{
	BOOL bResult = CPropertySheet::OnInitDialog();

	SetLocalString();

	//修改属性页tab的文字
	TC_ITEM item;
	item.mask = TCIF_TEXT;
	item.pszText = m_strTabCotent.GetBuffer();//_T("打印内容");
	m_strTabCotent.ReleaseBuffer();
	GetTabControl()->SetItem(0, &item);
	item.pszText = m_strTabConfig.GetBuffer();//_T("配置");
	m_strTabConfig.ReleaseBuffer();
	GetTabControl()->SetItem(1, &item);
	item.pszText = m_strTabJob.GetBuffer();//_T("任务");
	m_strTabJob.ReleaseBuffer();
	GetTabControl()->SetItem(2, &item);

	GetDlgItem(IDOK)->SetWindowText(m_strBtnOK);
	GetDlgItem(IDCANCEL)->SetWindowText(m_strBtnCancel);

	//m_bkBrush = ::CreateSolidBrush(RGB(255, 0, 0));

	//初始化各属性页
	OnInitPages();

	return bResult;
}

//初始化各属性页
void CMyPropertySheet::OnInitPages(void)
{
	CString strSection, strKey, strValue;

	//初始化打印内容属性页
	strSection = _T("PrintSetting");
	strKey = _T("SelectLogo");
	strValue = IniUtil::ReadSingleConfigInfo(strSection, strKey);
	m_propContent.m_bCheckLogo = strValue == _T("0") ? 0 : 1;
	strKey = _T("SelectJob");
	strValue = IniUtil::ReadSingleConfigInfo(strSection, strKey);
	m_propContent.m_bCheckJob = strValue == _T("0") ? 0 : 1;
	strKey = _T("SelectConfig");
	strValue = IniUtil::ReadSingleConfigInfo(strSection, strKey);
	m_propContent.m_bCheckConfig = strValue == _T("0") ? 0 : 1;
	strKey = _T("SelectComment");
	strValue = IniUtil::ReadSingleConfigInfo(strSection, strKey);
	m_propContent.m_bCheckComment = strValue == _T("0") ? 0 : 1;
	strKey = _T("SelectTrace");
	strValue = IniUtil::ReadSingleConfigInfo(strSection, strKey);
	m_propContent.m_bCheckTrace = strValue == _T("0") ? 0 : 1;
	strKey = _T("SelectEvent");
	strValue = IniUtil::ReadSingleConfigInfo(strSection, strKey);
	m_propContent.m_bCheckEvent = strValue == _T("0") ? 0 : 1;
	m_propContent.UpdateData(FALSE);


	//初始化配置属性页
	m_propConfig.OnReflash();
	//strKey = _T("OriginLocation");
	//m_propConfig.m_strStartLoc = IniUtil::ReadSingleConfigInfo(strSection, strKey);
	//strKey = _T("OriginCable");
	//m_propConfig.m_strStartCable = IniUtil::ReadSingleConfigInfo(strSection, strKey);
	//strKey = _T("OriginFiber");
	//m_propConfig.m_strStartFiber = IniUtil::ReadSingleConfigInfo(strSection, strKey);
	//strKey = _T("OriginColor");
	//m_propConfig.m_strStartColor = IniUtil::ReadSingleConfigInfo(strSection, strKey);
	//strKey = _T("EndLocation");
	//m_propConfig.m_strEndLoc = IniUtil::ReadSingleConfigInfo(strSection, strKey);
	//strKey = _T("EndCable");
	//m_propConfig.m_strEndCable = IniUtil::ReadSingleConfigInfo(strSection, strKey);
	//strKey = _T("EndFiber");
	//m_propConfig.m_strEndFiber = IniUtil::ReadSingleConfigInfo(strSection, strKey);
	//strKey = _T("EndColor");
	//m_propConfig.m_strEndColor = IniUtil::ReadSingleConfigInfo(strSection, strKey);
	//strKey = _T("SelfDefine");
	//strValue = IniUtil::ReadSingleConfigInfo(strSection, strKey);
	//m_propConfig.m_bSelfDefine = strValue == _T("0") ? 0 : 1;
	//
	//if(!m_propConfig.m_bSelfDefine)  //输入sor文件的内容
	//{
	//	if(g_sorFileArray.totalCurveNumber > 0)
	//	{
	//		m_propConfig.m_strWavelength.Format(_T("%d"), g_sorFileArray.sorFileList[g_sorFileArray.curSelection].pTestResultInfo->WaveLength);  //波长
	//		m_propConfig.m_strPulse.Format(_T("%d"), g_sorFileArray.sorFileList[g_sorFileArray.curSelection].pTestResultInfo->PulseWidth); //脉宽
	//		m_propConfig.m_strRange.Format(_T("%d"), g_sorFileArray.sorFileList[g_sorFileArray.curSelection].pTestResultInfo->Range); //脉宽
	//		m_propConfig.m_strLossThreshold.Format(_T("%.3f"), g_sorFileArray.sorFileList[g_sorFileArray.curSelection].pTestResultInfo->LossThreshold); //衰减门限
	//		m_propConfig.m_strRefThreshold.Format(_T("%.3f"), g_sorFileArray.sorFileList[g_sorFileArray.curSelection].pTestResultInfo->ReflectanceThreshold); //反射门限
	//		m_propConfig.m_strEndThreshold.Format(_T("%.3f"), g_sorFileArray.sorFileList[g_sorFileArray.curSelection].pTestResultInfo->EndOfFiberThreshold); //光纤末端门限
	//		m_propConfig.m_strRBS.Format(_T("%.3f"), g_sorFileArray.sorFileList[g_sorFileArray.curSelection].pTestResultInfo->BackscatterCoefficient); //光纤背向散射系数
	//		m_propConfig.m_strIndex.Format(_T("%.6f"),g_sorFileArray.sorFileList[g_sorFileArray.curSelection].pTestResultInfo->GroupIndex ); //折射率
	//		m_propConfig.m_strAverage.Format(_T("%.0f"),g_sorFileArray.sorFileList[g_sorFileArray.curSelection].pTestResultInfo->Average ); //平均次数

	//	}
	//}
	//else  //自定义
	//{	
	//	strKey = _T("Wavelength");
	//	m_propConfig.m_strWavelength = IniUtil::ReadSingleConfigInfo(strSection, strKey);
	//	strKey = _T("Pulse");
	//	m_propConfig.m_strPulse = IniUtil::ReadSingleConfigInfo(strSection, strKey);
	//	strKey = _T("LossThreshold");
	//	m_propConfig.m_strLossThreshold = IniUtil::ReadSingleConfigInfo(strSection, strKey);
	//	strKey = _T("ReflectionThreshold");
	//	m_propConfig.m_strRefThreshold = IniUtil::ReadSingleConfigInfo(strSection, strKey);
	//	strKey = _T("EndThreshold");
	//	m_propConfig.m_strEndThreshold = IniUtil::ReadSingleConfigInfo(strSection, strKey);
	//	strKey = _T("BackscatterCoeff");
	//	m_propConfig.m_strRBS = IniUtil::ReadSingleConfigInfo(strSection, strKey);
	//	strKey = _T("Index");
	//	m_propConfig.m_strIndex = IniUtil::ReadSingleConfigInfo(strSection, strKey);		
	//	strKey = _T("Average");
	//	m_propConfig.m_strAverage = IniUtil::ReadSingleConfigInfo(strSection, strKey);

	//}
	
	//初始化任务属性页
	strKey = _T("Job");
	m_propJob.m_strJobID = IniUtil::ReadSingleConfigInfo(strSection, strKey);
	strKey = _T("Contractor");
	m_propJob.m_strContractor = IniUtil::ReadSingleConfigInfo(strSection, strKey);
	strKey = _T("Customer");
	m_propJob.m_strCustomer = IniUtil::ReadSingleConfigInfo(strSection, strKey);
	strKey = _T("Operator");
	m_propJob.m_strOperator = IniUtil::ReadSingleConfigInfo(strSection, strKey);
	if(g_sorFileArray.totalCurveNumber > 0)
	{
		//(CSorFileBase*)m_lpVoid
		m_propJob.m_strFile = g_sorFileArray.sorFileList[g_sorFileArray.curSelection].fileName;//filePathName;
		CTime tm(g_sorFileArray.sorFileList[g_sorFileArray.curSelection].pTestResultInfo->MeasureTime);
		m_propJob.m_strTestDate = tm.Format(_T("%Y-%m-%d %H:%M:%S"));
	}
	strKey = _T("Comment");
    m_propJob.m_strComment = IniUtil::ReadSingleConfigInfo(strSection, strKey);
	
}



BOOL CMyPropertySheet::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	
	return CPropertySheet::PreTranslateMessage(pMsg);
}

//保存数据到config.ini以及打印要涉及的数据结构体
void CMyPropertySheet::SaveSetting()
{
	CString strSection, strKey, strValue;
    strSection = _T("PrintSetting");

	//保存打印内容属性页
	strKey = _T("SelectLogo");
	strValue.Format(_T("%d"), m_propContent.m_bCheckLogo);
	IniUtil::WriteSingleConfigInfo(strSection, strKey, strValue);
	strKey = _T("SelectJob");
	strValue.Format(_T("%d"), m_propContent.m_bCheckJob);
	IniUtil::WriteSingleConfigInfo(strSection, strKey, strValue);
	strKey = _T("SelectConfig");
	strValue.Format(_T("%d"), m_propContent.m_bCheckConfig);
	IniUtil::WriteSingleConfigInfo(strSection, strKey, strValue);
	strKey = _T("SelectComment");
	strValue.Format(_T("%d"), m_propContent.m_bCheckComment);
	IniUtil::WriteSingleConfigInfo(strSection, strKey, strValue);
	strKey = _T("SelectTrace");
	strValue.Format(_T("%d"), m_propContent.m_bCheckTrace);
	IniUtil::WriteSingleConfigInfo(strSection, strKey, strValue);
	strKey = _T("SelectEvent");
	strValue.Format(_T("%d"), m_propContent.m_bCheckEvent);
	IniUtil::WriteSingleConfigInfo(strSection, strKey, strValue);

	//保存配置属性页
	m_propConfig.OnSave();
	

	//保存任务属性页
	strKey = _T("Job");
	IniUtil::WriteSingleConfigInfo(strSection, strKey, m_propJob.m_strJobID);
	strKey = _T("Contractor");
	IniUtil::WriteSingleConfigInfo(strSection, strKey, m_propJob.m_strContractor);	
	strKey = _T("Customer");
	IniUtil::WriteSingleConfigInfo(strSection, strKey, m_propJob.m_strCustomer);	
	strKey = _T("Operator");
	IniUtil::WriteSingleConfigInfo(strSection, strKey, m_propJob.m_strOperator);	
	strKey = _T("Comment");
	IniUtil::WriteSingleConfigInfo(strSection, strKey, m_propJob.m_strComment);	

	
}

void CMyPropertySheet::SetLocalString()
{
	m_strTabCotent = IniUtil::ReadResString(_T("IDS_SETTING_TAB_CONTENT"));
	m_strTabConfig = IniUtil::ReadResString(_T("IDS_SETTING_TAB_CONFIG"));
	m_strTabJob = IniUtil::ReadResString(_T("IDS_SETTING_TAB_JOB"));
	m_strBtnOK = IniUtil::ReadResString(_T("IDS_SETTING_BTN_OK"));
	m_strBtnCancel = IniUtil::ReadResString(_T("IDS_SETTING_BTN_CANCEL"));
}
