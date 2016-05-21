#include "StdAfx.h"
#include "Util.h"

CUtil::CUtil(void)
{
	m_newFhpCommName=_T("");
	m_standFhpCommName=_T("");
}

CUtil::~CUtil(void)
{
}
//判断端口是不是在一个array
BOOL CUtil::isInArray(CStringArray& strArray,CString commname)
{
	INT it=strArray.GetSize()-1;
	for(int i=it;i>-1;i--)
	{
		if(commname==strArray.GetAt(i))
		{
			return TRUE;
		}
	}
	return FALSE;
}
/*****************************************************************************************************
函  数  名:GetTimeWaste
作      用:返回两次通过GetTickCount()获得的时间差的字符串表示
返	回  值:CString,时分秒的字符串
******************************************************************************************************/
CString CUtil::GetTimeWaste(LONG longbegintime,LONG longendtime)//用时
{
	CString str,str1;
	LONG t;
	t=longendtime-longbegintime;
	t%=3600000*60;
	str1.Format(_T(":%d"),t/3600000);
	str+=str1;
	t%=3600000;
	str1.Format(_T(":%d"),t/60000);
	str+=str1;
	t%=60000;
	str1.Format(_T(":%d"),t/1000);
	str+=str1;
	return str;
}
////////////////////////////////////////////////////////////////////////////////////
//函  数  名:getCurTimeStr
//作      用:得到当前时间的字符串表示，bContainHour：是否包含时:分:秒
//返  回  值:bContainHour:TRUE,返回形如"2011-04-15 10:16:30",bContainHour:FALSE,返回形如"2011-04-15"
////////////////////////////////////////////////////////////////////////////////////
CString CUtil::getCurTimeStr(BOOL bContainHour)
{
	CString temp;
	CTime time=CTime::GetCurrentTime();////时间
	if(bContainHour)
	{
		temp=time.Format(_T("%Y-%m-%d %H:%M:%S"));
	}
	else
	{
		temp=time.Format(_T("%Y-%m-%d"));
	}//eif
	return temp;
}
/*****************************************************************************************************
函  数  名:splitString
作      用:分解字符串
返	回  值:
******************************************************************************************************/
INT CUtil::splitString(CString str,char split,CStringArray& strArray)
{
	strArray.RemoveAll();
	CString strTemp=str;
	int nIndex=0;
	while(1)
	{
		nIndex=strTemp.Find(split);
		if(nIndex>=0)
		{
			strArray.Add(strTemp.Left(nIndex));
			strTemp=strTemp.Right(strTemp.GetLength()-nIndex-1);
		}
		else
		{
			break;
		}
	}
	strArray.Add(strTemp);
	return strArray.GetSize();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////
//作用:序列自动加1
//返回  值:序列号后last位加一后得到的字符串
///////////////////////////////////////////////////////////////////////////////////////////////////////
CString CUtil::getAddSerial(CString str,INT last)
{
	INT len=str.GetLength();
	if(len<last)//序列号长度和最后几位一样或更小，则不处理
	{
		return str;
	}
	CString left=str.Left(str.GetLength()-last);
	CString arr[5]={_T("%.4d"),_T("%.5d"),_T("%.6d"),_T("%.7d"),_T("%.8d")};
	CString temp2;
	temp2.Format(arr[last-4],_ttoi(str.Right(last))+1);
	return left+temp2;
}

/*****************************************************************************************************
函  数  名:GetTimeWaste
作      用://读取文本文件时为时stdiofile类能处理中文需要先转到本地
返	回  值:
******************************************************************************************************/
void CUtil::changeLocaleToChs()
{
	old_locale=_strdup(setlocale(LC_CTYPE,NULL));
	setlocale(LC_CTYPE,"chs");
}
/*****************************************************************************************************
函  数  名:recoverLoacle()
作      用:前面函数的反动作
返	回  值:
******************************************************************************************************/
void CUtil::recoverLoacle()
{
	setlocale(LC_CTYPE,old_locale);
	free(old_locale);
}
/*****************************************************************************************************
函  数  名:getExePath()
作      用://获取文件运行路径
返	回  值:
******************************************************************************************************/
CString CUtil::getExePath()
{
	CString sPath;
	GetModuleFileName(NULL,sPath.GetBufferSetLength(MAX_PATH+1),MAX_PATH);
	sPath.ReleaseBuffer();
	int nPos;
	nPos=sPath.ReverseFind('\\');
	sPath=sPath.Left(nPos);
	return sPath;
}
/*****************************************************************************************************
函  数  名:isFileExist
作      用:;//查看文件是否存在
返	回  值:
******************************************************************************************************/
BOOL CUtil::isFileExist(CString filename)
{
	CString strFile;
	CFileFind finder;
	//return finder.FindFile(filename);//改为下面的 cwcq2012.11.05
	BOOL bFind = finder.FindFile(filename);//wcq2012.11.05
	finder.Close();//wcq2012.11.05
	return bFind;//wcq2012.11.05
}
/*****************************************************************************************************
函  数  名:openFolder
作      用:打开文件夹
返	回  值:
******************************************************************************************************/
void CUtil::openFolder(CString filepath)
{
	ShellExecute(NULL,NULL,filepath,NULL,NULL,SW_SHOW);
}
/*****************************************************************************************************
函  数  名:setListToReport
作      用:修改list样式为报表形式
形  参  数:list:CListCtrl
返	回  值:
说      明：需要在属性中设view为report
******************************************************************************************************/
void CUtil::setListToReport(CListCtrl &list)
{
	DWORD dwStyle=::GetWindowLong(list.m_hWnd,GWL_STYLE);
	SetWindowLong(list.m_hWnd,GWL_STYLE,dwStyle|LVS_SHOWSELALWAYS|LVS_REPORT|LVS_NOLABELWRAP);
	ListView_SetExtendedListViewStyle(list.m_hWnd,LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);

}
/*****************************************************************************************************
函  数  名:setListToReport
作      用:修改list样式为报表形式,包含checkbox样式
形  参  数:list:CListCtrl
返	回  值:
说      明：需要在属性中设view为report
******************************************************************************************************/
void CUtil::setListToReportCheck(CListCtrl &list)
{
	DWORD dwStyle=::GetWindowLong(list.m_hWnd,GWL_STYLE);
	SetWindowLong(list.m_hWnd,GWL_STYLE,dwStyle|LVS_SHOWSELALWAYS|LVS_REPORT|LVS_NOLABELWRAP);
	ListView_SetExtendedListViewStyle(list.m_hWnd,LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_CHECKBOXES);

}
/*****************************************************************
** 函数名:GetPath
** 输 入: 无
** 输 出: CString strPath
**        strPath非空, 表示用户选择的文件夹路径
**        strPath为空, 表示用户点击了“取消”键，取消选择
** 功能描述:显示“选择文件夹”对话框，让用户选择文件夹 结果：如："E:\xxx个人文件夹\plc"
****************************************************************/
CString CUtil::getPath(HWND hwndOwner)
{
	CString strPath =_T("");
	BROWSEINFO bInfo;
	ZeroMemory(&bInfo, sizeof(bInfo));
	bInfo.hwndOwner = hwndOwner;
	bInfo.lpszTitle = _T("请选择路径: ");
	bInfo.ulFlags = BIF_RETURNONLYFSDIRS;    

	LPITEMIDLIST lpDlist; //用来保存返回信息的IDList
	lpDlist = SHBrowseForFolder(&bInfo) ; //显示选择对话框
	if(lpDlist != NULL)  //用户按了确定按钮
	{
		TCHAR chPath[255]; //用来存储路径的字符串
		SHGetPathFromIDList(lpDlist, chPath);//把项目标识列表转化成字符串
		strPath = chPath; //将TCHAR类型的字符串转换为CString类型的字符串
	}
	return strPath;
}
/*****************************************************************************************************
函  数  名:isFloat
作      用:;//判断一个浮点数是否为正常的浮点数
形  参  数:f:浮点数
返	回  值:含有"-1.#J"
说      明：
******************************************************************************************************/
BOOL CUtil::isFloat(FLOAT f)
{
	CString temp;
	temp.Format(_T("%.2f"),f);
	if(temp.Find(_T("-1.#"))!=-1)//
	{
		return FALSE;
	}
	return TRUE;
}

//将二进制资源保存为文件
//saveAsFileName:保存文件为该名称，resID：资源ID,folder:文件夹;bSubPathOfExe:是否放在应用程序文件夹下，true:是
BOOL CUtil::saveBinResourceAsFile(CString saveAsFileName,UINT resID,CString folder,BOOL bSubPathOfExe)
{
	//
	HRSRC hsrc=::FindResource(NULL,MAKEINTRESOURCE(resID),TEXT("BIN"));//定位资源
	if(NULL==hsrc)
	{
		return FALSE;
	}//end if

	DWORD dwSize=::SizeofResource(NULL,hsrc);//获取资源大小
	if(0==dwSize)
	{
		return FALSE;
	}//end if

	HGLOBAL hGlobal=::LoadResource(NULL,hsrc);//加载资源
	if(NULL==hGlobal)
	{
		return FALSE;
	}//end if

	LPBYTE p;
	p=(LPBYTE)GlobalAlloc(GPTR,dwSize);//分配内存
	if(NULL==p)
	{
		//AfxMessageBox(_T("内存分配失败"));
		return FALSE;
	}//end if

	::CopyMemory((LPVOID)p,(LPCVOID)::LockResource(hGlobal),dwSize);//复制数据到p
	GlobalUnlock(hGlobal);
	CString path;
	//构造文件路径
	if(bSubPathOfExe)//放在应用程序文件夹下
	{
		if(""==folder)//直接放在应用程序文件夹下
		{
			path.Format(_T("%s\\%s"),getExePath(),saveAsFileName);
		}
		else
		{
			path.Format(_T("%s\\%s\\%s"),getExePath(),folder,saveAsFileName);
		}//end if
	}
	else//放在自己选择的文件夹下
	{
		if(""==folder)
		{
			path.Format(_T("%s"),saveAsFileName);//此时包含文件夹和文件名
		}
		else
		{
			path.Format(_T("%s\\%s"),folder,saveAsFileName);
		}//end if
	}//end if
	CFile file(path,CFile::modeCreate|CFile::modeWrite);
	file.Write(p,dwSize);//写入
	file.Close();//关闭
	//释放资源
	UnlockResource(hGlobal);
	FreeResource(hGlobal);
	::GlobalFree((HGLOBAL)p);
	return TRUE;
}
//从注册表里初始化连续的选择(check.radio)
void CUtil::initMultiSelect(CDialog* dlg,INT startID,INT totalItem,CString regKey)
{
	CString temp=m_reg.readKey(regKey);
	CStringArray strarr;
	if(""!=temp)
	{
		splitString(temp,',',strarr);
		if(strarr.GetSize()>=totalItem)
		{
			for(int i=0;i<totalItem;i++)
			{
				if("1"==strarr.GetAt(i))
				{
					((CButton*)dlg->GetDlgItem(startID+i))->SetCheck(TRUE);
				}
				else
				{
					((CButton*)dlg->GetDlgItem(startID+i))->SetCheck(0);
				}
			}//end for
		}//end if
	}//end if
}//einitMultiSelect

//保存连续的选择到注册表里(check.radio)
void CUtil::saveMultiSelect(CDialog* dlg,INT startID,INT totalItem,CString regKey)
{
	CString temp=_T("");
	for(int i=0;i<totalItem;i++)
	{
		if(((CButton*)dlg->GetDlgItem(startID+i))->GetCheck())
		{
			temp+=_T("1,");
		}
		else
		{
			temp+=_T("0,");
		}
	}//end for
	m_reg.writeKey(regKey,temp);
}

//从注册表里初始化连续的选择(check.radio)
void CUtil::initMultiEdit(CDialog* dlg,INT startID,INT totalItem,CString regKey,CString defaultValues)
{
	CString temp=m_reg.readKey(regKey);
	CStringArray strarr;
	if(""!=temp)
	{
		splitString(temp,',',strarr);
		if(strarr.GetSize()>=totalItem)
		{
			for(int i=0;i<totalItem;i++)
			{
				dlg->GetDlgItem(startID+i)->SetWindowTextW(strarr.GetAt(i));
			}//end for
		}//end if
	}
	else if(""!=defaultValues)//未保存过，并且有默认值
	{
		splitString(defaultValues,',',strarr);
		if(strarr.GetSize()>=totalItem)
		{
			for(int i=0;i<totalItem;i++)
			{
				dlg->GetDlgItem(startID+i)->SetWindowTextW(strarr.GetAt(i));
			}//end for
		}//end if
	}
	//end if
}//einitMultiSelect

//得到多个连续的checkbox,radio选择了哪个，返回偏移第一个位置
INT CUtil::getMultiSelect(CDialog* dlg,UINT startID,INT totalItem)
{
	INT sele=0;
	for(int i=0;i<totalItem;i++)
	{
		if(((CButton*)dlg->GetDlgItem(startID+i))->GetCheck())
		{
			sele=i;
			break;
		}
	}//end for
	return sele;
}
//保存连续的选择到注册表里(check.radio)
void CUtil::saveMultiEdit(CDialog* dlg,INT startID,INT totalItem,CString regKey)
{
	CString temp=_T(""),values=_T("");
	for(int i=0;i<totalItem;i++)
	{
		dlg->GetDlgItem(startID+i)->GetWindowTextW(temp);
		values+=temp+_T(",");;
	}//end for
	m_reg.writeKey(regKey,values);
}
//在下拉选择事件中得到选择的文本
CString CUtil::getComboSelText(CComboBox* comboBox)
{
	CString temp;
	if(-1==comboBox->GetCurSel())
	{
		comboBox->GetWindowText(temp);
	}
	else
	{
		comboBox->GetLBText(comboBox->GetCurSel(),temp);
	}//end if
	return temp;
}
//重启程序
void CUtil::ReStart(BOOL bNormal) 
{ 
	PROCESS_INFORMATION   info; 
	STARTUPINFO startup; 
	TCHAR szPath[128]; 
	TCHAR *szCmdLine; 

	GetModuleFileName(AfxGetApp()-> m_hInstance,szPath,sizeof(szPath)); 
	szCmdLine= GetCommandLine(); 
	GetStartupInfo(&startup); 

	BOOL bSucc=CreateProcess(szPath,szCmdLine,NULL,NULL,FALSE,NORMAL_PRIORITY_CLASS,NULL, NULL,&startup,&info); 

	if(bNormal && bSucc) 
	{ 
		CWnd *pWnd= AfxGetMainWnd(); 

		if(pWnd != NULL) 
		{ 
			pWnd->PostMessage(WM_CLOSE,0,0); 
		} 
		else 
			ExitProcess(-1); 
	} 
	else 
		ExitProcess(-1); 
} 
void CUtil::setEditFloat(CDialog* dlg,UINT ID,FLOAT f,INT precision)
{
	CString temp;
	if(2==precision)
	{
		temp.Format(_T("%.2f"),f);
	}
	else
	{
		temp.Format(_T("%.3f"),f);
	}
	dlg->GetDlgItem(ID)->SetWindowTextW(temp);
}
//char*转化为CString
//#include <afxpriv.h>
CString CUtil::pCharToCString(char* pchar)
{
	///USES_CONVERSION;
	///return A2T(pchar);	
	//计算char *数组大小，以字节为单位，一个汉字占两个字节
	int charLen = strlen(pchar);
	//计算多字节字符的大小，按字符计算。
	int len = MultiByteToWideChar(CP_ACP,0,pchar,charLen,NULL,0);
	//为宽字节字符数组申请空间，数组大小为按字节计算的多字节字符大小
	TCHAR *buf = new TCHAR[len + 1];
	//多字节编码转换成宽字节编码
	///MultiByteToWideChar(CP_ACP,0,pchar,charLen,buf,len);
	MultiByteToWideChar(CP_ACP,0,pchar,charLen,buf,len);
	char m_pchar_zll;
	for(int i=0;i<len;i++)
	{
		m_pchar_zll=pchar[i];
	}
	buf[len] = '\0'; //添加字符串结尾，注意不是len+1
	//将TCHAR数组转换为CString
	CString pWideChar;
	pWideChar.Append(buf);
	//删除缓冲区
	delete []buf;
	return pWideChar;
}
//char*转化为CString
//#include <afxpriv.h>
CString CUtil::pTCharToCString(TCHAR* pchar)
{
	///USES_CONVERSION;
	///return A2T(pchar);	
	//计算char *数组大小，以字节为单位，一个汉字占两个字节
	int charLen = wcslen(pchar);
	//计算多字节字符的大小，按字符计算。
	int len = WideCharToMultiByte(CP_ACP,0,pchar,charLen,NULL,0,NULL,NULL);;//MultiByteToWideChar(CP_ACP,0,pchar,charLen,NULL,0);
	//为宽字节字符数组申请空间，数组大小为按字节计算的多字节字符大小
	char *buf = new char[len + 1];
	//多字节编码转换成宽字节编码
	///MultiByteToWideChar(CP_ACP,0,pchar,charLen,buf,len);
	//MultiByteToWideChar(CP_ACP,0,pchar,charLen,buf,len);
	CString ss;
	WideCharToMultiByte(CP_ACP,0,pchar,charLen,buf,len,NULL,NULL);;//MultiByteToWideChar(CP_ACP,0,pchar,charLen,NULL,0);
	buf[len] = '\0'; //添加字符串结尾，注意不是len+1
	//将TCHAR数组转换为CString
	CString pWideChar(buf);
	//pWideChar.Append(buf);
	//删除缓冲区
	//delete []buf;
	//return ss;
	return pWideChar;
}
//////////////////////////////////////////////////////////////////
//CString转化为char*
//返回实际字节长度，使用时pchar中该长度后的字符无效
//////////////////////////////////////////////////////////////////
INT CUtil::CStringToPChar(CString str, char* pchar)
{
	///USES_CONVERSION;
	///pchar=T2A(str);
	///CString str = _T("D:\\校内项目\\QQ.bmp");
	//注意：以下n和len的值大小不同,n是按字符计算的，len是按字节计算的
	int n = str.GetLength();     // n = 14, len = 18
	//获取宽字节字符的大小，大小是按字节计算的
	int len = WideCharToMultiByte(CP_ACP,0,str,str.GetLength(),NULL,0,NULL,NULL);
	//为多字节字符数组申请空间，数组大小为按字节计算的宽字节字节大小
	///char * pFileName = new char[len+1];   //以字节为单位
	//宽字节编码转换成多字节编码
	WideCharToMultiByte(CP_ACP,0,str,str.GetLength(),pchar,len,NULL,NULL);
	pchar[len+1] = '\0';   //多字节字符以'\0'结束
	return len;
}