#include "stdafx.h"
#include <math.h>
#include "20120801.h"


CString C20120801::Version_number()
{
	CString a_Version=_T("20130626-P");
	return a_Version;
}
Gabor_Result C20120801::K_Line_count(int puse_point,int wavelength,int pulsewidth,WORD input_data[],Gabor_Result sGabor_Result,float Index_N,float Sample_freaq)
{

	//先求各段光纤的斜率：只要计算删除点所在点的下一个点的特性
	int up_limit,low_limit;
	int step_event=puse_point*2;
	if(pulsewidth>=2000)/////////////////////////////////////////20130615
		step_event=pulsewidth/25*(Sample_freaq/25);
	if(step_event<20)step_event=20;
	float* sum_k=new float[2000];//短时傅里叶变化结果Cn
	memset(sum_k,0,2000*sizeof(float));
	//double sum_k[20];
	double dB_k_k_1=0,dB_k_k_2=0;

	double Light_fiber_speed=300/Index_N;//缩小了10^6倍
	//两个数据之间的时间间隔：
	double Sample_time=Sample_freaq;//放大了10^6倍
	Sample_time=1/Sample_time;
	//测量距离计算
	int Event_start_mark_cn=sGabor_Result.Event_Array[0];
	int for_num=0;
	int i=0,j=0,k=0;
	float damage_value=0;
	for(i=0;i<sGabor_Result.Valid_Num-1;i++)
	{
		for_num=0;

		low_limit=sGabor_Result.Event_Array[i+1]-10;
		if(low_limit<=0)low_limit=10;
		up_limit=sGabor_Result.Event_Array[i]+(sGabor_Result.Event_Array[i+1]-sGabor_Result.Event_Array[i])/8;
		if(up_limit<=0)break;
		if(up_limit<sGabor_Result.Event_Array[i]+3*step_event/2)up_limit=sGabor_Result.Event_Array[i]+3*step_event/2;//20130625

		low_limit=low_limit-(sGabor_Result.Event_Array[i+1]-sGabor_Result.Event_Array[i])/8;


		if(low_limit<=sGabor_Result.Event_Array[i]+step_event/8)//相邻两个事件的距离太近，不适合算斜率，
		{
			sGabor_Result.Line_k[i+1]=0;
			continue;
		}
		else
		{
			sGabor_Result.Line_k[i+1]=0;
			for_num=(sGabor_Result.Event_Array[i+1]-sGabor_Result.Event_Array[i])/8;
			//	if(for_num<20)for_num=20;
			//else
			if(for_num>200)for_num=200;
			for(k=4;k<for_num;k++)
			{
				if(up_limit-k<=0)break;
				dB_k_k_1=input_data[up_limit+k];
				dB_k_k_2=input_data[low_limit+k];
				if(low_limit-up_limit>0)
					sum_k[k]=(dB_k_k_2-dB_k_k_1)/((low_limit-up_limit)*(Sample_time*Light_fiber_speed)/2);
				sGabor_Result.Line_k[i+1]=sGabor_Result.Line_k[i+1]+sum_k[k];
			}
			if(k!=5)
				sGabor_Result.Line_k[i+1]=(sGabor_Result.Line_k[i+1]/(k-5));
			else sGabor_Result.Line_k[i+1]=sGabor_Result.Line_k[i+1];
			if(wavelength==1310)//1310
			{
				if(sGabor_Result.Line_k[i+1]<0.28||sGabor_Result.Line_k[i+1]>0.42)
				{
					//sGabor_Result.Line_k[i+1]=0;//0;//0.326;
					damage_value=(sGabor_Result.Line_k[i+1]-Avg_loss_1310)/10;
					if(((low_limit-up_limit)*(Sample_time*Light_fiber_speed)/2)>400)
					{
						sGabor_Result.Line_k[i+1]=Avg_loss_1310+damage_value;
					}
					else sGabor_Result.Line_k[i+1]=sGabor_Result.Line_k[i+1];
				}

			}
			else if(wavelength==1550)//1550
			{
				if(sGabor_Result.Line_k[i+1]<0.12||sGabor_Result.Line_k[i+1]>0.24)//sGabor_Result.Line_k[i+1]=0;//0;//0.184;
				{
					damage_value=(sGabor_Result.Line_k[i+1]-Avg_loss_1550)/10;
					if(((low_limit-up_limit)*(Sample_time*Light_fiber_speed)/2)>400)
					{
						sGabor_Result.Line_k[i+1]=Avg_loss_1550+damage_value;
					}
					else sGabor_Result.Line_k[i+1]=sGabor_Result.Line_k[i+1];
				}
			}
			else if(wavelength==1625)//1625
			{
				if(sGabor_Result.Line_k[i+1]<0.12||sGabor_Result.Line_k[i+1]>0.26)//sGabor_Result.Line_k[i+1]=0;//0;//0.184;
				{
					damage_value=(sGabor_Result.Line_k[i+1]-Avg_loss_1625)/10;
					if(((low_limit-up_limit)*(Sample_time*Light_fiber_speed)/2)>400)
					{
						sGabor_Result.Line_k[i+1]=Avg_loss_1625+damage_value;
					}
					else sGabor_Result.Line_k[i+1]=sGabor_Result.Line_k[i+1];
				}
			}

		}

	}
	delete[] sum_k;
	return sGabor_Result;
}

