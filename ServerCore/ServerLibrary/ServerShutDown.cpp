#include "stdafx.h"
#include "ServerShutDown.h"

namespace leeder
{

bool bShutDown = false;

void ShutDownServer()
{
	SysLogger::GetInstance().Log(L"Server Shutdown");
	bShutDown = true;
}
}

