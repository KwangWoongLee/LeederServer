#include "stdafx.h"
#include "TextureManager.h"

void TextureManager::Init(SDL_Renderer* renderer)
{
	PutTexture(renderer, "BackGround", "./Assets/bg.bmp");
	PutTexture(renderer, "BazzyIdle", "./Assets/idle.bmp");
	PutTexture(renderer, "BazzyDown", "./Assets/down.bmp");
	PutTexture(renderer, "BazzyLeft", "./Assets/left.bmp");
	PutTexture(renderer, "BazzyRight", "./Assets/right.bmp");
	PutTexture(renderer, "BazzyUp", "./Assets/up.bmp");
	PutTexture(renderer, "BazzyBubble", "./Assets/bazziBubble.bmp");
	PutTexture(renderer, "BazzyDie", "./Assets/bazziDie.bmp");
	PutTexture(renderer, "Bomb", "./Assets/bomb.bmp");
	PutTexture(renderer, "BoomCenter", "./Assets/boom_center.bmp");
	PutTexture(renderer, "BoomLeft", "./Assets/boom_left.bmp");
	PutTexture(renderer, "BoomUp", "./Assets/boom_up.bmp");
	PutTexture(renderer, "BoomRight", "./Assets/boom_right.bmp");
	PutTexture(renderer, "BoomDown", "./Assets/boom_down.bmp");
	PutTexture(renderer, "Tile", "./Assets/Tiles1.bmp");



}

bool TextureManager::PutTexture(SDL_Renderer* renderer, const std::string& name, const std::string& fileName)
{
	SDL_Texture* tex = nullptr;


	auto iter = mNameToTextures.find(name);
	if (iter != mNameToTextures.end())
	{
		tex = iter->second;
	}
	else
	{
		SDL_Surface* surf = IMG_Load(fileName.c_str());
		if (!surf)
		{
			SDL_Log("Failed to load texture file %s", fileName.c_str());
			return false;
		}

		//컬러키 적용
		SDL_SetColorKey(surf, SDL_TRUE, SDL_MapRGB(surf->format, 255, 0, 255));


		tex = SDL_CreateTextureFromSurface(renderer, surf);
		SDL_FreeSurface(surf);
		if (!tex)
		{
			SDL_Log("Failed to convert surface to texture for %s", fileName.c_str());
			return false;
		}

		mNameToTextures[name.c_str()] =  tex;
	}

	return true;
}

SDL_Texture* TextureManager::GetTexture(const std::string& inTextureName)
{
    return mNameToTextures[inTextureName];;
}

