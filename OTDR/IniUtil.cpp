#include "StdAfx.h"
#include "IniUtil.h"
#include "atlbase.h"

LANG g_currentLang;

//可执行文件路径
CString s_strExePath = IniUtil::GetExePath();

IniUtil::IniUtil(void)
{
}

IniUtil::~IniUtil(void)
{
}

CString IniUtil::GetExePath()//获取文件运行路径
{
	CString sPath;
	GetModuleFileName(NULL,sPath.GetBufferSetLength(MAX_PATH+1),MAX_PATH);
	sPath.ReleaseBuffer();
	int nPos;
	nPos=sPath.ReverseFind('\\');
	sPath=sPath.Left(nPos);
	return sPath;
}

/******************************************************************************/
/*    0:chinese config file, 1:english config file , 2:portugal config file   */
/****************************************************************************/

CString IniUtil::GetLangFilePath()
{
	CString strFilePath;
	int nCurLang = (int)g_currentLang;
	if (nCurLang == 0)
		strFilePath.Format(_T("%s%s"),s_strExePath,  CHS_FILE);
	else if (nCurLang == 1)
		strFilePath.Format(_T("%s%s"),s_strExePath,  ENG_FILE);
	else if(nCurLang == 2)  //lzy 2014.06.06 葡萄牙语
		strFilePath.Format(_T("%s%s"),s_strExePath,  PORT_FILE);

	return strFilePath;
}

CString IniUtil::GetConfigFilePath()
{
	CString strFilePath;
	strFilePath.Format(_T("%s%s"),s_strExePath,  CONFIG_FILE);

	return strFilePath;
}

void IniUtil::ReadLangSetting()
{
	CString strLang = ReadSingleConfigInfo(_T("Language"), _T("LanguageOpt"));

	if(strLang.CompareNoCase(_T("0")) == 0)  //设为中文
		g_currentLang = LANG::CHS;

	else if (strLang.CompareNoCase(_T("1")) == 0) //设为英语
		g_currentLang = LANG::ENG;

	else if (strLang.CompareNoCase(_T("2")) == 0) //设为葡萄牙语
		g_currentLang = LANG::PORT;
	
}

bool IniUtil::SaveLangSetting()
{
	CString strLang;
	strLang.Format(_T("%d"), (int)g_currentLang);
	return WritePrivateProfileString (_T("Language"), _T("LanguageOpt"), strLang, GetConfigFilePath());
}

CString IniUtil::ReadSingleConfigInfo(const CString& strSection, const CString strKey)
{
	CString strVal;
	GetPrivateProfileString(strSection, strKey, _T(""), strVal.GetBuffer(MAX_PATH), MAX_PATH, GetConfigFilePath());
	strVal.ReleaseBuffer();

	return strVal;
}

bool IniUtil::WriteSingleConfigInfo(const CString& strSection, const CString& strKey, const CString& strVal)
{
	return WritePrivateProfileString (strSection, strKey, strVal, GetConfigFilePath());
}

CString IniUtil::ReadResString(const CString strKey)
{
	CString strConfigFile = GetLangFilePath();
	CString strVal;
	GetPrivateProfileString(_T("String"), strKey, _T(""), strVal.GetBuffer(MAX_PATH), MAX_PATH, strConfigFile);
	strVal.ReleaseBuffer();

	return strVal;
}

