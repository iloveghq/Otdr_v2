#include "StdAfx.h"
#include "SorAssist.h"
#define C_LIGHT_SPEED 299792458.0//光速//wcq2011.10.19
template<typename To,typename From>
void vchar2string(To& t,From& f)
{
	t.assign(f.begin(),f.end());
}
CSorAssist::CSorAssist(void)
{
}

CSorAssist::~CSorAssist(void)
{
}
//SORDATA转化为TestResultInfo
void CSorAssist::convert(SORDATA sorData, TestResultInfo& testResultInfo)
{
	if (sorData.vAllData.size() > 0)//存在有效数据
	{
		if (NULL == testResultInfo.pDataPoints)
		{
			testResultInfo.pDataPoints = new WORD[sorData.vAllData[0].nCount];
			memcpy(testResultInfo.pDataPoints,&(sorData.vAllData[0].vData[0]),sorData.vAllData[0].nCount * 2);
		}
		else//已有数据
		{
			if (testResultInfo.DataPointsNum < sorData.vAllData[0].nCount)//已有数据长度比将要赋值的少
			{
				delete[] testResultInfo.pDataPoints;
				testResultInfo.pDataPoints = new WORD[sorData.vAllData[0].nCount];
			}//eif
			memcpy(testResultInfo.pDataPoints,&(sorData.vAllData[0].vData[0]),sorData.vAllData[0].nCount * 2);
		}//eif
		testResultInfo.DataPointsNum = sorData.vAllData[0].nCount;

		////if (1000 != sorData.vAllData[0].scaleFactor)//系数乘了1000,系数不为1，数据需要乘一个系数?????????????????
		////{
		////	float fTemp = sorData.vAllData[0].scaleFactor / 1000.0;
		////	for (int i=0; i < testResultInfo.DataPointsNum; i++)
		////	{
		////		testResultInfo.pDataPoints[i] *= fTemp;
		////	}//efor
		////}//eif
	}//eif

	//
	testResultInfo.Refraction = sorData.Refraction;
	testResultInfo.PulseWidth = sorData.PulseWidth;
	testResultInfo.WaveLength = sorData.WaveLength;
	testResultInfo.Frequency = sorData.Frequency;
	testResultInfo.DataSpacing = sorData.DataSpacing;

	////sorData.Length = testResultInfo.WaveLength;//???????????????????

	//事件列表
	int nTemp = sorData.vEvent.size();
	float fTempLocation = pow(10.0,-10) * C_LIGHT_SPEED / (testResultInfo.Refraction );
	float fTempIndex = (2 *testResultInfo.Frequency)/10000;
	for(int i=0; i<nTemp; i++)
	{	
		EventInfo eventInfo;


		if (sorData.vEvent[i].code.size()>5)//实际长度应该为6
		{
			eventInfo.nEventType = sorData.vEvent[i].code[0] - '0';//事件类型
			eventInfo.cEventType = sorData.vEvent[i].code[1];//事件是生成的还是用户添加或是尾端事件等
		}//eif
	
		//eventInfo.fLocation = sorData.vEvent[i].propagationTime * pow(10.0,-10) * C_LIGHT_SPEED / (testResultInfo.Refraction ) ;//位置//cwcq2012.07.24
		eventInfo.fLocation = sorData.vEvent[i].propagationTime * fTempLocation ;//位置//从上面公式简化而来 wcq2012.07.24
		//eventInfo.index = sorData.vEvent[i].propagationTime * (2 *testResultInfo.Frequency)/10000;//距离/光速(km/ps) * 折射率 位置//cwcq2012.07.24
		eventInfo.index = sorData.vEvent[i].propagationTime * fTempIndex;//距离/光速(km/ps) * 折射率 位置//从上面公式简化而来 wcq2012.07.24
		//防止超出范围
		if (eventInfo.index <0)
		{
			eventInfo.index =0;
		}
		else if (eventInfo.index > testResultInfo.DataPointsNum)
		{
			eventInfo.index = testResultInfo.DataPointsNum;//
		}//eif


		//eventInfo.index = (sorData.vEvent[i].propagationTime * (2 *testResultInfo.Frequency)/10000);//距离/光速(km/ps) * 折射率 位置
		eventInfo.fLineK = sorData.vEvent[i].attenCoefficent /1000.0;//斜率/衰减因子 dB/km 350=0.35
		eventInfo.fEventloss =  sorData.vEvent[i].eventLoss / 1000.0;////损耗
		eventInfo.fEventHigh = sorData.vEvent[i].reflectance / 1000.0;//反射

		if (0 == i)
		{
			eventInfo.fTotalLoss = eventInfo.fEventloss;
		}
		else
		{
			//ei.fTotalLoss = testResultInfo.EventList.vEventInfo[i-1].fTotalLoss/*前一事件总损耗*/ + ei.fEventloss/*当前事件损耗*/ + (ei.fLineK/1000) *(ei.fLocation - testResultInfo.EventList.vEventInfo[i-1].fLocation);
			eventInfo.fTotalLoss = testResultInfo.EventList.vEventInfo[i-1].fTotalLoss/*前一事件总损耗*/ + eventInfo.fEventloss/*当前事件损耗*/ + (eventInfo.fLineK/1000) *(eventInfo.fLocation - testResultInfo.EventList.vEventInfo[i-1].fLocation);
		}//eif
		eventInfo.fReflect = 0;//这个参数没有用到
		eventInfo.fTrailEnd =0;////这个参数没有用到C/(2nf))*index
		//eventInfo.nEventType = sorData.vEvent[i].code[0] - '0';
		//eventInfo.cEventType = sorData.vEvent[i].code[1];
		testResultInfo.EventList.vEventInfo.push_back(eventInfo);
	}//efor
}
//TestResultInfo转化为SORDATA
void CSorAssist::convert(TestResultInfo testResultInfo, SORDATA& sorData)
{
	//wince里只有一个缩放系数，所有的数据都是原始数据
	sorData.vAllData.clear();
	Data data;
	data.nCount = testResultInfo.DataPointsNum;
	//data.scaleFactor = testResultInfo.ScaleFactor;//没乘系数 *1000???????????
	data.scaleFactor = 1000;//没乘系数 *1000???????????
	data.vData.assign(testResultInfo.pDataPoints, testResultInfo.pDataPoints + data.nCount);
	sorData.vAllData.push_back(data);

	//
	sorData.Refraction = testResultInfo.Refraction;
	sorData.PulseWidth = testResultInfo.PulseWidth;
	sorData.WaveLength = testResultInfo.WaveLength;
	sorData.Frequency = testResultInfo.Frequency;
	////sorData.Length = testResultInfo.WaveLength;//???????????????????
	
	//事件列表wcq2012.06.11
	sorData.vEvent.clear();
	std::string pos="9999";
	int nTemp = testResultInfo.EventList.vEventInfo.size();
	for(int i=0; i<nTemp; i++)
	{
		Event event;		
		event.propagationTime = (testResultInfo.EventList.vEventInfo[i].index /(2 *testResultInfo.Frequency))*10000;//距离/光速(km/ps) * 折射率 位置
		event.attenCoefficent = testResultInfo.EventList.vEventInfo[i].fLineK * 1000;//斜率/衰减因子 dB/km 350=0.35
		event.eventLoss = testResultInfo.EventList.vEventInfo[i].fEventloss * 1000;////损耗
		event.reflectance = testResultInfo.EventList.vEventInfo[i].fEventHigh * 1000;//反射
		event.code.push_back(testResultInfo.EventList.vEventInfo[i].nEventType + '0');//wcq2012.07.09
		event.code.push_back(testResultInfo.EventList.vEventInfo[i].cEventType);//
		event.code.insert(event.code.end(), pos.begin(), pos.end());//不使用 landmark
		vchar2string(event.lossMeasurement,std::string("LS"));//几点法 我们用的是4点法
		sorData.vEvent.push_back(event);
	}//efor
}
