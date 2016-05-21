// Prop3.cpp : implementation file
//

#include "stdafx.h"
#include "PropStep3.h"
#include "MainFrm.h"

extern SorFileArray g_sorFileArray;

/////////////////////////////////////////////////////////////////////////////
// CPropStep3 property page

IMPLEMENT_DYNCREATE(CPropStep3, CPropertyPage)

CPropStep3::CPropStep3() : CPropertyPage(CPropStep3::IDD)
{
}

CPropStep3::~CPropStep3()
{
}

void CPropStep3::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_STEP3_WAVE_TITLE1, m_comboxWave1);
	DDX_Control(pDX, IDC_COMBO_STEP3_WAVE_TITLE2, m_comboxWave2);
	DDX_Control(pDX, IDC_COMBO_STEP3_WAVE_TITLE3, m_comboxWave3);
	DDX_Control(pDX, IDC_COMBO_STEP3_EVENT_TITLE1, m_comboxEvent1);
	DDX_Control(pDX, IDC_COMBO_STEP3_EVENT_TITLE2, m_comboxEvent2);
	DDX_Control(pDX, IDC_COMBO_STEP3_EVENT_TITLE3, m_comboxEvent3);
}


BEGIN_MESSAGE_MAP(CPropStep3, CPropertyPage)
	
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropStep3 message handlers

BOOL CPropStep3::OnSetActive() 
{
	// TODO: Add your specialized code here and/or call the base class
	//((CPropertySheet*)GetParent())->SetWizardButtons(PSWIZB_FINISH |PSWIZB_BACK);
	((CPropSheet*)GetParent())->SetReportHeadBtn(3);
	//重绘图形区域
	MAIN->m_pReportFormView->m_ReportViewDlg.m_DrawButton.ReDrawRect(3);
	((CPropSheet*)GetParent())->EnableNextBtn(TRUE);
	((CPropSheet*)GetParent())->EnablePrevBtn(TRUE);
	return CPropertyPage::OnSetActive();
}

