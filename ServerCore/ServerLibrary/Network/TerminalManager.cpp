#include "stdafx.h"
#include "TerminalManager.h"
namespace leeder
{
TerminalManager::TerminalManager()
	:mServer(nullptr)
{
}

TerminalManager::~TerminalManager()
{
}

void TerminalManager::Init(Server* server, XMLDocument* config)
{
	mServer = server;

	XMLElement* terminal = config->FirstChildElement("App")->FirstChildElement("Terminal");

	if (!terminal) {
		SysLogger::GetInstance().Log(L"No setting for terminal in config");
		return;
	}


	XMLNode* node = terminal->FirstChildElement();
	while (node) {
		auto terminalServer = node;
		std::string terminalName = terminalServer->Value();

		XMLElement* element = terminalServer->FirstChildElement("IP");
		std::string strIP = element->GetText();

		element = terminalServer->FirstChildElement("Port");
		std::string strPort = element->GetText();


		auto terminal = std::make_shared<Terminal>(mServer, terminalName);
		terminal->SetIP(strIP);
		terminal->SetPort(std::stoi(strPort));

		this->put(terminalName, std::move(terminal));

		node = node->NextSiblingElement();
	}


}

void TerminalManager::Run()
{
	for (auto& element : mStringToTerminal)
	{
		auto& terminal = element.second;
		terminal->Run();
	}
}
void TerminalManager::put(std::string terminalName, std::shared_ptr<Terminal>&& terminal)
{
	mStringToTerminal[terminalName] = std::move(terminal);
}

Terminal* TerminalManager::get(std::string terminalName)
{
	auto iter = mStringToTerminal.find(terminalName);
	if (iter != mStringToTerminal.end())
		return iter->second.get();

	return nullptr;
}
}
