#pragma once
#include "stdafx.h"

class SpriteComponent;

class RenderManager : public Singleton<RenderManager>
{

public:
	void Render(SDL_Renderer* renderer);

	//vert inefficient method of tracking scene graph...
	void AddComponent(SpriteComponent* component);
	void RemoveComponent(SpriteComponent* component);

private:
	std::vector<SpriteComponent*> mComponents;
};