int C20120801::lEventGenerate(int pulseNum,int wavelength,float Sample_freaq)
{
	float kkk=0;
	int Freaq_num=0;
	Freaq_num=Sample_freaq/25;
	if(Freaq_num<=0)Freaq_num=1;
	//int lEvent=200;
	////	Sample_freaq=25;
	//switch(pulseNum)
	//{
	//case 0://3ns
	//	lEvent=3;
	//	break;
	//case 1://5ns
	//	lEvent=10;
	//	break;
	//case 2://10ns
	//	lEvent=20;
	//	break;
	//case 3://20ns
	//	lEvent=25;
	//	break;
	//case 4://50ns
	//	lEvent=25;
	//	break;
	//case 5://100ns
	//	lEvent=35;
	//	break;
	//case 6://200ns
	//	lEvent=50;
	//	break;
	//case 7://500ns
	//	lEvent=55;	
	//	break;
	//case 8://1us
	//	lEvent=60;
	//	break;
	//case 9://2us
	//	lEvent=75;
	//	break;
	//case 10://5us
	//	lEvent=125;
	//	break;
	//case 11://10us
	//	lEvent=250;
	//	break;
	//default:
	//	lEvent=250;
	//	break;
	//}
	int lEvent=200;
	//	Sample_freaq=25;
	switch(pulseNum)
	{
	case 0://3ns
		//kkk=
		switch(Freaq_num)
		{
		case 1:lEvent=1;break;  //25M
		case 2:lEvent=2;break;  //50M
		case 4:lEvent=3;break;  //100M
		case 8:lEvent=5;break;  //200M
		case 16:lEvent=10;break;  //400M
		default:lEvent=5;break;
		}
		//lEvent=1;
		break;
	case 1://5ns
		//lEvent=2;
		switch(Freaq_num)
		{
		case 1:lEvent=4;break;  //25M
		case 2:lEvent=6;break;  //50M
		case 4:lEvent=7;break;  //100M
		case 8:lEvent=8;break;  //200M
		case 16:lEvent=10;break;  //400M
		default:lEvent=10;break;
		}
		break;
	case 2://10ns
		//lEvent=4;
		switch(Freaq_num)
		{
		case 1:lEvent=5;break;  //25M
		case 2:lEvent=7;break;  //50M
		case 4:lEvent=8;break;  //100M
		case 8:lEvent=9;break;  //200M
		case 16:lEvent=10;break;  //400M
		default:lEvent=10;break;
		}
		break;
	case 3://20ns
		//lEvent=25;
		switch(Freaq_num)
		{
		case 1:lEvent=6;break;  //25M
		case 2:lEvent=7;break;  //50M
		case 4:lEvent=8;break;  //100M
		case 8:lEvent=12;break;  //200M
		case 16:lEvent=14;break;  //400M
		default:lEvent=15;break;
		}
		break;
	case 4://50ns
		//lEvent=25;
		switch(Freaq_num)
		{
		case 1:lEvent=12;break;  //25M
		case 2:lEvent=18;break;  //50M
		case 4:lEvent=20;break;  //100M
		case 8:lEvent=40;break;  //200M
		case 16:lEvent=60;break;  //400M
		default:lEvent=100;break;
		}
		break;
	case 5://100ns
		//lEvent=35;
		switch(Freaq_num)
		{
		case 1:lEvent=30;break;  //25M
		case 2:lEvent=40;break;  //50M
		case 4:lEvent=80;break;  //100M
		case 8:lEvent=120;break;  //200M
		case 16:lEvent=140;break;  //400M
		default:lEvent=30;break;
		}
		break;
	case 6://200ns
		//lEvent=50;
		switch(Freaq_num)
		{
		case 1:lEvent=40;break;  //25M
		case 2:lEvent=60;break;  //50M
		case 4:lEvent=80;break;  //100M
		case 8:lEvent=140;break;  //200M
		case 16:lEvent=140;break;  //400M
		default:lEvent=50;break;
		}
		break;
	case 7://500ns
		//lEvent=55;
		switch(Freaq_num)
		{
		case 1:lEvent=60;break;  //25M
		case 2:lEvent=100;break;  //50M
		case 4:lEvent=160;break;  //100M
		case 8:lEvent=180;break;  //200M
		case 16:lEvent=200;break;  //400M
		default:lEvent=100;break;
		}
		break;
	case 8://1us
		//lEvent=60;
		switch(Freaq_num)
		{
		case 1:lEvent=100;break;  //25M
		case 2:lEvent=160;break;  //50M
		case 4:lEvent=200;break;  //100M
		case 8:lEvent=240;break;  //200M
		case 16:lEvent=280;break;  //400M
		default:lEvent=120;break;
		}
		break;
	case 9://2us
		//lEvent=75;
		switch(Freaq_num)
		{
		case 1:lEvent=160;break;  //25M
		case 2:lEvent=200;break;  //50M
		case 4:lEvent=240;break;  //100M
		case 8:lEvent=280;break;  //200M
		case 16:lEvent=320;break;  //400M
		default:lEvent=160;break;
		}
		break;
	case 10://5us
		//lEvent=100;
		switch(Freaq_num)
		{
		case 1:lEvent=180;break;  //25M
		case 2:lEvent=240;break;  //50M
		case 4:lEvent=280;break;  //100M
		case 8:lEvent=320;break;  //200M
		case 16:lEvent=400;break;  //400M
		default:lEvent=120;break;
		}
		break;
	case 11://10us
		//lEvent=200;
		switch(Freaq_num)
		{
		case 1:lEvent=300;break;  //25M
		case 2:lEvent=350;break;  //50M
		case 4:lEvent=400;break;  //100M
		case 8:lEvent=500;break;  //200M
		case 16:lEvent=600;break;  //400M
		default:lEvent=200;break;
		}
		break;
	case 12://20us
		switch(Freaq_num)
		{
		case 1:lEvent=450;break;  //25M
		case 2:lEvent=450;break;  //50M
		case 4:lEvent=500;break;  //100M
		case 8:lEvent=550;break;  //200M
		case 16:lEvent=700;break;  //400M
		default:lEvent=200;break;
		}
		break;
	default:
		//lEvent=200;
		switch(Freaq_num)
		{
		case 1:lEvent=1;break;  //25M
		case 2:lEvent=1;break;  //50M
		case 4:lEvent=1;break;  //100M
		case 8:lEvent=2;break;  //200M
		case 16:lEvent=3;break;  //400M
		default:lEvent=3;break;
		}
		break;
	}
	//lEvent=lEvent*Freaq_num;
	/*if(Sample_freaq<=25)Sample_freaq=25;
	if(Sample_freaq>=400)Sample_freaq=400;
	Sample_freaq=Sample_freaq/25;
	lEvent=lEvent*Sample_freaq;
	if(lEvent<=4)lEvent=3;*/
	return lEvent;
}
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
Gabor_Result C20120801::gabor_generate(int pulsewidth,int wavelength,WORD input_data[],int paramLen,double thresholdUpSet,double thresholdDownSet,float dB_Loss,float dB_reflex,float dB_End,float Sample_freaq,float Index_N,int link_point_number)
{
	//   L_event事件点，pulse=10us=1km，4m/点，即为250点。L_event=25~50。（1/10~1/5）*事件点
	WORD max_input_data=10000;//查找input_data数据最小值，其后忽略
	double gabor_std=0;//gabor_result结果的RMS值
	int input_length=paramLen;
	int length_real=0;//剔除最小值之后的真实长度
	int line_n=20;//直线拟合的抽样长度
	double ratio=0.001;//短时傅里叶变化系数0.001-1之间 

	double thresholdUp=thresholdUpSet;//发生概率，阈值上限
	double thresholdDown=thresholdDownSet;//发生概率，阈值下限

	int N_event=3;//事件长度点数 
	int i=0;//循环点数
	int j=0;//循环点数
	int k=0;//循环点数 
	int l=0;//循环点数
	char Char_type=0;
	float float_type=0;
	float int_type=0;
	int gabor_length=10;
	int maxCn_mark=10;//曲线尾段事件位置
	double min_db=2;//最小检测DB衰减值
	int part_sub_table=0;//分段事件分析时下限
	int part_up_table=0;//分段事件分析时上限
	int input_data_part_max_sub=0;//分段事件分析时最大值的下标
	int input_data_part_min_sub=0;
	double part_ref_reflect=0;//分段事件分析时反射情况的参考值
	double part_std=0;//分段事件分析时分段的标准差
	double part_mean=0;//分段事件分析时分段的平均值
	double input_data_part_min=0;//分段事件分析时最小值
	double input_data_part_max=0;//分段事件分析时最大值
	double gabor_result_sum= 0;//Gabor_result之和
	double gabor_result_mean=0;//Gabor_result平均值
	double std_end=0;//尾端的RMS值


	//int input_data_buff[128000];
	int* input_data_buff=new int[input_length];//短时傅里叶变化结果Cn
	memset(input_data_buff,0,input_length*sizeof(int));

	//double line_fit_factor[2][100];

	//double gaborResultTemp[2000];
	//memset(gaborResultTemp,0,sizeof(gaborResultTemp));
	Gabor_Result sGabor_Result;//结构体存贮结果
	//memset(line_fit_factor,0,sizeof(line_fit_factor));

	int return_flag=0;
	int pulseNum=11;
	int L_event=0;
	switch(pulsewidth)
	{
	case 3:
		pulseNum=0;
		break;
	case 5:
		pulseNum=1;
		break;
	case 10:
		pulseNum=2;
		break;
	case 20:
		pulseNum=3;
		break;
	case 50:
		pulseNum=4;
		break;
	case 100:
		pulseNum=5;
		break;
	case 200:
		pulseNum=6;
		break;
	case 500:
		pulseNum=7;	
		break;
	case 1000:
		pulseNum=8;
		break;
	case 2000:
		pulseNum=9;
		break;
	case 5000:
		pulseNum=10;
		break;
	case 10000:
		pulseNum=11;
		break;
	case 20000:
		pulseNum=12;
		break;

	default:pulseNum=12;break;
	}
	L_event=lEventGenerate(pulseNum,wavelength,Sample_freaq);

	int step_event=L_event*2;

	if(pulsewidth>=2000)/////////////////////////////////////////20130615
		step_event=pulsewidth/25*(Sample_freaq/25)*11/10;
	//step_event=900;

	//光在光纤中传播的速度
	double Light_fiber_speed=300/Index_N;//缩小了10^6倍
	//两个数据之间的时间间隔：
	double Sample_time=Sample_freaq;//放大了10^6倍
	Sample_time=1/Sample_time;
	//测量距离计算
	double Measure_distacne = Light_fiber_speed*Sample_time*paramLen;


	//int L_event=0;
	//switch(pulse_no)

	//switch(wavelength)//根据功率等级设置阈值上下限
	//{
	//case 0:
	//	{
	//		thresholdUp=2.08;//发生概率，阈值上限
	//		thresholdDown=2.08;//发生概率，阈值下限
	//	}
	//	break;
	//default:
	//	{
	//		thresholdUp=2.08;//发生概率，阈值上限
	//		thresholdDown=2.08;//发生概率，阈值下限
	//	}
	//	break;
	//}
	//末端判断

	long Sond_data_Sum=0;
	int More_than_flag_count=0;
	int Less_than_flag_count=0;
	int data_buff=0;
	int count_number=0;
	float percentage_flag=0;
	char end_ok_flag=0;
	int end_ok_count=0;
	long Avg_data=0;
	int data_length=0;
	if(input_length<3000)data_length=201;
	else data_length=501;
	if(input_length>501)
	{
		for(i=input_length-1;i>input_length-data_length;i--)
		{
			Sond_data_Sum=Sond_data_Sum+(65535-input_data[i]);
		}
		Sond_data_Sum=Sond_data_Sum/(data_length-1); //求噪声平均值
	}
	if(Sond_data_Sum>=5000)Sond_data_Sum=3000;//对噪声的最大平均值做限制
	//if(Sond_data_Sum>=4000)Sond_data_Sum=2000;//对噪声的最大平均值做限制20130710
	count_number=L_event;//L_event/2,20121108
	if(count_number<40)count_number=40;
	for(i=input_length-1;i>0;)//末端寻找
	{
		if(i>count_number)
		{

			//if(count_number<10)count_number=10;
			More_than_flag_count=0;
			Less_than_flag_count=0;
			Avg_data=0;
			for(j=i;j>i-count_number;j--)
			{
				data_buff=65535-input_data[j];
				Avg_data=Avg_data+data_buff;
				if(data_buff>(3*Sond_data_Sum))More_than_flag_count++;
				else Less_than_flag_count++;
			}
			Avg_data=Avg_data/count_number;

			percentage_flag=More_than_flag_count;
			percentage_flag=percentage_flag/count_number;
			if(percentage_flag>=0.8 && Avg_data>=3*Sond_data_Sum)
			{
				if(end_ok_flag==0)
					length_real=j+count_number*(percentage_flag);
				end_ok_flag=1;
				end_ok_count++;
				if(end_ok_count>=20)break;
				//break;
			}
			else 
			{
				i=i-count_number/2;
				end_ok_flag=0;
				end_ok_count=0;
			}

		}
		else 
		{
			length_real=input_length-1;
			break;
		}

	}
	if(length_real>=input_length-step_event)length_real=input_length-step_event-2;
	//length_real=input_length-L_event;
	//细化末端,选取末端的前后两个波长的数据用于分析
	if(length_real>3*step_event&&input_length>length_real-step_event)
	{
		//int input_data_buff_statick[4000];
		//int* input_data_buff_statick=new int[2*step_event+2];//短时傅里叶变化结果Cn
		//memset(input_data_buff_statick,0,(2*step_event+2)*sizeof(int));
		Avg_data=0;
		for(i=length_real-step_event;i<length_real+step_event;i++)
		{
			Avg_data=Avg_data+(65535-input_data[i]);
		}
		Avg_data=Avg_data/(2*step_event);
		//末端为反射类型的判断,求差值
		int MAX_input_data_buff_statick=0;
		int Avg_four_data=0;
		int MAX_cmaxk=0;
		for(i=length_real-step_event;i<length_real+step_event;i++)
		{
			input_data_buff[i+step_event-length_real]=65535-input_data[i];
			input_data_buff[i+step_event-length_real]=input_data_buff[i+step_event-length_real]-Avg_data;
			if(MAX_input_data_buff_statick<input_data_buff[i+step_event-length_real])
			{
				MAX_input_data_buff_statick=input_data_buff[i+step_event-length_real];
				MAX_cmaxk=i;
			}

		}
		int avg1=0;
		int avg2=0;
		int avg3=0;
		int max_avg=0;
		int max_avg_mark=0;

		int data_Max_j=0;
		int max_count_j=0;
		for(i=length_real-step_event;i<length_real-step_event+5;i++)
		{
			avg1=avg1+65535-input_data[i];
		}
		avg1=avg1/5;
		//if(MAX_cmaxk>=length_real-step_event+8)//最大值内置不在末端位置附近
		{
			for(i=MAX_cmaxk-8;i<MAX_cmaxk+8;i++)
			{
				data_Max_j=65535-input_data[i]-avg1;
				if(data_Max_j>100)
				{
					max_count_j++;
					if(max_count_j>=4)//反射峰判断
					{
						length_real=i-4;
						break;
					}
				}
				else max_count_j=0;
			}
		}

		if(max_count_j<4)//没有反射峰的情况判断，找突变的情况
		{

			for(i=length_real-step_event;i<length_real+step_event-1;i++)
			{
				input_data_buff[i+step_event-length_real]=input_data[i+1]-input_data[i];
				avg1=avg1+input_data_buff[i+step_event-length_real];
			}
			avg1=avg1/(2*step_event-1);
			for(i=length_real-step_event;i<length_real+step_event-1;i++)
			{
				if(input_data_buff[i+step_event-length_real]>avg1/2)
				{
					max_count_j++;
					if(max_count_j>=5)
					{
						length_real=i-5;
						break;
					}
				}
				else max_count_j=0;
			}
		}
		//	delete[] input_data_buff_statick;
	}
	//////////////////////////20ns以内的数据采用另外一种事件分析算法
	Measure_distacne = Light_fiber_speed*Sample_time*length_real;
	int Lange_count=0;
	j=0;
	//int input_data_buff[128000];
	//	int* input_data_buff=new int[input_length];//短时傅里叶变化结果Cn
	//	memset(input_data_buff,0,input_length*sizeof(int));
	int Max_data_true=0;
	float kkk=0;
	if(Measure_distacne<2000)kkk=2;//2km以内采用小脉宽测量时噪声系数不一样
	else kkk=3;
	if(pulsewidth<100)//针对脉宽<100ns的脉宽做特殊处理
	{
		Measure_distacne = Light_fiber_speed*Sample_time*length_real;
		//if(input_length>1001)
		{
			for(i=5;i<input_length;i++)
			{
				Sond_data_Sum=Sond_data_Sum+(65535-input_data[i]);
				if(Max_data_true<(65535-input_data[i]))
					Max_data_true=65535-input_data[i];
			}
			Sond_data_Sum=Sond_data_Sum/(input_length-5); //求噪声平均值
		}
		if(Max_data_true<3*Sond_data_Sum)kkk=1.5;
		for(i=5;i<input_length-3;i++)
		{
			input_data_buff[i]=input_data[i];
			input_data_buff[i]=(65535-input_data_buff[i])-kkk*Sond_data_Sum;
			if(input_data_buff[i]<=0)input_data_buff[i]=0;
		}
		for(i=5;i<input_length-4;i++)
		{
			input_data_buff[i]=input_data_buff[i+3]-input_data_buff[i];
			if(input_data_buff[i]<300)input_data_buff[i]=0;
		}
		/*sGabor_Result.Event_type[j]='F';
		sGabor_Result.Reflect_Type[j]=FALSE;
		sGabor_Result.Event_Array[j++]=4;*/
		int nn=0,mm=0,n=0,m=0,h=0;
		long Avg_xd_roise=0;
		if(Sample_freaq==400)
		{
			nn=L_event;//*、、*/20
			mm=3;
		}
		else if(Sample_freaq==200)
		{
			nn=L_event;//10
			mm=2;
		}
		else if(Sample_freaq==100)
		{
			nn=L_event;//10
			mm=2;
		}
		else if(Sample_freaq==50)
		{
			nn=L_event;//10
			mm=2;
		}
		else if(Sample_freaq==25)
		{
			nn=L_event;//10
			mm=2;
		}
		else
		{
			nn=L_event;//10
			mm=3;
		}
		//int LK=L_event/4;
		//if(LK<10)LK=10;
		for(i=5;i<input_length-3;i++)
		{
			if(input_data_buff[i]>0)
			{
				Lange_count++;
				if(Lange_count>nn)
				{


					if(j!=0)
						if((i+1-Lange_count)<sGabor_Result.Event_Array[j-1]+L_event/4)continue;
					sGabor_Result.Event_type[j]='F';
					sGabor_Result.Reflect_Type[j]=TRUE;
					sGabor_Result.Event_Loss[j]=0;
					sGabor_Result.Event_Hight[j]=0;
					sGabor_Result.Line_k[j]=0;
					sGabor_Result.Total_Loss[j]=0;

					sGabor_Result.Event_Array[j++]=i-Lange_count;//i+1-Lange_count  20120704
					Lange_count=0;
					if(j>=99)break;
				}
			}
			else 
			{
				if(Lange_count>=mm)
				{

					if(j!=0)
					{
						if((i+1-Lange_count)<sGabor_Result.Event_Array[j-1]+L_event/4)
						{
							Lange_count=0;
							continue;
						}
					}
					sGabor_Result.Event_type[j]='F';
					sGabor_Result.Reflect_Type[j]=TRUE;
					sGabor_Result.Event_Loss[j]=0;
					sGabor_Result.Event_Hight[j]=0;
					sGabor_Result.Line_k[j]=0;
					sGabor_Result.Total_Loss[j]=0;

					sGabor_Result.Event_Array[j++]=i-Lange_count;//i+1-Lange_count  20120704
					Lange_count=0;
					if(j>=99)break;
				}
				else Lange_count=0;
			}


		}
		if(j<20)//事件太多，判断不够严谨，重新判断，针对每一个事件做平均值分析
		{
			sGabor_Result.Event_type[j-1]='E';
			sGabor_Result.Valid_Num=j;
			return_flag=1;
			//求回波损耗
			//for()
			//return sGabor_Result;//element_result[1][1];
		}
		if(sGabor_Result.Valid_Num)
			length_real=sGabor_Result.Event_Array[sGabor_Result.Valid_Num-1];

	}
	//反射事件的判断
	if(return_flag==0)
	{
		//if(pulsewidth>=10000)L_event=200;
		int start_point_1=0;
		int end_point_1=start_point_1+L_event;
		int ratio_avg_1=0;
		int up_start_1=0;
		int less_count_time_1=0;
		int Lange_count_1=0;
		int for_count1=0;
		int for_count2=0;

		int length_count1=L_event/10;//L_event/4;
		int length_count2=L_event/12;//L_event/5;
		if(L_event<30)
		{
			length_count1=6;
			length_count2=4;
		}


		for(i=5;i<(length_real-L_event);)
		{

			for_count1++;
			if(for_count1>=(length_real-L_event))break;
			end_point_1=start_point_1+L_event;
			if(end_point_1>(length_real-L_event))
			{
				end_point_1=length_real+L_event;
				if(end_point_1>input_length)end_point_1=input_length-L_event-1;
			}
			ratio_avg_1=0;
			for(j=start_point_1;j<(end_point_1-2);j++)
			{
				input_data_buff[j-start_point_1]=input_data[j+2]-input_data[j];
				ratio_avg_1=ratio_avg_1+abs(input_data_buff[j-start_point_1]);
			}
			if((end_point_1-2)>start_point_1)
				ratio_avg_1=ratio_avg_1/(end_point_1-2-start_point_1);
			else ratio_avg_1=ratio_avg_1;
			j=start_point_1;
			for_count2=start_point_1;
			while(j<end_point_1-2)//for(j=start_point_1;j<(end_point_1-2);)//
			{
				for_count2++;
				if(for_count2>=(end_point_1-2))break;
				Lange_count_1=0;
				less_count_time_1=0;
				up_start_1=65535-input_data[j];
				for(k=j+1;k<(end_point_1-2);k++)
				{
					if((up_start_1+3*ratio_avg_1)<(65535-input_data[k]))
					{
						if(less_count_time_1)
						{

							Lange_count_1=0;
							less_count_time_1=0;
						}
						else 
						{
							//less_count_time=0;
							Lange_count_1++;
						}

					}
					else
					{
						if(Lange_count_1)
						{
							less_count_time_1=0;
							Lange_count_1=0;
						}
						else less_count_time_1++;
					}
					if(Lange_count_1>=length_count1)
					{
						if(sGabor_Result.Valid_Num<99)
						{

							sGabor_Result.Event_Array[sGabor_Result.Valid_Num]=abs(k-Lange_count_1);
							sGabor_Result.Reflect_Type[sGabor_Result.Valid_Num]=TRUE;
							sGabor_Result.Event_type[sGabor_Result.Valid_Num++]='F';
							j=k;
							break;
						}
					}
				}
				if((k==(end_point_1-2))&&(Lange_count_1>=length_count2))
				{
					if(sGabor_Result.Valid_Num<99)
					{
						sGabor_Result.Event_Array[sGabor_Result.Valid_Num]=abs(k-Lange_count_1);
						sGabor_Result.Reflect_Type[sGabor_Result.Valid_Num]=TRUE;
						sGabor_Result.Event_type[sGabor_Result.Valid_Num++]='F';
					}
					j=k;
					break;
				}
				j=k;

			}
			//start_point_1=end_point_1;
			i=start_point_1+L_event/2;
			start_point_1=i;

		}
	}
	j=sGabor_Result.Valid_Num;

	//增加反射事件
	if(return_flag==0)
	{
		j=0;
		for(i=0;i<sGabor_Result.Valid_Num;i++)
		{
			if(sGabor_Result.Reflect_Type[i]!=0)
			{
				int flag=0;
				for(k=0;k<j;k++)
				{
					if(abs(sGabor_Result.Event_Array[i]-sGabor_Result.Event_Array[k]<=L_event))
					{
						flag=1;
						break;
					}


				}
				if(flag==0)
				{

					sGabor_Result.Event_Array[j]=sGabor_Result.Event_Array[i];
					sGabor_Result.Reflect_Type[j]=sGabor_Result.Reflect_Type[i];
					j++;
				}

			}
		}
		sGabor_Result.Valid_Num=j;
	}
	//	sGabor_Result.Valid_Num=0;

	if(return_flag==0&&pulsewidth>=20)//&&pulsewidth>=20加于20130509
	{

		//L_event=step_event/2;
		int roias_count=0;
		int start_point =L_event;
		int end_point =length_real;
		int gabor_length=0;
		int buff_lenght=L_event+3;

		int Large_count_time=0;
		int small_count_time=0;

		float avg_buff=0;
		int No_count=0;
		No_count=6;//L_event/10;
		//if(No_count<=10)No_count=10;

		//double* Gabor_buff=new double[buff_lenght];
		//memset(Gabor_buff,0,buff_lenght*sizeof(double));

		//int* Gabor_buff=new int[buff_lenght+5];
		//memset(Gabor_buff,0,(buff_lenght+5)*sizeof(int));

		//int Gabor_buff[10000];

		gabor_length=2*(end_point-start_point)/L_event;
		int* result_loss=new int[100];//短时傅里叶变化结果Cn
			memset(result_loss,0,100*sizeof(int));
		int* result_loss2=new int[100];//短时傅里叶变化结果Cn
			memset(result_loss2,0,100*sizeof(int));
		//int result_loss[200];
		//int result_loss2[200];
		int result_loss_number=0;
		int result_loss_number2=0;
		for(i=0;i<gabor_length-1;i++)
		{
			

			//for(j=i*L_event;j<(i+1)*L_event+3;j++)//求出平均值
			//{
			//	avg_buff=avg_buff+65535-input_data[j];
			//}
			//avg_buff=avg_buff/(L_event+3);
			for(j=0;j<L_event;j++)
			{
				if(i*L_event/2+j+5>length_real)break;
				input_data_buff[j]=input_data[i*L_event/2+j+5]-input_data[i*L_event/2+j];///////i*L_event/2+j+5
				avg_buff=avg_buff+abs(input_data_buff[j]);
				//if(Gabor_buff[j]<10)Gabor_buff[j]=0;
			}
			if(j!=0)
				avg_buff=avg_buff/j;
			//	if(wavelength==1310)avg_buff=10;
			//	else avg_buff=8;
			//avg_buff=10;
			for(j=0;j<L_event;j++)
			{
				if(input_data_buff[j]>=avg_buff/4)
				{
					Large_count_time++;
					roias_count=0;
					if(Large_count_time>=No_count)
					{
						int cccc=0;
						int cccc_flag=0;
						if(result_loss_number<100)
						{

							for(k=0;k<result_loss_number;k++)
							{
								cccc=abs(i*L_event/2+j-Large_count_time-result_loss[k]);
								if(cccc<L_event)cccc_flag =1;

							}
							if(cccc_flag==0)
							{
								if((i*L_event/2+j-Large_count_time)>step_event/4)
								{

									if(abs(link_point_number-(i*L_event/2+j-Large_count_time))>=L_event/2)
										if(result_loss_number<99)
										result_loss[result_loss_number++]=i*L_event/2+j-Large_count_time;
									Large_count_time=0;
									break;
								}

							}
						}

						Large_count_time=0;
						break;
					}

				}
				else 
				{
					roias_count++;

					if(Large_count_time>=No_count)
					{
						int cccc=0;
						int cccc_flag=0;
						if(result_loss_number<100)
						{

							for(k=0;k<result_loss_number;k++)
							{
								cccc=abs(i*L_event/2+j-Large_count_time-result_loss[k]);
								if(cccc<L_event)cccc_flag =1;

							}
							if(cccc_flag==0)
							{
								if((i*L_event/2+j-Large_count_time)>step_event/4)
								{

									if(abs(link_point_number-(i*L_event/2+j-Large_count_time))>=L_event/2)
										if(result_loss_number<99)
										result_loss[result_loss_number++]=i*L_event/2+j-Large_count_time;
									Large_count_time=0;
									break;
								}

							}
						}
						if(roias_count>=5)
						{

							Large_count_time=0;
							break;
						}
					}
					Large_count_time=0;
				}

			}
		}
		//针对衰减事件变步进做第二次查找，然后两次查找的结果进行综合取舍20130628
		{
			int L_event_buff=2*L_event/3;;
			int step_event_buff=2*L_event_buff;
			for(i=0;i<gabor_length-1;i++)
			{


				//for(j=i*L_event;j<(i+1)*L_event+3;j++)//求出平均值
				//{
				//	avg_buff=avg_buff+65535-input_data[j];
				//}
				//avg_buff=avg_buff/(L_event+3);
				for(j=0;j<L_event_buff;j++)
				{
					if(i*L_event/2+j+5>length_real)break;
					input_data_buff[j]=input_data[i*L_event_buff/2+j+5]-input_data[i*L_event_buff/2+j];///////i*L_event/2+j+5
					avg_buff=avg_buff+abs(input_data_buff[j]);
					//if(Gabor_buff[j]<10)Gabor_buff[j]=0;
				}
				if(j!=0)
					avg_buff=avg_buff/j;
				//	if(wavelength==1310)avg_buff=10;
				//	else avg_buff=8;
				//avg_buff=10;
				for(j=0;j<L_event_buff;j++)
				{
					if(input_data_buff[j]>=avg_buff/4)
					{
						Large_count_time++;
						roias_count=0;
						if(Large_count_time>=No_count)
						{
							int cccc=0;
							int cccc_flag=0;
							if(result_loss_number<100)
							{

								for(k=0;k<result_loss_number;k++)
								{
									cccc=abs(i*L_event_buff/2+j-Large_count_time-result_loss2[k]);
									if(cccc<L_event_buff)cccc_flag =1;

								}
								if(cccc_flag==0)
								{
									if((i*L_event_buff/2+j-Large_count_time)>step_event_buff/4)
									{

										if(abs(link_point_number-(i*L_event_buff/2+j-Large_count_time))>=L_event_buff/2)
											if(result_loss_number2<99)
											result_loss2[result_loss_number2++]=i*L_event_buff/2+j-Large_count_time;
										Large_count_time=0;
										break;
									}

								}
							}

							Large_count_time=0;
							break;
						}

					}
					else 
					{
						roias_count++;

						if(Large_count_time>=No_count)
						{
							int cccc=0;
							int cccc_flag=0;
							if(result_loss_number<100)
							{

								for(k=0;k<result_loss_number;k++)
								{
									cccc=abs(i*L_event_buff/2+j-Large_count_time-result_loss2[k]);
									if(cccc<L_event_buff)cccc_flag =1;

								}
								if(cccc_flag==0)
								{
									if((i*L_event_buff/2+j-Large_count_time)>step_event_buff/4)
									{

										if(abs(link_point_number-(i*L_event_buff/2+j-Large_count_time))>=L_event_buff/2)
											if(result_loss_number2<99)
											result_loss2[result_loss_number2++]=i*L_event_buff/2+j-Large_count_time;
										Large_count_time=0;
										break;
									}

								}
							}
							if(roias_count>=5)
							{

								Large_count_time=0;
								break;
							}
						}
						Large_count_time=0;
					}

				}
			}
		}
		//对两次分析出来的衰减事件进行排序
		{
			int result_loss_data_buff=0;
			for(i=0;i<result_loss_number;i++)
			{
				for(j=i+1;j<result_loss_number;j++)
				{
					if(result_loss[j]<result_loss[i])
					{
						result_loss_data_buff=result_loss[i];
						result_loss[i]=result_loss[j];
						result_loss[j]=result_loss_data_buff;
					}
				}
			}
			for(i=0;i<result_loss_number2;i++)
			{
				for(j=i+1;j<result_loss_number2;j++)
				{
					if(result_loss2[j]<result_loss2[i])
					{
						result_loss_data_buff=result_loss2[i];
						result_loss2[i]=result_loss2[j];
						result_loss2[j]=result_loss_data_buff;
					}
				}
			}

		}
		//对两次分析出来的衰减事件进行整合取舍
		/*{
			for(i=0;i<result_loss_number;i++)
			{
				for(j=0;j<result_loss_number2;j++)
				{
					if(r)
				}
			}
		}*/

		//delete [] Gabor_buff;
		//	j=0;
		int flag=0;
		for(i=0;i<result_loss_number;i++)
		{
			flag=0;
			if(sGabor_Result.Valid_Num<100)
			{
				for(k=0;k<sGabor_Result.Valid_Num;k++)
				{
					if(abs(sGabor_Result.Event_Array[k]-result_loss[i])<step_event)
					{
						flag=1;break;
					}
				}
				if(flag==0)
				{
					if(sGabor_Result.Valid_Num<99)
					{

						sGabor_Result.Event_Array[sGabor_Result.Valid_Num]=result_loss[i];
						sGabor_Result.Event_Loss[sGabor_Result.Valid_Num]=0;
						sGabor_Result.Reflect_Type[sGabor_Result.Valid_Num]=0;
						sGabor_Result.Event_Hight[sGabor_Result.Valid_Num]=0;
						sGabor_Result.Total_Loss[sGabor_Result.Valid_Num]=0;
						sGabor_Result.Line_k[sGabor_Result.Valid_Num]=0;
						sGabor_Result.Event_type[sGabor_Result.Valid_Num]='F';
						sGabor_Result.Valid_Num++;
					}
				}
			}


		}
		flag=0;
		for(i=0;i<result_loss_number2;i++)
		{
			flag=0;
			if(sGabor_Result.Valid_Num<100)
			{
				for(k=0;k<sGabor_Result.Valid_Num;k++)
				{
					if(abs(sGabor_Result.Event_Array[k]-result_loss2[i])<step_event/2)
					{
						flag=1;break;
					}
				}
				if(flag==0)
				{
					if(sGabor_Result.Valid_Num<99)
					{

						sGabor_Result.Event_Array[sGabor_Result.Valid_Num]=result_loss2[i];
						sGabor_Result.Event_Loss[sGabor_Result.Valid_Num]=0;
						sGabor_Result.Reflect_Type[sGabor_Result.Valid_Num]=0;
						sGabor_Result.Event_Hight[sGabor_Result.Valid_Num]=0;
						sGabor_Result.Total_Loss[sGabor_Result.Valid_Num]=0;
						sGabor_Result.Line_k[sGabor_Result.Valid_Num]=0;
						sGabor_Result.Event_type[sGabor_Result.Valid_Num]='F';
						sGabor_Result.Valid_Num++;
					}
				}
			}


		}
		delete [] result_loss;
		delete [] result_loss2;

	}

	j=sGabor_Result.Valid_Num;

	i=0;
	k=0;



	if(sGabor_Result.Valid_Num)
	{

		if(sGabor_Result.Event_Array[sGabor_Result.Valid_Num-1]>=length_real)
		{
			sGabor_Result.Event_Array[sGabor_Result.Valid_Num-1]=length_real;
		}
		else if(sGabor_Result.Event_Array[sGabor_Result.Valid_Num-1]+step_event<=length_real)
			sGabor_Result.Event_Array[sGabor_Result.Valid_Num++]=length_real;
		//else if(sGabor_Result.Valid_Num==1)sGabor_Result.Event_Array[sGabor_Result.Valid_Num++]=step_event;//20130710
	}
	else 
	{

	}
	//对事件进行位置校准
	//if(return_flag==0)

	if(pulsewidth>=10)
	{

		if(sGabor_Result.Valid_Num)
		{
			//int* input_data_buff_statick=new int[2*step_event+2];
			//memset(input_data_buff_statick,0,(2*step_event+2)*sizeof(int));
			int avg1=0;
			int max_count_j=0;
			int max_count=0;
			for(i=0;i<sGabor_Result.Valid_Num;i++)
			{
				if(sGabor_Result.Reflect_Type[i]==0&&pulsewidth<5000)//非反射事件位置校准
				{

					if(sGabor_Result.Event_Array[i]>L_event/2)
					{

						if(i==sGabor_Result.Valid_Num-1)break;
						for(k=sGabor_Result.Event_Array[i]-L_event/2;k<sGabor_Result.Event_Array[i]+L_event/2-1;k++)
						{
							input_data_buff[k+L_event/2-sGabor_Result.Event_Array[i]]=input_data[k+1]-input_data[k];
							avg1=avg1+input_data_buff[k+L_event-sGabor_Result.Event_Array[i]];
						}
						avg1=avg1/(L_event-1);
						max_count_j=0;

						for(k=sGabor_Result.Event_Array[i]-L_event/2;k<sGabor_Result.Event_Array[i]+L_event/2-1;k++)
						{
							if(input_data_buff[k+L_event/2-sGabor_Result.Event_Array[i]]>avg1/2)
							{
								max_count_j++;
								if(max_count_j>=L_event/8)
								{
									sGabor_Result.Event_Array[i]=k-max_count_j;
									break;
								}
							}
							else max_count_j=0;
						}
					}
					/*if(k==sGabor_Result.Event_Array[i]+L_event/2-1)
					sGabor_Result.Event_Array[i]=sGabor_Result.Event_Array[i]+k/2;*/
				}
				else //if(length_real>sGabor_Result.Event_Array[i]+L_event/8)//反射事件位置校准
				{
					if(sGabor_Result.Event_Array[i]<10)continue;
					int min_data_buff=65535;
					int min_data_number=0;
					int data_buff_statick=0;
					int max_data_reflex=0;
					int max_data_reflex_mark_cn=0;
					float radio_avg=0;
					float data_avg=0;
					int mmm=0;
					k=L_event/4;
					if(k<6)k=6;
					mmm=sGabor_Result.Event_Array[i]+k;
					if(mmm>length_real)mmm=length_real;
					if(sGabor_Result.Event_Array[i]>=length_real)
					{
						continue;
					}
					for(j=sGabor_Result.Event_Array[i];j<mmm;j++)
					{
						data_buff_statick=65535-input_data[j];
						if(max_data_reflex<data_buff_statick)
						{
							max_data_reflex = data_buff_statick;
							max_data_reflex_mark_cn=j;
						}
					}

					//if(max_data_reflex_mark_cn=0)
					mmm=sGabor_Result.Event_Array[i]-k;
					if(mmm<0)mmm=0;
					for(j=max_data_reflex_mark_cn;j>mmm;j--)
					{
						data_buff_statick=65535-input_data[j];
						if(min_data_buff>data_buff_statick)
						{
							min_data_buff = data_buff_statick;
							min_data_number=j;
						}

					}

					/*for(j=sGabor_Result.Event_Array[i];j>abs(sGabor_Result.Event_Array[i]-k);j--)
					{
					data_buff_statick=65535-input_data[j];
					if(min_data_buff>data_buff_statick)
					{
					min_data_buff = data_buff_statick;
					min_data_number=j;
					}

					}
					for(j=sGabor_Result.Event_Array[i];j<sGabor_Result.Event_Array[i]+L_event/2;j++)
					{
					data_buff_statick=65535-input_data[j];
					if(max_data_reflex<data_buff_statick)
					{
					max_data_reflex = data_buff_statick;
					max_data_reflex_mark_cn=j;
					}
					}*/
					//求噪声平均值

					//int mark_start_cn=0
					mmm=min_data_number-10;
					if(mmm<0)mmm=0;
					for(j=mmm;j<min_data_number;j++)
					{
						data_buff_statick=input_data[j]-input_data[j-1];
						radio_avg=radio_avg+abs(data_buff_statick);
						data_avg=data_avg+(65535-input_data[j]);
					}
					if(min_data_number>mmm)
					{
						radio_avg=radio_avg/(min_data_number-mmm);
						data_avg=data_avg/(min_data_number-mmm);
						data_avg=data_avg+2*radio_avg;
					}
					else 
					{
						data_avg=0;
					}

					max_count_j=0;
					//找反射峰起始点
					for(j=min_data_number;j<max_data_reflex_mark_cn;j++)
					{
						data_buff_statick=65535-input_data[j];
						if(data_buff_statick>=data_avg)max_count_j++;
						else max_count_j=0;
					}
					sGabor_Result.Event_Array[i]=j-max_count_j-1;
					if((j-max_count_j-1)<0)sGabor_Result.Event_Array[i]=0;
					//sGabor_Result.Event_Array[i]=min_data_number+

				}

			}
			//delete[] input_data_buff_statick;
		}
	}

	//if(sGabor_Result.Valid_Num)
	//{
	//	j=1;
	//	sGabor_Result.Event_Array[0]=sGabor_Result.Event_Array[0];
	//	for(i=1;i<sGabor_Result.Valid_Num;i++)
	//	{
	//		
	//		if(sGabor_Result.Reflect_Type[i])
	//		{
	//			sGabor_Result.Event_Array[j]=sGabor_Result.Event_Array[i];
	//			sGabor_Result.Event_Loss[j]=sGabor_Result.Event_Loss[i];
	//			sGabor_Result.Reflect_Type[j]=sGabor_Result.Reflect_Type[i];
	//			sGabor_Result.Event_Hight[j]=sGabor_Result.Event_Hight[i];
	//			sGabor_Result.Total_Loss[j]=sGabor_Result.Total_Loss[i];
	//			sGabor_Result.Line_k[j]=sGabor_Result.Line_k[i];
	//			j++;
	//		}
	//		else
	//		{
	//			if(sGabor_Result.Event_Array[i]-sGabor_Result.Event_Array[i-1]>step_event/2)
	//			{
	//				sGabor_Result.Event_Array[j]=sGabor_Result.Event_Array[i];
	//				sGabor_Result.Event_Loss[j]=sGabor_Result.Event_Loss[i];
	//				sGabor_Result.Reflect_Type[j]=sGabor_Result.Reflect_Type[i];
	//				sGabor_Result.Event_Hight[j]=sGabor_Result.Event_Hight[i];
	//				sGabor_Result.Total_Loss[j]=sGabor_Result.Total_Loss[i];
	//				sGabor_Result.Line_k[j]=sGabor_Result.Line_k[i];
	//				j++;
	//			}
	//		}
	//	}
	//}

	//sGabor_Result.Valid_Num=j;

	int up_limit=0;
	int low_limit=0;
	double line_a=0;
	double line_b=0;
	double line_c=0;
	double line_d=0;


	if(step_event<20)step_event=20;
	float* sum_k=new float[2000];//短时傅里叶变化结果Cn
	memset(sum_k,0,2000*sizeof(float));
	//double sum_k[20];
	double dB_k_k_1=0;
	int Event_start_mark_cn=0;
	int Start_event_mark_cn=0;
	j=sGabor_Result.Valid_Num;
	//	if(j<100)
	{
		////////////////寻找开始点，去除第一个反射峰
		float avg_descore=0;
		float data_statick=0;
		int less_time=0;
		int max_mark_cn=0;
		int down_flag_time=0;
		int up_flag_time=0;
		int two_up_mark_cn=0;
		int min_data=0;
		int min_data_no=0;

		int first_pulse=0;

		int num_count=3*step_event+3;
		//if(num_count>sGabor_Result.Event_Array[1])num_count=4*step_event+3;
		//if(num_count>sGabor_Result.Event_Array[1])num_count=3*step_event+3;
		if(num_count>sGabor_Result.Event_Array[1])num_count=2*step_event+3;
		if(num_count>sGabor_Result.Event_Array[1])num_count=step_event+3;
		if(num_count>sGabor_Result.Event_Array[1])num_count=step_event/2+3;
		if(num_count>sGabor_Result.Event_Array[1])num_count=step_event/4+3;
		//if(num_count>sGabor_Result.Event_Array[1])num_count=step_event+3;




		float max_reflex=0;

		int Start_event_time=0;

		for(i=20;i<num_count;i++)
		{

			input_data_buff[i]=input_data[i+3]-input_data[i];//(input_data[i+7]+input_data[i+6]+input_data[i+5]-(input_data[i+2]+input_data[i+1]+input_data[i]))/3;

			if(max_reflex<=(65535-input_data[i]))//查找最高峰
			{
				if(first_pulse==0)
				{
					max_reflex=65535-input_data[i];
					max_mark_cn=i;
					down_flag_time=0;
				}
				else
				{
					up_flag_time++;
					if(up_flag_time>=5)
					{
						two_up_mark_cn=i-5;
						num_count=two_up_mark_cn;break;
					}
				}
			}
			else 
			{
				down_flag_time++;
				if(down_flag_time>=10)first_pulse=1;
				up_flag_time=0;
			}


			//if(down_flag_time>=5)break;
		}
		min_data=65535;
		for(i=max_mark_cn;i<num_count;i++)
		{
			if(min_data>=(65535-input_data[i]))//查找最高峰
			{
				min_data=65535-input_data[i];
				min_data_no=i;
				down_flag_time=0;
			}
			//down_flag_time++;
			//if(down_flag_time>=5)break;
		}

		avg_descore=0;

		for(i=max_mark_cn;i<num_count;i++)//寻找最高峰后的自然平滑衰减开始区
		{
			avg_descore=avg_descore+abs(input_data_buff[i]);
		}
		if(num_count!=max_mark_cn)
			avg_descore=avg_descore/(num_count-max_mark_cn);
		else avg_descore=abs(input_data_buff[i]);
		for(i=num_count;i!=max_mark_cn;i--)
		{
			data_statick=(input_data_buff[i]);
			/*if(data_statick<=10)less_time++;                  //2013.04.21
			else if(data_statick>=avg_descore)less_time++;*/
			if(data_statick>avg_descore)less_time++;
			else
			{
				less_time=0;
			}
			if(less_time>=5)
			{
				Event_start_mark_cn=i+10;//+step_event;
				Start_event_mark_cn=Event_start_mark_cn;
				break;
			}


		}

		for(i=num_count;i!=max_mark_cn;i--)
		{
			if(abs(num_count-max_mark_cn)<10)/////////////2013.04.16
			{
				Start_event_mark_cn=max_mark_cn+1;
				Event_start_mark_cn=Start_event_mark_cn;
				break;
			}
			data_statick=(input_data_buff[i]);
			if(data_statick>avg_descore)Start_event_time++;
			else Start_event_time=0;
			if(Start_event_time>10)
			{
				Start_event_mark_cn=i+Start_event_time;
				Event_start_mark_cn=Start_event_mark_cn;
				break;
			}
		}
		if(Event_start_mark_cn==0)
		{
			Event_start_mark_cn=max_mark_cn;//two_up_mark_cn;/////////////////修改为20130628
			Start_event_mark_cn=Event_start_mark_cn;
			//if(Event_start_mark_cn==0)

		}
		if(Event_start_mark_cn==0)//20130617
		{
			Event_start_mark_cn=step_event;/////////////////
			Start_event_mark_cn=Event_start_mark_cn;
			//if(Event_start_mark_cn==0)

		}
		if(pulsewidth<=200)
		{
			int point_p=0;
			if(sGabor_Result.Event_Array[0]-Event_start_mark_cn>2*step_event)
				point_p=3*step_event/2;
			else if(sGabor_Result.Event_Array[0]-Event_start_mark_cn>step_event)
				point_p=step_event;
			else point_p=step_event/2;//sGabor_Result.Event_Array[1]-Event_start_mark_cn-step_event/2;
			if(Event_start_mark_cn+point_p<sGabor_Result.Event_Array[0])
			{
				float kkk=0;
				float mmm=0;
				less_time=0;
				int large_time=0;
				for(i=Event_start_mark_cn;i<Event_start_mark_cn+point_p/2;i++)
				{
					input_data_buff[i]=input_data[i+point_p/2]-input_data[i];
					kkk=kkk+abs(input_data_buff[i]);
				}
				kkk=kkk/point_p/2;
				for(i=Event_start_mark_cn+point_p/2;i>Event_start_mark_cn;i--)
				{
					mmm=input_data_buff[i];
					if(mmm>kkk+2*avg_descore)
						large_time++;
					else 
					{
						less_time++;
						if(less_time>=2)
						{
							large_time=0;
							less_time=0;
						}
					}
					if(large_time>=8)
					{
						Event_start_mark_cn=i+large_time+less_time;
						Start_event_mark_cn=Event_start_mark_cn;
						break;
					}
				}
			}
		}
		/*for(i=num_count;i!=max_mark_cn;i--)
		{
		data_statick=(input_data_buff[i]);
		if(data_statick>avg_descore)Start_event_time++;
		else Start_event_time=0;
		if(Start_event_time>20)
		{
		Start_event_mark_cn=i+Start_event_time;
		Event_start_mark_cn=Start_event_mark_cn;
		break;
		}
		}*/

		k=1;
		for(i=1;i<sGabor_Result.Valid_Num;i++)
		{
			if(abs(sGabor_Result.Event_Array[i]-Event_start_mark_cn)>step_event/4||sGabor_Result.Reflect_Type[i]==1)
			{
				sGabor_Result.Event_Array[k]=sGabor_Result.Event_Array[i];
				sGabor_Result.Event_Loss[k]=sGabor_Result.Event_Loss[i];
				sGabor_Result.Reflect_Type[k]=sGabor_Result.Reflect_Type[i];
				sGabor_Result.Event_Hight[k]=sGabor_Result.Event_Hight[i];
				sGabor_Result.Total_Loss[k]=sGabor_Result.Total_Loss[i];
				sGabor_Result.Line_k[k]=sGabor_Result.Line_k[i];
				k++;
			}
		}
		sGabor_Result.Valid_Num=k;


		/////////////////////////RZG
		//先求各段光纤的斜率：
		//int for_num=0;
		//for(i=0;i<sGabor_Result.Valid_Num-1;i++)
		//{
		//	for_num=0;
		//	if(i==0)
		//	{
		//		up_limit=sGabor_Result.Event_Array[i+1]-2*step_event;//Event_start_mark_cn+6*step_event;
		//		if(up_limit<Event_start_mark_cn)up_limit=Event_start_mark_cn+step_event;
		//	}
		//	else
		//	{

		//		up_limit=sGabor_Result.Event_Array[i]+step_event;

		//	}
		//	low_limit=sGabor_Result.Event_Array[i+1]-step_event/4;
		//	if(low_limit<=up_limit+step_event/2)//相邻两个事件的距离太近，不适合算斜率，
		//	{
		//		sGabor_Result.Line_k[i+1]=0;
		//		continue;
		//	}
		//	else
		//	{
		//		sGabor_Result.Line_k[i+1]=0;
		//		for_num=low_limit-up_limit-step_event/2;
		//		if(for_num>=400)for_num=400;
		//		for(k=0;k<for_num;k++)
		//		{
		//			dB_k_k_1=input_data[up_limit+k];
		//			dB_k_k_1=(dB_k_k_1-input_data[low_limit-k]);
		//			//sum_k[k]=dB_k_k_1;
		//			if(low_limit-up_limit-2*k==0)break;
		//			sum_k[k]=dB_k_k_1/((low_limit-up_limit-2*k)*(Sample_time*Light_fiber_speed)/2);
		//			sGabor_Result.Line_k[i+1]=sGabor_Result.Line_k[i+1]+sum_k[k];
		//		}
		//		if(k!=0)
		//		sGabor_Result.Line_k[i+1]=-(sGabor_Result.Line_k[i+1]/k);
		//		else sGabor_Result.Line_k[i+1]=0;
		//		//斜率范围约束
		//		if(wavelength==1310)//1310
		//		{
		//			if(sGabor_Result.Line_k[i+1]<0.26||sGabor_Result.Line_k[i+1]>0.48)sGabor_Result.Line_k[i+1]=0;//0;//0.326;

		//		}
		//		else if(wavelength==1550)//1550
		//		{
		//			if(sGabor_Result.Line_k[i+1]<0.12||sGabor_Result.Line_k[i+1]>0.28)sGabor_Result.Line_k[i+1]=0;//0;//0.184;
		//		}
		//		else if(wavelength==1625)//1625
		//		{
		//			if(sGabor_Result.Line_k[i+1]<0.12||sGabor_Result.Line_k[i+1]>0.28)sGabor_Result.Line_k[i+1]=0;//0;//0.184;
		//		}

		//	}

		//}
		sGabor_Result=K_Line_count(L_event,wavelength,pulsewidth,input_data,sGabor_Result,Index_N,Sample_freaq);
		//}
	}

	//损耗计算,两点法计算
	double dB_Hight=0;
	double dB_Low=0;
	double dB_k_line=0;
	int max_data_no=0;
	int min_data_no=0;
	float max_data=0;
	float min_data=0;
	double Reflex_avg_dB=0;

	if(sGabor_Result.Valid_Num>1)
	{
		if(sGabor_Result.Valid_Num>100)sGabor_Result.Valid_Num=100;
		for(i=0;i<sGabor_Result.Valid_Num;i++)
		{
			if(sGabor_Result.Reflect_Type[i])//反射事件
			{
				//算反射高度
				if(i==0)
				{
					sGabor_Result.Reflect_Type[i]=1;
					continue;
				}
				up_limit=sGabor_Result.Event_Array[i]-step_event/4;
				if(up_limit<=0)up_limit=3;
				low_limit=sGabor_Result.Event_Array[i]+step_event/2;
				max_data=0;
				min_data=65535;
				for(k=up_limit;k<low_limit;k++)
				{
					if(max_data<65535-input_data[k])
					{
						max_data=65535-input_data[k];
						max_data_no=k;
					}
					if(min_data>65535-input_data[k])
					{
						min_data=65535-input_data[k];
						min_data_no=k;
					}
				}
				if(min_data_no<max_data_no)
				{
					//max_data=0;
					min_data=65535;
					for(k=up_limit;k<max_data_no;k++)
					{
						if(min_data>65535-input_data[k])
						{
							min_data=65535-input_data[k];
							min_data_no=k;
						}
					}
				}
				if(min_data_no>50)//对最小值做平均处理，消除噪声影响
				{
					Reflex_avg_dB=0;
					for(k=0;k<10;k++)
					{
						if(min_data_no<k)break;
						Reflex_avg_dB=Reflex_avg_dB+65535-input_data[min_data_no-k];
					}
					Reflex_avg_dB=Reflex_avg_dB/10;
					min_data=Reflex_avg_dB;
				}

				sGabor_Result.Event_Hight[i]=(max_data-min_data)/1000;
				//前一个事件也为反射事件，判断两个反射事件的距离是否满足1个波长的距离，如果满足就计算损耗，不满足就
				//不计算损耗
				if(sGabor_Result.Reflect_Type[i-1]||i==1)
				{
					if((sGabor_Result.Event_Array[i-1]+step_event)>sGabor_Result.Event_Array[i])
					{
						sGabor_Result.Event_Loss[i]=0;
						//continue;
					}
				}
				//算回波
			}
			//else//衰减事件  插入损耗计算
			{
				if(i==sGabor_Result.Valid_Num-1)
				{
					sGabor_Result.Event_Loss[i]=0;
					break;
				}
				if(sGabor_Result.Line_k[i]>0)dB_k_line=sGabor_Result.Line_k[i];
				else if(sGabor_Result.Line_k[i+1]>0)
				{
					dB_k_line=sGabor_Result.Line_k[i+1];
				}
				else
				{
					if(wavelength==1310)dB_k_line=Avg_loss_1310;//1310
					else if(wavelength==1550) dB_k_line=Avg_loss_1550;//1550
					else if(wavelength==1625) dB_k_line=Avg_loss_1625;//1550

				}
				if(i!=0)
				{

					int avg_up=0,avg_low=0;
					up_limit=sGabor_Result.Event_Array[i]-step_event/8;
					if(up_limit<=0)up_limit=3;
					avg_up=input_data[up_limit-3]+input_data[up_limit-2]+input_data[up_limit-1]+input_data[up_limit]+input_data[up_limit+1]+input_data[up_limit+2]+input_data[up_limit+3];
					avg_up=avg_up/7;
					low_limit=sGabor_Result.Event_Array[i]+step_event;
					avg_low=input_data[low_limit-3]+input_data[low_limit-2]+input_data[low_limit-1]+input_data[low_limit]+input_data[low_limit+1]+input_data[low_limit+2]+input_data[low_limit+3];
					avg_low=avg_low/7;
					dB_Hight=avg_up -avg_low ;
					dB_Hight=dB_Hight/1000+1.125*step_event*(Sample_time*Light_fiber_speed)/2000*dB_k_line;
					sGabor_Result.Event_Loss[i]=-(dB_Hight);
					if((sGabor_Result.Event_Loss[i])<=0.005)sGabor_Result.Event_Loss[i]=0;
					if(sGabor_Result.Reflect_Type[i])
					{
						if(sGabor_Result.Event_Loss[i]<=0.005)sGabor_Result.Event_Loss[i]=0;
					}
					;//算插入损耗	
				}
				else
				{
					sGabor_Result.Event_Loss[i]=0;
				}

			} 

		}
	}

	//利用损耗重新定位衰减事件
	if(return_flag==0)
	{
		int loss_max_cm=0;
		float max_loss=0;
		float loss_buff=0;
		int avg_up=0,avg_low=0;
		for(i=0;i<sGabor_Result.Valid_Num-1;i++)
		{

			loss_max_cm=sGabor_Result.Event_Array[i];
			if(sGabor_Result.Line_k[i]>0)dB_k_line=sGabor_Result.Line_k[i];
			else if(sGabor_Result.Line_k[i+1]>0)
			{
				dB_k_line=sGabor_Result.Line_k[i+1];
			}
			else
			{
				if(wavelength==1310)dB_k_line=Avg_loss_1310;//1310
				else if(wavelength==1550) dB_k_line=Avg_loss_1550;//1550
				else if(wavelength==1625) dB_k_line=Avg_loss_1625;//1550
			}

			if(sGabor_Result.Reflect_Type[i]==0&&pulsewidth<5000)
			{
				for(k=sGabor_Result.Event_Array[i]-L_event/4;k<sGabor_Result.Event_Array[i]+2*L_event;k=k+4)
				{

					up_limit=k-L_event/4;
					if(up_limit<=0)up_limit=3;
					avg_up=input_data[up_limit-3]+input_data[up_limit-2]+input_data[up_limit-1]+input_data[up_limit]+input_data[up_limit+1]+input_data[up_limit+2]+input_data[up_limit+3];
					avg_up=avg_up/7;
					if(pulsewidth>=5000)
						low_limit=k+L_event;
					else low_limit=k+L_event;
					avg_low=input_data[low_limit-3]+input_data[low_limit-2]+input_data[low_limit-1]+input_data[low_limit]+input_data[low_limit+1]+input_data[low_limit+2]+input_data[low_limit+3];
					avg_low=avg_low/7;
					dB_Hight=avg_up -avg_low ;
					dB_Hight=dB_Hight/1000+1.25*L_event*(Sample_time*Light_fiber_speed)/2000*dB_k_line;
					loss_buff=-(dB_Hight);
					if(loss_buff>max_loss)
					{
						max_loss=loss_buff;
						loss_max_cm=k;

					}

				}
			}
			if(loss_max_cm!=sGabor_Result.Event_Array[i])
			{
				sGabor_Result.Event_Array[i]=loss_max_cm;//-L_event/4;
				sGabor_Result.Event_Loss[i]=max_loss;
			}

		}
		//	sGabor_Result=K_Line_count(L_event,wavelength,pulsewidth,input_data,sGabor_Result,Index_N,Sample_freaq);
		//sGabor_Result=Acount_Loss(wavelength,pulsewidth,input_data,sGabor_Result,Index_N,Sample_freaq);
		//对数据进行重新取舍
		//事件排序

		for(i=0;i<sGabor_Result.Valid_Num;i++)
		{
			for(j=i+1;j<sGabor_Result.Valid_Num;j++)
			{
				if(sGabor_Result.Event_Array[i]>sGabor_Result.Event_Array[j])
				{
					data_buff=sGabor_Result.Event_Array[i];
					sGabor_Result.Event_Array[i]=sGabor_Result.Event_Array[j];
					sGabor_Result.Event_Array[j]=data_buff;

					data_buff=sGabor_Result.Reflect_Type[i];
					sGabor_Result.Reflect_Type[i]=sGabor_Result.Reflect_Type[j];
					sGabor_Result.Reflect_Type[j]=data_buff;

					Char_type=sGabor_Result.Event_type[i];
					sGabor_Result.Event_type[i]=sGabor_Result.Event_type[j];
					sGabor_Result.Event_type[j]=Char_type;

					float_type=sGabor_Result.Event_Loss[i];
					sGabor_Result.Event_Loss[i]=sGabor_Result.Event_Loss[j];
					sGabor_Result.Event_Loss[j]=float_type;

					float_type=sGabor_Result.Line_k[i];
					sGabor_Result.Line_k[i]=sGabor_Result.Line_k[j];
					sGabor_Result.Line_k[j]=float_type;

					float_type=sGabor_Result.Event_Hight[i];
					sGabor_Result.Event_Hight[i]=sGabor_Result.Event_Hight[j];
					sGabor_Result.Event_Hight[j]=float_type;


				}
			}
		}
		sGabor_Result=Acount_Loss(wavelength,pulsewidth,input_data,sGabor_Result,Index_N,Sample_freaq);

		j=1;
		for(i=1;i<sGabor_Result.Valid_Num;i++)
		{
			if(sGabor_Result.Reflect_Type[i]==1)
			{
				sGabor_Result.Event_Array[j]=sGabor_Result.Event_Array[i];
				sGabor_Result.Reflect_Type[j]=1;
				sGabor_Result.Event_Loss[j]=sGabor_Result.Event_Loss[i];
				sGabor_Result.Event_Hight[j]=sGabor_Result.Event_Hight[i];
				j++;
			}
			else
			{
				if(fabs(sGabor_Result.Event_Loss[i])>=dB_Loss)//损耗过滤判断
				{
					if(sGabor_Result.Reflect_Type[j-1]==1)/////
					{
						if(sGabor_Result.Event_Array[i]-step_event/2>=sGabor_Result.Event_Array[j-1])
						{
							sGabor_Result.Event_Array[j]=sGabor_Result.Event_Array[i];
							sGabor_Result.Reflect_Type[j]=0;
							sGabor_Result.Event_Loss[j]=sGabor_Result.Event_Loss[i];
							j++;
						}
					}
					else
					{
						if(sGabor_Result.Event_Array[i]-step_event/4>=sGabor_Result.Event_Array[j-1])
						{
							sGabor_Result.Event_Array[j]=sGabor_Result.Event_Array[i];
							sGabor_Result.Reflect_Type[j]=0;
							sGabor_Result.Event_Loss[j]=sGabor_Result.Event_Loss[i];
							j++;
						}
						else
						{
							//if(sGabor_Result.Event_Loss[i]>sGabor_Result.Event_Loss[j-1])
							{
								sGabor_Result.Event_Array[j-1]=sGabor_Result.Event_Array[i];
								sGabor_Result.Reflect_Type[j-1]=0;
								sGabor_Result.Event_Loss[j-1]=sGabor_Result.Event_Loss[i];
							}

						}
					}
				}
			}
		}
		sGabor_Result.Valid_Num=j;
		//非反射事件位置再一次校准
		int start_jz_mark=0;
		int end_jz_mark=0;
		//int* input_data_buff_statick=new int[2*step_event+2];
		//memset(input_data_buff_statick,0,(2*step_event+2)*sizeof(int));
		int avg_statick=0;
		int large_count=0;
		int loss_fault=0;
		for(i=1;i<sGabor_Result.Valid_Num-1;i++)
		{
			if(sGabor_Result.Reflect_Type[i]==0&&pulsewidth<5000)//非反射事件
			{
				start_jz_mark=sGabor_Result.Event_Array[i]-step_event/2;
				end_jz_mark=sGabor_Result.Event_Array[i]+step_event;
				//	if(start_jz_mark<sGabor_Result.Event_Array[i-1])continue;
				//	if(end_jz_mark>sGabor_Result.Event_Array[i+1])continue;
				if(start_jz_mark<=0)continue;
				for(j=start_jz_mark;j<end_jz_mark-4;j++)
				{
					input_data_buff[j-start_jz_mark]=input_data[j+4]-input_data[j];
					avg_statick=avg_statick+input_data_buff[j-start_jz_mark];

				}
				if(j==start_jz_mark)continue;
				avg_statick=avg_statick/(j-start_jz_mark);
				for(j=start_jz_mark;j<end_jz_mark-4;j++)
				{
					if(sGabor_Result.Event_Loss[i]<1)loss_fault=4*avg_statick/3;
					else loss_fault=avg_statick;
					if(input_data_buff[j-start_jz_mark]>loss_fault)
						large_count++;
					else large_count=0;
					if(large_count>=10)
					{
						sGabor_Result.Event_Array[i]=j-large_count;
						break;
					}

				}

			}
		}
		//delete[] input_data_buff;
		//delete[] input_data_buff_statick;
	}




	//int Last_reflex_event=0;
	//j=1;
	//for(i=1;i<sGabor_Result.Valid_Num-1;i++)
	//{
	//	if(sGabor_Result.Reflect_Type[i]==1)
	//	{
	//		Last_reflex_event=i;
	//		sGabor_Result.Reflect_Type[j]=1;
	//		sGabor_Result.Event_Array[j]=sGabor_Result.Event_Array[i];
	//		j++;
	//	}
	//	else
	//	{
	//		if((sGabor_Result.Event_Array[i]-sGabor_Result.Event_Array[Last_reflex_event])>step_event)
	//		{
	//			sGabor_Result.Reflect_Type[j]=0;
	//			sGabor_Result.Event_Array[j]=sGabor_Result.Event_Array[i];
	//			j++;
	//		}
	//	}
	//}

	//sGabor_Result.Reflect_Type[j]=sGabor_Result.Reflect_Type[sGabor_Result.Valid_Num-1];
	//sGabor_Result.Event_Array[j]=sGabor_Result.Event_Array[sGabor_Result.Valid_Num-1];
	//j++;
	//sGabor_Result.Valid_Num=j;
	//去除插入损耗小于0的衰减事件,去除回波损耗>=52的反射事件
	int nnn=sGabor_Result.Valid_Num;
	k=1;
	for(i=1;i<nnn-1;i++)
	{
		if(sGabor_Result.Reflect_Type[i]==0)// 衰减事件
		{
			//if(sGabor_Result.Event_Loss[i]>0)
			{
				sGabor_Result.Event_Array[k]=sGabor_Result.Event_Array[i];
				sGabor_Result.Event_Loss[k]=sGabor_Result.Event_Loss[i];
				sGabor_Result.Reflect_Type[k]=sGabor_Result.Reflect_Type[i];
				sGabor_Result.Event_Hight[k]=sGabor_Result.Event_Hight[i];
				sGabor_Result.Total_Loss[k]=sGabor_Result.Total_Loss[i];
				sGabor_Result.Line_k[k]=sGabor_Result.Line_k[i];
				k++;
			}
		}
		else //反射事件
		{
			//if(sGabor_Result.Event_Hight[i]>0.05)
			{
				sGabor_Result.Event_Array[k]=sGabor_Result.Event_Array[i];
				sGabor_Result.Event_Loss[k]=sGabor_Result.Event_Loss[i];
				sGabor_Result.Reflect_Type[k]=sGabor_Result.Reflect_Type[i];
				sGabor_Result.Event_Hight[k]=sGabor_Result.Event_Hight[i];
				sGabor_Result.Total_Loss[k]=sGabor_Result.Total_Loss[i];
				sGabor_Result.Line_k[k]=sGabor_Result.Line_k[i];
				k++;
			}
		}

	}
	//末端事件直接加载
	if(sGabor_Result.Event_Array[nnn-1]>sGabor_Result.Event_Array[0]+step_event/4)
	{

		sGabor_Result.Event_Array[k]=sGabor_Result.Event_Array[nnn-1];
		sGabor_Result.Event_Loss[k]=sGabor_Result.Event_Loss[nnn-1];
		sGabor_Result.Reflect_Type[k]=sGabor_Result.Reflect_Type[nnn-1];
		sGabor_Result.Event_Hight[k]=sGabor_Result.Event_Hight[nnn-1];
		sGabor_Result.Total_Loss[k]=sGabor_Result.Total_Loss[nnn-1];
		sGabor_Result.Line_k[k]=sGabor_Result.Line_k[nnn-1];
		k++;
	}

	sGabor_Result.Valid_Num=k;
	//光纤末端事件加入和事件起始端加入
	if(length_real>sGabor_Result.Event_Array[sGabor_Result.Valid_Num-1])
		if(length_real-sGabor_Result.Event_Array[sGabor_Result.Valid_Num-1]>step_event)
			sGabor_Result.Event_Array[sGabor_Result.Valid_Num++]=length_real;
	sGabor_Result.Event_Array[sGabor_Result.Valid_Num++]=Start_event_mark_cn;

	//事件排序
	//char Char_type=0;
	for(i=0;i<sGabor_Result.Valid_Num;i++)
	{
		for(j=i+1;j<sGabor_Result.Valid_Num;j++)
		{
			if(sGabor_Result.Event_Array[i]>sGabor_Result.Event_Array[j])
			{
				data_buff=sGabor_Result.Event_Array[i];
				sGabor_Result.Event_Array[i]=sGabor_Result.Event_Array[j];
				sGabor_Result.Event_Array[j]=data_buff;

				data_buff=sGabor_Result.Reflect_Type[i];
				sGabor_Result.Reflect_Type[i]=sGabor_Result.Reflect_Type[j];
				sGabor_Result.Reflect_Type[j]=data_buff;

				Char_type=sGabor_Result.Event_type[i];
				sGabor_Result.Event_type[i]=sGabor_Result.Event_type[j];
				sGabor_Result.Event_type[j]=Char_type;


			}
		}
	}



	j=0;
	for(i=0;i<sGabor_Result.Valid_Num;i++)
	{
		if(sGabor_Result.Event_Array[i]>=Start_event_mark_cn)
		{
			if(j)
			{
				if(sGabor_Result.Reflect_Type[i])
				{
					sGabor_Result.Event_Array[j]=sGabor_Result.Event_Array[i];
					sGabor_Result.Reflect_Type[j]=sGabor_Result.Reflect_Type[i];
					sGabor_Result.Event_type[j++]=sGabor_Result.Event_type[i];
				}
				else
				{
					if((sGabor_Result.Event_Array[i]-sGabor_Result.Event_Array[j-1])>L_event/2)
					{
						sGabor_Result.Event_Array[j]=sGabor_Result.Event_Array[i];
						sGabor_Result.Reflect_Type[j]=sGabor_Result.Reflect_Type[i];
						sGabor_Result.Event_type[j++]=sGabor_Result.Event_type[i];
					}
				}
			}
			else
			{

				sGabor_Result.Event_Array[j]=sGabor_Result.Event_Array[i];
				sGabor_Result.Reflect_Type[j]=sGabor_Result.Reflect_Type[i];
				sGabor_Result.Event_type[j++]=sGabor_Result.Event_type[i];
			}

		}
	}
	sGabor_Result.Valid_Num=j;
	//对紧随起始端的事件进行位置校准
	if(return_flag==0)
	{
		int L_end_point=0;
		max_data=0;
		if(sGabor_Result.Valid_Num>2)
		{
			if(sGabor_Result.Reflect_Type[1]==1)//其事件必须为反射事件
			{
				if((sGabor_Result.Event_Array[1]-sGabor_Result.Event_Array[0])<step_event)//与起始端的距离必须在一个波长之内
				{
					L_end_point=sGabor_Result.Event_Array[1]+step_event;
					if(L_end_point>sGabor_Result.Event_Array[2])L_end_point=sGabor_Result.Event_Array[2];
					for(i=sGabor_Result.Event_Array[1];i<L_end_point;i++)
					{
						data_buff=65535-input_data[i];//input_data[i+1]-input_data[i];
						if(max_data<data_buff)
						{
							max_data=data_buff;
							max_data_no=i;
						}
					}
					j=max_data_no;
					max_data=0;
					L_end_point=j-step_event/16;
					if((step_event/16)<6)L_end_point=j-6;
					if(L_end_point<=sGabor_Result.Event_Array[0])L_end_point=sGabor_Result.Event_Array[0];

					for(i=j;i>L_end_point;i--)//反修正
					{
						data_buff=input_data[i]-input_data[i+1];
						if(max_data<=data_buff)
						{
							max_data=data_buff;
							max_data_no=i;
						}
					}
					sGabor_Result.Event_Array[1]=max_data_no;
				}
			}
		}
	}

	//////////////////////20130617
	j=0;
	for(i=0;i<sGabor_Result.Valid_Num;i++)
	{
		if(sGabor_Result.Event_Array[i]>=Start_event_mark_cn)
		{
			if(j)
			{
				if(sGabor_Result.Reflect_Type[i])
				{
					if((sGabor_Result.Event_Array[i]-sGabor_Result.Event_Array[j-1])>L_event/4)//20130617
					{
						sGabor_Result.Event_Array[j]=sGabor_Result.Event_Array[i];
						sGabor_Result.Reflect_Type[j]=sGabor_Result.Reflect_Type[i];
						sGabor_Result.Event_type[j++]=sGabor_Result.Event_type[i];
					}

				}
				else
				{
					if((sGabor_Result.Event_Array[i]-sGabor_Result.Event_Array[j-1])>L_event/2)
					{
						sGabor_Result.Event_Array[j]=sGabor_Result.Event_Array[i];
						sGabor_Result.Reflect_Type[j]=sGabor_Result.Reflect_Type[i];
						sGabor_Result.Event_type[j++]=sGabor_Result.Event_type[i];
					}
				}
			}
			else
			{

				sGabor_Result.Event_Array[j]=sGabor_Result.Event_Array[i];
				sGabor_Result.Reflect_Type[j]=sGabor_Result.Reflect_Type[i];
				sGabor_Result.Event_type[j++]=sGabor_Result.Event_type[i];
			}

		}
	}
	sGabor_Result.Valid_Num=j;

	/////////////////////////对过滤后的事件进行计算
	sGabor_Result.Line_k[0]=0;

	sGabor_Result=K_Line_count(L_event,wavelength,pulsewidth,input_data,sGabor_Result,Index_N,Sample_freaq);
	sGabor_Result=Acount_Loss(wavelength,pulsewidth,input_data,sGabor_Result,Index_N,Sample_freaq);


	////////////////////////////////////////////
	int reflex_count=0;
	int loss_count=0;
	int max_reflex=0;
	int max_mark_cn=0;
	float avg_descore=0;
	int num_count=3*step_event+3;
	int less_time=0;
	int start_mark_cn=0;
	int less_count_time=0;
	less_count_time=step_event/4;
	if(less_count_time<=5)less_count_time=5;
	float start_loss_dB=0;
	float data_statick=0;
	//计算总损耗/////////////////////////////////////////////////////////////////////////////
	//if(sGabor_Result.Valid_Num!=0)
	//{


	//	if(wavelength==1310)
	//	{
	//		start_loss_dB=Avg_loss_1310*Event_start_mark_cn*Sample_time*Light_fiber_speed/2000;
	//	}
	//	else if (wavelength==1550)
	//	{
	//		start_loss_dB=Avg_loss_1550*Event_start_mark_cn*Sample_time*Light_fiber_speed/2000;
	//	}
	//	else if (wavelength==1625)
	//	{
	//		start_loss_dB=Avg_loss_1625*Event_start_mark_cn*Sample_time*Light_fiber_speed/2000;
	//	}
	//	for(i=1;i<sGabor_Result.Valid_Num;i++)
	//	{
	//		if(sGabor_Result.Event_Array[i]>=15)
	//		{
	//			sGabor_Result.Total_Loss[i]=(input_data[sGabor_Result.Event_Array[i]-15]+input_data[sGabor_Result.Event_Array[i]-14]+input_data[sGabor_Result.Event_Array[i]-13]+input_data[sGabor_Result.Event_Array[i]-12]+input_data[sGabor_Result.Event_Array[i]-11])/5
	//				-(input_data[Event_start_mark_cn]+input_data[Event_start_mark_cn+1]+input_data[Event_start_mark_cn+2]+input_data[Event_start_mark_cn+3]+input_data[Event_start_mark_cn+4])/5;
	//			sGabor_Result.Total_Loss[i]=(sGabor_Result.Total_Loss[i])/1000+start_loss_dB;
	//			if(sGabor_Result.Total_Loss[i]<=0.1)sGabor_Result.Total_Loss[i]=0;
	//			if(i>0)
	//			{

	//				for(j=0;j<i;j++)//如果后面的总损耗计算结果比前面的总损耗结果还要小，说明错误，记为0
	//				{
	//					if(sGabor_Result.Total_Loss[i]<sGabor_Result.Total_Loss[j])
	//						sGabor_Result.Total_Loss[i]=0;
	//				}
	//			}


	//		}
	//		else
	//		{
	//			sGabor_Result.Total_Loss[i]=0;
	//		}

	//	}
	//	
	//}

	if(sGabor_Result.Valid_Num!=0)
	{
		if(sGabor_Result.Event_Array[0]<step_event/5)
		{

			if(sGabor_Result.Event_Array[1]<step_event/2)
			{

				if((sGabor_Result.Event_Array[1]-sGabor_Result.Event_Array[0])<step_event/4)//step_event*2/3
				{

					for(j=0;j<sGabor_Result.Valid_Num;j++)
					{
						sGabor_Result.Event_Array[j]=sGabor_Result.Event_Array[j+1];
						sGabor_Result.Event_Loss[j]=sGabor_Result.Event_Loss[j+1];
						sGabor_Result.Total_Loss[j]=sGabor_Result.Total_Loss[j+1];
						sGabor_Result.Event_type[j]=sGabor_Result.Event_type[j+1];
						sGabor_Result.Event_Hight[j]=sGabor_Result.Event_Hight[j+1];
						sGabor_Result.Reflect_Type[j]=sGabor_Result.Reflect_Type[j+1];
					}
					sGabor_Result.Valid_Num=sGabor_Result.Valid_Num-1;
				}
			}
		}
	}


	if(sGabor_Result.Valid_Num!=0)
		sGabor_Result.Event_type[sGabor_Result.Valid_Num-1]='E';
	else sGabor_Result.Event_type[0]='E';
	sGabor_Result.Event_type[0]='S';

	//	sGabor_Result.Event_type[sGabor_Result.Valid_Num]='E';


	//delete[] Gabor_result;
	delete[] input_data_buff;
	delete[] sum_k;
	return sGabor_Result;//element_result[1][1];
}


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
Gabor_Result C20120801::gabor_arithmetic(int wavelength,int pulseWidth,WORD input_data[],INT paramLen,double thresholdUpSet,double thresholdDownSet,float dB_Loss,float dB_reflex,float dB_End,float Return_loss_Tybe,float Sample_freaq,float Index_N,int link_point_number)
{

	if(dB_Loss==0)dB_Loss=0.05;
	if(dB_reflex==0)dB_reflex=0.05;
	if(pulseWidth<=10)//对于10ns以内的反射事件做抬高门限处理。2013.04.21
	{
		if(dB_reflex<=1)dB_reflex=1;
	}
	int pulseNum=12;
	int i=0;
	int j=0;
	switch(pulseWidth)
	{
	case 3:
		pulseNum=0;
		break;
	case 5:
		pulseNum=1;
		break;
	case 10:
		pulseNum=2;
		break;
	case 20:
		pulseNum=3;
		break;
	case 50:
		pulseNum=4;
		break;
	case 100:
		pulseNum=5;
		break;
	case 200:
		pulseNum=6;
		break;
	case 500:
		pulseNum=7;	
		break;
	case 1000:
		pulseNum=8;
		break;
	case 2000:
		pulseNum=9;
		break;
	case 5000:
		pulseNum=10;
		break;
	case 10000:
		pulseNum=11;
		break;
	case 20000:
		pulseNum=12;
		break;
	default:pulseNum=12;break;
	}

	int lEvent=0;
	int imin,imax=0;
	int lEventTest=0;
	int lEventMax=0;
	int lEventMin=0;
	int lEventMean=0;
	int lEventPartMin=65535;
	int lEventPartMinSign=0;
	int k=0;
	double sumTotalLoss=0;
	lEvent=lEventGenerate(pulseNum,wavelength,Sample_freaq);


	Gabor_Result sGabor_Result;//


	sGabor_Result=gabor_generate(pulseWidth,wavelength,input_data,paramLen,thresholdUpSet,thresholdDownSet,dB_Loss,dB_reflex,dB_End,Sample_freaq,Index_N,link_point_number);

	//光在光纤中传播的速度
	double Light_fiber_speed=300/Index_N;//缩小了10^6倍
	//两个数据之间的时间间隔：
	double Sample_time=Sample_freaq;//放大了10^6倍
	Sample_time=1/Sample_time;

	double return_loss=0;

	//int pulse_statick=pulseWidth;
	//if(pulse_statick<1000)pulse_statick=1000;
	//else pulse_statick=pulseWidth;

	sumTotalLoss=0;
	//***************************************************************//
	//**********************加入门限值判断***************************//
	float float_data=0;
	int event_num=sGabor_Result.Valid_Num;
	int kk=0;
	int m=0,n=0,delete_flag=0;
	int delete_data_buff[100];

	sGabor_Result=K_Line_count(lEvent,wavelength,pulseWidth,input_data,sGabor_Result,Index_N,Sample_freaq);
	//sGabor_Result.Event_Array[0]=46;
	sGabor_Result=Acount_Loss(wavelength,pulseWidth,input_data,sGabor_Result,Index_N,Sample_freaq);
	//sGabor_Result=Acount_return_loss(wavelength,pulseWidth,input_data,sGabor_Result,Index_N,Sample_freaq,Return_loss_Tybe);
	sGabor_Result=Acount_Total_Loss(wavelength,pulseWidth,input_data,sGabor_Result,Index_N,Sample_freaq);
	sGabor_Result=K_Line_limite(wavelength,sGabor_Result);

	for(i=1;i<event_num-1;i++)//去除门限之内的事件
	{
		if(sGabor_Result.Reflect_Type[i]==1)//反射事件
		{
			//			sGabor_Result.Event_Hight[i]=Return_loss_Tybe+10*log10(pulseWidth/1000.0)+2*sGabor_Result.Event_Hight[i];
			float_data=sGabor_Result.Event_Hight[i];

			//float_data=sGabor_Result.Event_Hight[i];
			if(float_data<fabs(dB_reflex))
			{
				//sGabor_Result=deleteEvent(i-kk,wavelength,pulsewidthbox,data_from_file,sGabor_Result);
				if(sGabor_Result.Event_Loss[i]<dB_Loss)
				{

					delete_data_buff[kk]=i;
					kk++;

				}

			}
			//else if(fabs(sGabor_Result.Event_Loss[i])<fabs(dB_Loss))
			//{
			//	//sGabor_Result=deleteEvent(i-kk,wavelength,pulsewidthbox,data_from_file,sGabor_Result);
			//	delete_data_buff[kk]=i;
			//	kk++;
			//}
		}
		else
		{
			if(fabs(sGabor_Result.Event_Loss[i])<=fabs(dB_Loss)||sGabor_Result.Event_Loss[i]<=0)
			{
				//sGabor_Result=deleteEvent(i-kk,wavelength,pulsewidthbox,data_from_file,sGabor_Result);
				delete_data_buff[kk]=i;
				kk++;
			}
			/*else if(abs(sGabor_Result.Event_Array[i]-sGabor_Result.Event_Array[i-1])<lEvent)
			{
			for(m=0;m<i;m++)
			{
			if(n=0;n<kk;n++)
			if(m==delete_data_buff[kk])break;
			}
			delete_data_buff[kk]=i;
			kk++;
			}*/
			else if(i>1)
			{
				for(m=0;m<i-1;m++)
				{
					delete_flag=0;
					for(n=0;n<kk;n++)//
					{
						if(m==delete_data_buff[n])//如果已经列入删除表格，就不做计算，否则要做距离是否满足的判断20130617
						{
							delete_flag=1;
							break;
						}
					}
					if(delete_flag==0)//如果该事件的剧距离离前面保留事件非常近，做事件删除处理。20130617
					{
						if(abs(sGabor_Result.Event_Array[i]-sGabor_Result.Event_Array[m])<lEvent)
						{
							delete_data_buff[kk]=i;
							kk++;
						}
					}
				}

			}
		}
	}
	for(i=0;i<kk;i++)
	{
		sGabor_Result=deleteEvent(delete_data_buff[i]-i,wavelength,pulseWidth,input_data,sGabor_Result,Index_N,Sample_freaq,Return_loss_Tybe);
	}

	//删除拼接点位置事件
	if(link_point_number>1000)
	{

		int min_data_link=128000;
		int min_data_link_number=0;
		int min_data_buff=0;


		int for_number=sGabor_Result.Valid_Num;
		kk=0;
		for(i=0;i<for_number;i++)
		{
			min_data_buff=abs(sGabor_Result.Event_Array[i]-link_point_number);
			if(min_data_buff<2*lEvent)
			{
				if(sGabor_Result.Reflect_Type[i]==1)
				{
					if(min_data_buff<lEvent/2)
					{
						delete_data_buff[kk]=i;
						kk++;
					}
				}
				else
				{
					if(link_point_number<sGabor_Result.Event_Array[i])
					{
						if(min_data_buff<2*lEvent&&fabs(sGabor_Result.Event_Loss[i])<=0.02)
						{
							delete_data_buff[kk]=i;
							kk++;
						}
					}
					else
					{
						if(min_data_buff<lEvent)
						{
							delete_data_buff[kk]=i;
							kk++;
						}
					}
				}
			}
			//if(sGabor_Result.Reflect_Type[i])
		}

		for(i=0;i<kk;i++)
		{
			sGabor_Result=deleteEvent(delete_data_buff[i]-i,wavelength,pulseWidth,input_data,sGabor_Result,Index_N,Sample_freaq,Return_loss_Tybe);
		}


	}
	//杂波判断
	//
	if(sGabor_Result.Valid_Num>=3&&pulseWidth>=20)
	{

		int step_cc=lEvent;
		if(step_cc<=5)step_cc=5;
		int data_fork=0;
		int count_time=0;
		int fault_time=0;
		int I_for_time=sGabor_Result.Valid_Num-1;
		int max_fork=0;
		if(paramLen>2000)kk=500;
		else kk=200;
		int radio=0;


		//int* data_Loss_buff=new int[4*lEvent];
		//memset(data_Loss_buff,0,4*lEvent*sizeof(int));
		int data_Loss_buff;
		int mm=0,nn=0,jj=0;

		int up_time=0;
		int up_flag=0;


		for(i=paramLen-3;i>paramLen-503;i--)
		{
			radio=radio+(65535-input_data[i]);
		}
		radio=radio/500;
		if(radio<1000)radio=1000;
		//增加事件点前驱背景噪声的阈值加载判断
		float radio_event=0;
		int mmnn=0;

		for(i=I_for_time;i!=0;i--)
		{
			if(sGabor_Result.Reflect_Type[i])//反射事件，峰的宽度要达到半个步进
			{
				radio_event=0;
				mmnn=0;
				//增加事件点前驱背景噪声的阈值加载判断
				if(pulseWidth>=20)
				{
					if(sGabor_Result.Event_Array[i]-sGabor_Result.Event_Array[i-1]>20)
					{
						for(mmnn=sGabor_Result.Event_Array[i]-10;mmnn<sGabor_Result.Event_Array[i]-2;mmnn++)
						{
							radio_event=radio_event+abs((input_data[mmnn]-input_data[mmnn-1]));
						}
						radio_event=radio_event/8;
					}
				}

				data_fork=0;
				count_time=0;
				for(j=sGabor_Result.Event_Array[i]-5;j<(sGabor_Result.Event_Array[i]-1);j++)
				{

					if(65535-input_data[j]>0)
					{
						count_time++;
						data_fork=data_fork+(65535-input_data[j]);
					}
				}
				if(count_time!=0)data_fork=data_fork/count_time;
				else data_fork=65535-input_data[sGabor_Result.Event_Array[i]-1];
				count_time=0;
				if(data_fork<=radio)data_fork=radio+500;
				//if(data_fork>=2*radio+500)continue;
				up_flag=0;
				up_time=0;
				for(j=sGabor_Result.Event_Array[i]+2;j<(sGabor_Result.Event_Array[i]+2*step_cc);j++)
				{
					if(j>=paramLen-100)break;
					if(data_fork+2*radio_event<65535-input_data[j])
					{
						up_flag++;
						up_time=0;
						count_time++;
						if(count_time>=step_cc/8+1)continue;
					}
					else 
					{
						if(up_flag>=step_cc/8)
						{

							up_time++;
							if(up_time>=5)
								//count_time=0;
								break;
						}
					}

				}
				if(count_time<step_cc/8)
					sGabor_Result=deleteEvent(i,wavelength,pulseWidth,input_data,sGabor_Result,Index_N,Sample_freaq,Return_loss_Tybe);
				//else//20130509   利用脉冲宽度拍判断是否是假事件
				//{
				//	//if()
				//	count_time=0;
				//	for(j=sGabor_Result.Event_Array[i]+2;j<(sGabor_Result.Event_Array[i]+3*step_cc);j++)
				//	{
				//		if(65535-input_data[sGabor_Result.Event_Array[i]+2]>(65535-input_data[j]))
				//		{
				//			count_time++;
				//			if(count_time>=3)break;
				//		}
				//		else up_time++;
				//	}
				//	if(up_time<step_cc/2)
				//		sGabor_Result=deleteEvent(i,wavelength,pulseWidth,input_data,sGabor_Result,Index_N,Sample_freaq);

				//}
			}
			else//衰减事件
			{
				data_fork=0;
				count_time=0;
				if(sGabor_Result.Event_Array[i]<2*lEvent)//sGabor_Result.Event_Array[0]+
				{
					sGabor_Result=deleteEvent(i,wavelength,pulseWidth,input_data,sGabor_Result,Index_N,Sample_freaq,Return_loss_Tybe);
					continue;
				}
				for(j=sGabor_Result.Event_Array[i]-5;j<(sGabor_Result.Event_Array[i]-1);j++)
				{

					if(65535-input_data[j]>0)
					{
						count_time++;
						data_fork=data_fork+(65535-input_data[j]);
					}
				}
				if(count_time!=0)data_fork=data_fork/count_time;
				else data_fork=65535-input_data[sGabor_Result.Event_Array[i]-1];
				count_time=0;
				//if(data_fork<=radio)data_fork=radio+500;
				if(data_fork<=2*radio+500)
				{
					sGabor_Result=deleteEvent(i,wavelength,pulseWidth,input_data,sGabor_Result,Index_N,Sample_freaq,Return_loss_Tybe);
					continue;
				}
				for(j=sGabor_Result.Event_Array[i]+2;j<(sGabor_Result.Event_Array[i]+step_cc);j++)
				{
					if(data_fork>65535-input_data[j])
					{
						fault_time=0;
						count_time++;
						if(count_time>=step_cc/4+1)break;
					}
					else 
					{
						fault_time++;
						if(fault_time>=3)
						count_time=0;
						//break;
					}

				}
				if(count_time<step_cc/4)
				{
					sGabor_Result=deleteEvent(i,wavelength,pulseWidth,input_data,sGabor_Result,Index_N,Sample_freaq,Return_loss_Tybe);
					continue;
				}
				//如果衰减事件的前端曲线比较光滑，衰减事件有效，否则衰减事件无效
				//取半个步进，                                                    //20130421
				if(i>1)
				{
					if(sGabor_Result.Event_Array[i]-sGabor_Result.Event_Array[i-1]<3*lEvent)
					{
						sGabor_Result=deleteEvent(i,wavelength,pulseWidth,input_data,sGabor_Result,Index_N,Sample_freaq,Return_loss_Tybe);
						continue;
					}
					else
					{
						mm=sGabor_Result.Event_Array[i]-2*lEvent;
						nn=sGabor_Result.Event_Array[i]-lEvent;
						data_Loss_buff=0;
						for(jj=mm;jj<nn;jj++)
						{
							data_Loss_buff=data_Loss_buff+abs(input_data[jj]-input_data[jj+lEvent]);
						}
						data_Loss_buff=data_Loss_buff/lEvent;
						if(data_Loss_buff>300&&sGabor_Result.Event_Loss[i]<0.1)
						{
							sGabor_Result=deleteEvent(i,wavelength,pulseWidth,input_data,sGabor_Result,Index_N,Sample_freaq,Return_loss_Tybe);
							continue;
						}
						else if(data_Loss_buff>500&&sGabor_Result.Event_Loss[i]<0.4)
						{
							sGabor_Result=deleteEvent(i,wavelength,pulseWidth,input_data,sGabor_Result,Index_N,Sample_freaq,Return_loss_Tybe);
							continue;
						}

					}
				}



			}

		}
		//delete[] data_Loss_buff;
	}
	///////鬼影判断，以及消除鬼影
	if(sGabor_Result.Valid_Num>2)
	{
		//首先判断距离是否有成倍数关系，对成倍数的关系的点做标记
		//噪声幅度计算
		if(paramLen>2000)kk=500;
		else kk=200;
		int noise=0;
		int noise_s=0;
		int sample_point =0;
		int freaq_num=Sample_freaq/25;
		switch(freaq_num)
		{
		case 1:sample_point=2;break;
		case 2:sample_point=4;break;
		case 4:sample_point=4;break;
		case 8:sample_point=4;break;
		case 16:sample_point=4;break;
		default:sample_point=3;break;
		}

		for(i=0;i<kk;i++)
		{
			noise=noise+abs(input_data[paramLen-i-1]-input_data[paramLen-i-2]);
			noise_s=noise_s+(65535-input_data[paramLen-i-1]);
		}
		noise=noise/kk;//噪声区噪声高度
		noise_s=noise_s/kk;
		if(noise_s<=1000)noise_s=1500;
		else if(noise_s>3000)noise_s=3000;
		int flag_point[100];
		int flex_flag[100];
		int flex_flag_point=0;
		int data_fork=0;
		if(sGabor_Result.Valid_Num==3)
		{
			data_fork=65535-(input_data[sGabor_Result.Event_Array[2]-4]+input_data[sGabor_Result.Event_Array[2]-3]+input_data[sGabor_Result.Event_Array[2]-2]+input_data[sGabor_Result.Event_Array[2]-1])/4;
			if((2*noise+2*noise_s)>(data_fork))//事件点信号幅度接近噪声区
			{
				flag_point[0]=abs(sGabor_Result.Event_Array[2]-2*sGabor_Result.Event_Array[1]);
				if(flag_point[0]<=sample_point)//判断最后一个事件是否为鬼影，4个采样点之内认为为鬼影
				{
					sGabor_Result=deleteEvent(2,wavelength,pulseWidth,input_data,sGabor_Result,Index_N,Sample_freaq,Return_loss_Tybe);

				}
			}

		}
		else//
		{
			////
			//for(i=0;i<sGabor_Result.Valid_Num;i++)//求反射事件之间的段长
			//{
			//	if(sGabor_Result.Event_Hight[i]!=0)flex_flag[flex_flag_point++]=i;
			//	
			//}
			////if(sGabor_Result.Event_Hight[sGabor_Result.Valid_Num-1]!=0)flex_flag[flex_flag_point++]=i;
			//if(flex_flag_point>1)
			//{

			//	for(i=1;i<flex_flag_point;i++)
			//	{
			//		flag_point[i]=sGabor_Result.Event_Array[flex_flag[i]]-sGabor_Result.Event_Array[flex_flag[i-1]];
			//	}
			//	flag_point[0]=sGabor_Result.Event_Array[flex_flag[0]];
			//}
			//鬼影一般出现在最后一个事件，最后一个事件与第一个事件的关系和倒数第二个事件的关系
			//取出最后一个反射事件和倒数第二个反射事件以及第一个反射事件
			int k1=0,k2=0,k3=0;
			for(i=1;i<sGabor_Result.Valid_Num;i++)//找起始反射点
			{
				if(sGabor_Result.Event_Hight[i]!=0)
				{
					k1=i;
					break;
				}
			}
			k3=sGabor_Result.Valid_Num-1;
			for(i=sGabor_Result.Valid_Num-2;i>0;i--)//找最后两个反射事件
			{
				if(sGabor_Result.Event_Hight[i]!=0)
				{
					//if(k3==0)k3=i;
					//else 
					{
						k2=i;break;
					}
				}
			}
			if(k2==k1){k2=0;}
			if(k3!=k1)//最少有两个反射事件
			{
				if(k2==0)
				{
					data_fork=65535-(input_data[sGabor_Result.Event_Array[k3]-4]+input_data[sGabor_Result.Event_Array[k3]-3]+input_data[sGabor_Result.Event_Array[k3]-2]+input_data[sGabor_Result.Event_Array[k3]-1])/4;
					if((2*noise+noise_s)>(data_fork))//事件点信号幅度接近噪声区
					{
						flag_point[0]=abs(sGabor_Result.Event_Array[k3]-2*sGabor_Result.Event_Array[k1]);
						if(flag_point[0]<=sample_point)//判断最后一个事件是否为鬼影，4个采样点之内认为为鬼影
						{
							sGabor_Result=deleteEvent(k3,wavelength,pulseWidth,input_data,sGabor_Result,Index_N,Sample_freaq,Return_loss_Tybe);

						}
					}
				}
				else
				{
					flag_point[0]=sGabor_Result.Event_Array[k1];
					flag_point[1]=abs(sGabor_Result.Event_Array[k2]-sGabor_Result.Event_Array[k1]);
					flag_point[2]=abs(sGabor_Result.Event_Array[k3]-sGabor_Result.Event_Array[k2]);
					if(abs(flag_point[2]-flag_point[0])<=sample_point)//最后一个反射事件满足鬼影条件
					{
						data_fork=65535-(input_data[sGabor_Result.Event_Array[k3]-4]+input_data[sGabor_Result.Event_Array[k3]-3]+input_data[sGabor_Result.Event_Array[k3]-2]+input_data[sGabor_Result.Event_Array[k3]-1])/4;
						if((2*noise+noise_s)>(data_fork))
						{
							sGabor_Result=deleteEvent(k3,wavelength,pulseWidth,input_data,sGabor_Result,Index_N,Sample_freaq,Return_loss_Tybe);
							if(abs(flag_point[1]-flag_point[0])<=sample_point)
							{
								data_fork=65535-(input_data[sGabor_Result.Event_Array[k2]-4]+input_data[sGabor_Result.Event_Array[k2]-3]+input_data[sGabor_Result.Event_Array[k2]-2]+input_data[sGabor_Result.Event_Array[k2]-1])/4;
								if((2*noise+noise_s)>(data_fork))
									sGabor_Result=deleteEvent(k2,wavelength,pulseWidth,input_data,sGabor_Result,Index_N,Sample_freaq,Return_loss_Tybe);
							}

						}

					}
					else
					{
						flag_point[2]=abs(sGabor_Result.Event_Array[k3]-sGabor_Result.Event_Array[k1]);
						if(abs(flag_point[2]-flag_point[0])<=sample_point)//最后一个反射事件满足鬼影条件
						{

							data_fork=65535-(input_data[sGabor_Result.Event_Array[k3]-4]+input_data[sGabor_Result.Event_Array[k3]-3]+input_data[sGabor_Result.Event_Array[k3]-2]+input_data[sGabor_Result.Event_Array[k3]-1])/4;
							if((2*noise+noise_s)>(data_fork))
								sGabor_Result=deleteEvent(k3,wavelength,pulseWidth,input_data,sGabor_Result,Index_N,Sample_freaq,Return_loss_Tybe);
						}
						else
						{
							if(abs(flag_point[1]-flag_point[0])<=sample_point)
							{
								data_fork=65535-(input_data[sGabor_Result.Event_Array[k2]-4]+input_data[sGabor_Result.Event_Array[k2]-3]+input_data[sGabor_Result.Event_Array[k2]-2]+input_data[sGabor_Result.Event_Array[k2]-1])/4;
								if((2*noise+noise_s)>(data_fork))
									sGabor_Result=deleteEvent(k2,wavelength,pulseWidth,input_data,sGabor_Result,Index_N,Sample_freaq,Return_loss_Tybe);
							}
						}
					}
				}
			}
		}
		if(pulseWidth>=3&&pulseWidth<1000)
		{
			//计算事件的前段信号噪声，判断事件是否是在噪声中，如果是在噪声中，就做鬼影处理
			int ii=0,jj=0,kk=0;
			int avg_roise_event=0,avg_roise_h=0;
			int CC=0;
			CC=sGabor_Result.Valid_Num;
			for(j=2;j<CC;j++)
			{

				ii=sGabor_Result.Event_Array[sGabor_Result.Valid_Num-1];
				jj=sGabor_Result.Event_Array[sGabor_Result.Valid_Num-2];
				if((ii-jj)>=100)kk=50;
				else kk=20;
				if(kk<=(ii-jj))
				{
					for(i=0;i<kk;i++)
					{
						avg_roise_event=avg_roise_event+(65535-input_data[ii-i]);//信号高度
						avg_roise_h=avg_roise_h+abs(input_data[ii-i]-input_data[ii-i-2]);
					}
					avg_roise_event=avg_roise_event/kk;
					avg_roise_h=avg_roise_h/kk;
					if(avg_roise_h<500)avg_roise_h=500;
					if(avg_roise_event<(noise_s+2*avg_roise_h))//噪声区中的事件
					{
						//sGabor_Result.Event_Array[sGabor_Result.Valid_Num-1]=0;
						//sGabor_Result=deleteEvent(sGabor_Result.Valid_Num-1,wavelength,pulseWidth,input_data,sGabor_Result,Index_N,Sample_freaq);
						/*for(i=0;i<sGabor_Result.Valid_Num-1;i++)
						{
						if(sGabor_Result.Event_Array[j])
						}*/
						if(abs(ii-2*jj)<5*sample_point)sGabor_Result=deleteEvent(sGabor_Result.Valid_Num-1,wavelength,pulseWidth,input_data,sGabor_Result,Index_N,Sample_freaq,Return_loss_Tybe);

					}
				}
			}

		}

	}
	sGabor_Result=Acount_Loss(wavelength,pulseWidth,input_data,sGabor_Result,Index_N,Sample_freaq);

	//反射峰下降沿上的事件判断


	//对末端判断是否是由于大的反射峰的下降而造成的误判
	{


		float loss_dB_statick=0;
		float noise_buff=0;
		float Event_buff=0;
		int m=0;
		int Length_step=0;
		if(pulseWidth<=200)Length_step=14*lEvent;
		else Length_step=10*lEvent;
		if(sGabor_Result.Valid_Num>2)
		{
			if(sGabor_Result.Event_Array[sGabor_Result.Valid_Num-1]>3*lEvent)
			{
				if(sGabor_Result.Event_Array[sGabor_Result.Valid_Num-1]-sGabor_Result.Event_Array[sGabor_Result.Valid_Num-2]>2*lEvent)
				{
					if(sGabor_Result.Event_Array[sGabor_Result.Valid_Num-1]-sGabor_Result.Event_Array[sGabor_Result.Valid_Num-2]<5*lEvent)
					{
						//loss_dB_statick=(input_data[sGabor_Result.Event_Array[sGabor_Result.Valid_Num-1]]-input_data[sGabor_Result.Event_Array[sGabor_Result.Valid_Num-1]-2*lEvent])/1000;
						loss_dB_statick=(input_data[sGabor_Result.Event_Array[sGabor_Result.Valid_Num-1]-2*lEvent]+
							input_data[sGabor_Result.Event_Array[sGabor_Result.Valid_Num-1]-2*lEvent+1]+input_data[sGabor_Result.Event_Array[sGabor_Result.Valid_Num-1]-2*lEvent+2]+
							input_data[sGabor_Result.Event_Array[sGabor_Result.Valid_Num-1]-2*lEvent-1]+input_data[sGabor_Result.Event_Array[sGabor_Result.Valid_Num-1]-2*lEvent]-2)/5;
						loss_dB_statick=(input_data[sGabor_Result.Event_Array[sGabor_Result.Valid_Num-1]]-loss_dB_statick)/1000;
						if(loss_dB_statick>2)
						{
							//如果后面的反射峰比前面的反射峰高度要大很多，就保留后面的反射峰
							if(sGabor_Result.Event_Hight[sGabor_Result.Valid_Num-1]<sGabor_Result.Event_Hight[sGabor_Result.Valid_Num-2]+2)
								sGabor_Result=deleteEvent(sGabor_Result.Valid_Num-1,wavelength,pulseWidth,input_data,sGabor_Result,Index_N,Sample_freaq,Return_loss_Tybe);
						}
						else
						{
							noise_buff=0;
							for(m=sGabor_Result.Event_Array[sGabor_Result.Valid_Num-1]-1;m>sGabor_Result.Event_Array[sGabor_Result.Valid_Num-1]-lEvent/4;m--)
							{
								noise_buff=noise_buff+abs(input_data[m]-input_data[m-1]);
							}
							noise_buff=noise_buff/(lEvent/4-1);
							//	if(noise_buff>100)sGabor_Result=deleteEvent(sGabor_Result.Valid_Num-1,wavelength,pulseWidth,input_data,sGabor_Result,Index_N,Sample_freaq,Return_loss_Tybe);
						}
						if(sGabor_Result.Valid_Num)
							sGabor_Result.Event_type[sGabor_Result.Valid_Num-1]='E';
					}
					else if(sGabor_Result.Event_Array[sGabor_Result.Valid_Num-1]-sGabor_Result.Event_Array[sGabor_Result.Valid_Num-2]<Length_step)
					{
						if(sGabor_Result.Reflect_Type[sGabor_Result.Valid_Num-1]==0)
						{
							for(m=paramLen-1;m>paramLen-501;m--)
							{
								noise_buff=noise_buff+(65535-input_data[m]);
							}
							noise_buff=noise_buff/500;
							Event_buff=(input_data[sGabor_Result.Event_Array[sGabor_Result.Valid_Num-1]]+input_data[sGabor_Result.Event_Array[sGabor_Result.Valid_Num-1]-1]+input_data[sGabor_Result.Event_Array[sGabor_Result.Valid_Num-1]+1])/3;
							Event_buff=65535-Event_buff;

							if(Event_buff<3*noise_buff)
								sGabor_Result=deleteEvent(sGabor_Result.Valid_Num-1,wavelength,pulseWidth,input_data,sGabor_Result,Index_N,Sample_freaq,Return_loss_Tybe);
							else
							{
								noise_buff=0;
								for(m=sGabor_Result.Event_Array[sGabor_Result.Valid_Num-1]-1;m>sGabor_Result.Event_Array[sGabor_Result.Valid_Num-1]-lEvent/4;m--)
								{
									noise_buff=noise_buff+abs(input_data[m]-input_data[m-1]);
								}
								noise_buff=noise_buff/(lEvent/4-1);
								if(noise_buff>100)sGabor_Result=deleteEvent(sGabor_Result.Valid_Num-1,wavelength,pulseWidth,input_data,sGabor_Result,Index_N,Sample_freaq,Return_loss_Tybe);
							}
						}
						else //反射事件，求反射事件前的噪声，如果反射事件的高度，低于反射事件前的噪声的两倍，就认为是噪声干扰
						{
							noise_buff=0;
							for(i=0;i<5;i++)
								noise_buff=noise_buff+abs(input_data[sGabor_Result.Event_Array[sGabor_Result.Valid_Num-1]-1-i]-input_data[sGabor_Result.Event_Array[sGabor_Result.Valid_Num-1]-3-i]);
							noise_buff=noise_buff/5000;
							if(sGabor_Result.Event_Hight[sGabor_Result.Valid_Num-1]<2*noise_buff)
								sGabor_Result=deleteEvent(sGabor_Result.Valid_Num-1,wavelength,pulseWidth,input_data,sGabor_Result,Index_N,Sample_freaq,Return_loss_Tybe);

						}
					}
				}
				else//衰减事件距离过短
				{
					if(sGabor_Result.Reflect_Type[sGabor_Result.Valid_Num-1]==0)
						sGabor_Result=deleteEvent(sGabor_Result.Valid_Num-1,wavelength,pulseWidth,input_data,sGabor_Result,Index_N,Sample_freaq,Return_loss_Tybe);
					else if(pulseWidth>=100)
					{
						if(sGabor_Result.Event_Array[sGabor_Result.Valid_Num-1]-sGabor_Result.Event_Array[sGabor_Result.Valid_Num-2]<=lEvent)
							sGabor_Result=deleteEvent(sGabor_Result.Valid_Num-1,wavelength,pulseWidth,input_data,sGabor_Result,Index_N,Sample_freaq,Return_loss_Tybe);
					}
				}
			}
			if(pulseWidth>=1000)//消除反射峰后的误判断的衰减事件
			{
				j=0;
				int KK=sGabor_Result.Valid_Num;
				for(i=1;i<KK;i++)
				{
					if(sGabor_Result.Reflect_Type[i]==0)
					{
						if(sGabor_Result.Reflect_Type[i-1])
						{
							if(i==1)
							{
								if(sGabor_Result.Event_Array[i]<2*lEvent)
								{

									delete_data_buff[j]=i;
									j++;
								}
							}
							else if(sGabor_Result.Event_Array[i]-sGabor_Result.Event_Array[i-1]<2*lEvent)
							{

								delete_data_buff[j]=i;
								j++;
							}
						}
					}
				}
				for(i=0;i<j;i++)
				{
					sGabor_Result=deleteEvent(delete_data_buff[i]-i,wavelength,pulseWidth,input_data,sGabor_Result,Index_N,Sample_freaq,Return_loss_Tybe);
				}
			}
		}
	}
	if(sGabor_Result.Valid_Num>=3)
		if(sGabor_Result.Reflect_Type[sGabor_Result.Valid_Num-1]!=0&&sGabor_Result.Reflect_Type[sGabor_Result.Valid_Num-2]!=0)
		{
			if(sGabor_Result.Event_Array[sGabor_Result.Valid_Num-1]-sGabor_Result.Event_Array[sGabor_Result.Valid_Num-2]<lEvent/4)
				sGabor_Result=deleteEvent(sGabor_Result.Valid_Num-1,wavelength,pulseWidth,input_data,sGabor_Result,Index_N,Sample_freaq,Return_loss_Tybe);	
		}
		/////////////////////////////////////////////////////////////
		//对最后一个事件进行噪声分析，看其是否处于噪声中，如处于噪声中，需要做鬼影判断。

		/////////////////////////////////////////////////////////////////////


		//////////////////////////////事件过滤
		float_data=0;
		event_num=sGabor_Result.Valid_Num;
		kk=0;
		m=0;n=0;delete_flag=0;

		for(i=1;i<event_num-1;i++)//去除门限之内的事件
		{
			if(sGabor_Result.Reflect_Type[i]==1)//反射事件
			{
				//			sGabor_Result.Event_Hight[i]=Return_loss_Tybe+10*log10(pulseWidth/1000.0)+2*sGabor_Result.Event_Hight[i];
				float_data=sGabor_Result.Event_Hight[i];

				//float_data=sGabor_Result.Event_Hight[i];
				if(float_data<fabs(dB_reflex))
				{
					//sGabor_Result=deleteEvent(i-kk,wavelength,pulsewidthbox,data_from_file,sGabor_Result);
					if(sGabor_Result.Event_Loss[i]<dB_Loss)
					{

						delete_data_buff[kk]=i;
						kk++;

					}

				}

			}
			else
			{
				if(fabs(sGabor_Result.Event_Loss[i])<=fabs(dB_Loss)||sGabor_Result.Event_Loss[i]<=0)
				{
					//sGabor_Result=deleteEvent(i-kk,wavelength,pulsewidthbox,data_from_file,sGabor_Result);
					delete_data_buff[kk]=i;
					kk++;
				}

				else if(i>1)
				{
					for(m=0;m<i-1;m++)
					{
						delete_flag=0;
						for(n=0;n<kk;n++)//
						{
							if(m==delete_data_buff[n])//如果已经列入删除表格，就不做计算，否则要做距离是否满足的判断20130617
							{
								delete_flag=1;
								break;
							}
						}
						if(delete_flag==0)//如果该事件的剧距离离前面保留事件非常近，做事件删除处理。20130617
						{
							if(abs(sGabor_Result.Event_Array[i]-sGabor_Result.Event_Array[m])<lEvent)
							{
								delete_data_buff[kk]=i;
								kk++;
							}
						}
					}

				}
			}
		}
		for(i=0;i<kk;i++)
		{
			sGabor_Result=deleteEvent(delete_data_buff[i]-i,wavelength,pulseWidth,input_data,sGabor_Result,Index_N,Sample_freaq,Return_loss_Tybe);
		}

		sGabor_Result=K_Line_count(lEvent,wavelength,pulseWidth,input_data,sGabor_Result,Index_N,Sample_freaq);
		//float return_tybe=0;
		//for(i=0;i<sGabor_Result.Valid_Num;i++)
		//{
		//	if(i>0)
		//	{
		//		//sumTotalLoss=sumTotalLoss+sGabor_Result.Line_k[i]/1000*(sGabor_Result.Event_Array[i]-sGabor_Result.Event_Array[i-1])+sGabor_Result.Event_Loss[i];
		//		//sGabor_Result.Total_Loss[i]=sumTotalLoss;
		//		if(sGabor_Result.Reflect_Type[i])//如果为反射事件，求回波损耗
		//		{

		//			if(wavelength==1310)return_tybe=Return_loss_1310_tybe;
		//			else if(wavelength==1550)return_tybe=Return_loss_1550_tybe;
		//			else if(wavelength==1625)return_tybe=Return_loss_1625_tybe;

		//			return_loss=pow(10,sGabor_Result.Event_Hight[i]/5)-1;
		//			sGabor_Result.Event_Hight[i]=(10*log10(pulseWidth/1000000000.0));
		//			//if(wavelength==1550)
		//			//sGabor_Result.Event_Hight[i]=sGabor_Result.Event_Hight[i]+10*log10(return_loss)+(-70-Return_loss_Tybe);//-(sGabor_Result.Event_Hight[i]+10*log10(return_loss))+(Return_loss_Tybe)+70;//
		//			//else sGabor_Result.Event_Hight[i]=sGabor_Result.Event_Hight[i]+10*log10(return_loss)+10.5;

		//			sGabor_Result.Event_Hight[i]=-(sGabor_Result.Event_Hight[i]+10*log10(return_loss))-(return_tybe+Return_loss_Tybe);
		//			if(fabs(sGabor_Result.Event_Hight[i])<=12.0)sGabor_Result.Event_Hight[i]=12;
		//			else if(fabs(sGabor_Result.Event_Hight[i])>=70.0)sGabor_Result.Event_Hight[i]=70;
		//		}

		//		else sGabor_Result.Event_Hight[i]=0;
		//		//sGabor_Result.Event_Hight[i]=2*sGabor_Result.Event_Hight[i];
		//	}
		//}
		if(sGabor_Result.Valid_Num)
			sGabor_Result.Event_type[sGabor_Result.Valid_Num-1]='E';

		sGabor_Result=K_Line_count(lEvent,wavelength,pulseWidth,input_data,sGabor_Result,Index_N,Sample_freaq);
		//sGabor_Result.Event_Array[0]=46;
		sGabor_Result=Acount_Loss(wavelength,pulseWidth,input_data,sGabor_Result,Index_N,Sample_freaq);
		sGabor_Result=Acount_return_loss(wavelength,pulseWidth,input_data,sGabor_Result,Index_N,Sample_freaq,Return_loss_Tybe);
		sGabor_Result=Acount_Total_Loss(wavelength,pulseWidth,input_data,sGabor_Result,Index_N,Sample_freaq);
		sGabor_Result=K_Line_limite(wavelength,sGabor_Result);
		//末端衰减事件的判断
		if(sGabor_Result.Valid_Num>2)
		{
			int last_reflex_num=0;
			//寻找最后一个反射事件
			for(i=sGabor_Result.Valid_Num-1;i>0;i--)
			{
				if(sGabor_Result.Reflect_Type[i]!=0)break;
			}
			last_reflex_num=i;
			float Loss_buff_avg=0;
			float Avg_lEvent_loss=0;
			for(i=0;;i++)
			{
				if(sGabor_Result.Valid_Num>2)
				{
					if(last_reflex_num==sGabor_Result.Valid_Num-1)break;
					if(sGabor_Result.Reflect_Type[sGabor_Result.Valid_Num-1]==0)
					{
						if((sGabor_Result.Event_Array[sGabor_Result.Valid_Num-1]<sGabor_Result.Event_Array[last_reflex_num]+10*lEvent))
						{
							Loss_buff_avg=0;
							for(j=0;j<5;j++)
							{
								Loss_buff_avg=Loss_buff_avg+(input_data[sGabor_Result.Event_Array[sGabor_Result.Valid_Num-1]-j]-input_data[sGabor_Result.Event_Array[sGabor_Result.Valid_Num-1]-lEvent-j]);
							}
							Loss_buff_avg=Loss_buff_avg/5000;
							if(wavelength==1310)Avg_lEvent_loss=Avg_loss_1310*lEvent*(Sample_time*Light_fiber_speed)/2000;
							else if (wavelength==1550)Avg_lEvent_loss=Avg_loss_1550*lEvent*(Sample_time*Light_fiber_speed)/2000;
							else if(wavelength==1625)Avg_lEvent_loss=Avg_loss_1625*lEvent*(Sample_time*Light_fiber_speed)/2000;
							else Avg_lEvent_loss=Avg_loss_1550*lEvent*(Sample_time*Light_fiber_speed)/2000;
							if(Loss_buff_avg>=2*Avg_lEvent_loss)
								sGabor_Result=deleteEvent(sGabor_Result.Valid_Num-1,wavelength,pulseWidth,input_data,sGabor_Result,Index_N,Sample_freaq,Return_loss_Tybe);
							else break;
						}
						else break;
					}
					else break;
				}
				else break;
			}
			
			/*for(i=0;;i++)
			{
				if(sGabor_Result.Valid_Num>2)
				{
					if(sGabor_Result.Reflect_Type[sGabor_Result.Valid_Num-1]==0)
					{
						if(sGabor_Result.Event_Loss[sGabor_Result.Valid_Num-1]<0.5)
							sGabor_Result=deleteEvent(sGabor_Result.Valid_Num-1,wavelength,pulseWidth,input_data,sGabor_Result,Index_N,Sample_freaq,Return_loss_Tybe);
						else break;
					}
					else break;
				}
				else break;
			}*/
			
			
		}
		
		sGabor_Result=K_Line_count(lEvent,wavelength,pulseWidth,input_data,sGabor_Result,Index_N,Sample_freaq);
		//sGabor_Result.Event_Array[0]=46;
		float Return_loss_Tybe_status=0;
		if(wavelength==1310)Return_loss_Tybe_status=Return_loss_1310_tybe;
		else if(wavelength==1550)Return_loss_Tybe_status=Return_loss_1550_tybe;
		else if(wavelength==1625)Return_loss_Tybe_status=Return_loss_1625_tybe;
		else Return_loss_Tybe_status=Return_loss_1550_tybe;
		sGabor_Result=Acount_Loss(wavelength,pulseWidth,input_data,sGabor_Result,Index_N,Sample_freaq);
		//sGabor_Result=Acount_return_loss(wavelength,pulseWidth,input_data,sGabor_Result,Index_N,Sample_freaq,Return_loss_Tybe_status);
		//sGabor_Result=Acount_Total_Loss(wavelength,pulseWidth,input_data,sGabor_Result,Index_N,Sample_freaq);
		//sGabor_Result=K_Line_limite(wavelength,sGabor_Result);
		if(pulseWidth<=100)
		{
			float dB_mxz=0;
			if(pulseWidth==3)dB_mxz=3;
			else if(pulseWidth==5)dB_mxz=2;
			else if(pulseWidth==10)dB_mxz=1.5;
			else if(pulseWidth==20)dB_mxz=1;
			else if(pulseWidth==50)dB_mxz=0.4;
			else if(pulseWidth==100)dB_mxz=0.2;
			for(i=1;i<sGabor_Result.Valid_Num-1;i++)
			{
				if(sGabor_Result.Reflect_Type[i]!=0)
				{
					if(sGabor_Result.Event_Hight[i]<=dB_mxz)
					{
						delete_data_buff[kk]=i;
						kk++;
					}
				}
			}
			for(i=0;i<kk;i++)
			{
				sGabor_Result=deleteEvent(delete_data_buff[i]-i,wavelength,pulseWidth,input_data,sGabor_Result,Index_N,Sample_freaq,Return_loss_Tybe);
			}
		}
		sGabor_Result=K_Line_count(lEvent,wavelength,pulseWidth,input_data,sGabor_Result,Index_N,Sample_freaq);
		sGabor_Result=Acount_Loss(wavelength,pulseWidth,input_data,sGabor_Result,Index_N,Sample_freaq);
		sGabor_Result=Acount_return_loss(wavelength,pulseWidth,input_data,sGabor_Result,Index_N,Sample_freaq,Return_loss_Tybe);
		sGabor_Result=Acount_Total_Loss(wavelength,pulseWidth,input_data,sGabor_Result,Index_N,Sample_freaq);
		sGabor_Result=K_Line_limite(wavelength,sGabor_Result);
		return sGabor_Result;
}


