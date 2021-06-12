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

#include "Common.h"


//#include "ClientShutDown.h"


#include "Util/Input.h"
#include "Util/InputManager.h"

#include "Network/NetworkManager.h"

using leeder::NetworkManager;

#include "Graphics/RenderManager.h"
#include "Graphics/TextureManager.h"

#include "Components/SpriteComponent.h"
#include "Components/TileMapComponent.h"
#include "Components/AnimationComponent.h"




#include "Core/Client.h"
