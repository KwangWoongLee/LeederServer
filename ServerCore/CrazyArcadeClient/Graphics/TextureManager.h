#pragma once
#include "stdafx.h"

class TextureManager : public Singleton< TextureManager >
{

public:
	void Init(SDL_Renderer* renderer);

	bool		PutTexture(SDL_Renderer* renderer, const std::string& name, const std::string& fileName);
	
	SDL_Texture* GetTexture(const std::string& inTextureName);

	
private:
	std::unordered_map<std::string, SDL_Texture*> mNameToTextures;

};

