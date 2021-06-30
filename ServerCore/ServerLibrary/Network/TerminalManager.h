#pragma once
#include "stdafx.h"

namespace leeder
{
class TerminalManager : public Singleton<TerminalManager>
{
public:
	TerminalManager();
	~TerminalManager();

	void Init(Server* server , XMLDocument* config);
	void Run();



private:
	Server* mServer;
	std::unordered_map<std::string, std::shared_ptr<Terminal>> mStringToTerminal;


	void put(std::string terminalName, std::shared_ptr<Terminal>&& terminal);
	Terminal* get(std::string terminalName);
};

}

