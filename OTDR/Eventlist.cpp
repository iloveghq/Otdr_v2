// Eventlist.cpp : implementation file
//

#include "stdafx.h"
#include "OTDR.h"
#include <math.h>
#include "Eventlist.h"
#include "EndGabor.h"

extern SorFileArray g_sorFileArray;
extern LANG g_currentLang;

// CEventlist

IMPLEMENT_DYNCREATE(CEventlist, CFormView)

CEventlist::CEventlist()
	: CFormView(CEventlist::IDD),m_pImageList(NULL)
{

}

CEventlist::~CEventlist()
{
	if (m_pImageList)
	{
		delete m_pImageList;
		m_pImageList = NULL;
	}
}

void CEventlist::SetLocalString()
{
	//设置Frame标题
	CString strTitle = IniUtil::ReadResString(_T("IDS_FRM_EVENT_TITLE"));
	GetParentFrame()->SetWindowText(strTitle);

	//设置事件列表
	SetListViewText();
}

void CEventlist::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_EVENT, m_Eventlist);
}

BEGIN_MESSAGE_MAP(CEventlist, CFormView)
	ON_WM_SIZE()
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_EVENT, &CEventlist::OnLvnItemchangedListEvent)
END_MESSAGE_MAP()


// CEventlist diagnostics

#ifdef _DEBUG
void CEventlist::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CEventlist::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CEventlist message handlers

void CEventlist::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	/*                   初始化ImageList                                                             */
	//创建位图列表
	if (NULL == m_pImageList)
	{
		m_pImageList = new CImageList();
		m_pImageList->Create(16,16,ILC_COLOR32 | ILC_MASK, 1, 0);//显示文件夹或文件
		m_pImageList->Add(AfxGetApp()->LoadIcon(IDI_EVENT_NOREFLECT));
		m_pImageList->Add(AfxGetApp()->LoadIcon(IDI_EVENT_REFLECT));

	//******初始化m_Eventlist************************************************/
	m_Eventlist.SetImageList(m_pImageList, LVSIL_SMALL);
	m_Eventlist.SetWindowPos(NULL,0,0,964,400,SWP_NOMOVE|SWP_NOZORDER | SWP_NOACTIVATE);
	m_Eventlist.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT | LVS_EX_SUBITEMIMAGES);//使m_Eventlist有网格(LVS_EX_GRIDLINES)且任意行可被整行选中(LVS_EX_FULLROWSELECT)
    CString strname[7];
	strname[0]=_T("事件序号");
	//strname[1]=_T("类型");
	strname[1]=_T("距离(Km)");
	strname[2]=_T("段(Km)");
	strname[3]=_T("损耗(dB)");
	strname[4]=_T("总损耗(dB)");
	strname[5]=_T("斜率(dB/Km)");
	strname[6]=_T("反射(dB)");
	for(int i=0;i<7;i++)
	{
		m_Eventlist.InsertColumn(i,strname[i],LVCFMT_LEFT,137);
	}
	}
	
	resizeControl();//调整事件列表框位置
}

void CEventlist::SetListViewText()
{
	//序号
	CString strOrder = IniUtil::ReadResString(_T("IDS_FRM_VIEW_EVENT_ORDER"));
	SetListColName(0, strOrder);//更改列标题

	//类型
	//CString strType = IniUtil::ReadResString(_T("IDS_FRM_VIEW_EVENT_TYPE")); 
	//SetListColName(1,strType);//更改列标题

	//距离
	CString strText;
	if(_T("m")==g_sorFileArray.waveConfig.DistUnit)//横坐标单位为km
		strText=_T("(km)");
	else if(_T("mi")==g_sorFileArray.waveConfig.DistUnit)//横坐标单位为miles
		strText=_T("(miles)");
	else if(_T("ft")==g_sorFileArray.waveConfig.DistUnit)//横坐标单位为kft
	    strText=_T("(kft)");
	CString strDistance = IniUtil::ReadResString(_T("IDS_FRM_VIEW_EVENT_DIST"));
	SetListColName(1,strDistance+strText);//更改列标题

	//段
	CString strSegment = IniUtil::ReadResString(_T("IDS_FRM_VIEW_EVENT_SEGMENT"));
	SetListColName(2, strSegment+strText);//更改列标题

	//损耗
	CString strLoss = IniUtil::ReadResString(_T("IDS_FRM_VIEW_EVENT_LOSS"));
	SetListColName(3, strLoss);//更改列标题

	//总损耗
	CString strTotalLoss = IniUtil::ReadResString(_T("IDS_FRM_VIEW_EVENT_TOTAL_LOSS"));
	SetListColName(4, strTotalLoss);//更改列标题

	//斜率
	CString strSlope = IniUtil::ReadResString(_T("IDS_FRM_VIEW_EVENT_SLOPE"));
	SetListColName(5, strSlope);//更改列标题

	//反射
	CString strReflection = IniUtil::ReadResString(_T("IDS_FRM_VIEW_EVENT_REFLECTION"));
	SetListColName(6, strReflection);//更改列标题
}

