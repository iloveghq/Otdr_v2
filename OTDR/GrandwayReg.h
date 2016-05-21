#pragma once
/***********************************************************
类:GrandwayReg
wcq 090730
功能：注册表相关操作
*************************************************************/
class CGrandwayReg
{
public:
	CGrandwayReg(void);
public:
	~CGrandwayReg(void);
public:	
	void FindComm(CStringArray& arrReturn);//寻找注册表中的串口
	CString readKey(CString key);//读注册表
	void writeKey(CString key,CString value);//写注册表
};
