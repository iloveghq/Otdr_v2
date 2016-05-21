// PropStep5New.cpp : 实现文件
//

#include "stdafx.h"
#include "OTDR.h"
#include "PropStep5New.h"

extern SorFileArray g_sorFileArray;
// CPropStep5New 对话框

IMPLEMENT_DYNAMIC(CPropStep5New, CPropertyPage)

CPropStep5New::CPropStep5New()
	: CPropertyPage(CPropStep5New::IDD)
{

}

CPropStep5New::~CPropStep5New()
{
}

void CPropStep5New::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_PROP_CONTENT_LOGO, m_bCheckLogo);
	DDX_Check(pDX, IDC_CHECK_PROP_CONTENT_JOB, m_bCheckJob);
	DDX_Check(pDX, IDC_CHECK_PROP_CONTENT_CONFIG, m_bCheckConfig);
	DDX_Check(pDX, IDC_CHECK_PROP_CONTENT_COMMENT, m_bCheckComment);
	DDX_Check(pDX, IDC_CHECK_PROP_CONTENT_TRACE, m_bCheckTrace);
	DDX_Check(pDX, IDC_CHECK_PROP_CONTENT_EVENT, m_bCheckEvent);
}


BEGIN_MESSAGE_MAP(CPropStep5New, CPropertyPage)
	ON_BN_CLICKED(IDC_BTN_STEP5_MODIFY, &CPropStep5New::OnBtnStep5Modify)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CPropStep5New 消息处理程序

LRESULT CPropStep5New::OnWizardBack()
{
	// TODO: 在此添加专用代码和/或调用基类

	return CPropertyPage::OnWizardBack();
}

LRESULT CPropStep5New::OnWizardNext()
{
	UpdateData();
	((CPropSheet*)GetParent())->EnableNextBtn(TRUE);
	((CPropSheet*)GetParent())->EnablePrevBtn(TRUE);

	////显示主设置窗口
	//MAIN->ShowMainSetting(TRUE);
	////填充文件列表
	//MAIN->m_pMainSetting->m_pageMainSetting.AddFileList();
	MAIN->m_pReportFormView->m_ReportWizardDlg.m_pPropSheet->SetActivePage(REPORT_PAGE::PROP_OUTPUT);



	return CPropertyPage::OnWizardNext();
}

BOOL CPropStep5New::OnSetActive()
{
	((CPropSheet*)GetParent())->SetReportHeadBtn(2);
	//重绘图形区域
	MAIN->m_pReportFormView->m_ReportViewDlg.m_DrawButton.ReDrawRect(2);
	((CPropSheet*)GetParent())->EnableNextBtn(TRUE);
	((CPropSheet*)GetParent())->EnablePrevBtn(TRUE);

	return CPropertyPage::OnSetActive();
}

