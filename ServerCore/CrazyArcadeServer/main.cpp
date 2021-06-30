#include "stdafx.h"


int _tmain(int argc, _TCHAR* argv[])
{


	auto server = std::make_unique<Server>();

	server->Init();

	server->Run();


	return 0;
}

