#pragma once

struct OTDRConfig
{
	//系统配置项
	CString strLang; //0为中文，1为英文
	CString strLightLevel; //背光亮度,15-100
	CString strAutoPowerOff; //自动关机时间，单位为分钟
	CString strDateTime; //日期时间
	//CString strTime; //时间
	CString strTouchSreeenStatus; //触摸屏状态

	//测试配置项
	CString strTestWave; //测试波长
	CString strTestTime; //测试时间
	CString strTestMode; //测试模式，0为手动，1为自动
	CString strTestRange; //测量范围
	CString strUnit; //单位
	CString strPluseWidth; //脉宽选择
	CString strResolution; //分辨率
	CString strAutoSave; //自动保存
};