void CEventlist::SetListColName(int nColIndex, const CString& colName)
{
	LVCOLUMN col;
	col.mask= LVCF_TEXT;
	col.pszText= (LPWSTR)(LPCWSTR)colName;
	col.cchTextMax= 256;

	m_Eventlist.SetColumn(nColIndex, &col);
}

void CEventlist::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

	resizeControl();
}
void CEventlist::resizeControl()
{
	if(this->GetDlgItem(IDC_LIST_EVENT)->GetSafeHwnd()==NULL)
	{
		return;
	}
	CRect rect,rectlistview;
	GetWindowRect(&rect);
	this->GetDlgItem(IDC_LIST_EVENT)->GetWindowRect(&rectlistview);
	ScreenToClient(&rect);
	ScreenToClient(&rectlistview);
	this->GetDlgItem(IDC_LIST_EVENT)->SetWindowPos(NULL, 0, 0, rect.Width()-4, rect.bottom-rectlistview.top-2, SWP_NOMOVE);
}

//选中了不同的曲线后显示相应曲线的事件列表
void CEventlist::FreshEventList()
{
	CString strText = _T("");
	if (g_sorFileArray.totalCurveNumber == 0)
	{
		//距离表头
		if(_T("m")==g_sorFileArray.waveConfig.DistUnit)//横坐标单位为km
			strText=_T("(km)");
		else if(_T("mi")==g_sorFileArray.waveConfig.DistUnit)//横坐标单位为miles
			strText=_T("(miles)");
		else if(_T("ft")==g_sorFileArray.waveConfig.DistUnit)//横坐标单位为kft
			strText=_T("(kft)");
		CString strDistance = IniUtil::ReadResString(_T("IDS_FRM_VIEW_EVENT_DIST"));
		SetListColName(1,strDistance+strText);//更改列标题

		//段表头
		CString strSegment = IniUtil::ReadResString(_T("IDS_FRM_VIEW_EVENT_SEGMENT"));
		SetListColName(2, strSegment+strText);//更改列标题

		m_Eventlist.DeleteAllItems();
		return;
	}
	std::vector<EventInfo> ve = g_sorFileArray.sorFileList[g_sorFileArray.curSelection].pTestResultInfo->EventList.vEventInfo;//速度??
	int nSize=ve.size();
	//
	float preDistance=0;
	INT index = 0;
	INT curSele = 0;//列表框选中行号
	m_Eventlist.DeleteAllItems();
	for (int i=0;i< nSize;i++)//
	{		LV_ITEM lvi;
		lvi.mask = LVIF_TEXT| LVIF_IMAGE;
		lvi.iItem = i;
		lvi.iSubItem = 0;
		lvi.stateMask = 0xFFFF;     // get all state flags
		lvi.cchTextMax = 50;
		lvi.pszText = (LPWSTR)(LPCTSTR)strText;
		//类型
		if (0 == ve[i].nEventType)
		{
			lvi.iImage = 0;//0代表非反射事件符号
		}
		else if (1 == ve[i].nEventType)
		{
			lvi.iImage = 1;//1代表反射事件符号,zll 2013.12.25修改，原来的反了
		}
		strText.Format(_T("%d"), i+1);
		m_Eventlist.InsertItem(&lvi);
		//序号
		if (_T('S')==ve[i].cEventType)
		{
			m_Eventlist.SetItemText(i, 0, _T("(S)")+strText);
		}
		else if (_T('E')==ve[i].cEventType)
		{
			m_Eventlist.SetItemText(i, 0, _T("(E)")+strText);
		}
		else if (_T('A')==ve[i].cEventType)
		{
			m_Eventlist.SetItemText(i, 0, _T("(A)")+strText);
		}
		else if (_T('F')==ve[i].cEventType)
		{
			m_Eventlist.SetItemText(i, 0, _T("(F)")+strText);
		}
		else if (_T('O')==ve[i].cEventType)
		{
			m_Eventlist.SetItemText(i, 0, _T("(O)")+strText);
		}
		else
		{
			m_Eventlist.SetItemText(i, 0, _T("    ")+strText);
		}
		//m_Eventlist.SetItemText(i, 1, strText);

		//距离表头
		if(_T("m")==g_sorFileArray.waveConfig.DistUnit)//横坐标单位为km
			strText=_T("(km)");
		else if(_T("mi")==g_sorFileArray.waveConfig.DistUnit)//横坐标单位为miles
			strText=_T("(miles)");
		else if(_T("ft")==g_sorFileArray.waveConfig.DistUnit)//横坐标单位为kft
			strText=_T("(kft)");
		CString strDistance = IniUtil::ReadResString(_T("IDS_FRM_VIEW_EVENT_DIST"));
		SetListColName(1,strDistance+strText);//更改列标题

		//段表头
		CString strSegment = IniUtil::ReadResString(_T("IDS_FRM_VIEW_EVENT_SEGMENT"));
		SetListColName(2, strSegment+strText);//更改列标题

		//距离
		float m_float_zll=0.0f;
		if (0==i)
		{
			if(3==g_sorFileArray.waveConfig.UnitDistFig)
				strText.Format(_T("%.3f"),0.0f);//距离
			else if(4==g_sorFileArray.waveConfig.UnitDistFig)
				strText.Format(_T("%.4f"),0.0f);//距离
			else if(5==g_sorFileArray.waveConfig.UnitDistFig)
			    strText.Format(_T("%.5f"),0.0f);//距离
		}
		else
		{
			if(_T("m")==g_sorFileArray.waveConfig.DistUnit)//横坐标单位为km
			{
				if(3==g_sorFileArray.waveConfig.UnitDistFig)
					strText.Format(_T("%.3f"),ve[i].fLocation/1000);//距离
				else if(4==g_sorFileArray.waveConfig.UnitDistFig)
					strText.Format(_T("%.4f"),ve[i].fLocation/1000);//距离
				else if(5==g_sorFileArray.waveConfig.UnitDistFig)
				    strText.Format(_T("%.5f"),ve[i].fLocation/1000);//距离
			}
			else if(_T("mi")==g_sorFileArray.waveConfig.DistUnit)//横坐标单位为miles
			{
				m_float_zll=KmeterToOther(ve[i].fLocation/1000,UNIT_SHOW_MILE);
				if(3==g_sorFileArray.waveConfig.UnitDistFig)
					strText.Format(_T("%.3f"),m_float_zll);//距离
				else if(4==g_sorFileArray.waveConfig.UnitDistFig)
					strText.Format(_T("%.4f"),m_float_zll);//距离
				else if(5==g_sorFileArray.waveConfig.UnitDistFig)
				    strText.Format(_T("%.5f"),m_float_zll);//距离
			}
			else if(_T("ft")==g_sorFileArray.waveConfig.DistUnit)//横坐标单位为kft
			{
				m_float_zll=KmeterToOther(ve[i].fLocation/1000,UNIT_SHOW_KFEET);
				if(3==g_sorFileArray.waveConfig.UnitDistFig)
					strText.Format(_T("%.3f"),m_float_zll);//距离
				else if(4==g_sorFileArray.waveConfig.UnitDistFig)
					strText.Format(_T("%.4f"),m_float_zll);//距离
				else if(5==g_sorFileArray.waveConfig.UnitDistFig)
				    strText.Format(_T("%.5f"),m_float_zll);//距离
			}
		}
		m_Eventlist.SetItemText(i, 1, strText);
		//段,两事件点间距离
		float a,b=0.0f;
		if (0==i)
		{
			if(3==g_sorFileArray.waveConfig.UnitDistFig)
				strText.Format(_T("%.3f"),0.0f);//段
			else if(4==g_sorFileArray.waveConfig.UnitDistFig)
				strText.Format(_T("%.4f"),0.0f);//段
			else if(5==g_sorFileArray.waveConfig.UnitDistFig)
			    strText.Format(_T("%.5f"),0.0f);//段
		}
		else
		{
			if(_T("m")==g_sorFileArray.waveConfig.DistUnit)//横坐标单位为km
			{
				if(3==g_sorFileArray.waveConfig.UnitDistFig)
					strText.Format(_T("%.3f"),(ve[i].fLocation-preDistance)/1000);//段,两事件点间距离
				else if(4==g_sorFileArray.waveConfig.UnitDistFig)
					strText.Format(_T("%.4f"),(ve[i].fLocation-preDistance)/1000);//段,两事件点间距离
				else if(5==g_sorFileArray.waveConfig.UnitDistFig)
				    strText.Format(_T("%.5f"),(ve[i].fLocation-preDistance)/1000);//段,两事件点间距离
			}
			else if(_T("mi")==g_sorFileArray.waveConfig.DistUnit)//横坐标单位为miles
			{
				b=(ve[i].fLocation-preDistance)/1000;
				a=KmeterToOther(b,UNIT_SHOW_MILE);
				if(3==g_sorFileArray.waveConfig.UnitDistFig)
					strText.Format(_T("%.3f"),a);//段,两事件点间距离
				else if(4==g_sorFileArray.waveConfig.UnitDistFig)
					strText.Format(_T("%.4f"),a);//段,两事件点间距离
				else if(5==g_sorFileArray.waveConfig.UnitDistFig)
				    strText.Format(_T("%.5f"),a);//段,两事件点间距离
			}
			else if(_T("ft")==g_sorFileArray.waveConfig.DistUnit)//横坐标单位为kft
			{
				b=(ve[i].fLocation-preDistance)/1000;
				a=KmeterToOther(b,UNIT_SHOW_KFEET);
				if(3==g_sorFileArray.waveConfig.UnitDistFig)
					strText.Format(_T("%.3f"),a);//段,两事件点间距离
				else if(4==g_sorFileArray.waveConfig.UnitDistFig)
					strText.Format(_T("%.4f"),a);//段,两事件点间距离
				else if(5==g_sorFileArray.waveConfig.UnitDistFig)
				    strText.Format(_T("%.5f"),a);//段,两事件点间距离
			}
		}
		m_Eventlist.SetItemText(i, 2, strText);
		if (0==i)
		{
			preDistance = 0.0f;
		}
		else
		{
			preDistance = ve[i].fLocation;
		}
		//损耗
		if (0 == ve[i].fEventloss)//为0时显示"-"wcq2012.07.09
		{
			strText.Format(_T("--"));//总损耗
		}
		else
		{
			strText.Format(_T("%.3f"), ve[i].fEventloss);//损耗
		}
		m_Eventlist.SetItemText(i, 3, strText);
		//总损耗
		if (0 == ve[i].fTotalLoss)//为0时显示"-"wcq2012.07.09
		{
			strText.Format(_T("--"));//总损耗
		}
		else
		{
			strText.Format(_T("%.3f"),ve[i].fTotalLoss);//总损耗
		}
		m_Eventlist.SetItemText(i, 4, strText);
		//斜率
		if (fabs(ve[i].fLineK)>0.5)//zll 2012.10.31
		{
			strText.Format(_T("--"));
		}
		else if (0 == ve[i].fLineK)//为0时显示"-" zll 2012.10.31
		{
			strText.Format(_T("--"));
		}
		else
		{
			strText.Format(_T("%.3f"),ve[i].fLineK);//斜率
		}
		m_Eventlist.SetItemText(i,5,strText);
		//反射
		if (0 == ve[i].nEventType)//反射
		{
			m_Eventlist.SetItemText(i,6,_T("--"));
		}
		else if (0 == ve[i].fEventHigh)//为0时显示"-"wcq2012.07.09
		{
			m_Eventlist.SetItemText(i,6,_T("--"));
		}
		else
		{
			strText.Format(_T("%.3f"),ve[i].fEventHigh);//反射
			m_Eventlist.SetItemText(i, 6, strText);
		}
		

		index++;
	}
	//设置当前选中项
	m_Eventlist.SetItemState(0, LVIS_SELECTED, LVIS_SELECTED);
}

