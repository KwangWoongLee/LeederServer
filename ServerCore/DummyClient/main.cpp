#include "stdafx.h"

int _tmain(int argc, _TCHAR* argv[])
{
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != NO_ERROR)
	{
		printf("WSA Start Error : %d", WSAGetLastError());
	}
	
	using namespace leeder;



	std::unique_ptr<IOCPClient> DummyTestClient = std::make_unique<IOCPClient>(std::make_unique<DummyTestClientProcess>());

	DummyTestClient->Run();

	std::vector<std::shared_ptr<Client>> clients;
	for(int i = 0; i< 10 ; ++i)
		clients.push_back(DummyTestClient->CreateClient());


	for (auto client : clients)
	{
		client->Connect();
	}

	Sleep(1000);

	auto mThread = Thread([&]() {
		while (true)
		{
			for (auto client : clients)
			{
				if (client->GetState() == eClientState::WELCOMED)
				{
					client->Action();

					client->SendHeartBeat();

					//client->SendRequestReplication();
				}
			}
			Sleep(100);
		}
		
		});
	


	



	Sleep(10000000000000000);

    return 0;


};