#include <vector>
//测试结果结构体
struct EventInfo //事件点信息结构体
{
	unsigned int index;//位于数据数组的第几个点
	unsigned int nEventType;//事件类型0,开始事件，1反射事件，2非反射事件，3反射事件，4结束事件
	float fLocation;//事件位置,(C/(2nf))*index,根据第几点计算得到距离//2011.11.24//gabor_dll结构体里没这个字段 cwcq2012.06.11
	float fEventloss;//损耗//2011.11.24
	float fEventHigh;////事件点反射dB//2011.11.24 回损 wcq2012.06.11 
	float fLineK;////区段斜率//2011.11.24
	float fTotalLoss;////总损耗、累计损耗//2011.11.24
	float fReflect;//没有用到
	float fTrailEnd;////尾端位置 
	char  cEventType;//sor标准 event code 第二个参数 事件是生成的还是用户添加或是尾端事件等   A：用户自加；M: E:光纤末端；F:软件自行查找；O:超出测量范围的光纤末端 D: wcq2012.06.11
};
struct EventInfoList //事件点信息列表
{
	std::vector<EventInfo> vEventInfo; //
	//unsigned int EventNum; //
};
struct TestResultInfo 
{
	TestResultInfo()//wcq 2011.11.1
	{
		pDataPoints = NULL;
		pDataCurve = NULL;
		ptDrawData = NULL;
		pThumbData = NULL;
		dataCurveCount = 0;//wcq 2011.11.21
		IsMaxLength = FALSE;//方便判断是否需要重绘缩略图wcq2011.12.14
	}
	unsigned long int DataPointsNum; //采样点个数
	short ScaleFactor;//缩放系数，每个数据点都乘该系数
	int YOffset;//竖直方向偏移，用于平移
	WORD* pDataPoints; //采样点值 扩大了1000倍的值,原始数据
	//WORD* pDataCurve; //保存取点后计算过的数据
	CPoint* pDataCurve; //保存取点后计算过的数据 x:代表在原数据数组中的下标，y:代表对应的功率值（原始未除以1000）wcq2011.11.23
	CPoint*  pThumbData;//缩略图数据。用于绘制缩略图wcq2011.11.10
	CPoint* ptDrawData; //画图坐标点,每个类对应一个绘图数据指针,原来名称为COTDRPage类中m_ptRecord wcq2011.09.21
	//long int Frequency; //频率//为了和其他公司的sor文件同步，改为下面的cwcq2012.06.12
	//float Frequency; //频率wcq2012.06.12
	long int Range; //测量范围
	long int PulseWidth; //脉冲宽度
	long int WaveLength; //中心波长
	long int MeasureTime; //测量时间
	long int ChainLength; //链长
	int dataCurveCount;//pDataCurve点个数,不一定为COUNT_POINT_MAX_MIN，wcq2011.11.23
	float Frequency; //频率wcq2012.06.12
	float Refraction; //折射率
	float ChainLoss; //链损耗，扩大了1000倍
	float ChainAttenu; //链衰减，扩大了1000倍
	float MaxTestDistance; //测量曲线最大距离
	float Length; //曲线长度(光纤长度)wcq2011.11.10
	//INT DataSpacing;//for each pulse width used:one-way time(units of 100 psec) for 10000 data points//wcq2011.10.19理解为两点间距离//wcq2011.11.30
	float DataSpacing;//for each pulse width used:one-way time(units of 100 psec) for 10000 data points//wcq2011.10.19理解为两点间距离//wcq2011.11.30
	WORD DataType; //数据类型， 有数据8888，无数据4444
	WORD TestMode; //测试模式，平均测试2222，
	WORD Relay;//继电器//wcq2011.12.06
	WORD Joint1;//拼接点1//wcq2011.12.05
	WORD Joint2;//拼接点2//wcq2011.12.05
	WORD DataSerialNum; //数据包序号
	unsigned int StartPoint;  //曲线横坐标的起点
	unsigned int MaxPoint;    //曲线横坐标点数的最大值

