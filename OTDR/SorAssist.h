#pragma once
//#include "CurveData.h"
#include "EndGabor.h"
#include "SORDATA.h"
#include "math.h"

//sor辅助类
//主要是进行sor.dll调用参数转换
//wcq2012.07.23
class CSorAssist
{
public:
	CSorAssist(void);
	~CSorAssist(void);
public:
	void convert(SORDATA sorData, TestResultInfo& testResult);//SORDATA转化为TestResultInfo
	void convert(TestResultInfo* testResult, SORDATA& sorData);//TestResultInfo转化为SORDATA,zll 2012.8.20修改TestResultInfo为TestResultInfo*
	//void AddSordataToOtdrTrace(SORDATA sorData, COtdrTrace* pOtdrTrace, COLORREF colorLine=RGB(0, 0, 255));//将sor文件读到的数据转化为otdrtrace变量 zll2013.04.23
	void AddSordataToOtdrTrace(SORDATA sorData, COtdrTrace* pOtdrTrace, WORD* pDataSmooth=NULL, COLORREF colorLine=RGB(0, 0, 255));//将sor文件读到的数据转化为otdrtrace变量 zll2013.04.23
	void SetEvents(TestResultInfo& testResultInfo, CGraphElement* graphElement);//将事件列表转化为曲线显示用的事件列表zll2013.04.23
	void CloseCurvedata(int m_listItem,SORDATA sorData, COtdrTrace* pOtdrTrace, COLORREF colorLine=RGB(0, 0, 255));//将sor文件读到的数据转化为otdrtrace变量 zll2013.04.23
	void ReEventAutoSearch(TestResultInfo& testResultInfo, SORDATA sorData, COtdrTrace* pOtdrTrace, WORD* pDataSmooth, COLORREF colorLine,int m_listItem);//zll 2013.6.18 执行自动搜索时，将sor文件读到的数据转化为otdrtrace变量
	int GetTraceYmax(SORDATA &sorData);  //获取一条曲线的最大值。lzy2014.06.24
public:
	CEndGabor m_endgabor;//zll 2013.8.19
};
