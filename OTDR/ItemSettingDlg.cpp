// ItemSettingDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "OTDR.h"
#include "ItemSettingDlg.h"
#include "PropStep2.h"

extern SorFileArray g_sorFileArray;

// CItemSettingDlg 对话框

IMPLEMENT_DYNAMIC(CItemSettingDlg, CDialog)

CItemSettingDlg::CItemSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CItemSettingDlg::IDD, pParent)
{

}

CItemSettingDlg::~CItemSettingDlg()
{
}

void CItemSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_SELECT, m_list);
	DDX_Control(pDX, IDC_LIST_USE_ITEM, m_listSelect);
}


BEGIN_MESSAGE_MAP(CItemSettingDlg, CDialog)
	ON_BN_CLICKED(IDC_BTN_SETTING, &CItemSettingDlg::OnBnClickedBtnSetting)
	ON_BN_CLICKED(IDC_BTN_CLEARALL, &CItemSettingDlg::OnBnClickedBtnClearall)
	ON_BN_CLICKED(IDC_BTN_CLEAR, &CItemSettingDlg::OnBnClickedBtnClear)
	ON_LBN_SELCHANGE(IDC_LIST_SELECT, &CItemSettingDlg::OnLbnSelchangeListSelect)
	ON_LBN_DBLCLK(IDC_LIST_SELECT, &CItemSettingDlg::OnLbnDblclkListSelect)
END_MESSAGE_MAP()


// CItemSettingDlg 消息处理程序

void CItemSettingDlg::OnBnClickedBtnSetting()
{
	CString strSelectItem;
	GetDlgItem(IDC_EDIT_ITEM)->GetWindowText(strSelectItem);
	if (strSelectItem.IsEmpty())
		return;

	int nPos = m_listSelect.FindString(0, strSelectItem);
	if (nPos != LB_ERR)
		return;
	//添加到选择项目
	m_listSelect.AddString(strSelectItem);
}

void CItemSettingDlg::OnBnClickedBtnClearall()
{
	//重置listbox
	m_listSelect.ResetContent();
	//添加新项
	m_listSelect.AddString(_T("(注释)"));
	m_listSelect.AddString(_T("(无)"));
}

void CItemSettingDlg::OnBnClickedBtnClear()
{
	CString strSelectItem;
	int nCurPos = m_listSelect.GetCurSel();
	if (nCurPos == LB_ERR)
		return;

	m_listSelect.GetText(nCurPos, strSelectItem);
	if (!strSelectItem.IsEmpty())
	{
		if (strSelectItem.CompareNoCase(_T("(注释)")) != 0 &&
			strSelectItem.CompareNoCase(_T("(无)")) != 0 )
		{
			m_listSelect.DeleteString(nCurPos);
		}
	}
}

