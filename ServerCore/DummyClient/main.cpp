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




	std::unique_ptr<IOCPClient> DummyTestClient = std::make_unique<IOCPClient>(std::make_shared<DummyTestClientProcess>());

	std::vector<std::shared_ptr<Client>> clients;
	for (int i = 0; i < 100; ++i)
		clients.push_back(DummyTestClient->CreateClient());


	DummyTestClient->Run();


	for (auto client : clients)
	{
		client->Connect();
	}

	NetworkManager::GetInstance().Init(DummyTestClient->GetProcess());

	//단일 로직 스레드
	while (!bShutDown)
	{
		NetworkManager::GetInstance().ProcessQueuedPacket();

		for (auto client : clients)
		{
			auto time = Clock::GetInstance().GetSystemTimeFloat();

			if (client->GetState() == eClientState::WELCOMED)
			{
				if (time > client->mLastPacketSendTime + 0.000003f)
				{
					client->SendInputPacket();
					client->mLastPacketSendTime = time;

				}

				//HeartBeat 패킷 전송
				if (time > client->mLastHeartBeatTime + 0.003f)
				{
					client->SendHeartBeat();
					client->mLastHeartBeatTime = time;
				}

			}

			}
		}

	return 0;

};