//打开曲线后，通过dll的计算公式得到事件列表
TestResultInfo* CEventlist::GetEventList()
{
	//return;//暂时不调用???????????????????????????????????????????????
	TestResultInfo* pCurveData = g_sorFileArray.sorFileList[g_sorFileArray.curSelection].pTestResultInfo;

	long l= GetTickCount();
	int waveNum = (1550 == pCurveData->WaveLength) ? 1:0; 
	float flo_BackScatter,flo_Refraction=0.0f;
	if (1310 == pCurveData->WaveLength)
	{
		flo_BackScatter=g_sorFileArray.eventAnalConfig.BackScatter;
		flo_Refraction=g_sorFileArray.eventAnalConfig.Refraction;
	}
	else if (1550 == pCurveData->WaveLength)
	{
		flo_BackScatter=g_sorFileArray.eventAnalConfig.BackScatter1550;
		flo_Refraction=g_sorFileArray.eventAnalConfig.Refraction1550;
	}
	else if (1625 == pCurveData->WaveLength)//zll,2013.5.10 晚
	{
		flo_BackScatter=g_sorFileArray.eventAnalConfig.BackScatter1625;
		flo_Refraction=g_sorFileArray.eventAnalConfig.Refraction1625;
	}
	__try{//zll 2013.6.8 晚 检查是否gabor_arithmetic算法有问题
		pCurveData->m_GaborResult = m_endgabor.gabor_arithmetic(pCurveData->WaveLength,//波长
			                            pCurveData->PulseWidth, //脉宽			 
										pCurveData->pDataPoints, //测试点数据
										pCurveData->DataPointsNum, //点数
										2.08, //傅里叶变化阈值上限。默认为2.08
										2.08, //傅里叶变化阈值下限.默认为2.08
										g_sorFileArray.eventAnalConfig.BendTH, //衰减判断门限 弯曲
										g_sorFileArray.eventAnalConfig.ReflectTH, //反射判断门限
										g_sorFileArray.eventAnalConfig.Termial, //dB_End：末端判断门限默认为3
										flo_BackScatter, //典型散射系数
										pCurveData->Frequency,
										flo_Refraction,//折射率,zll
										0);//0为曲线拼接点，暂定为0,zll//调用DLL函数，获得事件列表、尾端位置等
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		return pCurveData;
	}
	pCurveData->EventList.vEventInfo.clear();
	EventInfo eventinfo;
	int len = pCurveData->m_GaborResult.Valid_Num;//wcq2012.06.07
	//float fDisTwo = MAIN->m_pWaveFormView->m_sorDrawStatic.Get_XCoord(1,pCurveData);//*1000可转换为正确的公里数 zll 2012.10.16cwcq2013.03.29
	float fDisTwo = MAIN->m_pWaveFormView->m_sorDrawStatic.Get_XCoord(1,pCurveData);//zll 2013.6.7
	for(int i=0;i<len;i++)
	{
		if (0==fDisTwo)
		{
			int ff=0;
		}//eif
		eventinfo.index = pCurveData->m_GaborResult.Event_Array[i];//下标，第几点
		eventinfo.nEventType = pCurveData->m_GaborResult.Reflect_Type[i];//事件类型 //zll 2013.6.8 去掉"非"
		//eventinfo.nEventType = !pCurveData->m_GaborResult.Reflect_Type[i];//事件类型
		eventinfo.cEventType=pCurveData->m_GaborResult.Event_type[i];
		eventinfo.fLocation = fDisTwo * pCurveData->m_GaborResult.Event_Array[i];//两点间距离，以km为单位
		eventinfo.fEventloss = pCurveData->m_GaborResult.Event_Loss[i];
		eventinfo.fEventHigh = pCurveData->m_GaborResult.Event_Hight[i];
		eventinfo.fLineK = pCurveData->m_GaborResult.Line_k[i];//zll 2013.6.8 去掉fDisTwo
		//eventinfo.fLineK = pCurveData->m_GaborResult.Line_k[i] / fDisTwo;
		eventinfo.fTotalLoss = pCurveData->m_GaborResult.Total_Loss[i];
		eventinfo.fReflect =pCurveData->m_GaborResult.Reflect;
		eventinfo.fTrailEnd = pCurveData->m_GaborResult.Trail_End;
		pCurveData->EventList.vEventInfo.push_back(eventinfo);
		//MAIN->m_pWaveFormView->m_sorDrawStatic.m_vAllData[i].index=eventinfo.index;//zll 2013.6.14
		//MAIN->m_pWaveFormView->m_sorDrawStatic.m_vAllData[i].fLocation=eventinfo.fLocation;//zll 2013.6.14
	}
	return pCurveData;
}

void CEventlist::OnLvnItemchangedListEvent(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	POSITION pos = m_Eventlist.GetFirstSelectedItemPosition();
	int nIndex = m_Eventlist.GetNextSelectedItem(pos);
	if (nIndex != LB_ERR)
	{
		//MAIN->m_pWaveFormView->m_sorDrawStatic.MoveEventList(nIndex);// cwcq2013.03.29
		MAIN->m_pWaveFormView->m_sorDrawStatic.MoveEventList(nIndex);
	}

	*pResult = 0;
}
