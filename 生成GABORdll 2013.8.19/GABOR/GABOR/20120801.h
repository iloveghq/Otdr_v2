#include <math.h>
#include "EndGabor.h"
#define Avg_loss_1310  0.325
#define Avg_loss_1550  0.182
#define Avg_loss_1625  0.192
#define Return_loss_1310_tybe 79.5
#define Return_loss_1550_tybe 81
#define Return_loss_1625_tybe 80.5

class C20120801
{
public:
Gabor_Result K_Line_count(int puse_point,int wavelength,int pulsewidth,WORD input_data[],Gabor_Result sGabor_Result,float Index_N,float Sample_freaq);
Gabor_Result Acount_Loss(int wavelength,int pulsewidth,WORD input_data[],Gabor_Result sGabor_Result,float Index_N,float Sample_freaq);
Gabor_Result Acount_Total_Loss(int wavelength,int pulsewidth,WORD input_data[],Gabor_Result sGabor_Result,float Index_N,float Sample_freaq);
Gabor_Result Acount_return_loss(int wavelength,int pulsewidth,WORD input_data[],Gabor_Result sGabor_Result,float Index_N,float Sample_freaq,float Return_loss_Tybe);
Gabor_Result K_Line_limite(int wavelength,Gabor_Result sGabor_Result);
//******************************************************************//
//版本号//
CString Version_number();
int lEventGenerate(int pulseNum,int wavelength,float Sample_freaq);
//******************************************************************//
//     事件分析
//     L_event:分析事件步进，跟脉宽和采样分辨率有关系
//     wavelength:曲线波长
//     input_data[]：分析的原始数据
//     paramLen:原始数据长度
//     thresholdUpSet:短时傅里叶变化阈值上限
//     thresholdDownSet:短时傅里叶变化阈值下限
//     dB_Loss:衰减门限
//     dB_reflex:反射门限
//     dB_End:末端判断门限
//******************************************************************//
Gabor_Result gabor_generate(int pulsewidth,int wavelength,WORD input_data[],int paramLen,double thresholdUpSet,double thresholdDownSet,float dB_Loss,float dB_reflex,float dB_End,float Sample_freaq,float Index_N,int link_point_number);
/***********************************************************************/
///pulseWidth：脉宽
///wavelength:波长值，0:1310,1:1550
///input_data[]:采样数据
///paramLen：采样数据长度
//double thresholdUpSet:傅里叶变化阈值上限。默认为2.08
//double thresholdDownSet傅里叶变化阈值下限.默认为2.08
///dB_Loss:衰减判断门限
///dB_reflex：反射判断门限
///dB_End：末端判断门限默认为3
///Return_loss_Tybe：典型散射系数
// Sample_freaq:采样频率
/**********************************************************************/
Gabor_Result gabor_arithmetic(int wavelength,int pulseWidth,WORD input_data[],INT paramLen,double thresholdUpSet,double thresholdDownSet,float dB_Loss,float dB_reflex,float dB_End,float Return_loss_Tybe,float Sample_freaq,float Index_N,int link_point_number);
//******************************************************************//
//     添加事件
//     insertNum:所要插入的事件点的数据点编号
//     wavelength:曲线波长
//     pulsewidth：曲线脉宽
//     input_data[]：分析的原始数据
//     Return_loss_Tybe:典型散射系数
//     paramLen:原始数据长度
//******************************************************************//
Gabor_Result insertEvent(int insertNum,int wavelength,int pulsewidth,WORD input_data[],int paramLen,Gabor_Result sGabor_Result,float Index_N,float Sample_freaq,float Return_loss_Tybe);
//******************************************************************//
//     删除事件
//     deleteNum:所删除事件点序号
//     wavelength:曲线波长
//     pulsewidth：曲线脉宽
//     input_data[]：分析的原始数据
//******************************************************************//
Gabor_Result deleteEvent(int deleteNum,int wavelength,int pulsewidth,WORD input_data[],Gabor_Result sGabor_Result,float Index_N,float Sample_freaq,float Return_loss_Tybe);
//用于光纤光栅的事件分析，将每一个事件的位置定位为顶峰
Gabor_Result Optical_grating(int wavelength,int pulsewidth,WORD input_data[],INT paramLen,Gabor_Result sGabor_Result,float Index_N,float Sample_freaq,float dB_reflex);

};