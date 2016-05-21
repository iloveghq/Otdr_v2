#pragma once
#include <vector>

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
//读写sor文件时，传入传出参数
//为使修改程序中对应的参数时不会影响到sor.dll修改，所以单独拿出来
//wcq2012.07.23
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
//事件
struct Event
{
	INT propagationTime;//连接后第一个e事件点时间
	short attenCoefficent;//衰减系数
	short eventLoss;//事件损耗
	INT   reflectance;//事件反射
	std::vector<char> code;//事件标志固定6字节6
	std::vector<char> lossMeasurement;//固定2字节2
	std::vector<char> comment;//注释
};
//数据
struct Data
{
	INT nCount;//编号
	short scaleFactor;//编码2字节 系数
	std::vector<unsigned short> vData;//数据
};
struct SORDATA
{
	std::vector<Data> vAllData;//所有数据
	long int Range; //测量范围 sor里对应 Acquisition Range Distance(Acquisition Range是用时间表示距离) ,单位:m
	long int PulseWidth; //脉冲宽度
	long int WaveLength; //中心波长
	float Frequency; //频率
	float Refraction; //折射率
	float Length; //曲线长度(光纤长度),
	float DataSpacing;//for each pulse width used:one-way time(units of 100 psec) for 10000 data points//wcq2011.10.19理解为两点间距离//wcq2011.11.30

	////wcq2012.10.30
	float BackscatterCoefficient;//散射系数 wcq2012.10.30
	float LossThreshold;//衰减门限 wcq2012.10.30
	float ReflectanceThreshold;//反射门限 wcq2012.10.30
	float EndOfFiberThreshold;//光纤末端门限 wcq2012.10.30
	time_t TestTimestamp;    //测量的时间点lzy2014.8.8
	UINT AverageCount;   //平均的次数lzy2014.8.8

	std::vector<Event> vEvent; //
};