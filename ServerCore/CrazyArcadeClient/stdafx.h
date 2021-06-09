#pragma once
#include "stdafx.h"

#define WIN32_LEAN_AND_MEAN    

//미리 컴파일된 헤더

//SDL 64 bit
#pragma comment(lib, "SDL2.lib")
#pragma comment(lib, "SDL2main.lib")
#pragma comment(lib, "SDL2_image.lib")

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_scancode.h>

#include "ServerLibrary.h"

using leeder::Singleton;
using leeder::Thread;
using leeder::ThreadSafeQueue;

using leeder::eInputType;
using leeder::GameObject;
using leeder::Packet;
using leeder::ePacketType;


using leeder::XMLDocument;
using leeder::XMLElement;
using leeder::BUF_SIZE;

using leeder::loadConfig;



//#include "ClientShutDown.h"


#include "Util/Input.h"
#include "Util/InputManager.h"

#include "Network/NetworkManager.h"

using leeder::NetworkManager;



#include "Core/Client.h"
