#pragma once

class Command;
class CCommandManager;

class CFileSaveCommand :
	public Command
{
public:
	CFileSaveCommand(CCommandManager* pCommandManager);
	~CFileSaveCommand(void);
	virtual void Execute(); 
};
