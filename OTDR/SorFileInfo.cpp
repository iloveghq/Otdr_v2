// ChildFrm.cpp : implementation of the CChildFrame class
//
#include "stdafx.h"
#include "SorFileInfo.h"

extern SorFileArray g_sorFileArray;

//加载曲线时查找第一个未加载的曲线下标
int GetNextCurveNum(SorFileArray& sorFileArray)
{
	int index = -1;
	sorFileArray.maxDistCurveIndex = 0;
	for (int i=0;i<MAX_CURVE_NUM;i++)
	//for (int i=0;;i++)//zll 2012.9.21
	{
		if (!sorFileArray.sorFileList[i].bValid)
		{
			index = i;
			break;
		}
	}
	return index;
}

//加载曲线
BOOL OpenCurveFile(const CString& strFile)
{
	MAIN->m_pWaveFormView->GetDlgItem(IDC_EVENT_ADD)->ShowWindow(FALSE);//zll 2013.6.5 晚
	MAIN->m_pWaveFormView->GetDlgItem(IDC_EVENT_DELETE)->ShowWindow(FALSE);//zll 2013.6.5 晚
	CString strMsg;
	int index = GetNextCurveNum(g_sorFileArray);
	if (LB_ERR == index)//全部都加载过了
	{
		//strMsg.Format(_T("最多加载%d条曲线.      "), MAX_CURVE_NUM);
		//MessageBox(NULL, strMsg, _T("提示"), MB_OK|MB_ICONEXCLAMATION);
		return FALSE;
	}

	CString strFilePath = g_sorFileArray.reportConfig.Folder1;//得到文件路径名
	CString strFileName;
	strFileName.Format(_T("%s\\%s"),strFilePath, strFile);//路径加文件名
	
	//判断文件是否已经加载
	if (IsFileAlreadyOpen(strFileName))
	{
		return FALSE;
	}

	//读取文件
	CSorFile sorFile;
	SORDATA m_sorDATA;
	CSorAssist m_sorAssist;
	TestResultInfo* pTestResultInfo;//zll 2013.5.12 中
	if (1 == sorFile.readSorFile(strFileName, m_sorDATA))
	{
		pTestResultInfo =g_sorFileArray.sorFileList[index].pTestResultInfo;
		INT nStartPoint = pTestResultInfo->StartPoint;
		INT nMaxPoint = pTestResultInfo->MaxPoint;
		m_sorAssist.convert(m_sorDATA,*(g_sorFileArray.sorFileList[index].pTestResultInfo));
		g_sorFileArray.sorFileList[index].bValid = TRUE;
		g_sorFileArray.sorFileList[index].fileName = strFile; //strFileName; 无路径名的文件名lzy 2014.8.25
		g_sorFileArray.sorFileList[index].filePathName = strFileName;//strFilePath;  带路径名的文件名lzy 2014.8.25
		g_sorFileArray.totalCurveNumber++;
		pTestResultInfo->StartPoint = nStartPoint;
		pTestResultInfo->MaxPoint = nMaxPoint;

#ifdef COMPILE_SHOW_SMOOTH_DATA////显示时用平滑过的数据，保存时用实际数据2013.05.12
		MAIN->m_smooth.Smooth(pTestResultInfo->pDataPoints, pTestResultInfo->DataPointsNum, pTestResultInfo->pDataPointsSmooth, pTestResultInfo->PulseWidth, pTestResultInfo->Frequency);
#endif
		g_sorFileArray.sorFileList[index].pTestResultInfo->DataSpacing = MAIN->m_pWaveFormView->m_sorDrawStatic.Get_XCoord2nf(1,g_sorFileArray.sorFileList[index].pTestResultInfo);
	}
	else//出错，进行错误提示????????????????????????
	{

	}
	if (MAIN)
	{
		MAIN->m_pWaveFormView->PopulateData();
		//填充曲线数据
	    long a=GetTickCount();
		////MAIN->m_pWaveFormView->m_sorDrawStatic.FillCurveDataFromFile(g_sorFileArray.sorFileList[index].pTestResultInfo); cwcq2013.03.29
		//	MAIN->m_pWaveFormView->m_sorDrawStatic.Invalidate();//redraw cwcq2013.03.29

		//以下判断一下数据的最大值，来修改Y轴的范围,lzy2014.6.23
		int iYmax = m_sorAssist.GetTraceYmax(m_sorDATA);
		int iChange = abs(iYmax * 1.2 - g_nTraceYmax) / 5.0;    //超出几个单位的5dB		
		if(!index) //第一条曲线，需要作缩小范围的修改
		{
			if(iYmax * 1.2 > g_nTraceYmax)
				g_nTraceYmax += (1 + iChange) * 5;
			else
				g_nTraceYmax -= iChange * 5;
		}
		else  //其余的曲线如果范围扩大则修改
		{
			if(iYmax * 1.2 > g_nTraceYmax)
				g_nTraceYmax += (1 + iChange) * 5;
		}
		if(g_nTraceYmax < OTDR_TRACE_Y_MAX)
			g_nTraceYmax = OTDR_TRACE_Y_MAX;   //设定最小范围
		//if(!index)  //第一条曲线，需要作缩小范围的修改
		//{
		//	if(iYmax > OTDR_TRACE_Y_MAX)
		//	{
		//		if(abs(iYmax - (int)(g_nTraceYmax / OTDR_TRACE_Y_RATE)) > 0)
		//			g_nTraceYmax = (int)(iYmax * OTDR_TRACE_Y_RATE);
		//	}
		//	else
		//		g_nTraceYmax = OTDR_TRACE_Y_MAX;

		//}
		//else if(index > 0)
		//{
		//	if(iYmax > (int)(g_nTraceYmax / OTDR_TRACE_Y_RATE))
		//		g_nTraceYmax = (int)(iYmax * OTDR_TRACE_Y_RATE);
		//}

/////////////////////////zll 2013.5.12 中
#ifdef COMPILE_SHOW_SMOOTH_DATA////显示时用平滑过的数据，保存时用实际数据 wcq2013.04.26
		m_sorAssist.AddSordataToOtdrTrace(m_sorDATA,&MAIN->m_pWaveFormView->m_sorDrawStatic, pTestResultInfo->pDataPointsSmooth);//zll 2013.5.7
#else
		m_sorAssist.AddSordataToOtdrTrace(m_sorDATA,&MAIN->m_pWaveFormView->m_sorDrawStatic);//zll 2013.5.7
#endif
        COtdrTrace* temp=&MAIN->m_pWaveFormView->m_sorDrawStatic;//zll 2013.6.6 调试用
		MAIN->m_pWaveFormView->m_sorDrawStatic.ReCalMaxDistance();
		MAIN->m_pWaveFormView->m_sorDrawStatic.SetMaxDistance(MAIN->m_pWaveFormView->m_sorDrawStatic.GetMaxDistance());
		//m_pWaveFormView->m_sorDrawStatic.SetHorScroll();
		//InvalidateRect(FALSE);
		MAIN->m_pThumbFormView->GetAllThumbData();//zll 2013.5.10 晚
///////////////////////////////////////////////////////////////

		MAIN->m_pThumbFormView->Invalidate(); //redraw thumbnail
		//TRACE(_T("111 %d\n"),GetTickCount()-a);//zll 2012.10.12

		//激活保存和打印菜单
		MAIN->m_bFileMenuEnable = TRUE;
	}
	return TRUE;
}

