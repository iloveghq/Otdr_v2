// PageMainSetting.cpp : 实现文件
//

#include "stdafx.h"
#include "OTDR.h"
#include "PageMainSetting.h"
#include "Command.h"
#include "EventGenPageSheet.h"

extern SorFileArray g_sorFileArray;
// CPageMainSetting 对话框

IMPLEMENT_DYNAMIC(CPageMainSetting, CPropertyPage)

CPageMainSetting::CPageMainSetting()
	: CPropertyPage(CPageMainSetting::IDD)
{

}

CPageMainSetting::~CPageMainSetting()
{
}

void CPageMainSetting::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_STEP5_WAVE, m_fileList);
}


BEGIN_MESSAGE_MAP(CPageMainSetting, CPropertyPage)
	ON_BN_CLICKED(IDC_BTN_STEP5_OPENFILE, &CPageMainSetting::OnBnClickedBtnStep5Openfile)
	ON_BN_CLICKED(IDC_BTN_STEP5_AUTO, &CPageMainSetting::OnBnClickedBtnStep5Auto)
END_MESSAGE_MAP()


// CPageMainSetting 消息处理程序

BOOL CPageMainSetting::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
	//初始化ListFile
	InitListFile();

	SetLocalString();

	/*                      主事件设置                            */
	GetDlgItem(IDC_BTN_STEP5_OPENFILE)->EnableWindow(FALSE);
	CButton* pBtnFilePos = (CButton*)GetDlgItem(IDC_RADIO_STEP5_POS);
	pBtnFilePos->SetCheck(TRUE);
	//反向
	GetDlgItem(IDC_RADIO_STEP5_NEG)->EnableWindow(FALSE);

	m_potdrTrace=&MAIN->m_pWaveFormView->m_sorDrawStatic;

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CPageMainSetting::SetLocalString()
{
	/*                      主事件设置                            */
	CString strMasterSetting = IniUtil::ReadResString(_T("IDS_REPORT_STEP5_MASTEREVENT_SETTING"));
	GetDlgItem(IDC_STATIC_STEP5_TITLE)->SetWindowText(strMasterSetting);
	//打开文件
	CString strOpenFile = IniUtil::ReadResString(_T("IDS_REPORT_STEP5_OPENFILE"));
	GetDlgItem(IDC_BTN_STEP5_OPENFILE)->SetWindowText(strOpenFile);
	//文件名
	CString strFileName = IniUtil::ReadResString(_T("IDS_REPORT_STEP5_FILENAME"));
	GetDlgItem(IDC_STATIC_STEP5_FILENAME)->SetWindowText(strFileName);
	//自动生成
	CString strAuto = IniUtil::ReadResString(_T("IDS_REPORT_STEP5_AUTO_GENERATE"));
	GetDlgItem(IDC_BTN_STEP5_AUTO)->SetWindowText(strAuto);
	/*                      波形文件                            */
	CString strWaveFile = IniUtil::ReadResString(_T("IDS_REPORT_STEP5_WAVEFILE"));
	GetDlgItem(IDC_STATIC_STEP5_WAVEFILE)->SetWindowText(strWaveFile);
	//方向
	CString strDirect = IniUtil::ReadResString(_T("IDS_REPORT_STEP5_DIRECT"));
	GetDlgItem(IDC_STATIC_STEP5_DIRECT)->SetWindowText(strDirect);
	//正向
	CString strPos = IniUtil::ReadResString(_T("IDS_REPORT_STEP5_DIRECT_POS"));
	CButton* pBtnFilePos = (CButton*)GetDlgItem(IDC_RADIO_STEP5_POS);
	pBtnFilePos->SetWindowText(strPos);
	//反向
	CString strNeg = IniUtil::ReadResString(_T("IDS_REPORT_STEP5_DIRECT_NEG"));
	GetDlgItem(IDC_RADIO_STEP5_NEG)->SetWindowText(strNeg);
	//正向波形
	CString strWavePos = IniUtil::ReadResString(_T("IDS_REPORT_STEP5_WAVE_POS"));
	GetDlgItem(IDC_STATIC_STEP5_POS_WAVE)->SetWindowText(strWavePos);
	//反向波形
	CString strWaveNeg = IniUtil::ReadResString(_T("IDS_REPORT_STEP5_WAVE_NEG"));
	GetDlgItem(IDC_STATIC_STEP5_NEG_WAVE)->SetWindowText(strWaveNeg);
	/*                      编辑模式                            */
	CString strEditMode= IniUtil::ReadResString(_T("IDS_REPORT_STEP5_EDIT_MODE"));
	GetDlgItem(IDC_STATIC_STEP5_EDITMODE)->SetWindowText(strEditMode);
	//编辑事件
	CString strEventEdit = IniUtil::ReadResString(_T("IDS_REPORT_STEP5_EVENT_MODE"));
	GetDlgItem(IDC_RADIO_STEP5_EDITEVENT)->SetWindowText(strEventEdit);
	//编辑总损耗标记
	CString strAllLossMark = IniUtil::ReadResString(_T("IDS_REPORT_STEP5_EDIT_ALLLOSS_MARK"));
	GetDlgItem(IDC_RADIO_STEP5_LOSSMARK)->SetWindowText(strAllLossMark);
	//编辑输出波形比例
	CString strWaveScale = IniUtil::ReadResString(_T("IDS_REPORT_STEP5_WAVE_SCALE"));
	GetDlgItem(IDC_RADIO_STEP5_OUTPUT_WAVE)->SetWindowText(strWaveScale);
	//存储主文件
	CString strMaster = IniUtil::ReadResString(_T("IDS_REPORT_STEP5_MASTER"));
	GetDlgItem(IDC_BTN_STEP5_STORAGE)->SetWindowText(strMaster);
}