BOOL CPropStep3::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	SetLocalString();

	//GetDlgItem(IDC_EDIT_STEP3_WAVEVAL)->EnableWindow(FALSE);//zll 2012.8.23
	//测量方向
	CComboBox* pCommboxTestDirect = (CComboBox*)GetDlgItem(IDC_COMBO_STEP3_TEST_DIRECT);
	pCommboxTestDirect->InsertString(-1, _T("单向"));
	pCommboxTestDirect->InsertString(-1, _T("双向"));
	int nMeasureDir = _wtoi(g_sorFileArray.reportConfig.MeasureDir);
	pCommboxTestDirect->SetCurSel(nMeasureDir);
	//pCommboxTestDirect->EnableWindow(FALSE);//zll 2012.8.23

	//事件顺序
	CComboBox* pCommboxEventSort = (CComboBox*)GetDlgItem(IDC_COMBO_STEP3_EVENT_ORDER);
	pCommboxEventSort->InsertString(-1, _T("按芯号排序"));
	pCommboxEventSort->InsertString(-1, _T("按波形排序"));
	int nEventSort = _wtoi(g_sorFileArray.reportConfig.EventSort);
	pCommboxEventSort->SetCurSel(nEventSort);
	//pCommboxEventSort->EnableWindow(FALSE);//zll 2012.8.23
	//双向波形顺序;
	CComboBox* pCommbox2WaveOrder = (CComboBox*)GetDlgItem(IDC_COMBO_STEP3_2DIRECT_ORDER);
	pCommbox2WaveOrder->InsertString(-1, _T("单双向交替"));
	int n2WaveOrder = _wtoi(g_sorFileArray.reportConfig.UpDownSort);
	pCommbox2WaveOrder->SetCurSel(n2WaveOrder);
	//pCommbox2WaveOrder->EnableWindow(FALSE);//zll 2012.8.23
	/*                        波形图像                                            */
	//图像
	CComboBox* pCommboxPic = (CComboBox*)GetDlgItem(IDC_COMBO_STEP3_PIC);
	pCommboxPic->InsertString(-1, _T("标准"));
	int nPic = _wtoi(g_sorFileArray.reportConfig.FileFormat);
	pCommboxPic->SetCurSel(nPic);
	//pCommboxPic->EnableWindow(FALSE);//zll 2012.8.23
	//增加一个事件标记
	//输出波形的每个事件
	//GetDlgItem(IDC_CHECK_OUTPUT_EVENT)->EnableWindow(FALSE);//zll 2012.8.23
	//初始化Combox
	InitCombox();
	//编辑项
	//GetDlgItem(IDC_BTN_STEP3_EDIT)->EnableWindow(FALSE);//zll 2012.8.23
	//设置
	//GetDlgItem(IDC_BTN_STEP3_SETTING)->EnableWindow(FALSE);//zll 2012.8.23
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPropStep3::SetLocalString()
{
	/*                      波形输出格式                            */
	CString strWaveFormat = IniUtil::ReadResString(_T("IDS_REPORT_STEP3_OUTPUT_FORMAT"));
	GetDlgItem(IDC_STATIC_STEP3_WAVEFORMAT)->SetWindowText(strWaveFormat);
	//波长数
	CString strWaveCount = IniUtil::ReadResString(_T("IDS_REPORT_STEP3_WAVE_COUNT"));
	GetDlgItem(IDC_STATIC_STEP3_WAVEVAL)->SetWindowText(strWaveCount);
	//测量方向
	CString strTestDirect = IniUtil::ReadResString(_T("IDS_REPORT_STEP3_TEST_DIRECTION"));
	GetDlgItem(IDC_STATIC_STEP3_TEST_DIRECT)->SetWindowText(strTestDirect);
	/*                      波形输出顺序                                       */
	CString strOutOrder = IniUtil::ReadResString(_T("IDS_REPORT_STEP3_OUTPUT_ORDER"));
	GetDlgItem(IDC_STATIC_STEP3_WAVEFORDER)->SetWindowText(strOutOrder);
	//输出顺序
	CString strOutPutOrder = IniUtil::ReadResString(_T("IDS_REPORT_STEP3_OUTPUT_ORDER"));
	GetDlgItem(IDC_STATIC_STEP3_OUTPUT_ORDER)->SetWindowText(strOutPutOrder);

	//事件顺序
	CString strEventOrder = IniUtil::ReadResString(_T("IDS_REPORT_STEP3_EVENT_ORDER"));
	GetDlgItem(IDC_STATIC_STEP3_EVENT_ORDER)->SetWindowText(strEventOrder);

	//双向波形顺序
	CString str2WaveOrder = IniUtil::ReadResString(_T("IDS_REPORT_STEP3_DOUBLE_WAVE_ORDER"));
	GetDlgItem(IDC_STATIC_STEP3_2DIRECT_ORDER)->SetWindowText(str2WaveOrder);
	/*                        波形图像                                            */
	CString strWavePic = IniUtil::ReadResString(_T("IDS_REPORT_STEP3_WAVE_PIC"));
	GetDlgItem(IDC_STATIC_STEP3_WAVEPIC)->SetWindowText(strWavePic);
	//图像
	CString strPic = IniUtil::ReadResString(_T("IDS_REPORT_STEP3_PIC"));
	GetDlgItem(IDC_STATIC_STEP3_PIC)->SetWindowText(strPic);
	//增加一个事件标记
	CString strEventMark = IniUtil::ReadResString(_T("IDS_REPORT_STEP3_ADD_EVENT_MARK"));
	GetDlgItem(IDC_CHECK_ADD_EVENTMARK)->SetWindowText(strEventMark);
	//输出波形的每个事件
	CString strWaveEvent = IniUtil::ReadResString(_T("IDS_REPORT_STEP3_OUTPUT_EVENT"));
	GetDlgItem(IDC_CHECK_OUTPUT_EVENT)->SetWindowText(strWaveEvent);
	//GetDlgItem(IDC_CHECK_OUTPUT_EVENT)->EnableWindow(FALSE);//zll 2012.8.23
	/*                              波形标题                                   */
	CString strWaveTitle = IniUtil::ReadResString(_T("IDS_REPORT_STEP3_WAVE_TITLE"));
	GetDlgItem(IDC_STATIC_WAVE_TITLE)->SetWindowText(strWaveTitle);
	//整个标题
	CString strWholeTitle = IniUtil::ReadResString(_T("IDS_REPORT_STEP3_WHOLE_WAVE"));
	GetDlgItem(IDC_STATIC_STEP3_WAVE_TITLE)->SetWindowText(strWholeTitle);
	//事件波形
	CString strEventWave = IniUtil::ReadResString(_T("IDS_REPORT_STEP3_EVENT_WAVE"));
	GetDlgItem(IDC_STATIC_STEP3_EVENT_TITLE)->SetWindowText(strEventWave);
	/*                              总损耗                                   */
	CString strAllLoss = IniUtil::ReadResString(_T("IDS_REPORT_STEP3_ALL_LOSS"));
	GetDlgItem(IDC_STATIC_TOTALLOSS)->SetWindowText(strAllLoss);
	//计算方法
	CString strCaclMethod = IniUtil::ReadResString(_T("IDS_REPORT_STEP3_CAL_METHOD"));
	GetDlgItem(IDC_STATIC_STEP3_MEHTOD)->SetWindowText(strCaclMethod);
	CString strCumulativeLoss = IniUtil::ReadResString(_T("IDS_SCREEN_CUMULATIVE_LOSS"));
	GetDlgItem(IDC_EDIT_STEP3_MEHTOD)->SetWindowText(strCumulativeLoss);
	CString strTotalLoss = IniUtil::ReadResString(_T("IDS_REPORT_STEP3_ALL_LOSS"));
	GetDlgItem(IDC_EDIT_STEP3_ITEM)->SetWindowText(strTotalLoss);
	//项目号
	CString strItemNumber = IniUtil::ReadResString(_T("IDS_REPORT_STEP3_ITEM_NUMBER"));
	GetDlgItem(IDC_STATIC_STEP3_ITEM)->SetWindowText(strItemNumber);

	//编辑项
	CString strItemEdit = IniUtil::ReadResString(_T("IDS_REPORT_STEP3_ITEM_EDIT"));
	GetDlgItem(IDC_BTN_STEP3_EDIT)->SetWindowText(strItemEdit);
	//设置
	CString strSetting = IniUtil::ReadResString(_T("IDS_REPORT_STEP3_SETTING"));
	GetDlgItem(IDC_BTN_STEP3_SETTING)->SetWindowText(strSetting);
}

