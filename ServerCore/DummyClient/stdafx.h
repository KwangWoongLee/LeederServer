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

using leeder::Singleton;
using leeder::Thread;
using leeder::ThreadSafeQueue;
using leeder::Clock;

using leeder::Session;
using leeder::IOCPSession;

using leeder::eInputType;
using leeder::Input;


using leeder::eObjectState;
using leeder::eObjectType;
using leeder::ObjectInfo;

using leeder::GameObject;
using leeder::Packet;
using leeder::ePacketType;


using leeder::Package;

using leeder::ContentsProcess;

using leeder::OutputStream;
using leeder::InputStream;


using leeder::XMLDocument;
using leeder::XMLElement;
using leeder::BUF_SIZE;

using leeder::loadConfig;

#include "ClientOverlapped.h"

#include "ClientPackage.h"

#include "Client.h"


#include "ClientContentsProcess.h"


#include "NetworkManager.h"


#include "DummyTestClientProcess.h"

#include "IOCPClient.h"