void CPageMainSetting::InitListFile()
{
	//******初始化m_fileList
	m_fileList.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	CString strOrder=_T("No");
	CString strName=_T("文件名");
	m_fileList.InsertColumn(0,strOrder,LVCFMT_LEFT,30);
	m_fileList.InsertColumn(1,strName,LVCFMT_LEFT,130);
}

void CPageMainSetting::PopulateFileList()
{
	g_sorFileArray.Reset();
	//填充文件列表
	int item_Count=MAIN->m_pReportFormView->m_ReportWizardDlg.m_pPropSheet->m_PropStep4.m_pageFileSelect.m_fileSelectDlg.m_strSelectFiles.GetCount();
	if (0!=item_Count)
	{
		m_fileList.DeleteAllItems();
	}
	for (int i=0; i < item_Count; i++)
	{
		CString strFileIndex;
		strFileIndex.Format(_T("%d"), i+1);
		CString strFile;
		strFile = MAIN->m_pReportFormView->m_ReportWizardDlg.m_pPropSheet->m_PropStep4.m_pageFileSelect.m_fileSelectDlg.m_strSelectFiles.GetAt(i);
		m_fileList.InsertItem(i, _T(""));
		m_fileList.SetItemText(i, 0, strFileIndex);
		m_fileList.SetItemText(i, 1, strFile);
		//显示曲线
		OpenCurveFile(strFile);
	}
}

void CPageMainSetting::AddFileList()
{
	//填充文件列表
	int item_Count=MAIN->m_pReportFormView->m_ReportWizardDlg.m_pPropSheet->m_PropStep4.m_pageFileSelect.m_fileSelectDlg.m_strSelectFiles.GetCount();
	if (0!=item_Count)
	{
		m_fileList.DeleteAllItems();
	}
	m_potdrTrace->m_vAllData.clear();//zll 2013.6.6 晚 及时清空m_vAllData的数据，否则m_vAllData数据一直增加，若用户反复对同一组曲线“上一页”“下一页”，则会出现曲线打开但是颜色为黑色的情况
	for (int i=0; i < MAIN->m_pReportFormView->m_ReportWizardDlg.m_pPropSheet->m_PropStep4.m_pageFileSelect.m_fileSelectDlg.m_strSelectFiles.GetCount(); i++)
	{
		CString strFileIndex;
		strFileIndex.Format(_T("%d"), i+1);
		CString strFile;
		strFile = MAIN->m_pReportFormView->m_ReportWizardDlg.m_pPropSheet->m_PropStep4.m_pageFileSelect.m_fileSelectDlg.m_strSelectFiles.GetAt(i);
		m_fileList.InsertItem(i, _T(""));
		m_fileList.SetItemText(i, 0, strFileIndex);
		m_fileList.SetItemText(i, 1, strFile);
		//仅显示8条曲线 
		if (i<8)
		{
			OpenCurveFile(strFile);//打开并显示曲线
		}
		else
			continue;//第9条曲线开始不用打开并显示，否则浪费时间 zll 2013.6.8
	}
	//_CrtDumpMemoryLeaks();//调试内存泄露
}

void CPageMainSetting::OnBnClickedBtnStep5Openfile()
{
	MAIN->OnOpen();
}

void CPageMainSetting::OnBnClickedBtnStep5Auto()
{
	CEventGenPageSheet configSheet(_T("设置"));
	configSheet.DoModal();
}
