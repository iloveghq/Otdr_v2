#pragma once

#include "VARIABLE.h"
#include "WaveConfig.h"
#include "ReportConfig.h"
#include "OTDRConfig.h"
#include "PrintConfig.h"
#include "EventAnalConfig.h"
#include "OtdrTrace.h"

#define  M_FSTARTDB0 -65.63//-65.53//wcq2011.12.05
#define  M_FMAXDB0   -5.63//-9.23//wcq2011.12.05   zll 2012.10.26 由-9.23改为-15.63 lzy 2014.6.17由-15.63改为-5.63
#define  M_FSTARTDB1 -58.3//当所曲线最小值都 > ZOOM_VERTICAL_STANDARD 30*1000时使用//wcq2011.09.26
#define  M_FMAXDB1   -8.3//wcq2011.09.26  zll 2012.10.26 由-1改为-8.3
#define  ZOOM_VERTICAL_LEAST_RANGE 0.8//竖直方向放大到最大后总共显示多少dB//wcq2011.09.28
#define  ZOOM_VERTICAL_PER_TIME 0.5//竖直方向放大因子//wcq2011.09.28
#define  ZOOM_VERTICAL_COUNT 47//竖直方向最大放大次数//wcq2011.09.28 //  zll 2012.12.31修改
#define  ZOOM_VERTICAL_MAX 98//竖直方向放大到最大时有多少dB//wcq2011.11.11上
#define  EVENT_POINT_HEIGHT 100//在曲线绘制事件点时会绘一条竖直线，该值为曲线高度//wcq2011.11.15
#define  EVENT_POINT_NUM_HEIGHT 20//在曲线绘制事件点时会绘一条竖直线，该值为竖直线下方的1,2..竖直方向位置//wcq2011.11.15
#define  COUNT_POINT_MAX_MIN 686*3//绘图用。曲线数据x轴上各象素最大最小值点个数,可能修改//wcq2011.11.23
#define  COUNT_GRIDLINE_HORIZONE 10//绘图用。水平方向几列//wcq2011.12.19
#define  COUNT_GRIDLINE_VERTICAL 8//绘图用。竖直方向几列//wcq2011.12.19

COLORREF GetCurveColor(int nCurveIndex); //获取当前曲线颜色

//单条SOR曲线
struct SorFileInfo 
{
	SorFileInfo()
	{
		bValid = false;
		pTestResultInfo = new TestResultInfo();
	}

	~SorFileInfo()
	{
		if (pTestResultInfo)
		{
			delete pTestResultInfo;
			pTestResultInfo = NULL;
		}
	}

	void Reset()
	{
		if (pTestResultInfo)
		{
			delete pTestResultInfo;
			pTestResultInfo = new TestResultInfo();
		}
		bValid = FALSE;
		fileName.Empty();
		filePathName.Empty();
	}
	
	BOOL bValid;//未加载/加载出错/删除，为false
	float fOffset;
	CString fileName;
	CString filePathName;
	TestResultInfo* pTestResultInfo;
};
//多条SOR曲线
struct SorFileArray
{
	//移动数组中的连个元素
	void Move(int nForm, int nTo)
	{
		//判断越界
		if (nForm >= MAX_CURVE_NUM ||
			nTo < 0)
		//if (nTo < 0)//zll 2012.9.21 
		{
			return;
		}
		//成员赋值
		sorFileList[nTo].bValid = sorFileList[nForm].bValid;
		sorFileList[nTo].fOffset = sorFileList[nForm].fOffset;
		sorFileList[nTo].fileName = sorFileList[nForm].fileName;
		sorFileList[nTo].filePathName = sorFileList[nForm].filePathName;
		sorFileList[nTo].pTestResultInfo = sorFileList[nForm].pTestResultInfo;
		if (NULL != sorFileList[nTo].pTestResultInfo)
			sorFileList[nTo].pTestResultInfo->curveColor = GetCurveColor(nForm);

		//置空原数据
		sorFileList[nForm].pTestResultInfo = new TestResultInfo();
		sorFileList[nForm].bValid = FALSE;
		sorFileList[nForm].fileName.Empty();
		sorFileList[nForm].filePathName.Empty();
	}
	
	//清除数据
	void Reset()
	{
		//清除SorFileInfo
		for (int nIndex = 0; nIndex < totalCurveNumber; nIndex++)
		{
			sorFileList[nIndex].Reset();
		}
		//重置数据
		curSelection = 0;
		totalCurveNumber = 0;
		maxDistCurveIndex = 0;
	}

	//删除当前选择数据
	void RemoveCurSel()
	{
		//清除SorFileInfo
		sorFileList[curSelection].Reset();

		//重置曲线顺序
		for (int nIndex = 0;nIndex < totalCurveNumber; nIndex++)
		{
			if (sorFileList[nIndex].bValid /*有效数据*/)
				continue;
			else //数据已经被删除,前移数据
			{
				if (sorFileList[nIndex+1].bValid)
					Move(nIndex+1, nIndex);
			}
		}

		if (curSelection > 0)
			curSelection = curSelection - 1;
		totalCurveNumber = totalCurveNumber - 1;

		maxDistCurveIndex = 0;
	}
	
	SorFileInfo sorFileList[MAX_CURVE_NUM]; //最多加载8条曲线 zll 2012.9.24
	int curSelection;//文件列表当前选中哪个
	int totalCurveNumber; //加载曲线总数
	int maxDistCurveIndex; //最长曲线索引号
	WaveConfig waveConfig;
	ReportConfig reportConfig;
	OTDRConfig otdrConfig;
	PrintConfig printConfig;
	EventAnalConfig eventAnalConfig;
};

//加载曲线时查找第一个未加载的曲线下标
int GetNextCurveNum(SorFileArray& sorFileArray);
BOOL OpenCurveFile(const CString& strFile);//打开曲线文件
CString GetItemValById(int nIndex); //获取Item的值
BOOL IsFileAlreadyOpen(CString& strFileName);
//单位转换
BOOL IsUnitKm(); //检查当前是不是以km为单位
float MeterToOther(float values,CString strUnit);//米转化为feet 或 mile
float KmeterToOther(float values,CString strUnit);//千米转化为Kfeet 或 mile
CString UnitConvert(CString configUnit);
