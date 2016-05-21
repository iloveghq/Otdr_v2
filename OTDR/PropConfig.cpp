// PropConfig.cpp : 实现文件
//

#include "stdafx.h"
#include "OTDR.h"
#include "PropConfig.h"


// CPropConfig 对话框

IMPLEMENT_DYNAMIC(CPropConfig, CPropertyPage)

CPropConfig::CPropConfig()
	: CPropertyPage(CPropConfig::IDD)
	, m_strStartLoc(_T(""))
	, m_strStartCable(_T(""))
	, m_strStartFiber(_T(""))
	, m_strStartColor(_T(""))
	, m_strEndLoc(_T(""))
	, m_strEndCable(_T(""))
	, m_strEndFiber(_T(""))
	, m_strEndColor(_T(""))
	, m_strWavelength(_T(""))
	, m_strIndex(_T(""))
	, m_strPulse(_T(""))
	, m_strResolution(_T(""))
	, m_strRBS(_T(""))
	, m_strRange(_T(""))
	, m_strLossThreshold(_T(""))
	, m_strRefThreshold(_T(""))
	, m_strEndThreshold(_T(""))
	, m_strAverage(_T(""))
	, m_strStLink(_T(""))
	, m_strStOrigin(_T(""))
	, m_strStEnd(_T(""))
	, m_strStLocation(_T(""))
	, m_strStCable(_T(""))
	, m_bSelfDefine(FALSE)
{
	CString strSection = _T("PrintSetting");
	CString strKey = _T("SelfDefine");
	CString strValue = IniUtil::ReadSingleConfigInfo(strSection, strKey);
	m_bSelfDefine = strValue == _T("0") ? 0 : 1;
	SetLocalString();
}

CPropConfig::~CPropConfig()
{
}

void CPropConfig::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_PROP_CONFIG_LOC_S, m_strStartLoc);
	DDX_Text(pDX, IDC_EDIT_PROP_CONFIG_CABLE_S, m_strStartCable);
	DDX_Text(pDX, IDC_EDIT_PROP_CONFIG_FIBER_S, m_strStartFiber);
	DDX_Text(pDX, IDC_EDIT_PROP_CONFIG_CLR_S, m_strStartColor);
	DDX_Text(pDX, IDC_EDIT_PROP_CONFIG__LOC_E, m_strEndLoc);
	DDX_Text(pDX, IDC_EDIT_PROP_CONFIG_CABLE_E, m_strEndCable);
	DDX_Text(pDX, IDC_EDIT_PROP_CONFIG_FIBER_E, m_strEndFiber);
	DDX_Text(pDX, IDC_EDIT_PROP_CONFIG_CLR_E, m_strEndColor);
	DDX_Text(pDX, IDC_EDIT_PROP_CONFIG_WAVE, m_strWavelength);
	DDX_Text(pDX, IDC_EDIT_PROP_CONFIG_INDEX, m_strIndex);
	DDX_Text(pDX, IDC_EDIT_PROP_CONFIG_PULSE, m_strPulse);
	//DDX_Text(pDX, IDC_EDIT_PROP_CONFIG_RES, m_strResolution);
	DDX_Text(pDX, IDC_EDIT_PROP_CONFIG_RBS, m_strRBS);
	DDX_Text(pDX, IDC_EDIT_PROP_CONFIG_RANGE, m_strRange);
	DDX_Text(pDX, IDC_EDIT_PROP_CONFIG_LT, m_strLossThreshold);
	DDX_Text(pDX, IDC_EDIT_PROP_CONFIG_RT, m_strRefThreshold);
	DDX_Text(pDX, IDC_EDIT_PROP_CONFIG_ET, m_strEndThreshold);
	DDX_Text(pDX, IDC_EDIT_PROP_CONFIG_AVERAGE, m_strAverage);
	DDX_Text(pDX, IDS_CFG_LINK, m_strStLink);
	DDX_Text(pDX, IDS_CFG_ORIGIN, m_strStOrigin);
	DDX_Text(pDX, IDS_CFG_END, m_strStEnd);
	DDX_Text(pDX, IDS_CFG_LOCATION, m_strStLocation);
	DDX_Text(pDX, IDS_CFG_CABLE, m_strStCable);
	DDX_Text(pDX, IDS_CFG_FIBER, m_strStFiber);
	DDX_Text(pDX, IDS_CFG_COLOR, m_strStColor);
	DDX_Text(pDX, IDC_BTN_PROP_CONFIG_CLR, m_strStClear);
	DDX_Text(pDX, IDS_CFG_MEASUREMENT, m_strStMeasurement);
	DDX_Text(pDX, IDS_CFG_WAVE, m_strStWave);
	DDX_Text(pDX, IDS_CFG_PULSE, m_strStPulse);
	DDX_Text(pDX, IDS_CFG_RANGE, m_strStRange);
	DDX_Text(pDX, IDS_CFG_LOSS_TH, m_strStLossTh);
	DDX_Text(pDX, IDS_CFG_REFLECTION_TH, m_strStRefTh);
	DDX_Text(pDX, IDS_CFG_END_TH, m_strStEndTh);
	DDX_Text(pDX, IDS_CFG_BACKSCATTER, m_strStBack);
	DDX_Text(pDX, IDS_CFG_INDEX, m_strStIndex);
	DDX_Text(pDX, IDS_CFG_AVERAGE,m_strStAverage);

	DDX_Check(pDX, IDC_CHECK_MANUAL, m_bSelfDefine);
}


