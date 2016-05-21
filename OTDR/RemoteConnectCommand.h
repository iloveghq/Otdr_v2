#pragma once

class Command;
class CCommandManager;

class CRemoteConnectCommand :
	public Command
{
public:
	CRemoteConnectCommand(CCommandManager* pCommandManager);
	~CRemoteConnectCommand(void);
	virtual void Execute(); 
};
