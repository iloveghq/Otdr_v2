#pragma once

class Command;
class CCommandManager;

class CEventAnalConCommand :
	public Command
{
public:
	CEventAnalConCommand(CCommandManager* pCommandManager);
	~CEventAnalConCommand(void);
	virtual void Execute(); 
};