LRESULT CPropStep3::OnWizardNext() 
{
	UpdateData();
	RecordSetting();
	return 0;
	//return CPropertyPage::OnWizardNext();
}

void CPropStep3::InitCombox()
{
	InitCombox(m_comboxWave1, g_sorFileArray.reportConfig.Step3Item1);
	InitCombox(m_comboxWave2, g_sorFileArray.reportConfig.Step3Item2);
	InitCombox(m_comboxWave3, g_sorFileArray.reportConfig.Step3Item3);
	InitCombox(m_comboxEvent1, g_sorFileArray.reportConfig.Step3Item4);
	//m_comboxEvent1.EnableWindow(FALSE);//zll 2012.8.23
	InitCombox(m_comboxEvent2, g_sorFileArray.reportConfig.Step3Item5);
	//m_comboxEvent2.EnableWindow(FALSE);//zll 2012.8.23
	InitCombox(m_comboxEvent3, g_sorFileArray.reportConfig.Step3Item6);
	//m_comboxEvent3.EnableWindow(FALSE);//zll 2012.8.23
}

void CPropStep3::InitCombox(CComboBox& combox, CString strText)
{
	if (NULL == combox)
		return;

	combox.ResetContent();
	if (!g_sorFileArray.reportConfig.Step3ComboBox1.IsEmpty())
		combox.AddString(g_sorFileArray.reportConfig.Step3ComboBox1);
	if (!g_sorFileArray.reportConfig.Step3ComboBox2.IsEmpty())
		combox.AddString(g_sorFileArray.reportConfig.Step3ComboBox2);
	if (!g_sorFileArray.reportConfig.Step3ComboBox3.IsEmpty())
		combox.AddString(g_sorFileArray.reportConfig.Step3ComboBox3);
	if (!g_sorFileArray.reportConfig.Step3ComboBox4.IsEmpty())
		combox.AddString(g_sorFileArray.reportConfig.Step3ComboBox4);
	if (!g_sorFileArray.reportConfig.Step3ComboBox5.IsEmpty())
		combox.AddString(g_sorFileArray.reportConfig.Step3ComboBox5);
	if (!g_sorFileArray.reportConfig.Step3ComboBox6.IsEmpty())
		combox.AddString(g_sorFileArray.reportConfig.Step3ComboBox6);
	if (!g_sorFileArray.reportConfig.Step3ComboBox7.IsEmpty())
		combox.AddString(g_sorFileArray.reportConfig.Step3ComboBox7);
	if (!g_sorFileArray.reportConfig.Step3ComboBox8.IsEmpty())
		combox.AddString(g_sorFileArray.reportConfig.Step3ComboBox8);
	if (!g_sorFileArray.reportConfig.Step3ComboBox9.IsEmpty())
		combox.AddString(g_sorFileArray.reportConfig.Step3ComboBox9);
	if (!g_sorFileArray.reportConfig.Step3ComboBox10.IsEmpty())
		combox.AddString(g_sorFileArray.reportConfig.Step3ComboBox10);

	int nPos = combox.FindString(0, strText);
	if (nPos != LB_ERR)
	{
		combox.SetCurSel(nPos);
	}
}

