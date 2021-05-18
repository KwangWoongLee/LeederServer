#pragma once
#include "stdafx.h"

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "mswsock.lib")
#pragma comment(lib, "Winmm.lib")
#pragma comment(lib, "dbghelp.lib")

#include <Ws2tcpip.h>
#include <winsock2.h>
#include <mswsock.h>
#include <Mmsystem.h>
#include <Ws2spi.h>
#include <Mstcpip.h>

#include <windows.h>
#include <iostream>
#include <io.h>
#include <cstdlib>
#include <cstdio>
#include <cstdint>

#include <chrono>

#include <assert.h> 
#include <fcntl.h>
#include <algorithm>  
#include <functional>

#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <memory>

#include <string>
#include <tchar.h>
#include <atlconv.h>
#include <atlbase.h>


#include <random>
#include <typeinfo>    //typeinfo

#include <map>
#include <queue>
#include <unordered_map>
#include <list>
#include <array>

//xml library
#include "Util/tinyxml/tinyxml.h"


// Util Header


#include "Util/Type.h"
#include "Util/StringUtil.h"

#include "Util/Singleton.h"
#include "Util/Clock.h"

#include "Util/Logger.h"

#include "Util/MemoryLeakDetector.h"
#include "Util/Minidump.h"
#include "Util/Assert.h"

#include "Util/Config.h"


#include "Util/Thread.h"

#include "Util/Socket.h"



//Network
#include "Network/Stream.h"
#include "Network/Overlapped.h"

#include "Network/Session.h"
#include "Network/IOCPSession.h"
#include "Network/SessionManager.h"

#include "Network/Server.h"
#include "Network/IOCPServer.h"