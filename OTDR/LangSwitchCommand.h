#pragma once

class Command;
class CCommandManager;

class CLangSwitchCommand :
	public Command
{
public:
	CLangSwitchCommand(CCommandManager* pCommandManager);
	~CLangSwitchCommand(void);
	virtual void Execute(); 
};
