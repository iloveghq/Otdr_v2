#pragma once
#include <map>

class Command;

class CCommandManager
{
public:
	CCommandManager();

	~CCommandManager(void);
	void CreateCommands();
	void DeleteCommands();
	Command* GetCommandByID(int commandID);

private:
	std::map<const int, Command*> m_mapCommands;
};
