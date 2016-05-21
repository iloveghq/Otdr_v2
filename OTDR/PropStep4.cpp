// PropStep4.cpp : 实现文件
//

#include "stdafx.h"
#include "OTDR.h"
#include "PropStep4.h"
#include "MainFrm.h"

extern SorFileArray g_sorFileArray;
// CPropStep4 对话框

IMPLEMENT_DYNAMIC(CPropStep4, CPropertyPage)

CPropStep4::CPropStep4()
: CPropertyPage(CPropStep4::IDD)
{

}

CPropStep4::~CPropStep4()
{
}

void CPropStep4::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SPIN_STEP4_CORENUM, m_spinFiberNum);
	DDX_Control(pDX, IDC_SPIN_STEP4_START_CORENUM, m_spinStartFiber);
	DDX_Control(pDX, IDC_EDIT_STEP4_CORENUM, m_editFiberNum);
	DDX_Control(pDX, IDC_EDIT_STEP4_START_CORENUM, m_editStartFiber);
	DDX_Control(pDX, IDC_TAB_STEP4, m_tab);
}


BEGIN_MESSAGE_MAP(CPropStep4, CPropertyPage)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_STEP4_CORENUM, &CPropStep4::OnDeltaposSpinFiberNum)
END_MESSAGE_MAP()


// CPropStep4 消息处理程序
BOOL CPropStep4::OnSetActive() 
{
	// TODO: Add your specialized code here and/or call the base class
	//((CPropertySheet*)GetParent())->SetWizardButtons(PSWIZB_NEXT);
	((CPropSheet*)GetParent())->SetReportHeadBtn(4);
	//重绘图形区域
	MAIN->m_pReportFormView->m_ReportViewDlg.m_DrawButton.ReDrawRect(4);
	((CPropSheet*)GetParent())->EnableNextBtn(TRUE);
	((CPropSheet*)GetParent())->EnablePrevBtn(TRUE);
	return CPropertyPage::OnSetActive();
}

LRESULT CPropStep4::OnWizardNext() 
{
	// TODO: Add your specialized code here and/or call the base class
	UpdateData();
	RecordSetting();
	if (m_pageFileSelect.IsFilePosSelect() == FALSE)
	{
		CString strMsg=IniUtil::ReadResString(_T("IDS_REPORT_STEP4_CHOOSEFILES"));
		MessageBox(strMsg, _T("OTDR Viewer"), MB_OK|MB_ICONEXCLAMATION);
		return -1;
	}
	
	//显示主设置窗口
	MAIN->ShowMainSetting(TRUE);
	//填充文件列表
	MAIN->m_pMainSetting->m_pageMainSetting.AddFileList();

	return 0;
}

BOOL CPropStep4::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	//初始化Tab页
	InitTab();
	//设置本地化
	SetLocalString();
	int nNumerOfFiber = _wtoi(g_sorFileArray.reportConfig.NumberOfFiber);
	m_spinFiberNum.SetSpin(0, 1, 8, 1);
	m_spinFiberNum.SetPos(nNumerOfFiber);
	//起始芯
	int nStartOfFiberNo = _wtoi(g_sorFileArray.reportConfig.StartOfFiberNo);
	m_spinStartFiber.SetSpin(0, 1, 8, 1);
	m_spinStartFiber.SetPos(nStartOfFiberNo);
	GetDlgItem(IDC_EDIT_STEP4_WAVENUM)->SetWindowText(g_sorFileArray.reportConfig.NumberOfWaveLength);
	GetDlgItem(IDC_EDIT_STEP4_WAVENUM)->EnableWindow(FALSE);
	//测试方向
	if (g_sorFileArray.reportConfig.MeasureDir.CompareNoCase(_T("1")))
		GetDlgItem(IDC_EDIT_STEP4_TEST_DIRECT)->SetWindowText(_T("双向"));
	else
		GetDlgItem(IDC_EDIT_STEP4_TEST_DIRECT)->SetWindowText(_T("单向"));
	GetDlgItem(IDC_EDIT_STEP4_TEST_DIRECT)->EnableWindow(FALSE);
	//文件类型
	CComboBox* pCommboxFileType = (CComboBox*)GetDlgItem(IDC_COMBO_FILETYPE);
	pCommboxFileType->InsertString(-1, _T("SOR(*.SOR)"));
	pCommboxFileType->SetCurSel(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pag	es should return FALSE
}

