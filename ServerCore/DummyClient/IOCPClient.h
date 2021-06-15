#pragma once
#include "stdafx.h"

class IOCPClient
{
public:
	IOCPClient(std::shared_ptr<ClientContentsProcess>&& contents);
	~IOCPClient();


	void Init(XMLDocument* config);
	void Run(); // 순수 가상함수 처리. 

	HANDLE& GetIOCP() { return mIOCP; }


	bool			RegistCompletionPort(SOCKET socket, ULONG_PTR key);

	void			PutPackage(std::shared_ptr<ClientPackage>&& package);

	std::shared_ptr<ClientContentsProcess>& GetProcess() { return mContentsProcess; }

	std::string		GetIP() const { return mServerIP; }
	void			SetIP(std::string ip) { mServerIP = ip; }

	uint16_t		GetPort() const { return mServerPort; }
	void			SetPort(uint16_t port) { mServerPort = port; }

	size_t			GetThreadCount() const { return mThreadCount; }
	void			SetThreadCount(size_t threadCount) { mThreadCount = threadCount; }

	std::shared_ptr<Client>	CreateClient();


private:
	bool			createCompletionPort();



	HANDLE									mIOCP;
	SOCKADDR_IN								mServerInfo;
	std::string								mServerIP;
	uint16_t								mServerPort;
	size_t									mThreadCount;
	std::vector<std::unique_ptr<Thread>>	mWorkThreadPool;

	std::shared_ptr<ClientContentsProcess>		mContentsProcess;


};