	BOOL IsMaxLength; //是否是最长曲线
	BOOL IsLowerStandard;//wcq2011.09.26根据与ZOOM_VERTICAL_STANDARD比较而为true或false.用于竖直方向缩放时确定最大量程
	BOOL IsLoadFromFile;//是否从文件加载上来的曲线//wcq2011.10.19
	int FullPointCount;//曲线缩放时为多少个点时曲线占满整个绘图区域,用以确定曲线宽度wcq2011.09.16
	int FullPointCountMin;//曲线未缩放,为多少个点时曲线占满整个绘图区域,用以确定曲线宽度,FullPointCount为该数的倍数。wcq2011.09.21
	int LastZoomPointCount;//曲线缩放时最后一次缩放共多少点，和前面的缩放的点的个数不一定一样。wcq2011.09.21
	unsigned int ThumbDrawXCount;//缩略图绘制时x坐标值最大值，最长曲线为缩略图区域宽度wcq2011.11.10
	EventInfoList EventList;//事件列表
};




////#include <vector>
////struct EventInfo //事件点信息结构体
////{
////	unsigned long int ulLocation;//事件位置
////	long int lEventType;//事件类型0,开始事件，1反射事件，2非反射事件，3反射事件，4结束事件
////	float lReflectValue;//反射值
////	float lInsertloss;//插入损耗
////	float lAttenuCoefficient;//衰减系数
////	float lCumulativeLoss;//累计损耗*/
////};
////
////struct EventInfoList//事件点信息列表
////{
////	std::vector<EventInfo> vEventInfo; //从网络接收的事件数组
////};
////struct TestResultInfo 
////{
////	BOOL IsMaxLength; //是否是最长曲线
////	BOOL IsLowerStandard;//wcq2011.09.26根据与ZOOM_VERTICAL_STANDARD比较而为true或false.用于竖直方向缩放时确定最大量程
////	BOOL IsLoadFromFile;//是否从文件加载上来的曲线//wcq2011.10.19
////	WORD DataType; //数据类型， 有数据8888，无数据4444
////	WORD TestMode; //测试模式，平均测试2222，
////	WORD DataSerialNum; //数据包序号
////	WORD* pDataPoints; //采样点值 扩大了1000倍的值
////	WORD* pDataCurve; //保存取点后计算过的数据
////	short ScaleFactor;//缩放系数，每个数据点都乘该系数
////	int YOffset;//竖直方向偏移，用于平移
////	unsigned int StartPoint;  //曲线横坐标的起点
////	unsigned int MaxPoint;    //曲线横坐标点数的最大值
////	int FullPointCount;//曲线缩放时为多少个点时曲线占满整个绘图区域,用以确定曲线宽度wcq2011.09.16
////	int FullPointCountMin;//曲线未缩放,为多少个点时曲线占满整个绘图区域,用以确定曲线宽度,FullPointCount为该数的倍数。wcq2011.09.21
////	int LastZoomPointCount;//曲线缩放时最后一次缩放共多少点，和前面的缩放的点的个数不一定一样。wcq2011.09.21
////	INT DataSpacing;//for each pulse width used:one-way time(units of 100 psec) for 10000 data points//wcq2011.10.19理解为两点间距离
////	float Refraction; //折射率
////	float ChainLoss; //链损耗，扩大了1000倍
////	float ChainAttenu; //链衰减，扩大了1000倍
////	float MaxTestDistance; //测量曲线最大距离
////	unsigned long int DataPointsNum; //采样点个数
////	long int Frequency; //频率
////	long int Range; //测量范围
////	long int PulseWidth; //脉冲宽度
////	long int WaveLength; //中心波长
////	long int MeasureTiem; //测量时间
////	long int ChainLength; //链长
////	CPoint* ptDrawData; //画图坐标点,每个类对应一个绘图数据指针,原来名称为COTDRPage类中m_ptRecord wcq2011.09.21
////	EventInfoList EventList;//事件列表
////};