COLORREF GetCurveColor(int nCurveIndex)
{
	switch (nCurveIndex)
	{
	case 1:
		return g_sorFileArray.waveConfig.ColorCurve1;
	case 2:
		return g_sorFileArray.waveConfig.ColorCurve2;
	case 3:
		return g_sorFileArray.waveConfig.ColorCurve3;
	case 4:
		return g_sorFileArray.waveConfig.ColorCurve4;
	case 5:
		return g_sorFileArray.waveConfig.ColorCurve5;
	case 6:
		return g_sorFileArray.waveConfig.ColorCurve6;
	case 7:
		return g_sorFileArray.waveConfig.ColorCurve7;
	case 8:
		return g_sorFileArray.waveConfig.ColorCurve8;
	default:
		return g_sorFileArray.waveConfig.ColorCurve1;
	}
}

CString GetItemValById(int nIndex) //获取Item的值
{
	switch (nIndex)
	{
	case 1:
		return g_sorFileArray.reportConfig.Item1Val;
	case 2:
		return g_sorFileArray.reportConfig.Item2Val;
	case 3:
		return g_sorFileArray.reportConfig.Item3Val;
	case 4:
		return g_sorFileArray.reportConfig.Item4Val;
	case 5:
		return g_sorFileArray.reportConfig.Item5Val;
	case 6:
		return g_sorFileArray.reportConfig.Item6Val;
	case 7:
		return g_sorFileArray.reportConfig.Item7Val;
	case 8:
		return g_sorFileArray.reportConfig.Item8Val;
	case 9:
		return g_sorFileArray.reportConfig.Item9Val;
	case 10:
		return g_sorFileArray.reportConfig.Item10Val;
	case 11:
		return g_sorFileArray.reportConfig.Item11Val;
	case 12:
		return g_sorFileArray.reportConfig.Item12Val;
	default:
		return _T("");
	}
}

BOOL IsFileAlreadyOpen(CString& strFileName)//文件是否已经打开
{
	for (int nIndex = 0; nIndex < g_sorFileArray.totalCurveNumber; nIndex++)
	{
		if (strFileName.CompareNoCase(g_sorFileArray.sorFileList[nIndex].filePathName) == 0)
		{
			return true;
		}
	}

	return false;
}

//检查当前是不是以km为单位 wcq2012.04.28
BOOL IsUnitKm()
{
	return (UNIT_SHOW_KM == g_sorFileArray.waveConfig.DistUnit);
}
//米转化为feet 或 mile
float MeterToOther(float values,CString strUnit)
{
	if (UNIT_SHOW_KFEET == strUnit)//1 英尺 = 0.3048 米
	{
		return values * 3.281;
	}
	else if (UNIT_SHOW_MILE == strUnit)//1 英里 = 1609.344 米
	{
		//return values * 0.00062137;
		return values * 0.000621388181;
	}
	else//km
	{
		return values;
	}//
}
//千米转化为Kfeet 或 mile wcq2012.05.09
float KmeterToOther(float values,CString strUnit)
{
	if (UNIT_SHOW_KFEET == strUnit)//1 英尺 = 0.3048 米
	{
		return values * 3.281;
	}
	else if (UNIT_SHOW_MILE == strUnit)//1 英里 = 1609.344 米
	{
		//return values * 0.00062137;
		return values * 0.621388181;
	}
	else//km
	{
		return values;
	}//
}
//将配置文件中的单位转化为显示单位 wcq2012.04.28
//如:m-->km ft-->Kft
CString UnitConvert(CString configUnit)
{
	CString re=_T("Km");
	if (UNIT_SHOW_KM == configUnit)
	{
		re=_T("Km");
	}
	else if (UNIT_SHOW_KFEET == configUnit)
	{
		re=_T("Kft");
	}
	else if (UNIT_SHOW_MILE == configUnit)
	{
		re=_T("mi");
	}//eif
	return re;
}
