// PageSystemSetting.cpp : 实现文件
//

#include "stdafx.h"
#include "OTDR.h"
#include "PageSystemSetting.h"
#include "RemotePageSheet.h"
#include "ClientSocket.h"

extern SorFileArray g_sorFileArray;
CString str_net_connect=_T("");

// CPageSystemSetting 对话框

IMPLEMENT_DYNAMIC(CPageSystemSetting, CPropertyPage)

CPageSystemSetting::CPageSystemSetting()
	: CPropertyPage(CPageSystemSetting::IDD)
{

}

CPageSystemSetting::~CPageSystemSetting()
{
}

void CPageSystemSetting::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER_BRIGHT, m_slideTouchLight);
	DDX_Control(pDX, IDC_DP_DATE, m_date);
	DDX_Control(pDX, IDC_DP_TIME, m_time);
}


BEGIN_MESSAGE_MAP(CPageSystemSetting, CPropertyPage)
	ON_WM_VSCROLL()
	ON_BN_CLICKED(IDC_BTN_OTDR2PC, &CPageSystemSetting::OnBnClickedBtnOtdr2pc)
	ON_BN_CLICKED(IDC_BTN_PC2OTDR, &CPageSystemSetting::OnBnClickedBtnPc2otdr)
END_MESSAGE_MAP()

