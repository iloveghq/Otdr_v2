// Step4FileSelect.cpp : 实现文件
//

#include "stdafx.h"
#include "OTDR.h"
#include "Step4FileSelect.h"

extern SorFileArray g_sorFileArray;

// CStep4FileSelect 对话框

IMPLEMENT_DYNAMIC(CStep4FileSelect, CPropertyPage)

CStep4FileSelect::CStep4FileSelect()
	: CPropertyPage(CStep4FileSelect::IDD)
{

}

CStep4FileSelect::~CStep4FileSelect()
{
}

void CStep4FileSelect::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_STEP4, m_listFilePos);
	DDX_Control(pDX, IDC_LIST_STEP4_FLIP, m_listFileNeg);
}


BEGIN_MESSAGE_MAP(CStep4FileSelect, CPropertyPage)
	ON_BN_CLICKED(IDC_BTN_STEP4_FILESELECT_POS, &CStep4FileSelect::OnBnClickedBtnStep4FileselectPos)
	ON_BN_CLICKED(IDC_BTN_STEP4_FILESELECT_NEG, &CStep4FileSelect::OnBnClickedBtnStep4FileselectNeg)
END_MESSAGE_MAP()


// CStep4FileSelect 消息处理程序
BOOL CStep4FileSelect::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	//初始化文件列表
	InitListFilePos();
	InitListFileNeg();
	//设置本地化
	SetLocalString();

	GetDlgItem(IDC_EDIT_STEP4_FOLDER_POS)->SetWindowText(g_sorFileArray.reportConfig.Folder1);
	GetDlgItem(IDC_EDIT_STEP4_FOLDER_NEG)->SetWindowText(g_sorFileArray.reportConfig.Folder2);
	if (g_sorFileArray.reportConfig.MeasureDir.CompareNoCase(_T("1")) == 1)//双向
	{
		GetDlgItem(IDC_EDIT_STEP4_FOLDER_NEG)->EnableWindow(TRUE);
		GetDlgItem(IDC_LIST_STEP4_FLIP)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_STEP4_FILESELECT_NEG)->EnableWindow(TRUE);
	}
	else//单向
	{
		GetDlgItem(IDC_EDIT_STEP4_FOLDER_NEG)->EnableWindow(FALSE);
		GetDlgItem(IDC_LIST_STEP4_FLIP)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_STEP4_FILESELECT_NEG)->EnableWindow(FALSE);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pag	es should return FALSE
}

void CStep4FileSelect::SetLocalString()
{
	/*                      文件选择                            */
	//正向
	CString strFileSelectPos = IniUtil::ReadResString(_T("IDS_REPORT_STEP4_FILE_SELECT_POS"));
	GetDlgItem(IDC_STATIC_STEP4_FILESELECT_POS)->SetWindowText(strFileSelectPos);
	GetDlgItem(IDC_EDIT_STEP4_FOLDER_POS)->SetWindowText(g_sorFileArray.reportConfig.Folder1);
	//反向
	CString strFileSelectNeg = IniUtil::ReadResString(_T("IDS_REPORT_STEP4_FILE_SELECT_NEG"));
	GetDlgItem(IDC_STATIC_STEP4_FILESELECT_NEG)->SetWindowText(strFileSelectNeg);
	GetDlgItem(IDC_EDIT_STEP4_FOLDER_NEG)->SetWindowText(g_sorFileArray.reportConfig.Folder2);
	if (g_sorFileArray.reportConfig.MeasureDir.CompareNoCase(_T("1")) == 1)//双向
	{
		GetDlgItem(IDC_EDIT_STEP4_FOLDER_NEG)->EnableWindow(TRUE);
		GetDlgItem(IDC_LIST_STEP4_FLIP)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_STEP4_FILESELECT_NEG)->EnableWindow(TRUE);
	}
	else//单向
	{
		GetDlgItem(IDC_EDIT_STEP4_FOLDER_NEG)->EnableWindow(FALSE);
		GetDlgItem(IDC_LIST_STEP4_FLIP)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_STEP4_FILESELECT_NEG)->EnableWindow(FALSE);
	}
	//文件夹
	CString strFolder = IniUtil::ReadResString(_T("IDS_REPORT_STEP4_FILE_FOLDER"));
	GetDlgItem(IDC_STATIC_STEP4_FOLDER_POS)->SetWindowText(strFolder);
	GetDlgItem(IDC_STATIC_STEP4_FOLDER_NEG)->SetWindowText(strFolder);
	//文件选择
	CString strFileSelect = IniUtil::ReadResString(_T("IDS_REPORT_STEP4_FILE_SELECT"));
	GetDlgItem(IDC_BTN_STEP4_FILESELECT_POS)->SetWindowText(strFileSelect);
	GetDlgItem(IDC_BTN_STEP4_FILESELECT_NEG)->SetWindowText(strFileSelect);

	//ListCtrl
	SetListViewText(m_listFileNeg);
	SetListViewText(m_listFilePos);
}

