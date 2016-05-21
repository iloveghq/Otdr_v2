// PropStep3Rpt.cpp : 实现文件
//

#include "stdafx.h"
#include "OTDR.h"
#include "PropStep3Rpt.h"

extern SorFileArray g_sorFileArray;
// CPropStep3Rpt 对话框

IMPLEMENT_DYNAMIC(CPropStep3Rpt, CPropertyPage)

CPropStep3Rpt::CPropStep3Rpt()
	: CPropertyPage(CPropStep3Rpt::IDD)
{

}

CPropStep3Rpt::~CPropStep3Rpt()
{
}

void CPropStep3Rpt::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPropStep3Rpt, CPropertyPage)
END_MESSAGE_MAP()


// CPropStep3Rpt 消息处理程序

BOOL CPropStep3Rpt::OnSetActive()
{
	// TODO: 在此添加专用代码和/或调用基类
	((CPropSheet*)GetParent())->SetReportHeadBtn(3);
	//重绘图形区域
	MAIN->m_pReportFormView->m_ReportViewDlg.m_DrawButton.ReDrawRect(3);
	((CPropSheet*)GetParent())->EnableNextBtn(TRUE);
	((CPropSheet*)GetParent())->EnablePrevBtn(TRUE);
	return CPropertyPage::OnSetActive();
}

LRESULT CPropStep3Rpt::OnWizardNext()
{
	// TODO: 在此添加专用代码和/或调用基类

	UpdateData();
	RecordSetting();
	return 0;
}

void CPropStep3Rpt::RecordSetting() //保存配置信息
{
	//GetDlgItem(IDC_EDIT_STEP2_TITLE)->GetWindowText(g_sorFileArray.reportConfig.Title);

	//GetDlgItem(IDC_EDIT_STEP2_ITEM1)->GetWindowText(g_sorFileArray.reportConfig.Item1Val);
	//m_combox1.GetWindowText(g_sorFileArray.reportConfig.Item1);

	//GetDlgItem(IDC_EDIT_STEP2_ITEM2)->GetWindowText(g_sorFileArray.reportConfig.Item2Val);
	//m_combox2.GetWindowText(g_sorFileArray.reportConfig.Item2);

	//GetDlgItem(IDC_EDIT_STEP2_ITEM3)->GetWindowText(g_sorFileArray.reportConfig.Item3Val);
	//m_combox3.GetWindowText(g_sorFileArray.reportConfig.Item3);

	//GetDlgItem(IDC_EDIT_STEP2_ITEM4)->GetWindowText(g_sorFileArray.reportConfig.Item4Val);
	//m_combox4.GetWindowText(g_sorFileArray.reportConfig.Item4);

	//GetDlgItem(IDC_EDIT_STEP2_ITEM5)->GetWindowText(g_sorFileArray.reportConfig.Item5Val);
	//m_combox5.GetWindowText(g_sorFileArray.reportConfig.Item5);

	//GetDlgItem(IDC_EDIT_STEP2_ITEM6)->GetWindowText(g_sorFileArray.reportConfig.Item6Val);
	//m_combox6.GetWindowText(g_sorFileArray.reportConfig.Item6);

	//GetDlgItem(IDC_EDIT_STEP2_ITEM7)->GetWindowText(g_sorFileArray.reportConfig.Item7Val);
	//m_combox7.GetWindowText(g_sorFileArray.reportConfig.Item7);

	//GetDlgItem(IDC_EDIT_STEP2_ITEM8)->GetWindowText(g_sorFileArray.reportConfig.Item8Val);
	//m_combox8.GetWindowText(g_sorFileArray.reportConfig.Item8);

	//GetDlgItem(IDC_EDIT_STEP2_ITEM9)->GetWindowText(g_sorFileArray.reportConfig.Item9Val);
	//m_combox9.GetWindowText(g_sorFileArray.reportConfig.Item9);

	//GetDlgItem(IDC_EDIT_STEP2_ITEM10)->GetWindowText(g_sorFileArray.reportConfig.Item10Val);
	//m_combox10.GetWindowText(g_sorFileArray.reportConfig.Item10);

	//GetDlgItem(IDC_EDIT_STEP2_ITEM11)->GetWindowText(g_sorFileArray.reportConfig.Item11Val);
	//m_combox11.GetWindowText(g_sorFileArray.reportConfig.Item11);

	//GetDlgItem(IDC_EDIT_STEP2_ITEM12)->GetWindowText(g_sorFileArray.reportConfig.Item12Val);
	//m_combox12.GetWindowText(g_sorFileArray.reportConfig.Item12);
}

BOOL CPropStep3Rpt::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
	SetLocalString();

	/*                      总损耗                            */
	//CString strWaveFormat = IniUtil::ReadResString(_T("IDS_REPORT_STEP3_OUTPUT_FORMAT"));
	//GetDlgItem(IDC_STATIC_STEP3_WAVEFORMAT)->SetWindowText(strWaveFormat);
	////附加总损耗
	//CString strWaveCount = IniUtil::ReadResString(_T("IDS_REPORT_STEP3_WAVE_COUNT"));
	//GetDlgItem(IDC_STATIC_STEP3_WAVEVAL)->SetWindowText(strWaveCount);
	//CString strWave;
	//int nNumberOfWaveLength = _wtoi(g_sorFileArray.reportConfig.NumberOfWaveLength);
	//switch (nNumberOfWaveLength)
	//{
	//case 0:
	//	strWave=_T("波长1");
	//	break;
	//case 1:
	//	strWave=_T("波长2");
	//	break;
	//case 2:
	//	strWave=_T("波长3");
	//	break;
	//}


	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CPropStep3Rpt::SetLocalString()
{
	/*                      总损耗                            */
	//CString strWaveFormat = IniUtil::ReadResString(_T("IDS_REPORT_STEP3_OUTPUT_FORMAT"));
	//GetDlgItem(IDC_STATIC_STEP3_WAVEFORMAT)->SetWindowText(strWaveFormat);
	////附加总损耗
	//CString strWaveCount = IniUtil::ReadResString(_T("IDS_REPORT_STEP3_WAVE_COUNT"));
	//GetDlgItem(IDC_STATIC_STEP3_WAVEVAL)->SetWindowText(strWaveCount);
	//CString strWave;
	//int nNumberOfWaveLength = _wtoi(g_sorFileArray.reportConfig.NumberOfWaveLength);
	//switch (nNumberOfWaveLength)
	//{
	//case 0:
	//	strWave=_T("波长1");
	//	break;
	//case 1:
	//	strWave=_T("波长2");
	//	break;
	//case 2:
	//	strWave=_T("波长3");
	//	break;
	//}
}