//******************************************************************//
//     添加事件
//     insertNum:所要插入的事件点的数据点编号
//     wavelength:曲线波长
//     pulsewidth：曲线脉宽
//     input_data[]：分析的原始数据
//     Return_loss_Tybe:典型散射系数
//     paramLen:原始数据长度
//******************************************************************//
Gabor_Result C20120801::insertEvent(int insertNum,int wavelength,int pulsewidth,WORD input_data[],int paramLen,Gabor_Result sGabor_Result,float Index_N,float Sample_freaq,float Return_loss_Tybe)
/*insertPosition插入物理位置
sGaborResult事件分析结果
insertNum插入序号
*/
{

	int i=0;
	int k=0;
	int insertIndex=0;
	i=sGabor_Result.Valid_Num;
	if(insertNum>=paramLen)return sGabor_Result;

	int pulseNum=11;
	int L_event=0;
	switch(pulsewidth)
	{
	case 3:
		pulseNum=0;
		break;
	case 5:
		pulseNum=1;
		break;
	case 10:
		pulseNum=2;
		break;
	case 20:
		pulseNum=3;
		break;
	case 50:
		pulseNum=4;
		break;
	case 100:
		pulseNum=5;
		break;
	case 200:
		pulseNum=6;
		break;
	case 500:
		pulseNum=7;	
		break;
	case 1000:
		pulseNum=8;
		break;
	case 2000:
		pulseNum=9;
		break;
	case 5000:
		pulseNum=10;
		break;
	case 10000:
		pulseNum=11;
		break;
	case 20000:
		pulseNum=12;
		break;
	default:pulseNum=12;break;
	}
	L_event=lEventGenerate(pulseNum,wavelength,Sample_freaq);
	//////////////////////////////////////////////
	//插入点判断，判断其是否已经存在事件，如果已经存在事件就禁止插入
	int j=0;
	int flag=1;
	if(sGabor_Result.Valid_Num>0)
	{
		if(insertNum>sGabor_Result.Event_Array[i-1])
		{

			if((insertNum-sGabor_Result.Event_Array[i-1])>L_event)
			{

				flag=0;
			}
			else flag=1;


		}
		else
		{
			if(i>1)
			{
				for(j=0;j<i;j++)
				{
					if(insertNum<sGabor_Result.Event_Array[0])
					{
						if((insertNum+L_event)<sGabor_Result.Event_Array[0])
						{

							flag=0;
							break;

						}
						else 
						{
							flag=1;
							break;
						}
					}
					if(insertNum>sGabor_Result.Event_Array[j]&&insertNum<sGabor_Result.Event_Array[j+1])
					{
						int k=0;
						if((insertNum-sGabor_Result.Event_Array[j])>L_event&&(sGabor_Result.Event_Array[j+1]-insertNum)>L_event)
						{
							flag=0;
							break;
						}
						else
						{
							flag=1;
							break;
						}
					}
				}

			}
			else
			{
				if((insertNum+L_event)<sGabor_Result.Event_Array[0])
				{
					flag=0;

				}

			}
		}
	}
	else flag=0;
	if(insertNum<=sGabor_Result.Event_Array[0]+L_event/2)flag=1;


	if(flag)return sGabor_Result;



	int step_event=2*L_event;
	float dB_k_k_1=0;
	//	double Light_fiber_speed=300/Index_N;//缩小了10^6倍
	//两个数据之间的时间间隔：
	//	double Sample_time=Sample_freaq;//放大了10^6倍
	//	Sample_time=1/Sample_time;




	float return_loss=0;

	////////////////////////////////////////////
	int* input_data_buff=new int[paramLen];//短时傅里叶变化结果Cn
	memset(input_data_buff,0,paramLen*sizeof(int));

	if(sGabor_Result.Event_Array[sGabor_Result.Valid_Num-1]<insertNum)//尾端插入事件
	{
		//if(insertNum>=paramLen)return sGabor_Result;//插入的事件点超过了所有的数据
		sGabor_Result.Event_Array[sGabor_Result.Valid_Num++]=insertNum;
		//判断插入事件类型
		int up_limit=0;
		int low_limit=0;
		double line_a=0;
		double line_b=0;
		double line_c=0;
		double line_d=0;
		int insertMaxIndex=0;
		int insertMinIndex=0;
		double insertMax=0;
		double insertMin=65535;
		double input_data_buff_statick=0;
		low_limit=insertNum-L_event/4;
		up_limit=insertNum+L_event/2;
		if(low_limit<4) low_limit=4;






		// 阮志光版本
		//寻找插入点后面的趋势
		for(k=low_limit;k<up_limit;k++)
		{
			input_data_buff_statick=65535-input_data[k];
			if(input_data_buff_statick<insertMin)
			{
				insertMin=input_data_buff_statick;
				insertMinIndex=k;
			}
			if(input_data_buff_statick>insertMax)
			{
				insertMax=input_data_buff_statick;
				insertMaxIndex=k;
			}
		}
		if(insertMinIndex<=insertNum&&insertMaxIndex>=insertNum)//反射事件
		{
			sGabor_Result.Reflect_Type[sGabor_Result.Valid_Num-1]=1;

		}
		else if(insertMinIndex<=insertNum&&insertMaxIndex<insertNum)
		{
			if(insertMinIndex<insertMaxIndex)//衰减事件
			{
				sGabor_Result.Reflect_Type[sGabor_Result.Valid_Num-1]=0;
			}
			else if(insertMaxIndex<insertMinIndex)//反射事件
			{
				sGabor_Result.Reflect_Type[sGabor_Result.Valid_Num-1]=1;
			}
		}
		else if(insertMinIndex>insertNum&&insertMaxIndex>insertNum)
		{
			if(insertMin<=insertMaxIndex)//衰减事件
			{
				sGabor_Result.Reflect_Type[sGabor_Result.Valid_Num-1]=1;
			}
			else 
			{
				sGabor_Result.Reflect_Type[sGabor_Result.Valid_Num-1]=0;
			}
		}
		sGabor_Result.Event_type[sGabor_Result.Valid_Num-1]='A';

	}
	else// 中间插入事件
	{
		int Insert_event_no=0;
		for(i=0;i<sGabor_Result.Valid_Num;i++)
		{
			if(insertNum<sGabor_Result.Event_Array[i])
			{
				Insert_event_no=i;

				for(j=sGabor_Result.Valid_Num;j>Insert_event_no-1;j--)
				{
					sGabor_Result.Event_Array[j]=sGabor_Result.Event_Array[j-1];
					sGabor_Result.Total_Loss[j]=sGabor_Result.Total_Loss[j-1];
					sGabor_Result.Event_Hight[j]=sGabor_Result.Event_Hight[j-1];
					sGabor_Result.Event_Loss[j]=sGabor_Result.Event_Loss[j-1];
					sGabor_Result.Event_type[j]=sGabor_Result.Event_type[j-1];
					sGabor_Result.Line_k[j]=sGabor_Result.Line_k[j-1];
					sGabor_Result.Reflect_Type[j]=sGabor_Result.Reflect_Type[j-1];

				}
				sGabor_Result.Event_Array[i]=insertNum;
				sGabor_Result.Total_Loss[i]=0;
				sGabor_Result.Event_Hight[i]=0;
				sGabor_Result.Event_Loss[i]=0;
				sGabor_Result.Event_type[i]='A';
				sGabor_Result.Line_k[i]=0;
				break;
			}
		}
		sGabor_Result.Valid_Num++;

		//判断插入事件类型
		int up_limit=0;
		int low_limit=0;
		double line_a=0;
		double line_b=0;
		double line_c=0;
		double line_d=0;
		int insertMaxIndex=0;
		int insertMinIndex=0;
		double insertMax=0;
		double insertMin=65535;
		double input_data_buff_statick=0;
		low_limit=insertNum-L_event/4;
		up_limit=insertNum+L_event/2;
		if(low_limit<4) low_limit=4;






		// 阮志光版本
		//寻找插入点后面的趋势
		for(k=low_limit;k<up_limit;k++)
		{
			input_data_buff_statick=65535-input_data[k];
			if(input_data_buff_statick<insertMin)
			{
				insertMin=input_data_buff_statick;
				insertMinIndex=k;
			}
			if(input_data_buff_statick>insertMax)
			{
				insertMax=input_data_buff_statick;
				insertMaxIndex=k;
			}
		}
		if(insertMinIndex<=insertNum&&insertMaxIndex>=insertNum)//反射事件
		{
			//sGabor_Result.Reflect_Type[sGabor_Result.Valid_Num-1]=1;
			sGabor_Result.Reflect_Type[Insert_event_no]=1;

		}
		else if(insertMinIndex<=insertNum&&insertMaxIndex<insertNum)
		{
			if(insertMinIndex<insertMaxIndex)//衰减事件
			{
				sGabor_Result.Reflect_Type[Insert_event_no]=0;
			}
			else if(insertMaxIndex<insertMinIndex)//反射事件
			{
				sGabor_Result.Reflect_Type[Insert_event_no]=1;
			}
		}
		else if(insertMinIndex>insertNum&&insertMaxIndex>insertNum)
		{
			if(insertMin<=insertMaxIndex)//衰减事件
			{
				sGabor_Result.Reflect_Type[Insert_event_no]=1;
			}
			else 
			{
				sGabor_Result.Reflect_Type[Insert_event_no]=0;
			}
		}




	}
	/*sGabor_Result=K_Line_count(L_event,wavelength,pulsewidth,input_data,sGabor_Result,Index_N,Sample_freaq);
	sGabor_Result=Acount_Total_Loss(wavelength,pulsewidth,input_data,sGabor_Result,Index_N,Sample_freaq);*/

	sGabor_Result=K_Line_count(L_event,wavelength,pulsewidth,input_data,sGabor_Result,Index_N,Sample_freaq);////////////////////
	sGabor_Result.Event_Loss[sGabor_Result.Valid_Num-1]=0;
	sGabor_Result=Acount_Loss(wavelength,pulsewidth,input_data,sGabor_Result,Index_N,Sample_freaq);
	sGabor_Result=Acount_return_loss(wavelength,pulsewidth,input_data,sGabor_Result,Index_N,Sample_freaq,Return_loss_Tybe);
	sGabor_Result=Acount_Total_Loss(wavelength,pulsewidth,input_data,sGabor_Result,Index_N,Sample_freaq);
	sGabor_Result=K_Line_limite(wavelength,sGabor_Result);
	return sGabor_Result;
}

