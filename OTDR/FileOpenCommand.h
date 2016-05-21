#pragma once

class Command;
class CCommandManager;

class CFileOpenCommand :
	public Command
{
public:
	CFileOpenCommand(CCommandManager* pCommandManager);
	~CFileOpenCommand(void);
	virtual void Execute(); 
};