void CPropStep3::RecordSetting() //保存配置信息
{
	GetDlgItem(IDC_EDIT_STEP3_WAVE_TITLE1)->GetWindowText(g_sorFileArray.reportConfig.Step3Edit1);
	m_comboxWave1.GetWindowText(g_sorFileArray.reportConfig.Step3Item1);

	GetDlgItem(IDC_EDIT_STEP3_WAVE_TITLE1)->GetWindowText(g_sorFileArray.reportConfig.Step3Edit2);
	m_comboxWave2.GetWindowText(g_sorFileArray.reportConfig.Step3Item2);

	GetDlgItem(IDC_EDIT_STEP3_WAVE_TITLE1)->GetWindowText(g_sorFileArray.reportConfig.Step3Edit3);
	m_comboxWave3.GetWindowText(g_sorFileArray.reportConfig.Step3Item3);

	GetDlgItem(IDC_EDIT_STEP3_EVENT_TITLE1)->GetWindowText(g_sorFileArray.reportConfig.Step3Edit4);
	m_comboxEvent1.GetWindowText(g_sorFileArray.reportConfig.Step3Item4);

	GetDlgItem(IDC_EDIT_STEP3_EVENT_TITLE2)->GetWindowText(g_sorFileArray.reportConfig.Step3Edit5);
	m_comboxEvent2.GetWindowText(g_sorFileArray.reportConfig.Step3Item5);

	GetDlgItem(IDC_EDIT_STEP3_EVENT_TITLE3)->GetWindowText(g_sorFileArray.reportConfig.Step3Edit6);
	m_comboxEvent3.GetWindowText(g_sorFileArray.reportConfig.Step3Item6);
}