//******************************************************************//
//     删除事件
//     deleteNum:所删除事件点序号
//     wavelength:曲线波长
//     pulsewidth：曲线脉宽
//     input_data[]：分析的原始数据
//******************************************************************//
Gabor_Result C20120801::deleteEvent(int deleteNum,int wavelength,int pulsewidth,WORD input_data[],Gabor_Result sGabor_Result,float Index_N,float Sample_freaq,float Return_loss_Tybe)
{
	int i=0;
	i=sGabor_Result.Valid_Num;
	int pulseNum=0;
	int L_event=200;
	switch(pulsewidth)
	{
	case 3:
		pulseNum=0;
		break;
	case 5:
		pulseNum=1;
		break;
	case 10:
		pulseNum=2;
		break;
	case 20:
		pulseNum=3;
		break;
	case 50:
		pulseNum=4;
		break;
	case 100:
		pulseNum=5;
		break;
	case 200:
		pulseNum=6;
		break;
	case 500:
		pulseNum=7;	
		break;
	case 1000:
		pulseNum=8;
		break;
	case 2000:
		pulseNum=9;
		break;
	case 5000:
		pulseNum=10;
		break;
	case 10000:
		pulseNum=11;
		break;
	case 20000:
		pulseNum=12;
	default:pulseNum=12;break;
	}
	L_event=lEventGenerate(pulseNum,wavelength,Sample_freaq);




	//先求各段光纤的斜率：只要计算删除点所在点的下一个点的特性
	int up_limit,low_limit;
	int step_event=L_event*2;
	if(step_event<20)step_event=20;
	//float* sum_k=new float[2000];//短时傅里叶变化结果Cn
	//memset(sum_k,0,2000*sizeof(float));
	//double sum_k[20];
	double dB_k_k_1=0;

	double Light_fiber_speed=300/Index_N;//缩小了10^6倍
	//两个数据之间的时间间隔：
	double Sample_time=Sample_freaq;//放大了10^6倍
	Sample_time=1/Sample_time;
	//测量距离计算
	int Event_start_mark_cn=sGabor_Result.Event_Array[0];


	int j=0,k=0;
	j=sGabor_Result.Valid_Num;
	k=0;
	if(deleteNum==0||deleteNum>=sGabor_Result.Valid_Num)return sGabor_Result;
	for(i=0;i<sGabor_Result.Valid_Num;i++)
	{
		if(i==deleteNum)
			continue;
		else
		{
			sGabor_Result.Event_Array[k]=sGabor_Result.Event_Array[i];
			sGabor_Result.Event_Loss[k]=sGabor_Result.Event_Loss[i];
			sGabor_Result.Total_Loss[k]=sGabor_Result.Total_Loss[i];
			sGabor_Result.Event_type[k]=sGabor_Result.Event_type[i];
			sGabor_Result.Event_Hight[k]=sGabor_Result.Event_Hight[i];
			sGabor_Result.Reflect_Type[k]=sGabor_Result.Reflect_Type[i];
			k++;

		}
	}
	if(sGabor_Result.Valid_Num)sGabor_Result.Valid_Num--;
	sGabor_Result.Line_k[0]=0;

	sGabor_Result=K_Line_count(L_event,wavelength,pulsewidth,input_data,sGabor_Result,Index_N,Sample_freaq);////////////////////
	sGabor_Result=Acount_Loss(wavelength,pulsewidth,input_data,sGabor_Result,Index_N,Sample_freaq);
	sGabor_Result.Event_Loss[sGabor_Result.Valid_Num-1]=0;
	sGabor_Result=Acount_return_loss(wavelength,pulsewidth,input_data,sGabor_Result,Index_N,Sample_freaq,Return_loss_Tybe);
	sGabor_Result=Acount_Total_Loss(wavelength,pulsewidth,input_data,sGabor_Result,Index_N,Sample_freaq);
	sGabor_Result=K_Line_limite(wavelength,sGabor_Result);

	return sGabor_Result;
}





