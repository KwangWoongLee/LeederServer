#include "stdafx.h"


int _tmain(int argc, _TCHAR* argv[])
{

	using namespace leeder;

	std::unique_ptr<IOCPServer> CrazyArcadeServer = std::make_unique<IOCPServer>(std::make_unique<GameProcess>());

	CrazyArcadeServer->Run();

	Sleep(100000000);

	return 0;
}