BEGIN_MESSAGE_MAP(CPropConfig, CPropertyPage)
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_BTN_PROP_CONFIG_CLR, &CPropConfig::OnClear)
	ON_BN_CLICKED(IDC_CHECK_MANUAL, &CPropConfig::OnCheckManual)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CPropConfig 消息处理程序

BOOL CPropConfig::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	return TRUE;
	//return CPropertyPage::OnEraseBkgnd(pDC);
}

void CPropConfig::OnClear()
{
	m_strStartLoc.Empty();
	m_strStartCable.Empty();
	m_strStartFiber.Empty();
	m_strStartColor.Empty();

	m_strEndLoc.Empty();
	m_strEndCable.Empty();
	m_strEndFiber.Empty();
	m_strEndColor.Empty();

	m_strWavelength.Empty();
	m_strPulse.Empty();
	m_strRange.Empty();
	m_strLossThreshold.Empty();
	m_strRefThreshold.Empty();
	m_strEndThreshold.Empty();
	m_strRBS.Empty();
	m_strIndex.Empty();
	m_strAverage.Empty();

	UpdateData(FALSE);
}

void CPropConfig::OnCheckManual()
{
	UpdateData();
	//BOOL x= m_bSelfDefine;
	if(!m_bSelfDefine)
	{
		GetDlgItem(IDC_EDIT_PROP_CONFIG_WAVE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_PROP_CONFIG_PULSE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_PROP_CONFIG_RANGE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_PROP_CONFIG_LT)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_PROP_CONFIG_RT)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_PROP_CONFIG_ET)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_PROP_CONFIG_RBS)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_PROP_CONFIG_INDEX)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_PROP_CONFIG_AVERAGE)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_EDIT_PROP_CONFIG_WAVE)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_PROP_CONFIG_PULSE)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_PROP_CONFIG_RANGE)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_PROP_CONFIG_LT)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_PROP_CONFIG_RT)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_PROP_CONFIG_ET)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_PROP_CONFIG_RBS)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_PROP_CONFIG_INDEX)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_PROP_CONFIG_AVERAGE)->EnableWindow(TRUE);

	}

	OnReflash();

	UpdateData(FALSE);
}

void CPropConfig::SetLocalString()
{
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
	m_strStClear = IniUtil::ReadResString(_T("IDC_BTN_PROP_CONFIG_CLR"));
	m_strStAverage = IniUtil::ReadResString(_T("IDS_CFG_AVERAGE"));
	
	

	//CString strCheck = IniUtil::ReadResString(_T("IDC_CHECK_MANUAL"));
	//GetDlgItem(IDC_CHECK_MANUAL)->SetWindowText(strCheck);
	
}

void CPropConfig::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	//SetLocalString();
}

