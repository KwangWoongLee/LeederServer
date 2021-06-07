#include "stdafx.h"
#include <crtdbg.h>


int _tmain(int argc, _TCHAR* argv[])
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	//_CrtSetBreakAlloc(1726);

	using namespace leeder;

	std::unique_ptr<IOCPServer> DummyTestServer = std::make_unique<IOCPServer>(std::make_unique<DummyTestProcess>());

	DummyTestServer->Run();

	while (true)
	{
		UserManager::GetInstance().UpdateUsers();

		UserManager::GetInstance().Replication();

		Sleep(100);
	}


	return 0;
}