#pragma once

class Command;
class CCommandManager;

class CRemoteSettingCommand :
	public Command
{
public:
	CRemoteSettingCommand(CCommandManager* pCommandManager);
	~CRemoteSettingCommand(void);
	virtual void Execute(); 
};
