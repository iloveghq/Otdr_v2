#pragma once
//#include "CurveData.h"
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
	void convert(TestResultInfo testResult, SORDATA& sorData);//TestResultInfo转化为SORDATA
};
