#include "StdAfx.h"
#include "Command.h"
#include "LangSwitchCommand.h"
#include "CommandManager.h"
#include "SorFileInfo.h"
#include "MainFrm.h"

extern LANG g_currentLang;

CLangSwitchCommand::CLangSwitchCommand(CCommandManager* pCommandManager) : Command
	(pCommandManager)
{
	m_CommandID = Commands::LANG_SWITCH;
}

CLangSwitchCommand::~CLangSwitchCommand(void)
{
}

void CLangSwitchCommand::Execute()
{
	//修改Frame字体
	CMainFrame *pMain = MAIN;
	if (pMain)
	{
		pMain->SetLocaleString();

		//保存语言设置
		IniUtil::SaveLangSetting();
	}
}
