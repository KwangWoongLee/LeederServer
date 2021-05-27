#include "stdafx.h"
#include <crtdbg.h>

void a()
{};

int _tmain(int argc, _TCHAR* argv[])
{



	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	//_CrtSetBreakAlloc(660);

	using namespace leeder;

	std::unique_ptr<IOCPServer> loginServer = std::make_unique<IOCPServer>(std::make_unique<ChattingProcess>());

	loginServer->Run();



	return 1;
}