void CPageSystemSetting::SetLocaleString()
{
	/*                       语言                                           */
	CString strLang = IniUtil::ReadResString(_T("IDS_REMOTE_SETTING_SYSTEM_LANG"));
	GetDlgItem(IDC_STATIC_LANG_SETTING)->SetWindowText(strLang);

	//英语
	CString strEng = IniUtil::ReadResString(_T("IDS_REMOTE_SETTING_SYSTEM_ENG"));
	CButton *pBtnEng = (CButton*)GetDlgItem(IDC_RADIO_ENG);
	pBtnEng->SetWindowText(strEng);
	//中文
	CString strChs = IniUtil::ReadResString(_T("IDS_REMOTE_SETTING_SYSTEM_CHS"));
	CButton *pBtnChs = (CButton*)GetDlgItem(IDC_RADIO_CHS);
	pBtnChs->SetWindowText(strChs);
	CString strLanguage = g_sorFileArray.otdrConfig.strLang;
	if (strLanguage.CompareNoCase(_T("0")) == 0)
	{
		pBtnChs->SendMessage(BM_CLICK, 0, 0);
	}
	else
	{
		pBtnEng->SendMessage(BM_CLICK, 0, 0);
	}
	
	/*                              自动关机                                                  */
	CString strAutoPowerOff = IniUtil::ReadResString(_T("IDS_REMOTE_SETTING_SYSTEM_AUTOPOWEROFF"));
	CButton *pBtnAutoPowerOff = (CButton*)GetDlgItem(IDC_STATIC_AUTO_POWEROFF);
	pBtnAutoPowerOff->SetWindowText(strAutoPowerOff);
	//不关机
	CString strNever = IniUtil::ReadResString(_T("IDS_REMOTE_SETTING_SYSTEM_AUTOPOWEROFF_NEVER"));
	CButton *pBtnNever = (CButton*)GetDlgItem(IDC_RADIO_NEVER);
	pBtnNever->SetWindowText(strNever);
	CButton *pBtn1Min = (CButton*)GetDlgItem(IDC_RADIO_1MIN);
	CButton *pBtn3Min = (CButton*)GetDlgItem(IDC_RADIO_3MIN);
	CButton *pBtn5Min = (CButton*)GetDlgItem(IDC_RADIO_5MIN);
	CButton *pBtn10Min = (CButton*)GetDlgItem(IDC_RADIO_10MIN);
	CButton *pBtn20Min = (CButton*)GetDlgItem(IDC_RADIO_20MIN);
	CButton *pBtn30Min = (CButton*)GetDlgItem(IDC_RADIO_30MIN);
	CButton *pBtn60Min = (CButton*)GetDlgItem(IDC_RADIO_60MIN);
	CString strAutoPower = g_sorFileArray.otdrConfig.strAutoPowerOff;
	if (strAutoPower.CompareNoCase(_T("Never")) == 0)
	{
		pBtnNever->SendMessage(BM_CLICK, 0, 0);
	}
	else if (strAutoPower.CompareNoCase(_T("1")) == 0)
	{
		pBtn1Min->SendMessage(BM_CLICK, 0, 0);
	}
	else if (strAutoPower.CompareNoCase(_T("3")) == 0)
	{
		pBtn3Min->SendMessage(BM_CLICK, 0, 0);
	}
	else if (strAutoPower.CompareNoCase(_T("5")) == 0)
	{
		pBtn5Min->SendMessage(BM_CLICK, 0, 0);
	}
	else if (strAutoPower.CompareNoCase(_T("10")) == 0)
	{
		pBtn10Min->SendMessage(BM_CLICK, 0, 0);
	}
	else if (strAutoPower.CompareNoCase(_T("20")) == 0)
	{
		pBtn20Min->SendMessage(BM_CLICK, 0, 0);
	}
	else if (strAutoPower.CompareNoCase(_T("30")) == 0)
	{
		pBtn30Min->SendMessage(BM_CLICK, 0, 0);
	}
	else if (strAutoPower.CompareNoCase(_T("60")) == 0)
	{
		pBtn60Min->SendMessage(BM_CLICK, 0, 0);
	}
	//亮度
	CString strBrighten = IniUtil::ReadResString(_T("IDS_REMOTE_SETTING_SYSTEM_BRIGHTEN"));
	GetDlgItem(IDC_STATIC_BRIGHT)->SetWindowText(strBrighten);
	int nPos = _wtoi(g_sorFileArray.otdrConfig.strLightLevel);
	m_slideTouchLight.SetRange(15 ,100);
	m_slideTouchLight.SetPos(115 - nPos);
	//日期
	CString strDate = IniUtil::ReadResString(_T("IDS_REMOTE_SETTING_SYSTEM_DATE"));
	GetDlgItem(IDC_STATIC_DATE)->SetWindowText(strDate);

	//时间
	CString strTime = IniUtil::ReadResString(_T("IDS_REMOTE_SETTING_SYSTEM_TIME"));
	GetDlgItem(IDC_STATIC_TIME)->SetWindowText(strTime);
	//触摸屏
	CString strTouchScreen = IniUtil::ReadResString(_T("IDS_REMOTE_SETTING_SYSTEM_TOUCH"));
	GetDlgItem(IDC_STATIC_TOUCH)->SetWindowText(strTouchScreen);
	CString strTouchStartUp = IniUtil::ReadResString(_T("IDS_REMOTE_SETTING_SYSTEM_TOUCH_STARTUP"));
	CButton *pBtnTouchEnable = (CButton*)GetDlgItem(IDC_RADIO_TOUCH_ENABLE);
	pBtnTouchEnable->SetWindowText(strTouchStartUp);
	CString strTouchShutdown = IniUtil::ReadResString(_T("IDS_REMOTE_SETTING_SYSTEM_TOUCH_SHUTDOWN"));
	CButton *pBtnTouchDisnable = (CButton*)GetDlgItem(IDC_RADIO_TOUCH_DISABLE);
	pBtnTouchDisnable->SetWindowText(strTouchShutdown);
	CString strTouch = g_sorFileArray.otdrConfig.strTouchSreeenStatus;
	if (strTouch.CompareNoCase(_T("1")) == 0)
	{
		pBtnTouchDisnable->SendMessage(BM_CLICK, 0, 0);
	}
	else if (strTouch.CompareNoCase(_T("0")) == 0)
	{
		pBtnTouchEnable->SendMessage(BM_CLICK, 0, 0);
	}
	str_net_connect=IniUtil::ReadResString(_T("IDS_NET_CONNECT"));
}

