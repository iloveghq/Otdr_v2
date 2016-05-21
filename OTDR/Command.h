#pragma once

class CCommandManager;

class Command
{
public:
	Command(void);
	Command(CCommandManager* pCommandManager);
	virtual ~Command(void);
	virtual void Execute(); 
	
public:
	int m_CommandID;
	CCommandManager* m_pCommandManager;
};
