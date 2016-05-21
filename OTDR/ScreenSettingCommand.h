#pragma once

class Command;
class CCommandManager;

class CScrenSettingCommand :
	public Command
{
public:
	CScrenSettingCommand(CCommandManager* pCommandManager);
	~CScrenSettingCommand(void);
	virtual void Execute(); 
};
