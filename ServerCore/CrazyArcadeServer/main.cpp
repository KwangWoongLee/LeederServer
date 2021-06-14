#include "stdafx.h"


int _tmain(int argc, _TCHAR* argv[])
{

	using namespace leeder;

	std::unique_ptr<IOCPServer> CrazyArcadeServer = std::make_unique<IOCPServer>(std::make_shared<GameProcess>());

	CrazyArcadeServer->Run();


	NetworkManager::GetInstance().Init(CrazyArcadeServer->GetProcess());

	//auto start = std::chrono::high_resolution_clock::now();

	//auto finish = std::chrono::high_resolution_clock::now();

	//std::chrono::duration<double, std::milli> elapsed = finish - start;
	//
	//elapsed.count()

	//단일 로직 스레드
	while (!bShutDown)
	{
		NetworkManager::GetInstance().ProcessQueuedPacket();
		
		World::GetInstance().Update();

		NetworkManager::GetInstance().Replication();


	}

	return 0;
}