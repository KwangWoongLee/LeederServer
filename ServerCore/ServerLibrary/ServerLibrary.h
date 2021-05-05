#pragma once
#include "stdafx.h"

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "mswsock.lib")
#pragma comment(lib, "Winmm.lib")

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
#include <stdio.h>
#include <cstdint>

#include <assert.h> 
#include <fcntl.h>
#include <algorithm>  
#include <functional>

#include <thread>
#include <mutex>
#include <memory>

#include <string>
#include <tchar.h>
#include <atlconv.h>
#include <atlbase.h>

#include <ctime>
#include <random>
#include <typeinfo>    //typeinfo

#include <map>
#include <unordered_map>
#include <list>
#include <array>

//xml library
#include "Util/tinyxml/tinyxml.h"


#include "Util/Type.h"
#include "Util/StringUtil.h"

#include "Util/Config.h"

#include "Util/Singleton.h"
#include "Util/Logger.h"
