#include "stdafx.h"
#include "Server.h"


namespace leeder
{
Server::Server(std::shared_ptr<ContentsProcess>&& contents)
	:mListenSocket(nullptr)
	, mContentsProcess(std::move(contents))
{
	SetState(eServerState::UN_INITIALIZE);

	// 윈속 초기화
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != NO_ERROR)
	{
		//에러처리
		return;
	}

	mListenSocket = std::make_shared<Socket>();

	XMLDocument config;

	const char* path = "./config.xml";

	if (!loadConfig(&config, path)) 
	{
		printf("Server Config Reading is failed\n");
		exit(0);
		return;
	}

	this->Init(&config);


}
Server::~Server()
{
	SetState(eServerState::STOP);


	WSACleanup();

	ShutDownServer();

}
void Server::Init(XMLDocument* config)
{
	XMLElement* app = config->FirstChildElement("App");

	mName = app->FirstChildElement("Name")->GetText();

	XMLElement* server = 	app->FirstChildElement("Server");

	if (!server) {
		SysLogger::GetInstance().Log(L"No setting for server in config");
		return;
	}

	XMLElement* element = server->FirstChildElement("IP");
	mIP = element->GetText();

	element = server->FirstChildElement("Port");
	std::string strPort = element->GetText();
	mPort = std::stoi(strPort);

	element = server->FirstChildElement("ThreadCount");
	std::string strThreadCount = element->GetText();
	mThreadCount = std::stoi(strThreadCount);


	SetState(eServerState::READY);

}

void Server::PutPackage(std::shared_ptr<Package>&& package)
{
	mContentsProcess->PutPackage(std::forward<std::shared_ptr<Package>>(package));
}

}