#include "stdafx.h"

#include "ServerLibrary.h"

int _tmain(int argc, _TCHAR* argv[])
{
	using namespace leeder;

	std::unique_ptr<IOCPServer> loginServer = std::make_unique<IOCPServer>();

	loginServer->Run();


	return 0;
}