//用于光纤光栅的事件分析，将每一个事件的位置定位为顶峰

Gabor_Result C20120801::K_Line_limite(int wavelength,Gabor_Result sGabor_Result)
{
	int i=0;
	for(i=0;i<sGabor_Result.Valid_Num;i++)
	{
		if(wavelength==1310)//1310
		{
			if(sGabor_Result.Line_k[i]<0.28||sGabor_Result.Line_k[i]>0.42)
			{
				sGabor_Result.Line_k[i]=0;//0;//0.326;			
			}

		}
		else if(wavelength==1550)//1550
		{
			if(sGabor_Result.Line_k[i]<0.12||sGabor_Result.Line_k[i]>0.24)
			{
				sGabor_Result.Line_k[i]=0;//0;//0.326;			
			}
		}
		else if(wavelength==1625)//1625
		{
			if(sGabor_Result.Line_k[i]<0.12||sGabor_Result.Line_k[i]>0.26)
			{
				sGabor_Result.Line_k[i]=0;//0;//0.326;			
			}
		}
	}
	return sGabor_Result;
}


Gabor_Result C20120801::Optical_grating(int wavelength,int pulsewidth,WORD input_data[],INT paramLen,Gabor_Result sGabor_Result,float Index_N,float Sample_freaq,float dB_reflex)
{
	int Event_start=sGabor_Result.Event_Array[0];
	int Event_end=sGabor_Result.Event_Array[sGabor_Result.Valid_Num-1];
	int i=0,j=0;
	int Event_Array[200]; 
	int step_point=20;
	int up_time=0,down_time=0;
	int data_buff=0;
	long sum_data=0;
	int data_max=0;
	int data_min=0;
	int data_max_no=0,data_min_no=0;
	int data_avg=0;
	int* input_data_buff=new int[paramLen];//短时傅里叶变化结果Cn
	memset(input_data_buff,0,paramLen*sizeof(int));
	int DB_reflex_door=dB_reflex*1000;

	for(i=Event_start;i<Event_end;)//找到所有的上升沿
	{
		//取一段，求平均值
		sum_data=0;
		data_min=65535;
		data_min_no=i;
		data_max=0;
		data_max_no=i;
		//求段和
		for(j=i;j<i+step_point;j++)
		{
			data_buff=65535-input_data[j];
			sum_data=sum_data+data_buff;
			if(data_max<=data_buff)
			{
				data_max=data_buff;
				data_max_no=j;
			}
			if(data_min>=data_buff)
			{
				data_min=data_buff;
				data_min_no=j;
			}
		}
		//求段平均值
		data_avg=sum_data/step_point;
		//求段高于平均值+阈值的位置
		for(j=i;j<i+step_point;j++)
		{
			data_buff=65535-input_data[j];
			if(data_buff>data_avg+DB_reflex_door)
			{
				input_data_buff[j]=data_buff;
			}
			else input_data_buff[j]=0;
		}
		i=i+step_point;
	}


	sGabor_Result.Valid_Num=1;
	for(i=Event_start;i<Event_end;i++)
	{
		if(input_data_buff[i]!=0)
		{
			up_time++;
			if(up_time==5)
			{
				if(sGabor_Result.Valid_Num<99)
				{
					if(i-5>=Event_start+step_point/2)
						sGabor_Result.Event_Array[sGabor_Result.Valid_Num++]=i-5;
				}

			}
		}
		if(input_data_buff[i]==0)
		{
			down_time++;
			if(up_time)
			{
				if(down_time>=3)up_time=0;
			}
			else 
			{
				down_time=0;
				up_time=0;
			}
		}
	}
	//将事件下标移到反射峰最高点
	for(i=0;i<sGabor_Result.Valid_Num;i++)
	{
		/*data_max=0;
		data_max_no=0;
		for(j=sGabor_Result.Event_Array[i];j<sGabor_Result.Event_Array[i]+step_point;j++)
		{
		if(data_max<input_data_buff[j])
		{
		data_max=input_data_buff[j];
		data_max_no=j;
		}
		}
		sGabor_Result.Event_Array[i]=data_max_no;*/
		sGabor_Result.Event_type[i]='F';
		sGabor_Result.Reflect_Type[i]=1;
		sGabor_Result.Total_Loss[i]=0;
		sGabor_Result.Event_Loss[i]=0;
		sGabor_Result.Event_Hight[i]=0;
	}
	sGabor_Result.Event_Array[sGabor_Result.Valid_Num]=Event_end;
	sGabor_Result.Event_type[sGabor_Result.Valid_Num]='F';
	sGabor_Result.Reflect_Type[sGabor_Result.Valid_Num]=1;
	sGabor_Result.Total_Loss[sGabor_Result.Valid_Num]=0;
	sGabor_Result.Event_Loss[sGabor_Result.Valid_Num]=0;
	sGabor_Result.Event_Hight[sGabor_Result.Valid_Num++]=0;
	delete[]input_data_buff;
	return sGabor_Result;

}