void CPageSystemSetting::RecordSetting()
{
	/*                语言                                       */
	CButton *pBtnEng = (CButton*)GetDlgItem(IDC_RADIO_ENG);
	if (pBtnEng->GetCheck())
	{
		g_sorFileArray.otdrConfig.strLang = _T("1");
	}
	else
	{
		g_sorFileArray.otdrConfig.strLang = _T("0");
	}
	IniUtil::WriteSingleConfigInfo(_T("OTDR"), _T("Language"), g_sorFileArray.otdrConfig.strLang);
	/*                              自动关机                                                  */
	CButton *pBtnNever = (CButton*)GetDlgItem(IDC_RADIO_NEVER);
	CButton *pBtn1Min = (CButton*)GetDlgItem(IDC_RADIO_1MIN);
	CButton *pBtn3Min = (CButton*)GetDlgItem(IDC_RADIO_3MIN);
	CButton *pBtn5Min = (CButton*)GetDlgItem(IDC_RADIO_5MIN);
	CButton *pBtn10Min = (CButton*)GetDlgItem(IDC_RADIO_10MIN);
	CButton *pBtn20Min = (CButton*)GetDlgItem(IDC_RADIO_20MIN);
	CButton *pBtn30Min = (CButton*)GetDlgItem(IDC_RADIO_30MIN);
	CButton *pBtn60Min = (CButton*)GetDlgItem(IDC_RADIO_60MIN);
	if (pBtnNever->GetCheck())
	{
		g_sorFileArray.otdrConfig.strAutoPowerOff = _T("Never");
	}
	else if (pBtn1Min->GetCheck())
	{
		g_sorFileArray.otdrConfig.strAutoPowerOff = _T("1");
	}
	else if (pBtn3Min->GetCheck())
	{
		g_sorFileArray.otdrConfig.strAutoPowerOff = _T("3");
	}
	else if (pBtn5Min->GetCheck())
	{
		g_sorFileArray.otdrConfig.strAutoPowerOff = _T("5");
	}
	else if (pBtn10Min->GetCheck())
	{
		g_sorFileArray.otdrConfig.strAutoPowerOff = _T("10");
	}
	else if (pBtn20Min->GetCheck())
	{
		g_sorFileArray.otdrConfig.strAutoPowerOff = _T("20");
	}
	else if (pBtn30Min->GetCheck())
	{
		g_sorFileArray.otdrConfig.strAutoPowerOff = _T("30");
	}
	else if (pBtn60Min->GetCheck())
	{
		g_sorFileArray.otdrConfig.strAutoPowerOff = _T("60");
	}
	IniUtil::WriteSingleConfigInfo(_T("OTDR"), _T("AutoPowerOff"), g_sorFileArray.otdrConfig.strAutoPowerOff);
	/*                                亮度                                                 */
	IniUtil::WriteSingleConfigInfo(_T("OTDR"), _T("LightLevel"), g_sorFileArray.otdrConfig.strLightLevel);
	/*                                   时间                                                              */
	SYSTEMTIME sysDate;
	m_date.GetTime(&sysDate);
	SYSTEMTIME sysTime;
	m_time.GetTime(&sysTime);
	sysDate.wHour = sysTime.wHour;
	sysDate.wMinute = sysTime.wMinute;
	sysDate.wSecond = sysTime.wSecond;
	SetLocalTime(&sysDate);
	CTime time(sysDate);
	g_sorFileArray.otdrConfig.strDateTime  = time.Format(_T("%Y-%m-%d %H:%M:%S"));//转换时间格式
	/*                                触摸屏                                                                */
	CButton *pBtnTouchEnable = (CButton*)GetDlgItem(IDC_RADIO_TOUCH_ENABLE);
	CButton *pBtnTouchDisnable = (CButton*)GetDlgItem(IDC_RADIO_TOUCH_DISABLE);
	if (pBtnTouchEnable->GetCheck())
	{
		g_sorFileArray.otdrConfig.strTouchSreeenStatus = _T("0");
	}
	else if (pBtnTouchDisnable->GetCheck())
	{
		g_sorFileArray.otdrConfig.strTouchSreeenStatus = _T("1");
	}
}

