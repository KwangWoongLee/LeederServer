#include "stdafx.h"


int _tmain(int argc, _TCHAR* argv[])
{
	using namespace leeder;

	std::unique_ptr<IOCPServer> LoginServer = std::make_unique<IOCPServer>(std::make_shared<LoginProcess>());

	LoginServer->Run();
}