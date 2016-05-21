#include <vector>
#include "EndGabor.h"
struct EventInfo //事件点信息结构体
{
	unsigned int index;//位于数据数组的第几个点
	unsigned int nEventType;//事件类型，0反射事件，1非反射事件
	float fLocation;//事件位置,(C/(2nf))*index,根据第几点计算得到距离//2011.11.24
	float fEventloss;//损耗//2011.11.24
	float fEventHigh;////事件点反射dB//2011.11.24
	float fLineK;////区段斜率//2011.11.24
	float fTotalLoss;////总损耗、累计损耗//2011.11.24
	float fReflect;//反射值
	float fTrailEnd;////尾端位置
	char  cEventType;// event code 第二个参数  S:开始事件 E:光纤末端即结束事件；A：用户自加； F:软件自行查找；O:超出测量范围的光纤末端 D: wcq2012.06.11
};

struct EventInfoList//事件点信息列表
{
	std::vector<EventInfo> vEventInfo; //从网络接收的事件数组
};
//struct DataScales//wcq20111101
//{
//	short scaleFactor;//编码2字节
//	std::vector<unsigned short> vData;//
//};
//std::vector<DataScales> vAllData;//所有数据
struct TestResultInfo 
{
	TestResultInfo()//wcq 2011.11.1
	{
		IsMaxLength = FALSE;
		IsLowerStandard = FALSE;
		IsLoadFromFile = TRUE;
		DataType = 8888; //数据类型， 有数据8888，无数据4444
		TestMode = 2222; //测试模式，平均测试2222，
		DataSerialNum = 0; //数据包序号
		pDataPoints = NULL; //采样点值 扩大了1000倍的值
		pDataCurve = NULL; //保存取点后计算过的数据
		ScaleFactor = 1;//缩放系数，每个数据点都乘该系数
		YOffset = 0;//竖直方向偏移，用于平移
		StartPoint = 0;  //曲线横坐标的起点
		MaxPoint = 0;    //曲线横坐标点数的最大值
		FullPointCount = 0;//曲线缩放时为多少个点时曲线占满整个绘图区域,用以确定曲线宽度wcq2011.09.16
		FullPointCountMin = 0;//曲线未缩放,为多少个点时曲线占满整个绘图区域,用以确定曲线宽度,FullPointCount为该数的倍数。wcq2011.09.21
		LastZoomPointCount = 0;//曲线缩放时最后一次缩放共多少点，和前面的缩放的点的个数不一定一样。wcq2011.09.21
		DataSpacing = 0;//for each pulse width used:one-way time(units of 100 psec) for 10000 data points//wcq2011.10.19理解为两点间距离
		Refraction = 0.0f; //折射率
		ChainLoss = 0.0f; //链损耗，扩大了1000倍
		ChainAttenu = 0.0f; //链衰减，扩大了1000倍
		MaxTestDistance = 0.0f; //测量曲线最大距离
		DataPointsNum = 0; //采样点个数
		Frequency = 0; //频率
		Range = 0; //测量范围
		PulseWidth = 0; //脉冲宽度
		WaveLength = 0; //中心波长
		MeasureTime = 0; //测量时间
		ChainLength = 0; //链长
		ptDrawData = NULL; //画图坐标点,每个类对应一个绘图数据指针,原来名称为COTDRPage类中m_ptRecord wcq2011.09.21

		//
		Atten = 0.0f;//衰减
		Average = 0.0f;//平均
		SampleCount = 0;//采样数
		FusionLoss = 0.0f;//熔接损耗
		ReturnLoss = 0.0f;//回波损耗

		//附加属性
		Relay = 0;//继电器//wcq2011.12.06
		Joint1 = 0;//拼接点1//wcq2011.12.05
		Joint2 = 0;//拼接点2//wcq2011.12.05
		Length = 0; //曲线长度(光纤长度)
		ThumbDrawXCount = 0;//缩略图绘制时x坐标值最大值，最长曲线为缩略图区域宽度
		dataCurveCount = 0;//pDataCurve点个数,不一定为COUNT_POINT_MAX_MIN，wcq2011.11.23
		curveColor = RGB(0,0,255); //曲线颜色
		pThumbData = NULL;//缩略图数据
//显示时用平滑过的数据，保存时用实际数据 wcq2013.04.26
		pDataPointsSmooth = NULL;

	}
	~TestResultInfo()//wcq 2011.11.1
	{
		if (NULL != pDataPoints)
		{
			delete[] pDataPoints;
			pDataPoints = NULL;
		}
		if (NULL != pDataCurve)
		{
			delete[] pDataCurve;
			pDataCurve = NULL;
		}
		if (NULL != ptDrawData)
		{
			//delete[] ptDrawData;
			ptDrawData = NULL;
		}
		if (NULL != pThumbData)
		{
			delete[] pThumbData;
			pThumbData = NULL;
		}
		EventList.vEventInfo.clear();
//显示时用平滑过的数据，保存时用实际数据 wcq2013.04.26
		//m_TestResult.pDataPointsSmooth=NULL;
		if (NULL != pDataPointsSmooth)
		{
			delete[] pDataPointsSmooth;
			pDataPointsSmooth=NULL;
		}

	}
	BOOL IsMaxLength; //是否是最长曲线
	BOOL IsLowerStandard;//wcq2011.09.26根据与ZOOM_VERTICAL_STANDARD比较而为true或false.用于竖直方向缩放时确定最大量程
	BOOL IsLoadFromFile;//是否从文件加载上来的曲线//wcq2011.10.19
	WORD DataType; //数据类型， 有数据8888，无数据4444
	WORD TestMode; //测试模式，平均测试2222，
	WORD DataSerialNum; //数据包序号
	WORD* pDataPoints; //采样点值 扩大了1000倍的值
	CPoint* pDataCurve; //保存取点后计算过的数据
	short ScaleFactor;//缩放系数，每个数据点都乘该系数
	int YOffset;//竖直方向偏移，用于平移
	int StartPoint;  //曲线横坐标的起点
	int MaxPoint;    //曲线横坐标点数的最大值
	int FullPointCount;//曲线缩放时为多少个点时曲线占满整个绘图区域,用以确定曲线宽度wcq2011.09.16
	int FullPointCountMin;//曲线未缩放,为多少个点时曲线占满整个绘图区域,用以确定曲线宽度,FullPointCount为该数的倍数。wcq2011.09.21
	int LastZoomPointCount;//曲线缩放时最后一次缩放共多少点，和前面的缩放的点的个数不一定一样。wcq2011.09.21
	float DataSpacing;//for each pulse width used:one-way time(units of 100 psec) for 10000 data points//wcq2011.10.19理解为两点间距离
	float Refraction; //折射率1310nm
	float Refraction1550; //折射率1550nm//zll 2012.9.18
	float GroupIndex;   //折射率lzy 2014.8.13
	float ChainLoss; //链损耗，扩大了1000倍
	float ChainAttenu; //链衰减，扩大了1000倍
	float MaxTestDistance; //测量曲线最大距离
	unsigned long int DataPointsNum; //采样点个数
	float Frequency; //频率
	long int Range; //测量范围
	long int PulseWidth; //脉冲宽度
	long int WaveLength; //中心波长
	long int MeasureTime; //测量时间
	long int ChainLength; //链长
	CPoint* ptDrawData; //画图坐标点,每个类对应一个绘图数据指针,原来名称为COTDRPage类中m_ptRecord wcq2011.09.21
	EventInfoList EventList;//事件列表
//显示时用平滑过的数据，保存时用实际数据
	WORD* pDataPointsSmooth; //采样点值 扩大了1000倍的值,原始数据经过平滑处理后的数据


	//
	float Atten;//衰减
	float Average;//平均
	int   SampleCount;//采样数
	float FusionLoss;//熔接损耗
	float ReturnLoss;//回波损耗

	//附加属性
	WORD Relay;//继电器//wcq2011.12.06
	WORD Joint1;//拼接点1//wcq2011.12.05
	WORD Joint2;//拼接点2//wcq2011.12.05
	float Length; //曲线长度(光纤长度)
	unsigned int ThumbDrawXCount;//缩略图绘制时x坐标值最大值，最长曲线为缩略图区域宽度
	int dataCurveCount;//pDataCurve点个数,不一定为COUNT_POINT_MAX_MIN，wcq2011.11.23
	COLORREF curveColor; //曲线颜色
	CPoint*  pThumbData;//缩略图数据

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//后面添加 wcq2012.10.30
	float BackscatterCoefficient;//散射系数 wcq2012.10.30
	float LossThreshold;//衰减门限 wcq2012.10.30
	float ReflectanceThreshold;//反射门限 wcq2012.10.30
	float EndOfFiberThreshold;//光纤末端门限 wcq2012.10.30
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//
	Gabor_Result m_GaborResult;//事件列表
};