BOOL CPropConfig::OnInitDialog()
{
	CPropertyPage::OnInitDialog();	

	CString strCheck = IniUtil::ReadResString(_T("IDC_CHECK_MANUAL"));
	GetDlgItem(IDC_CHECK_MANUAL)->SetWindowText(strCheck);

	OnCheckManual();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CPropConfig::OnReflash()
{
	CString strSection = _T("PrintSetting");
	CString strKey, strValue;
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
	//strKey = _T("SelfDefine");
	//strValue = IniUtil::ReadSingleConfigInfo(strSection, strKey);
	//m_bSelfDefine = strValue == _T("0") ? 0 : 1;

	if(!m_bSelfDefine)  //输入sor文件的内容
	{
		if(g_sorFileArray.totalCurveNumber > 0)
		{
			m_strWavelength.Format(_T("%d"), g_sorFileArray.sorFileList[g_sorFileArray.curSelection].pTestResultInfo->WaveLength);  //波长
			m_strPulse.Format(_T("%d"), g_sorFileArray.sorFileList[g_sorFileArray.curSelection].pTestResultInfo->PulseWidth); //脉宽
			m_strRange.Format(_T("%d"), g_sorFileArray.sorFileList[g_sorFileArray.curSelection].pTestResultInfo->Range); //脉宽
			m_strLossThreshold.Format(_T("%.3f"), g_sorFileArray.sorFileList[g_sorFileArray.curSelection].pTestResultInfo->LossThreshold); //衰减门限
			m_strRefThreshold.Format(_T("%.3f"), g_sorFileArray.sorFileList[g_sorFileArray.curSelection].pTestResultInfo->ReflectanceThreshold); //反射门限
			m_strEndThreshold.Format(_T("%.3f"), g_sorFileArray.sorFileList[g_sorFileArray.curSelection].pTestResultInfo->EndOfFiberThreshold); //光纤末端门限
			m_strRBS.Format(_T("%.3f"), g_sorFileArray.sorFileList[g_sorFileArray.curSelection].pTestResultInfo->BackscatterCoefficient); //光纤背向散射系数
			m_strIndex.Format(_T("%.6f"),g_sorFileArray.sorFileList[g_sorFileArray.curSelection].pTestResultInfo->GroupIndex ); //折射率
			m_strAverage.Format(_T("%.0f"),g_sorFileArray.sorFileList[g_sorFileArray.curSelection].pTestResultInfo->Average ); //平均次数

		}
	}
	else  //自定义
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

	//UpdateData(FALSE);

}

//Save data
void CPropConfig::OnSave()
{
	CString strSection = _T("PrintSetting");
	CString strKey, strValue;
	
	strKey = _T("OriginLocation");
	IniUtil::WriteSingleConfigInfo(strSection, strKey, m_strStartLoc);
	strKey = _T("OriginCable");
	IniUtil::WriteSingleConfigInfo(strSection, strKey, m_strStartCable);
	strKey = _T("OriginFiber");
	IniUtil::WriteSingleConfigInfo(strSection, strKey, m_strStartFiber);
	strKey = _T("OriginColor");
	IniUtil::WriteSingleConfigInfo(strSection, strKey, m_strStartColor);
	strKey = _T("EndLocation");
	IniUtil::WriteSingleConfigInfo(strSection, strKey, m_strEndLoc);
	strKey = _T("EndCable");
	IniUtil::WriteSingleConfigInfo(strSection, strKey, m_strEndCable);
	strKey = _T("EndFiber");
	IniUtil::WriteSingleConfigInfo(strSection, strKey, m_strEndFiber);
	strKey = _T("EndColor");
	IniUtil::WriteSingleConfigInfo(strSection, strKey, m_strEndColor);
	strKey = _T("SelfDefine");
	strValue.Format(_T("%d"), m_bSelfDefine);
	IniUtil::WriteSingleConfigInfo(strSection, strKey, strValue);
	strKey = _T("Wavelength");
	//strValue.Format(_T("%d"), m_strWavelength);
	IniUtil::WriteSingleConfigInfo(strSection, strKey, m_strWavelength);
	strKey = _T("Pulse");
	//strValue.Format(_T("%d"), m_strPulse);
	IniUtil::WriteSingleConfigInfo(strSection, strKey, m_strPulse);
	strKey = _T("LossThreshold");
	//strValue.Format(_T("%d"), m_strLossThreshold);
	IniUtil::WriteSingleConfigInfo(strSection, strKey, m_strLossThreshold);
	strKey = _T("ReflectionThreshold");
	//strValue.Format(_T("%d"), m_strRefThreshold);
	IniUtil::WriteSingleConfigInfo(strSection, strKey, m_strRefThreshold);
	strKey = _T("EndThreshold");
	//strValue.Format(_T("%d"), m_strEndThreshold);
	IniUtil::WriteSingleConfigInfo(strSection, strKey, m_strEndThreshold);
	strKey = _T("BackscatterCoeff");
	//strValue.Format(_T("%d"), m_strRBS);
	IniUtil::WriteSingleConfigInfo(strSection, strKey, m_strRBS);
	strKey = _T("Index");
	//strValue.Format(_T("%d"), m_strIndex);
	IniUtil::WriteSingleConfigInfo(strSection, strKey, m_strIndex);
	strKey = _T("Average");
	//strValue.Format(_T("%d"), m_strAverage);
	IniUtil::WriteSingleConfigInfo(strSection, strKey, m_strAverage);


}