void CPageSystemSetting::OnVScroll(UINT nSBCode,UINT nPos,CScrollBar *pScrollBar)
{
	CPropertyPage::OnVScroll(nSBCode,nPos,pScrollBar);
	//将位置转化为15-100之间
	int nCurPos = 115 - m_slideTouchLight.GetPos();
	g_sorFileArray.otdrConfig.strLightLevel.Format(_T("%d"), nCurPos);
}

// CPageSystemSetting 消息处理程序

BOOL CPageSystemSetting::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	SetLocaleString();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CPageSystemSetting::OnBnClickedBtnOtdr2pc()
{
	FillGetMsg();
	GetConfigData();
}

void CPageSystemSetting::OnBnClickedBtnPc2otdr()
{
	RecordSetting(); //保存设置
	FillSendMsg();
	SendConfigData();
}

BOOL CPageSystemSetting::OnApply()
{
	// TODO: 在此添加专用代码和/或调用基类
	RecordSetting();
	return CPropertyPage::OnApply();
}

void CPageSystemSetting::Send()
{
	//UpdateData();
	//m_pClientSocket->SendLine(m_sSend);
}

void CPageSystemSetting::FillGetMsg()
{
	m_strGetMsg.Empty(); //清空字符串
	//获取数据命令
	m_strGetMsg = NET_TYPE_SYSTEM_GET;
}

void CPageSystemSetting::FillSendMsg()//填充发送消息文本
{
	m_strSendMsg.Empty(); //清空字符串
	//命令
	CString strCommand; //发送数据命令
	strCommand.Format(_T("1"));
	m_strSendMsg += strCommand;
	//系统配置项
	CString strLang; //语言
	strLang.Format(_T("Language=%s,"), g_sorFileArray.otdrConfig.strLang);
	m_strSendMsg += strLang;

	CString strAutoPoweOff; //自动关机
	strAutoPoweOff.Format(_T("AutoPowerOff=%s,"), g_sorFileArray.otdrConfig.strAutoPowerOff);
	m_strSendMsg += strAutoPoweOff;

	CString strBright; //亮度
	strBright.Format(_T("LightLevel=%s,"), g_sorFileArray.otdrConfig.strLightLevel);
	m_strSendMsg += strBright;

	//CString strTestDate; //日期
	//strTestDate.Format(_T("Date=%s,"), g_sorFileArray.otdrConfig.strDate);
	//m_strSendMsg += strTestDate;

	CString strDateTime; //日期时间
	strDateTime.Format(_T("DateTime=%s,"), g_sorFileArray.otdrConfig.strDateTime);
	m_strSendMsg += strDateTime;

	CString strTouchStatus; //触摸屏
	strTouchStatus.Format(_T("TouchSreeenStatus=%s,"), g_sorFileArray.otdrConfig.strTouchSreeenStatus);
	m_strSendMsg += strTouchStatus;
}

void CPageSystemSetting::SendConfigData()
{
	CClientSocket* pClientSocket = MAIN->m_pRemoteConnDlg->m_pClientSocket;
	if (!pClientSocket)
		return;

	int nSocketStatus = pClientSocket->GetSocketState();
	if (nSocketStatus <= CCESocket::CREATED)
	{
		MessageBox(str_net_connect, _T("OTDR Viewer"), MB_OK|MB_ICONEXCLAMATION);
		return;
	}
	pClientSocket->SendLine(m_strSendMsg);
}

void CPageSystemSetting::GetConfigData()
{
	CClientSocket* pClientSocket = MAIN->m_pRemoteConnDlg->m_pClientSocket;
	if (!pClientSocket)
		return;
	int nSocketStatus = pClientSocket->GetSocketState();
	if (nSocketStatus <= CCESocket::CREATED)
	{
		MessageBox(str_net_connect, _T("OTDR Viewer"), MB_OK|MB_ICONEXCLAMATION);
		return;
	}

	//发送获取配置信息数据
	pClientSocket->SendLine(m_strGetMsg);
}
