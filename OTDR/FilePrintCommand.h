#pragma once

class Command;
class CCommandManager;

class CFilePrintCommand :
	public Command
{
public:
	CFilePrintCommand(CCommandManager* pCommandManager);
	~CFilePrintCommand(void);
	virtual void Execute(); 
};
