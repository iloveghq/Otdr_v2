// EndGabor.cpp : 实现文件
//

#include "stdafx.h"
#include "GABOR.h"
#include "EndGabor.h"
#include "20120801.h"

// CEndGabor

CEndGabor::CEndGabor()
{
	m_lpVoid=new C20120801();
}

CEndGabor::~CEndGabor()
{
	if (m_lpVoid)
	{
		delete[] m_lpVoid;
	}
}


// CEndGabor 消息处理程序
Gabor_Result CEndGabor::deleteEvent(int deleteNum,int wavelength,int pulsewidth,WORD input_data[],Gabor_Result sGabor_Result,float Index_N,float Sample_freaq,float Return_loss_Tybe)
{
	return ((C20120801*)m_lpVoid)->deleteEvent(deleteNum,wavelength,pulsewidth,input_data,sGabor_Result,Index_N,Sample_freaq,Return_loss_Tybe);
}

Gabor_Result CEndGabor::insertEvent(int insertNum,int wavelength,int pulsewidth,WORD input_data[],int paramLen,Gabor_Result sGabor_Result,float Index_N,float Sample_freaq,float Return_loss_Tybe)
{
	return((C20120801*)m_lpVoid)->insertEvent(insertNum,wavelength,pulsewidth,input_data,paramLen,sGabor_Result,Index_N,Sample_freaq,Return_loss_Tybe);
}

Gabor_Result CEndGabor::gabor_arithmetic(int wavelength,int pulseWidth,WORD input_data[],INT paramLen,double thresholdUpSet,double thresholdDownSet,float dB_Loss,float dB_reflex,float dB_End,float Return_loss_Tybe,float Sample_freaq,float Index_N,int link_point_number)
{
	return ((C20120801*)m_lpVoid)->gabor_arithmetic( wavelength, pulseWidth, input_data, paramLen, thresholdUpSet, thresholdDownSet, dB_Loss, dB_reflex, dB_End, Return_loss_Tybe, Sample_freaq, Index_N, link_point_number);
}

