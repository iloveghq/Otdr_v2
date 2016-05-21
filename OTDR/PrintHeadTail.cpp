// PrintHeadTail.cpp : 实现文件
//

#include "stdafx.h"
#include "OTDR.h"
#include "PrintHeadTail.h"

extern SorFileArray g_sorFileArray;

// CPrintHeadTail 对话框

IMPLEMENT_DYNAMIC(CPrintHeadTail, CPropertyPage)

CPrintHeadTail::CPrintHeadTail()
	: CPropertyPage(CPrintHeadTail::IDD)
{

}

CPrintHeadTail::~CPrintHeadTail()
{
}

void CPrintHeadTail::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SPIN_STARTNUM, m_spinStartNum);
	DDX_Control(pDX, IDC_COMBO_UPDOWN, m_comboxUpDown);
	DDX_Control(pDX, IDC_COMBO_LEFTRIGHT, m_comboxLeftRight);
	DDX_Control(pDX, IDC_SPIN_CHAPTERNUM, m_spinSectionNum);
}


BEGIN_MESSAGE_MAP(CPrintHeadTail, CPropertyPage)
END_MESSAGE_MAP()


// CPrintHeadTail 消息处理程序

BOOL CPrintHeadTail::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	/*               页号                                                  */
	CString strPageNum = IniUtil::ReadResString(_T("IDS_PRINT_PAGENUM"));
	GetDlgItem(IDC_STATIC_PAGENUM)->SetWindowText(strPageNum);
	//增加页号
	CString strAddPage = IniUtil::ReadResString(_T("IDS_PRINT_ADDPAGE"));
	CButton* pBtnAddPage = (CButton*)GetDlgItem(IDC_CHECK_ADDNUM);
	pBtnAddPage->SetWindowText(strAddPage);
	if (g_sorFileArray.printConfig.AddPage.CompareNoCase(_T("1")) == 0)
	{
		pBtnAddPage->SetCheck(TRUE);
	}
	else
	{
		pBtnAddPage->SetCheck(FALSE);
	}
	//起始号
	CString strStartNum = IniUtil::ReadResString(_T("IDS_PRINT_STARTNO"));
	GetDlgItem(IDC_STATIC_START_NUM)->SetWindowText(strStartNum);
	int nPageNum = _wtoi(g_sorFileArray.printConfig.PageNum);
	m_spinStartNum.SetSpin(0,1,200000,1);
	m_spinStartNum.SetPos(nPageNum);
	//打印位置
	CString strPrintPos = IniUtil::ReadResString(_T("IDS_PRINT_POS"));
	GetDlgItem(IDC_STATIC_PRINT_POS)->SetWindowText(strPrintPos);
	m_comboxUpDown.InsertString(-1, _T("上"));/* 上 */
	m_comboxUpDown.InsertString(-1, _T("下"));/* 下 */
	int nPagePos1 = _wtoi(g_sorFileArray.printConfig.PagePos1);
	m_comboxUpDown.SetCurSel(nPagePos1);

	m_comboxLeftRight.InsertString(-1, _T("左"));/* 左 */
	m_comboxLeftRight.InsertString(-1, _T("中"));/* 中 */
	m_comboxLeftRight.InsertString(-1, _T("右"));/* 右 */
	int nPagePos2 = _wtoi(g_sorFileArray.printConfig.PagePos2);
	m_comboxLeftRight.SetCurSel(nPagePos2);
	/*               章节号                                                    */
	CString strSection = IniUtil::ReadResString(_T("IDS_PRINT_SECTION"));
	GetDlgItem(IDC_STATIC_CHAPTER)->SetWindowText(strSection);
	//增加章节号
	CString strAddSection = IniUtil::ReadResString(_T("IDS_PRINT_ADDSECTION"));
	CButton* pBtnAddSection = (CButton*)GetDlgItem(IDC_CHECK_ADDCHAPTER);
	pBtnAddSection->SetWindowText(strAddSection);
	if (g_sorFileArray.printConfig.AddSection.CompareNoCase(_T("1")) == 0)
	{
		pBtnAddSection->SetCheck(TRUE);
	}
	else
	{
		pBtnAddSection->SetCheck(FALSE);
	}
	//章节号
	CString strSectionNum = IniUtil::ReadResString(_T("IDS_PRINT_SECTIONNUM"));
	GetDlgItem(IDC_STATIC_CHAPTERNUM)->SetWindowText(strSectionNum);
	int nSectionNum = _wtoi(g_sorFileArray.printConfig.SectionNum);
	m_spinSectionNum.SetSpin(0,1,1000,1);
	m_spinSectionNum.SetPos(nSectionNum);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

BOOL CPrintHeadTail::OnApply()
{
	// TODO: 在此添加专用代码和/或调用基类

	return CPropertyPage::OnApply();
}
