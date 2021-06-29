#pragma once
#include "stdafx.h"

class SpriteComponent;

class RenderManager : public Singleton<RenderManager>
{

public:
	void Init(SDL_Renderer* renderer);

	void Render(SDL_Renderer* renderer);

	SDL_Renderer* GetRenderer() { return mRenderer; }

	//vert inefficient method of tracking scene graph...
	void AddComponent(SpriteComponent* component);
	void RemoveComponent(SpriteComponent* component);

private:
	std::vector<SpriteComponent*> mComponents;
	SDL_Renderer* mRenderer;
};

