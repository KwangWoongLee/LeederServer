#include "stdafx.h"
#include <crtdbg.h>


int _tmain(int argc, _TCHAR* argv[])
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	//_CrtSetBreakAlloc(1726);

	using namespace leeder;

	std::unique_ptr<IOCPServer> DummyTestServer = std::make_unique<IOCPServer>(std::make_shared<DummyTestProcess>());

	DummyTestServer->Run();


	NetworkManager::GetInstance().Init(DummyTestServer->GetProcess());


	//단일 로직 스레드
	while (!bShutDown)
	{
		NetworkManager::GetInstance().ProcessQueuedPacket();

		World::GetInstance().Update();
			
		NetworkManager::GetInstance().Replication();
	}



	return 0;
}