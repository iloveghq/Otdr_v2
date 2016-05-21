#pragma once
#include "GrandwayReg.h"
#include "locale.h"//
/***********************************************************
类:Util
wcq 090730
功能：辅助类，用于得到FHP2系列光功率计或FH2S光源的串口名称等
注意：该类需要引用GrandwayComm和GrandwayReg类
*************************************************************/
class CUtil
{
public:
	CUtil(void);
public:
	~CUtil(void);
public:
	CString m_newFhpCommName;//FHS系列光源串口名称
	CString m_standFhpCommName;//标准表串口名称
	CGrandwayReg m_reg;
public:
	CString GetTimeWaste(LONG longbegintime,LONG longendtime);//用时
	CString getCurTimeStr(BOOL bContainHour=TRUE);//得到当前时间的字符串表示，bContainHour：是否包含时:分:秒
	//
	static INT splitString(CString str,char split,CStringArray& strArray);//分解字符串
	CString getAddSerial(CString str,INT last);//序列自动加1
	BOOL isInArray(CStringArray& strArray,CString commname);
	//
	char* old_locale;
	void changeLocaleToChs();//读取文本文件时为时stdiofile类能处理中文需要先转到本地
	void recoverLoacle();//前面函数的反动作

	//
	static CString getExePath();//获取文件运行路径
	static BOOL isFileExist(CString filename);//查看文件是否存在
	void openFolder(CString filepath);//打开文件夹

	//
	void setListToReport(CListCtrl &list);//修改list样式为报表形式
	void setListToReportCheck(CListCtrl &list);//修改list样式为报表形式

	//
	CString getPath(HWND hwndOwner);//文件夹选择框

	//
	BOOL isFloat(FLOAT f);//判断一个浮点数是否为正常的浮点数
	//将二进制资源保存为文件
	BOOL saveBinResourceAsFile(CString saveAsFileName,UINT resID,CString folder=_T(""),BOOL bSubPathOfExe=TRUE);

	
	////从注册表里初始化连续的选择(check.radio)
	void initMultiSelect(CDialog* dlg,INT startID,INT totalItem,CString regKey);//从注册表里初始化连续的选择(check.radio)
	//保存连续的选择到注册表里(check.radio)
	void saveMultiSelect(CDialog* dlg,INT startID,INT totalItem,CString regKey);//保存连续的选择到注册表里(check.radio)
	//保存连接的文本框内容到注册表
	void saveMultiEdit(CDialog* dlg,INT startID,INT totalItem,CString regKey);//保存连接的文本框内容到注册表
	//从注册表里初始化连续的文本框内容
	void initMultiEdit(CDialog* dlg,INT startID,INT totalItem,CString regKey,CString defaultValues=_T(""));//从注册表里初始化连续的文本框内容
	INT getMultiSelect(CDialog* dlg,UINT startID,INT totalItem);//得到多个连续的checkbox,radio选择了哪个，返回偏移第一个位置
	//在下拉选择事件中得到选择的文本
	CString getComboSelText(CComboBox* comboBox);//在下拉选择事件中得到选择的文本
	
	////给文本框赋float值
	void setEditFloat(CDialog* dlg,UINT ID,FLOAT f,INT precision=2);
	//
	void ReStart(BOOL bNormal);//重启程序
	//
	CString pCharToCString(char* pchar);//char*转化为CString
	CString pTCharToCString(TCHAR* pchar);//TCHAR*转化为CString
	INT CStringToPChar(CString str, char* pchar);//CString转化为char*
};