BOOL CItemSettingDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	/*          m_list                            */
	m_list.AddString(_T("位置"));
	m_list.AddString(_T("光纤类型"));
	m_list.AddString(_T("光缆长度"));
	m_list.AddString(_T("熔接点数"));
	m_list.AddString(_T("熔接点数"));
	m_list.AddString(_T("反射事件数"));
	m_list.AddString(_T("测量时间"));
	m_list.AddString(_T("操作者"));
	m_list.AddString(_T("波长"));
	m_list.AddString(_T("测试设备"));
	m_list.AddString(_T("测试地点1"));
	m_list.AddString(_T("测试地点2"));
	m_list.AddString(_T("气候"));
	m_list.AddString(_T("温度"));
	
	/*          m_listSelect                            */
	if (!g_sorFileArray.reportConfig.Combox1.IsEmpty())
		m_listSelect.AddString(g_sorFileArray.reportConfig.Combox1);
	if (!g_sorFileArray.reportConfig.Combox2.IsEmpty())
		m_listSelect.AddString(g_sorFileArray.reportConfig.Combox2);
	if (!g_sorFileArray.reportConfig.Combox3.IsEmpty())
		m_listSelect.AddString(g_sorFileArray.reportConfig.Combox3);
	if (!g_sorFileArray.reportConfig.Combox4.IsEmpty())
		m_listSelect.AddString(g_sorFileArray.reportConfig.Combox4);
	if (!g_sorFileArray.reportConfig.Combox5.IsEmpty())
		m_listSelect.AddString(g_sorFileArray.reportConfig.Combox5);
	if (!g_sorFileArray.reportConfig.Combox6.IsEmpty())
		m_listSelect.AddString(g_sorFileArray.reportConfig.Combox6);
	if (!g_sorFileArray.reportConfig.Combox7.IsEmpty())
		m_listSelect.AddString(g_sorFileArray.reportConfig.Combox7);
	if (!g_sorFileArray.reportConfig.Combox8.IsEmpty())
		m_listSelect.AddString(g_sorFileArray.reportConfig.Combox8);
	if (!g_sorFileArray.reportConfig.Combox9.IsEmpty())
		m_listSelect.AddString(g_sorFileArray.reportConfig.Combox9);
	if (!g_sorFileArray.reportConfig.Combox10.IsEmpty())
		m_listSelect.AddString(g_sorFileArray.reportConfig.Combox10);
	if (!g_sorFileArray.reportConfig.Combox11.IsEmpty())
		m_listSelect.AddString(g_sorFileArray.reportConfig.Combox11);
	if (!g_sorFileArray.reportConfig.Combox12.IsEmpty())
		m_listSelect.AddString(g_sorFileArray.reportConfig.Combox12);
	if (!g_sorFileArray.reportConfig.Combox13.IsEmpty())
		m_listSelect.AddString(g_sorFileArray.reportConfig.Combox13);

	int nPos = m_listSelect.FindString(0, _T("(注释)"));
	if (nPos == LB_ERR)
		m_listSelect.AddString(_T("(注释)"));

	nPos = m_listSelect.FindString(0, _T("(无)"));
	if (nPos == LB_ERR)
		m_listSelect.AddString(_T("(无)"));

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CItemSettingDlg::OnLbnSelchangeListSelect()
{
	CString strSelectItem;
	int nCurPos = m_list.GetCurSel();
	m_list.GetText(nCurPos, strSelectItem);
	GetDlgItem(IDC_EDIT_ITEM)->SetWindowText(strSelectItem);
}

void CItemSettingDlg::OnLbnDblclkListSelect()
{
	CString strSelectItem;
	int nCurPos = m_list.GetCurSel();
	m_list.GetText(nCurPos, strSelectItem);

	int nPos = m_listSelect.FindString(0, strSelectItem);
	if (nPos != LB_ERR)
		return;
	//添加到选择项目
	m_listSelect.AddString(strSelectItem);
}

void CItemSettingDlg::OnOK()
{
	int nCount = m_listSelect.GetCount();
	CString listItems[15];
	if (nCount >=15)
		nCount = 14;
	for (int i=0; i<nCount; i++)
	{
		m_listSelect.GetText(i, listItems[i]);
	}

	g_sorFileArray.reportConfig.Combox1 = listItems[0];
	g_sorFileArray.reportConfig.Combox2 = listItems[1];
	g_sorFileArray.reportConfig.Combox3 = listItems[2];
	g_sorFileArray.reportConfig.Combox4 = listItems[3];
	g_sorFileArray.reportConfig.Combox5 = listItems[4];
	g_sorFileArray.reportConfig.Combox6 = listItems[5];
	g_sorFileArray.reportConfig.Combox7 = listItems[6];
	g_sorFileArray.reportConfig.Combox8 = listItems[7];
	g_sorFileArray.reportConfig.Combox9 = listItems[8];
	g_sorFileArray.reportConfig.Combox10 = listItems[9];
	g_sorFileArray.reportConfig.Combox11 = listItems[10];
	g_sorFileArray.reportConfig.Combox12 = listItems[11];
	g_sorFileArray.reportConfig.Combox13 = listItems[12];
	g_sorFileArray.reportConfig.Combox14 = listItems[13];
	g_sorFileArray.reportConfig.Combox15 = listItems[14];

	CDialog::OnOK();
}