void CPropStep4::SetLocalString()
{
	/*                      文件信息                            */
	TCITEM item;
	CString strTitle = IniUtil::ReadResString(_T("IDS_REPORT_STEP5_TAB_TITLE"));
	item.pszText=strTitle.GetBuffer();
	item.mask=TCIF_TEXT;
	m_tab.SetItem(0,&item);
	strTitle.ReleaseBuffer();

	CString strFileInfo = IniUtil::ReadResString(_T("IDS_REPORT_STEP4_FILE_INFO"));
	GetDlgItem(IDC_STATIC_STEP4_FILEINFO)->SetWindowText(strFileInfo);
	//芯数
	CString strFiberNumber = IniUtil::ReadResString(_T("IDS_REPORT_STEP4_FIBER_NUMBER"));
	GetDlgItem(IDC_STATIC_STEP4_CORENUM)->SetWindowText(strFiberNumber);
	//起始芯
	CString strStartFiber = IniUtil::ReadResString(_T("IDS_REPORT_STEP4_START_FIBER"));
	GetDlgItem(IDC_STATIC_STEP4_START_CORENUM)->SetWindowText(strStartFiber);
	//文件选择方法
	CString strFileSelect = IniUtil::ReadResString(_T("IDS_REPORT_STEP4_FILE_SELECT_METHOD"));
	GetDlgItem(IDC_STATIC_STEP4_FILEMETHOD)->SetWindowText(strFileSelect);
	//波长数
	CString strWaveNum = IniUtil::ReadResString(_T("IDS_REPORT_STEP4_WAVE_COUNT"));
	GetDlgItem(IDC_STATIC_STEP4_WAVENUM)->SetWindowText(strWaveNum);
	//测试方向
	CString strTestDirect = IniUtil::ReadResString(_T("IDS_REPORT_STEP4_TEST_DIRECTION"));
	GetDlgItem(IDC_STATIC_STEP4_TESTDIRECT)->SetWindowText(strTestDirect);
	//文件类型
	CString strFileFormat= IniUtil::ReadResString(_T("IDS_REPORT_STEP4_FILE_FORMAT"));
	GetDlgItem(IDC_STATIC_STEP4_FILETYPE)->SetWindowText(strFileFormat);
	m_pageFileSelect.SetLocalString();
}

void CPropStep4::InitTab()
{
	//初始化m_tab控件上的属性页标签
	//设置tab标题
	CString strTitle = IniUtil::ReadResString(_T("IDS_REPORT_STEP5_TAB_TITLE"));
	m_tab.InsertItem(0,strTitle);

	//创建属性页;
	m_pageFileSelect.Create(IDD_REPORT_FILE_SELECT,GetDlgItem(IDC_TAB_STEP4));

	//获取TAB的客户端矩形框，从而设置各属性页在TAB上的物理位置
	CRect rs;
	m_tab.GetClientRect(&rs);
	rs.top+=20;
	rs.bottom-=4;
	rs.left+=4;
	rs.right-=4;

	//设置属性页的大小和位置
	m_pageFileSelect.MoveWindow(&rs);

	//默认第一页显示
	m_pageFileSelect.ShowWindow(TRUE);
	m_tab.SetCurSel(0);

}

void CPropStep4::RecordSetting() //保存配置信息
{
	GetDlgItem(IDC_EDIT_STEP4_CORENUM)->GetWindowText(g_sorFileArray.reportConfig.NumberOfFiber);
	GetDlgItem(IDC_EDIT_STEP4_START_CORENUM)->GetWindowText(g_sorFileArray.reportConfig.StartOfFiberNo);
}

void CPropStep4::OnDeltaposSpinFiberNum(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItem(IDC_EDIT_STEP4_CORENUM)->GetWindowText(g_sorFileArray.reportConfig.NumberOfFiber);
	*pResult = 0;
}