void CPropStep5New::SetLocalString()
{
	CString strText;

	m_strGroupSelect = IniUtil::ReadResString(_T("IDS_SETTING_TAB_CONTENT")); //    打印内容
	m_strGroupConfig = 	IniUtil::ReadResString(_T("IDS_SETTING_TAB_CONFIG"));
	m_strGroupJob = IniUtil::ReadResString(_T("IDS_SETTING_TAB_JOB"));//	
	
	m_strStInfo = IniUtil::ReadResString(_T("IDS_JOB_INFO"));
	m_strStID = IniUtil::ReadResString(_T("IDS_JOB_ID"));
	m_strStContractor = IniUtil::ReadResString(_T("IDS_JOB_CONTRACTOR"));
	m_strStCustomer = IniUtil::ReadResString(_T("IDS_JOB_CUSTOMER"));
	m_strStOperator = IniUtil::ReadResString(_T("IDS_JOB_OPERATOR"));
	m_strStFile = IniUtil::ReadResString(_T("IDS_JOB_FILE"));
	m_strStTime = IniUtil::ReadResString(_T("IDS_JOB_TIME"));
	m_strStComment = IniUtil::ReadResString(_T("IDS_JOB_COMMENT"));
	//m_strBtnClear = IniUtil::ReadResString(_T("IDC_BTN_JOB_CLEAR"));

	//设置注释
	m_strStLink = IniUtil::ReadResString(_T("IDS_CFG_LINK"));
	m_strStOrigin = IniUtil::ReadResString(_T("IDS_CFG_ORIGIN"));
	m_strStEnd = IniUtil::ReadResString(_T("IDS_CFG_END"));
	m_strStLocation = IniUtil::ReadResString(_T("IDS_CFG_LOCATION"));
	m_strStCable = IniUtil::ReadResString(_T("IDS_CFG_CABLE"));
	m_strStFiber = IniUtil::ReadResString(_T("IDS_CFG_FIBER"));
	m_strStColor = IniUtil::ReadResString(_T("IDS_CFG_COLOR"));
	m_strStClear = IniUtil::ReadResString(_T("IDS_CFG_CLEAR"));
	m_strStMeasurement = IniUtil::ReadResString(_T("IDS_CFG_MEASUREMENT"));
	m_strStWave = IniUtil::ReadResString(_T("IDS_CFG_WAVE"));
	m_strStPulse = IniUtil::ReadResString(_T("IDS_CFG_PULSE"));
	m_strStRange = IniUtil::ReadResString(_T("IDS_CFG_RANGE"));
	m_strStLossTh = IniUtil::ReadResString(_T("IDS_CFG_LOSS_TH"));
	m_strStRefTh = IniUtil::ReadResString(_T("IDS_CFG_REFLECTION_TH"));
	m_strStEndTh = IniUtil::ReadResString(_T("IDS_CFG_END_TH"));
	m_strStBack = IniUtil::ReadResString(_T("IDS_CFG_BACKSCATTER"));
	m_strStIndex = IniUtil::ReadResString(_T("IDS_CFG_INDEX"));
	m_strStAverage = IniUtil::ReadResString(_T("IDS_CFG_AVERAGE"));

	//选择打印的内容
	m_strSelect = IniUtil::ReadResString(_T("IDS_CNT_SELECT"));
	m_strLogo = IniUtil::ReadResString(_T("IDC_CHECK_PROP_CONTENT_LOGO"));
	m_strJob = IniUtil::ReadResString(_T("IDC_CHECK_PROP_CONTENT_JOB"));
	m_strCfg = IniUtil::ReadResString(_T("IDC_CHECK_PROP_CONTENT_CONFIG"));
	m_strCmt = IniUtil::ReadResString(_T("IDC_CHECK_PROP_CONTENT_COMMENT"));
	m_strTrace = IniUtil::ReadResString(_T("IDC_CHECK_PROP_CONTENT_TRACE"));
	m_strEvent = IniUtil::ReadResString(_T("IDC_CHECK_PROP_CONTENT_EVENT"));

	//打印内容选择
	GetDlgItem(IDC_STATIC_CONTENT)->SetWindowText(m_strSelect);	
	GetDlgItem(IDC_CHECK_PROP_CONTENT_LOGO)->SetWindowText(m_strLogo);
	GetDlgItem(IDC_CHECK_PROP_CONTENT_JOB)->SetWindowText(m_strJob);
	GetDlgItem(IDC_CHECK_PROP_CONTENT_CONFIG)->SetWindowText(m_strCfg);
	GetDlgItem(IDC_CHECK_PROP_CONTENT_COMMENT)->SetWindowText(m_strCmt);
	GetDlgItem(IDC_CHECK_PROP_CONTENT_TRACE)->SetWindowText(m_strTrace);
	GetDlgItem(IDC_CHECK_PROP_CONTENT_EVENT)->SetWindowText(m_strEvent);	
	CString strSection = _T("PrintSetting");
	CString strKey = _T("SelectLogo");
	CString strValue = IniUtil::ReadSingleConfigInfo(strSection, strKey);
	m_bCheckLogo = strValue == _T("0") ? 0 : 1;
	strKey = _T("SelectJob");
	strValue = IniUtil::ReadSingleConfigInfo(strSection, strKey);
	m_bCheckJob = strValue == _T("0") ? 0 : 1;
	strKey = _T("SelectConfig");
	strValue = IniUtil::ReadSingleConfigInfo(strSection, strKey);
	m_bCheckConfig = strValue == _T("0") ? 0 : 1;
	strKey = _T("SelectComment");
	strValue = IniUtil::ReadSingleConfigInfo(strSection, strKey);
	m_bCheckComment = strValue == _T("0") ? 0 : 1;
	strKey = _T("SelectTrace");
	strValue = IniUtil::ReadSingleConfigInfo(strSection, strKey);
	m_bCheckTrace = strValue == _T("0") ? 0 : 1;
	strKey = _T("SelectEvent");
	strValue = IniUtil::ReadSingleConfigInfo(strSection, strKey);
	m_bCheckEvent = strValue == _T("0") ? 0 : 1;	

    //任务
	GetDlgItem(IDC_STATIC_JOB)->SetWindowText(m_strGroupJob);
	strKey = _T("Job");
	m_strJobID = IniUtil::ReadSingleConfigInfo(strSection, strKey);
	strKey = _T("Contractor");
	m_strContractor = IniUtil::ReadSingleConfigInfo(strSection, strKey);
	strKey = _T("Customer");
	m_strCustomer = IniUtil::ReadSingleConfigInfo(strSection, strKey);
	strKey = _T("Operator");
	m_strOperator = IniUtil::ReadSingleConfigInfo(strSection, strKey);
	if(g_sorFileArray.totalCurveNumber > 0)
	{
		//(CSorFileBase*)m_lpVoid
		m_strFile = g_sorFileArray.sorFileList[g_sorFileArray.curSelection].fileName;//filePathName;
		CTime tm(g_sorFileArray.sorFileList[g_sorFileArray.curSelection].pTestResultInfo->MeasureTime);
		m_strTestDate = tm.Format(_T("%Y-%m-%d %H:%M:%S"));
	}
	
    strText = m_strStID + _T(":") + m_strJobID;
	GetDlgItem(IDC_STATIC_JOB_ID)->SetWindowText(strText); //任务编号
	strText = m_strStContractor + _T(":") + m_strContractor;
	GetDlgItem(IDC_STATIC_CONTRACTOR)->SetWindowText(strText); //承包商
	strText = m_strStCustomer + _T(":") + m_strCustomer;
	GetDlgItem(IDC_STATIC_CUSTOMER)->SetWindowText(strText); //客户
	strText = m_strStFile + _T(":") + m_strFile;
	GetDlgItem(IDC_STATIC_FILE)->SetWindowText(strText); //文件
	strText = m_strStTime + _T(":") + m_strTestDate;
	GetDlgItem(IDC_STATIC_DATE)->SetWindowText(strText); //测量时间
	strText = m_strStOperator + _T(":") + m_strOperator;
	GetDlgItem(IDC_STATIC_OPERATOR)->SetWindowText(strText); //操作员

	//注释
	GetDlgItem(IDC_STATIC_COMMENT)->SetWindowText(m_strStComment);
	strKey = _T("Comment");
    m_strComment = IniUtil::ReadSingleConfigInfo(strSection, strKey);  //注释内容
	GetDlgItem(IDC_STATIC_COMMENT_TEXT)->SetWindowText(m_strComment);
	
	//配置信息
	GetDlgItem(IDC_STATIC_CONFIG)->SetWindowText(m_strGroupConfig);
	strKey = _T("OriginLocation");
	m_strStartLoc = IniUtil::ReadSingleConfigInfo(strSection, strKey);
	strKey = _T("OriginCable");
	m_strStartCable = IniUtil::ReadSingleConfigInfo(strSection, strKey);
	strKey = _T("OriginFiber");
	m_strStartFiber = IniUtil::ReadSingleConfigInfo(strSection, strKey);
	strKey = _T("OriginColor");
	m_strStartColor = IniUtil::ReadSingleConfigInfo(strSection, strKey);
	strKey = _T("EndLocation");
	m_strEndLoc = IniUtil::ReadSingleConfigInfo(strSection, strKey);
	strKey = _T("EndCable");
	m_strEndCable = IniUtil::ReadSingleConfigInfo(strSection, strKey);
	strKey = _T("EndFiber");
	m_strEndFiber = IniUtil::ReadSingleConfigInfo(strSection, strKey);
	strKey = _T("EndColor");
	m_strEndColor = IniUtil::ReadSingleConfigInfo(strSection, strKey);

	strKey = _T("SelfDefine");
	strValue = IniUtil::ReadSingleConfigInfo(strSection, strKey);
	BOOL bSelfDefine = strValue == _T("0") ? 0 : 1;
	if(! bSelfDefine)
	{
		if(g_sorFileArray.totalCurveNumber > 0)
		{
			m_strWavelength.Format(_T("%d"), g_sorFileArray.sorFileList[g_sorFileArray.curSelection].pTestResultInfo->WaveLength);  //波长
			m_strPulse.Format(_T("%d"), g_sorFileArray.sorFileList[g_sorFileArray.curSelection].pTestResultInfo->PulseWidth); //脉宽
			m_strRange.Format(_T("%d"), g_sorFileArray.sorFileList[g_sorFileArray.curSelection].pTestResultInfo->Range); //脉宽
			m_strLossThreshold.Format(_T("%.3f"), g_sorFileArray.sorFileList[g_sorFileArray.curSelection].pTestResultInfo->LossThreshold); //衰减门限
			m_strRefThreshold.Format(_T("%.3f"), g_sorFileArray.sorFileList[g_sorFileArray.curSelection].pTestResultInfo->ReflectanceThreshold); //反射门限
			m_strEndThreshold.Format(_T("%.3f"), g_sorFileArray.sorFileList[g_sorFileArray.curSelection].pTestResultInfo->EndOfFiberThreshold); //光纤末端门限
			m_strRBS.Format(_T("%.3f"), g_sorFileArray.sorFileList[g_sorFileArray.curSelection].pTestResultInfo->BackscatterCoefficient); //光纤末端门限
			m_strIndex.Format(_T("%.6f"),g_sorFileArray.sorFileList[g_sorFileArray.curSelection].pTestResultInfo->GroupIndex );
			m_strAverage.Format(_T("%.0f"),  g_sorFileArray.sorFileList[g_sorFileArray.curSelection].pTestResultInfo->Average);//平均次数
			//	strText = g_sorFileArray.reportConfig.Configuration.strAverage + _T(": ") + g_sorFileArray.reportConfig.Configuration.Average;  //写Average counts  
		}
	}
	else
	{
		strKey = _T("Wavelength");
		m_strWavelength = IniUtil::ReadSingleConfigInfo(strSection, strKey);
		strKey = _T("Pulse");
		m_strPulse = IniUtil::ReadSingleConfigInfo(strSection, strKey);
		strKey = _T("Range");
		m_strRange = IniUtil::ReadSingleConfigInfo(strSection, strKey);
		strKey = _T("LossThreshold");
		m_strLossThreshold = IniUtil::ReadSingleConfigInfo(strSection, strKey);
		strKey = _T("ReflectionThreshold");
		m_strRefThreshold = IniUtil::ReadSingleConfigInfo(strSection, strKey);
		strKey = _T("EndThreshold");
		m_strEndThreshold = IniUtil::ReadSingleConfigInfo(strSection, strKey);
		strKey = _T("BackscatterCoeff");
		m_strRBS = IniUtil::ReadSingleConfigInfo(strSection, strKey);
		strKey = _T("Index");
		m_strIndex = IniUtil::ReadSingleConfigInfo(strSection, strKey);		
		strKey = _T("Average");
		m_strAverage = IniUtil::ReadSingleConfigInfo(strSection, strKey);

	}
	
	strText = m_strStWave + _T(":") + m_strWavelength + _T("nm");
	GetDlgItem(IDC_STATIC_WAVE)->SetWindowText(strText); //波长
	strText = m_strStPulse + _T(":") + m_strPulse + _T("ns");
	GetDlgItem(IDC_STATIC_PULSE)->SetWindowText(strText); //脉宽
	strText = m_strStOrigin;
	GetDlgItem(IDC_STATIC_ORIGIN)->SetWindowText(strText); //起始点
	strText = _T("  ") + m_strStLocation + _T(":") + m_strStartLoc;
	GetDlgItem(IDC_STATIC_LOCATION_O)->SetWindowText(strText); //起始位置
	strText = _T("  ") + m_strStCable + _T(":") + m_strStartCable;
	GetDlgItem(IDC_STATIC_CABLE_O)->SetWindowText(strText); //起始光缆
	strText = _T("  ") + m_strStFiber + _T(":") + m_strStartFiber;
	GetDlgItem(IDC_STATIC_FIBER_O)->SetWindowText(strText); //起始光纤
	strText = _T("  ") + m_strStColor + _T(":") + m_strStartColor;
	GetDlgItem(IDC_STATIC_COLOR_O)->SetWindowText(strText); //起始颜色

	strText = m_strStRange + _T(":") + m_strRange + _T("m");
	GetDlgItem(IDC_STATIC_RANGE)->SetWindowText(strText); //测量范围
	strText = m_strStAverage + _T(":") + m_strAverage;
	GetDlgItem(IDC_STATIC_AVERAGE)->SetWindowText(strText); //平均
	strText = m_strStEnd;
	GetDlgItem(IDC_STATIC_END)->SetWindowText(strText); //结束点
	strText = _T("  ") + m_strStLocation + _T(":") + m_strEndLoc;
	GetDlgItem(IDC_STATIC_LOCATION_E)->SetWindowText(strText); //结束位置
	strText = _T("  ") + m_strStCable + _T(":") + m_strEndLoc;
	GetDlgItem(IDC_STATIC_CABLE_E)->SetWindowText(strText); //结束光缆
	strText = _T("  ") + m_strStFiber + _T(":") + m_strEndFiber;
	GetDlgItem(IDC_STATIC_FIBER_E)->SetWindowText(strText); //起始光纤
	strText = _T("  ") + m_strStColor + _T(":") + m_strEndColor;
	GetDlgItem(IDC_STATIC_COLOR_E)->SetWindowText(strText); //起始颜色

	strText = m_strStBack + _T(":") + m_strRBS + _T("dB");
	GetDlgItem(IDC_STATIC_BACKSCATTER)->SetWindowText(strText); //背向散射系数
	strText = m_strStLossTh + _T(":") + m_strLossThreshold + _T("dB");
	GetDlgItem(IDC_STATIC_LOSS_TH)->SetWindowText(strText); //loss threshold
	strText = m_strStRefTh + _T(":") + m_strRefThreshold + _T("dB");
	GetDlgItem(IDC_STATIC_REFLECTION_TH)->SetWindowText(strText); //reflection threshold
	strText = m_strStEndTh + _T(":") + m_strEndThreshold + _T("dB");
	GetDlgItem(IDC_STATIC_END_TH)->SetWindowText(strText); //end threshold
	strText = m_strStIndex + _T(":") + m_strIndex;
	GetDlgItem(IDC_STATIC_INDEX)->SetWindowText(strText); //折射率
	
	//修改
	CString strModify = IniUtil::ReadResString(_T("IDC_BTN_STEP5_MODIFY"));
	GetDlgItem(IDC_BTN_STEP5_MODIFY)->SetWindowText(strModify);

	UpdateData(FALSE);

}

BOOL CPropStep5New::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	//SetLocalString();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

//修改报表头的一些参数
void CPropStep5New::OnBtnStep5Modify()
{
	//MAIN->OnSetting();
	CString strTitle = IniUtil::ReadResString(_T("IDS_SETTING_TITLE"));
	CMyPropertySheet myProp(strTitle);

	//myProp.m_propConfig.SetLocalString();  //根据所选的语言设定静态文本
	if(myProp.DoModal() == IDOK)
	{
		myProp.SaveSetting(); 
		Invalidate();
	}
	else
	{
		//AfxMessageBox(_T("cancel button pushed"));
	}

}

void CPropStep5New::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CPropertyPage::OnPaint()
	SetLocalString();
}
