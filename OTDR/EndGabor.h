// GABOR.h : GABOR DLL 的主头文件
//

#pragma once
#define CLASSINDLL_CLASS_DECL __declspec(dllexport)

#ifndef __AFXWIN_H__
#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号


struct Gabor_Result     //存贮结构体
{
	Gabor_Result()
	{
		memset(Event_Array,0,sizeof(Event_Array));
		memset(Event_Loss,0,sizeof(Event_Loss));
		memset(Reflect_Type,0,sizeof(Reflect_Type));
		memset(Event_Hight,0,sizeof(Event_Hight));
		memset(Line_k,0,sizeof(Line_k));
		memset(Total_Loss,0,sizeof(Total_Loss));
		Trail_End=10;
		Valid_Num=0;
	}
	int Reflect_Type[100];  //事件类型	//Event Code
	int Event_Array[100];  //事件位置  //Event Propagation Time
	float  Event_Loss[100];       //损耗//Event Loss
	float Event_Hight[100]; //事件点反射dB
	float Line_k[100];//区段斜率
	float  Total_Loss[100];  //总损耗、累计损耗
	float  Reflect;   //发射
	int Valid_Num;    //有效长度
	float Trail_End;//尾端位置
	char Event_type[100];// event code 第二个参数  S:开始事件 E:光纤末端即结束事件；A：用户自加； F:软件自行查找；O:超出测量范围的光纤末端 D: zll2012.08.22
};//结构体的定义和位置随意

//extern "C" _declspec(dllexport) Gabor_Result deleteEvent(int deleteNum,int wavelength,int pulsewidth,WORD input_data[],Gabor_Result sGabor_Result,float Index_N,float Sample_freaq);
//extern "C" _declspec(dllexport) Gabor_Result insertEvent(int insertNum,int wavelength,int pulsewidth,WORD input_data[],int paramLen,Gabor_Result sGabor_Result,float Index_N,float Sample_freaq,float Return_loss_Tybe);
//extern "C" _declspec(dllexport) Gabor_Result gabor_arithmetic(int wavelength,int pulseWidth,WORD input_data[],INT paramLen,double thresholdUpSet,double thresholdDownSet,float dB_Loss,float dB_reflex,float dB_End,float Return_loss_Tybe,float Sample_freaq,float Index_N,int link_point_number);

class CLASSINDLL_CLASS_DECL CEndGabor
{
public:
	CEndGabor(void);
	~CEndGabor(void);
public:
	Gabor_Result deleteEvent(int deleteNum,int wavelength,int pulsewidth,WORD input_data[],Gabor_Result sGabor_Result,float Index_N,float Sample_freaq,float Return_loss_Tybe);
	Gabor_Result insertEvent(int insertNum,int wavelength,int pulsewidth,WORD input_data[],int paramLen,Gabor_Result sGabor_Result,float Index_N,float Sample_freaq,float Return_loss_Tybe);
	Gabor_Result gabor_arithmetic(int wavelength,int pulseWidth,WORD input_data[],INT paramLen,double thresholdUpSet,double thresholdDownSet,float dB_Loss,float dB_reflex,float dB_End,float Return_loss_Tybe,float Sample_freaq,float Index_N,int link_point_number);
	Gabor_Result Acount_Total_Loss(int wavelength,int pulsewidth,WORD input_data[],Gabor_Result sGabor_Result,float Index_N,float Sample_freaq);
public:
	LPVOID m_lpVoid;
};



// CEndGaborApp
// 有关此类实现的信息，请参阅 GABOR.cpp
//

class CEndGaborApp : public CWinApp
{
public:
	CEndGaborApp();

	// 重写
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
