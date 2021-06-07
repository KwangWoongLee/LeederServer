#pragma once
#include "stdafx.h"

#define WIN32_LEAN_AND_MEAN    

//미리 컴파일된 헤더

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "mswsock.lib")
#pragma comment(lib, "Winmm.lib")
#pragma comment(lib, "dbghelp.lib")

#pragma comment(lib, "SDL2.lib")
#pragma comment(lib, "SDL2main.lib")
#pragma comment(lib, "SDL2_image.lib")

#include <SDL.h>

#define NOMINMAX

#include <Ws2tcpip.h>
#include <winsock2.h>
#include <mswsock.h>
#include <Mmsystem.h>
#include <Ws2spi.h>
#include <Mstcpip.h>

#include <windows.h>
#include <iostream>

#include "tchar.h"



#include "ServerLibrary.h"

#include "ClientOverlapped.h"

#include "ClientPackage.h"

#include "Client.h"

#include "ClientContentsProcess.h"

#include "DummyTestClientProcess.h"

#include "IOCPClient.h"
