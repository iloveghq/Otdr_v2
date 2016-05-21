#pragma once

#define CONFIG_FILE _T("\\Config\\Config.ini")
#define CHS_FILE _T("\\Config\\Multi_ch.ini")
#define ENG_FILE _T("\\Config\\Multi_eng.ini")
#define PORT_FILE _T("\\Config\\Multi_port.ini")

class IniUtil
{
public:
	IniUtil(void);
	~IniUtil(void);

	static CString GetExePath();//获取文件运行路径
	static void ReadLangSetting();
	static CString GetLangFilePath();
	static CString GetConfigFilePath();
	static bool SaveLangSetting();
	static CString ReadSingleConfigInfo(const CString& strSection, const CString strKey);
	static bool WriteSingleConfigInfo(const CString& strSection, const CString& strKey, const CString& strVal);
	static CString ReadResString(const CString strKey);
};