Gabor_Result C20120801::Acount_Loss(int wavelength,int pulsewidth,WORD input_data[],Gabor_Result sGabor_Result,float Index_N,float Sample_freaq)
{
	int i=0,k=0;
	int up_limit=0,low_limit=0;
	//float dB_k_line=0,Reflex_avg_dB=0,dB_Hight=0;

	double dB_Hight=0;
	double dB_Low=0;
	double dB_k_line=0;
	int max_data_no=0;
	int min_data_no=0;
	float max_data=0;
	float min_data=0;
	double Reflex_avg_dB=0;


	int step_event=0;

	int pulseNum=0;
	int L_event=200;
	switch(pulsewidth)
	{
	case 3:
		pulseNum=0;
		break;
	case 5:
		pulseNum=1;
		break;
	case 10:
		pulseNum=2;
		break;
	case 20:
		pulseNum=3;
		break;
	case 50:
		pulseNum=4;
		break;
	case 100:
		pulseNum=5;
		break;
	case 200:
		pulseNum=6;
		break;
	case 500:
		pulseNum=7;	
		break;
	case 1000:
		pulseNum=8;
		break;
	case 2000:
		pulseNum=9;
		break;
	case 5000:
		pulseNum=10;
		break;
	case 10000:
		pulseNum=11;
		break;
	case 20000:
		pulseNum=12;
	default:pulseNum=12;break;
	}
	L_event=lEventGenerate(pulseNum,wavelength,Sample_freaq);
	step_event=L_event*2;
	if(step_event<=20)step_event=20;


	if(pulsewidth>=2000)/////////////////////////////////////////20130615
		step_event=pulsewidth/25*(Sample_freaq/25);

	//光在光纤中传播的速度
	double Light_fiber_speed=300/Index_N;//缩小了10^6倍
	//两个数据之间的时间间隔：
	double Sample_time=Sample_freaq;//放大了10^6倍
	Sample_time=1/Sample_time;
	//测量距离计算
	//  double Measure_distacne = Light_fiber_speed*Sample_time*paramLen;


	if(sGabor_Result.Valid_Num>1)
	{
		for(i=0;i<sGabor_Result.Valid_Num;i++)
		{
			if(sGabor_Result.Reflect_Type[i])//反射事件
			{
				//算反射高度
				if(i==0)
				{
					sGabor_Result.Reflect_Type[i]=1;
					continue;
				}
				up_limit=sGabor_Result.Event_Array[i]-step_event/4;
				if(up_limit<sGabor_Result.Event_Array[i-1])up_limit=sGabor_Result.Event_Array[i-1];//sGabor_Result.Event_Array[i]+2;
				//if(up_limit<=0)up_limit=3;
				low_limit=sGabor_Result.Event_Array[i]+step_event/2;
				if(i<sGabor_Result.Valid_Num-1)
				{
					if(low_limit>sGabor_Result.Event_Array[i+1])
						low_limit=sGabor_Result.Event_Array[i+1]-step_event/10;
					if(low_limit<sGabor_Result.Event_Array[i])low_limit=sGabor_Result.Event_Array[i+1]-2;
				}

				max_data=0;
				min_data=65535;
				for(k=up_limit;k<low_limit;k++)
				{
					if((65535-max_data)>input_data[k])
					{
						max_data=65535-input_data[k];
						max_data_no=k;
					}
					if((65535-min_data)<input_data[k])
					{
						min_data=65535-input_data[k];
						min_data_no=k;
					}
				}
				if(min_data_no>max_data_no)
				{
					//max_data=0;
					min_data=65535;
					for(k=up_limit;k<max_data_no;k++)
					{
						if(min_data>(65535-input_data[k]))
						{
							min_data=65535-input_data[k];
							min_data_no=k;
						}
					}
				}
				if(min_data_no>50)//对最小值做平均处理，消除噪声影响
				{
					Reflex_avg_dB=0;
					for(k=0;k<5;k++)
					{
						Reflex_avg_dB=Reflex_avg_dB+65535-input_data[min_data_no-k];
					}
					Reflex_avg_dB=Reflex_avg_dB/5;
					min_data=Reflex_avg_dB;
				}

				sGabor_Result.Event_Hight[i]=(max_data-min_data)/1000.0;

				//前一个事件也为反射事件，判断两个反射事件的距离是否满足1个波长的距离，如果满足就计算损耗，不满足就
				//不计算损耗
				if(sGabor_Result.Reflect_Type[i-1]||i==1)
				{
					if((sGabor_Result.Event_Array[i-1]+3*step_event/2)>sGabor_Result.Event_Array[i])
					{
						sGabor_Result.Event_Loss[i]=0;
						//		continue;
					}
				}
				//算回波损耗



			}
			//else//衰减事件  插入损耗计算/////////该段代码决定最终损耗的准确性
			{
				/*if(i==sGabor_Result.Valid_Num-1)
				{
					sGabor_Result.Event_Loss[i]=0;
					break;
				}*/
				if(sGabor_Result.Line_k[i]>0)dB_k_line=sGabor_Result.Line_k[i];
				else if(sGabor_Result.Line_k[i+1]>0)
				{
					dB_k_line=sGabor_Result.Line_k[i+1];
				}
				else
				{
					if(wavelength==1310)dB_k_line=Avg_loss_1310;//1310
					else if(wavelength==1550)dB_k_line=Avg_loss_1550;//1550
					else if(wavelength==1625)dB_k_line=Avg_loss_1625;//1550
					else dB_k_line=0.186;//1550
				}
				if(i!=0)
				{

					int avg_up=0,avg_low=0;
					up_limit=sGabor_Result.Event_Array[i]-step_event/8;
					if(up_limit<=0)up_limit=3;
					avg_up=input_data[up_limit-3]+input_data[up_limit-2]+input_data[up_limit-1]+input_data[up_limit]+input_data[up_limit+1]+input_data[up_limit+2]+input_data[up_limit+3];
					avg_up=avg_up/7;
					low_limit=sGabor_Result.Event_Array[i]+step_event;
					avg_low=input_data[low_limit-3]+input_data[low_limit-2]+input_data[low_limit-1]+input_data[low_limit]+input_data[low_limit+1]+input_data[low_limit+2]+input_data[low_limit+3];
					avg_low=avg_low/7;
					dB_Hight=avg_up -avg_low ;
					if(sGabor_Result.Reflect_Type[i]==0)/////////////////////////20130617
						dB_Hight=dB_Hight/1000+1.125*step_event*(Sample_time*Light_fiber_speed)/2000*dB_k_line;
					else dB_Hight=dB_Hight/1000;
					sGabor_Result.Event_Loss[i]=-(dB_Hight);
					if((sGabor_Result.Event_Loss[i])<=0.005)
						sGabor_Result.Event_Loss[i]=0;
					if(sGabor_Result.Reflect_Type[i])
					{
						if(sGabor_Result.Event_Loss[i]<=0.005)sGabor_Result.Event_Loss[i]=0;
					}
					;//算插入损耗	
				}
				else
				{
					sGabor_Result.Event_Loss[i]=0;
				}

			} 

		}
	}
	if(sGabor_Result.Valid_Num)sGabor_Result.Event_Loss[sGabor_Result.Valid_Num-1]=0;
	return sGabor_Result;
}
Gabor_Result C20120801::Acount_return_loss(int wavelength,int pulsewidth,WORD input_data[],Gabor_Result sGabor_Result,float Index_N,float Sample_freaq,float Return_loss_Tybe)
{
	int i=0;
	float return_tybe=0;
	float return_loss=0;
	float Pulse_width_status=0;
	if(pulsewidth==3)Pulse_width_status=5;
	else if(pulsewidth==5)Pulse_width_status=7.5;
	else Pulse_width_status=pulsewidth;
	for(i=0;i<sGabor_Result.Valid_Num;i++)
	{
		if(i>0)
		{
			//sumTotalLoss=sumTotalLoss+sGabor_Result.Line_k[i]/1000*(sGabor_Result.Event_Array[i]-sGabor_Result.Event_Array[i-1])+sGabor_Result.Event_Loss[i];
			//sGabor_Result.Total_Loss[i]=sumTotalLoss;
			if(sGabor_Result.Reflect_Type[i])//如果为反射事件，求回波损耗
			{

				if(wavelength==1310)return_tybe=Return_loss_1310_tybe;
				else if(wavelength==1550)return_tybe=Return_loss_1550_tybe;
				else if(wavelength==1625)return_tybe=Return_loss_1625_tybe;

				return_loss=sGabor_Result.Event_Hight[i]/5;
				return_loss=pow(10,return_loss)-1;
				sGabor_Result.Event_Hight[i]=(10*log10(Pulse_width_status/1000000000.0));
				//if(wavelength==1550)
				//sGabor_Result.Event_Hight[i]=sGabor_Result.Event_Hight[i]+10*log10(return_loss)+(-70-Return_loss_Tybe);//-(sGabor_Result.Event_Hight[i]+10*log10(return_loss))+(Return_loss_Tybe)+70;//
				//else sGabor_Result.Event_Hight[i]=sGabor_Result.Event_Hight[i]+10*log10(return_loss)+10.5;
                if(return_loss>0)
				sGabor_Result.Event_Hight[i]=-(sGabor_Result.Event_Hight[i]+10*log10(return_loss))-(return_tybe+Return_loss_Tybe);
				else sGabor_Result.Event_Hight[i]=70;
				if((sGabor_Result.Event_Hight[i])<=12.0)sGabor_Result.Event_Hight[i]=0;
				else if((sGabor_Result.Event_Hight[i])>=70.0)sGabor_Result.Event_Hight[i]=70;
			}

			else sGabor_Result.Event_Hight[i]=0;
			//sGabor_Result.Event_Hight[i]=2*sGabor_Result.Event_Hight[i];
		}
	}
	return sGabor_Result;

}
Gabor_Result C20120801::Acount_Total_Loss(int wavelength,int pulsewidth,WORD input_data[],Gabor_Result sGabor_Result,float Index_N,float Sample_freaq)
{
	float start_loss_dB=0;
	int Event_start_mark_cn=sGabor_Result.Event_Array[0];

	//光在光纤中传播的速度
	double Light_fiber_speed=300/Index_N;//缩小了10^6倍
	//两个数据之间的时间间隔：
	double Sample_time=Sample_freaq;//放大了10^6倍
	Sample_time=1/Sample_time;

	int i=0,j=0;
	/////////////////////////////////////////////////////////////////////////////////
	//	int pulseNum=0;
	//	switch(pulsewidth)
	//	{
	//	case 3:
	//		pulseNum=0;
	//		break;
	//	case 5:
	//		pulseNum=1;
	//		break;
	//	case 10:
	//		pulseNum=2;
	//		break;
	//	case 20:
	//		pulseNum=3;
	//		break;
	//	case 50:
	//		pulseNum=4;
	//		break;
	//	case 100:
	//		pulseNum=5;
	//		break;
	//	case 200:
	//		pulseNum=6;
	//		break;
	//	case 500:
	//		pulseNum=7;	
	//		break;
	//	case 1000:
	//		pulseNum=8;
	//		break;
	//	case 2000:
	//		pulseNum=9;
	//		break;
	//	case 5000:
	//		pulseNum=10;
	//		break;
	//	case 10000:
	//		pulseNum=11;
	//		break;
	//	case 20000:
	//		pulseNum=12;
	//		break;
	//	default:pulseNum=12;break;
	//	}
	//	//查找计算损耗的最佳起始点。
	//	int step_L=0;
	//	int Start_point=0,End_point=0;
	//	float avg_roias=0;
	//	int up_time=0,fault_time=0;
	//	int data1=0,data2=0;
	//	int count_success_time=0;
	//
	//	step_L=2*lEventGenerate(pulseNum, wavelength,Sample_freaq);
	//	if(sGabor_Result.Event_Array[0]<step_L)Start_point=step_L;
	//	else Start_point=sGabor_Result.Event_Array[0];
	//	if((sGabor_Result.Event_Array[1]-Start_point)<4*step_L)
	//	{
	//		End_point=sGabor_Result.Event_Array[1]-step_L/4;
	//	}
	//	else End_point=Start_point+4*step_L;
	//	for(i=Start_point;i<End_point;i++)
	//	{
	//		//求出段噪声。
	//		avg_roias=avg_roias+abs(input_data[i]-input_data[i+1]);
	//
	//	}
	//	avg_roias=2*avg_roias/(End_point-Start_point);
	//	if(avg_roias<100)avg_roias=100;
	//	if((End_point-Start_point)>20)
	//	{
	//		for(i=End_point;i>Start_point;i--)
	//		{
	//			fault_time=0;
	//			up_time=0;
	//			count_success_time=0;
	//			for(j=i;j>Start_point;j--)
	//			{
	//				data1=input_data[i];
	//				data2=input_data[j];
	//				if((data1-data2)>avg_roias)
	//				{
	//					up_time++;
	//					fault_time=0;
	//					if(up_time>=10)
	//					{
	//						//Start_point=i;
	//						count_success_time++;
	//						if(count_success_time>=5)
	//						{
	//							Start_point=i;
	//							break;
	//						}
	//						
	//					}
	//					continue;
	//				}
	//				else
	//				{
	//					fault_time++;
	//					
	//					if(fault_time>=3)
	//					{
	//						count_success_time=0;
	//						break;
	//					}
	//					
	//				}
	//			}
	//			if(j<=Start_point)Start_point=i;
	//		}
	//	}
	//	sGabor_Result.Event_Array[0]=Start_point;
	//
	///////////////////////////////////////////////////////////////////////////////////
	if(sGabor_Result.Valid_Num==2)
	{
		if(sGabor_Result.Line_k[1]==0)
		{
			sGabor_Result.Total_Loss[i]=0;
		}
		else
		{
			sGabor_Result.Total_Loss[1]=sGabor_Result.Line_k[1]*sGabor_Result.Event_Array[1]*Sample_time*Light_fiber_speed/2000+0.02;
		}
	}
	else if(sGabor_Result.Valid_Num>2)
	{
		if(sGabor_Result.Line_k[1]!=0)
		{
			start_loss_dB=sGabor_Result.Line_k[1]*sGabor_Result.Event_Array[1]*Sample_time*Light_fiber_speed/2000;
		}
		else if(sGabor_Result.Line_k[2]!=0)
		{
			start_loss_dB=sGabor_Result.Line_k[2]*sGabor_Result.Event_Array[1]*Sample_time*Light_fiber_speed/2000;
		}
		else 
		{
			if(wavelength==1310)
			{
				start_loss_dB=Avg_loss_1310*sGabor_Result.Event_Array[1]*Sample_time*Light_fiber_speed/2000;
			}
			else if(wavelength==1550)
			{
				start_loss_dB=Avg_loss_1550*sGabor_Result.Event_Array[1]*Sample_time*Light_fiber_speed/2000;
			}
			else if(wavelength==1625)
			{
				start_loss_dB=Avg_loss_1625*sGabor_Result.Event_Array[1]*Sample_time*Light_fiber_speed/2000;
			}
		}
		start_loss_dB=start_loss_dB+0.02;
		sGabor_Result.Total_Loss[1]=start_loss_dB;
		for(i=2;i<sGabor_Result.Valid_Num;i++)
		{
			if(sGabor_Result.Event_Array[i]>=15&&pulsewidth>10&&sGabor_Result.Event_Array[1]>=10)
			{
				sGabor_Result.Total_Loss[i]=(input_data[sGabor_Result.Event_Array[i]-15]+input_data[sGabor_Result.Event_Array[i]-14]+input_data[sGabor_Result.Event_Array[i]-13]+input_data[sGabor_Result.Event_Array[i]-12]+input_data[sGabor_Result.Event_Array[i]-11])/5
					-(input_data[sGabor_Result.Event_Array[1]-6]+input_data[sGabor_Result.Event_Array[1]-7]+input_data[sGabor_Result.Event_Array[1]-8]+input_data[sGabor_Result.Event_Array[1]-9]+input_data[sGabor_Result.Event_Array[1]-10])/5;
				sGabor_Result.Total_Loss[i]=(sGabor_Result.Total_Loss[i])/1000+start_loss_dB;//-sGabor_Result.Event_Loss[0];///-sGabor_Result.Event_Loss[0]20130617
				if(sGabor_Result.Total_Loss[i]<=0.1)sGabor_Result.Total_Loss[i]=0;
				if(i>0)
				{

					for(j=0;j<i;j++)//如果后面的总损耗计算结果比前面的总损耗结果还要小，说明错误，记为0
					{
						if(sGabor_Result.Total_Loss[i]<sGabor_Result.Total_Loss[j])
							sGabor_Result.Total_Loss[i]=0;
					}
				}


			}
			else
			{
				sGabor_Result.Total_Loss[i]=0;
			}

		}
	}
	return sGabor_Result;
}