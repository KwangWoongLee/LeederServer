#include "stdafx.h"
#include "IOCPClient.h"

namespace leeder
{

	std::function < void(IOCPClient*)> ioWorkerThreadFunction = [](IOCPClient* iocp) {

		while (true)
		{
			ClientOverlapped* overlapped = nullptr;
			std::shared_ptr<Client> client = nullptr;
			DWORD			transferSize;

			BOOL ret = GetQueuedCompletionStatus(iocp->GetIOCP(), &transferSize, (PULONG_PTR)&client, (LPOVERLAPPED*)&overlapped, INFINITE);

			if (!ret)
			{
				return;
			}



			std::shared_ptr<ClientIOData> ioData = overlapped->GetIOData();

			client = (ioData == nullptr) ? nullptr : ioData->GetClient();

			if (client == nullptr)
			{
				return;
			}

			switch (ioData->GetType())
			{
			case eClientIOType::CONNECT:
				client->OnConnect();

				break;

			case eClientIOType::RECV:
			{	
				if (transferSize == 0)
				{
					client->Disconnect();
					continue;
				}

				std::shared_ptr<ClientPackage> package = client->OnRecv(transferSize);

				if (package)
					iocp->PutPackage(std::move(package));
				
				break;
			}
			
			case eClientIOType::SEND:
				if (transferSize == 0)
				{
					client->Disconnect();
					continue;
				}

				client->OnSend(transferSize);

				break;

			case eClientIOType::NONE:
				printf("Client Close by Client Error ");
				continue;

				break;

			default:
				break;
			
			}

		}
		
	};

	LPFN_CONNECTEX		mFnConnectEx = nullptr;



	IOCPClient::IOCPClient(std::unique_ptr<ClientContentsProcess>&& contents)
		:mContentsProcess(std::move(contents))
		, mThreadCount(5)
	{
		printf("Create IOCPClient");
	
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

	IOCPClient::~IOCPClient()
	{
		CloseHandle(mIOCP);
	}


	void IOCPClient::Init(XMLDocument* config)
	{

		XMLElement* app = config->FirstChildElement("App");

		XMLElement* server = app->FirstChildElement("Server");

		if (!server) {
			SysLogger::GetInstance().Log(L"No setting for server in config");
			return;
		}

		XMLElement* element = server->FirstChildElement("IP");
		mServerIP = element->GetText();

		element = server->FirstChildElement("Port");
		sscanf_s(element->GetText(), "%d", &mServerPort);

		// ConnectEx Image Create
		{
		DWORD bytes = 0;


		SOCKET tmpSocket = socket(AF_INET, SOCK_STREAM, 0);

		if (tmpSocket == INVALID_SOCKET)
		{
			//俊矾贸府;
			return;
		}

		GUID guidConnectEx = WSAID_CONNECTEX;
		if (SOCKET_ERROR == WSAIoctl(tmpSocket, SIO_GET_EXTENSION_FUNCTION_POINTER,
			&guidConnectEx, sizeof(GUID), &mFnConnectEx, sizeof(LPFN_CONNECTEX), &bytes, NULL, NULL))
		{	//俊矾贸府
			printf("error");
			return;
		}

		closesocket(tmpSocket);

		}

		if (!createCompletionPort())
		{
			//俊矾贸府
		}




	}

	void IOCPClient::Run()
	{
		for (size_t i = 0; i < GetThreadCount(); ++i) {
			mWorkThreadPool.push_back(std::make_unique<Thread>(ioWorkerThreadFunction, this));
		}

	}


	std::shared_ptr<Client> IOCPClient::CreateClient()
	{
		auto client = std::make_shared<Client>(mServerIP, mServerPort);
		
		if (!RegistCompletionPort(client->GetSocket(), (ULONG_PTR)&client))
		{
			//俊矾贸府
			printf("error : %d" , GetLastError() );
		}

		return client;
	}

	bool IOCPClient::createCompletionPort()
	{
		mIOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);

		if (mIOCP == NULL)
			return false;

		return true;
	}


	bool IOCPClient::RegistCompletionPort(SOCKET socket, ULONG_PTR key)
	{
		HANDLE handle = CreateIoCompletionPort((HANDLE)socket, mIOCP, key, 0);

		if (handle != mIOCP)
		{
			SysLogger::GetInstance().Log(L"Socket failed to regist for IOCP %d", GetLastError());

			return false;
		}
	}

	void IOCPClient::PutPackage(std::shared_ptr<ClientPackage>&& package)
	{
		mContentsProcess->PutPackage(std::forward<std::shared_ptr<ClientPackage>>(package));
	}


}