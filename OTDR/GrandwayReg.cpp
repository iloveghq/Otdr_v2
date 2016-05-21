#include "StdAfx.h"
#include "GrandwayReg.h"
/***********************************************************
类:GrandwayReg
wcq 090730
功能：注册表相关操作
说明：使用时要把LPCTSTR data_set=_T("SOFTWARE\\COpticalPowerAutoTest");//键值类似的COpticalPowerAutoTest改为自己想要的名称
*************************************************************/
CGrandwayReg::CGrandwayReg(void)
{
}

CGrandwayReg::~CGrandwayReg(void)
{
}
/**********************************************************
函  数  名:FindComm
作      用:寻找注册表中的串口
形  参  数:CStrinArray,引用调用，保存读到的串口数组
返	回  值:BOOL,有串口时返回真，反之假
***********************************************************/
void CGrandwayReg::FindComm(CStringArray& arrReturn)
{
	HKEY hKey;//指向子键的句柄
	INT rtn;//注册表操作是否成功标志
	rtn=RegOpenKeyEx(HKEY_LOCAL_MACHINE,_T("Hardware\\DeviceMap\\SerialComm"),NULL,KEY_READ,&hKey);//打开注册表中串口所在子键
	if(rtn!=ERROR_SUCCESS)//打开失败
	{
		return;
	}

	INT i=0;//串口在注册表中位置
	TCHAR portName[256],commName[256];//portName：串口名称，
	while(1)
	{
		DWORD dwLong,dwSize;
		dwSize=sizeof(portName);
		dwLong=dwSize;
		rtn=RegEnumValue(hKey,i,portName,&dwLong,NULL,NULL,(PUCHAR)commName,&dwSize);//枚举串口
		if(rtn==ERROR_SUCCESS)//有串口
		{
			arrReturn.Add((CString)commName);
		}
		if(rtn==ERROR_NO_MORE_ITEMS)//无或最后一个
		{
			break;
		}
		i++;
	}
	RegCloseKey(hKey);//关闭句柄
}
/**********************************************************
函  数  名:readKey
作      用:读注册表
形  参  数:key:键值
返	回  值:对应的字符串
***********************************************************/
CString CGrandwayReg::readKey(CString key)//读注册表
{
	HKEY hkey;
	LPCTSTR data_set=_T("SOFTWARE\\PLCSC");//键值
	long ret=::RegOpenKeyEx(HKEY_LOCAL_MACHINE,data_set,0,KEY_READ,&hkey);//打开键
	if(ret!=ERROR_SUCCESS)//未能打开键
	{
		//添加键值
		::RegCreateKey(HKEY_LOCAL_MACHINE,_T("SOFTWARE\\PLCSC\\"),&hkey);
		//AfxMessageBox(_T("无法打开注册表"));
		//::RegCloseKey(hkey);
		//return _T("");
		ret=::RegOpenKeyEx(HKEY_LOCAL_MACHINE,data_set,0,KEY_READ,&hkey);//再次尝试打开
		if(ret!=ERROR_SUCCESS)
		{
			AfxMessageBox(_T("无法打开注册表"));
			::RegCloseKey(hkey);
			return _T("");
		}
	}
	BYTE data[1000];
	DWORD type=REG_SZ;
	DWORD cbdata=1000;
	ret=::RegQueryValueEx(hkey,key,NULL,&type,data,&cbdata);//读值
	if(ret!=ERROR_SUCCESS)
	{
		//AfxMessageBox(_T("无法查看有关注册表数据"));
		::RegCloseKey(hkey);
		writeKey(key,_T(""));////////?????????????????????????????????????????????????????
		return _T("");
	}
	::RegCloseKey(hkey);//关闭键值
	if(cbdata>0)
	{
		return CString((LPCTSTR)data);
	}
	else
	{
		return _T("");
	}
}
/**********************************************************
函  数  名:writeKey
作      用:写注册表
形  参  数:key:键值,value:对应的要写入的字符串
返	回  值:
注意：该函数有可能在readKey读取数据出错时被readKey调用
***********************************************************/
void CGrandwayReg::writeKey(CString key,const CString value)//写注册表
{
	HKEY hkey;
	LPCTSTR data_set=_T("SOFTWARE\\PLCSC");
	long ret=::RegOpenKeyEx(HKEY_LOCAL_MACHINE,data_set,0,KEY_WRITE,&hkey);
	if(ret!=ERROR_SUCCESS)
	{
		if(ERROR_SUCCESS!=::RegCreateKey(HKEY_LOCAL_MACHINE,_T("SOFTWARE\\PLCSC\\"),&hkey))//创建key
		{
			AfxMessageBox(_T("无法打开注册表"));
			return;
		}//end if
	}//end if
//	char lb[1024];
	//TCHAR *va=value;
	//_tcscpy(lb,value);
	//WideCharToMultiByte(CP_OEMCP,0,(LPCTSTR)value,-1,lb,value.GetLength(),0,false);
	DWORD type=REG_SZ;
	DWORD cbdata=(lstrlen(value)+1)*sizeof(TCHAR);
	ret=::RegSetValueEx(hkey,key,NULL,type,(const BYTE*)(LPCTSTR)value,cbdata);
	if(ret!=ERROR_SUCCESS)
	{
		AfxMessageBox(_T("无法修改相应注册表键值"));
	}
	::RegCloseKey(hkey);
}