void CStep4FileSelect::SetListViewText(CListCtrl& listCtrl)
{
	//序号
	CString strOrder = IniUtil::ReadResString(_T("IDS_REPORT_STEP4_FILESELECT_ORDER"));
	SetListColName(listCtrl, 0, strOrder);//更改列标题

	//文件名
	CString strFileName = IniUtil::ReadResString(_T("IDS_REPORT_STEP4_FILESELECT_FILENAME")); 
	SetListColName(listCtrl,1,strFileName);//更改列标题
}

void CStep4FileSelect::SetListColName(CListCtrl& listCtrl, int nColIndex, const CString& colName)
{
	LVCOLUMN col;
	col.mask= LVCF_TEXT;
	col.pszText= (LPWSTR)(LPCWSTR)colName;
	col.cchTextMax= 256;

	listCtrl.SetColumn(nColIndex, &col);
}

void CStep4FileSelect::InitListFilePos()
{
	//******初始化m_listFilePos
	m_listFilePos.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	CString strOrder=_T("序号");
	CString strName=_T("文件名");
	m_listFilePos.InsertColumn(0,strOrder,LVCFMT_LEFT,100);
	m_listFilePos.InsertColumn(1,strName,LVCFMT_LEFT,250);
}

void CStep4FileSelect::InitListFileNeg()
{
	//******初始化m_listFileNeg
	m_listFileNeg.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	CString strOrder=_T("序号");
	CString strName=_T("文件名");
	m_listFileNeg.InsertColumn(0,strOrder,LVCFMT_LEFT,100);
	m_listFileNeg.InsertColumn(1,strName,LVCFMT_LEFT,250);
}

//填充文件列表
void CStep4FileSelect::PopulateFileList()
{
	m_listFilePos.DeleteAllItems();
	//g_sorFileArray.Reset();//清除所有上次填充过的曲线,zll 2012.9.22
	for (int i=0; i < m_fileSelectDlg.m_strSelectFiles.GetCount(); i++)
	{
		CString strFileIndex;
		strFileIndex.Format(_T("%d"), i+1);
		CString strFile;
		strFile = m_fileSelectDlg.m_strSelectFiles.GetAt(i);
		m_listFilePos.InsertItem(i, _T(""));
		m_listFilePos.SetItemText(i, 0, strFileIndex);
		m_listFilePos.SetItemText(i, 1, strFile);
	}
}

void CStep4FileSelect::OnBnClickedBtnStep4FileselectPos()
{
	//m_fileSelectDlg.m_strSelectFiles.RemoveAll();//重置文件选择列表
	if(IDOK == m_fileSelectDlg.DoModal())
	{
		GetDlgItem(IDC_EDIT_STEP4_FOLDER_POS)->SetWindowText(g_sorFileArray.reportConfig.Folder1);
		PopulateFileList();
	}
}

void CStep4FileSelect::OnBnClickedBtnStep4FileselectNeg()
{
	m_fileSelectDlg.DoModal();
}

BOOL CStep4FileSelect::IsFilePosSelect() //是否已经选择文件
{
	if(m_listFilePos.GetItemCount() > 0)
		return TRUE;
	else
		return FALSE;
}