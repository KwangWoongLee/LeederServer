#include "stdafx.h"
#include "Server.h"


namespace leeder
{
	Server::Server()
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



		XMLDocument config;

		if (!loadConfig(&config)) {
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

		//TODO
		//ShutdownServer();

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
		sscanf_s(element->GetText(), "%d", &mPort);

		element = server->FirstChildElement("ThreadCount");
		sscanf_s(element->GetText(), "%d", &mThreadCount);
		

		SetState(eServerState::READY);

	}

}