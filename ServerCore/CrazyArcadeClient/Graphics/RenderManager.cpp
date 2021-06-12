#include "stdafx.h"
#include "RenderManager.h"

void RenderManager::Render(SDL_Renderer* renderer)
{
	for (auto component : mComponents)
		component->Draw(renderer);
}

void RenderManager::AddComponent(SpriteComponent* component)
{
	mComponents.push_back(component);
}

void RenderManager::RemoveComponent(SpriteComponent* component)
{
	auto iter = mComponents.begin();
	auto iterEnd = mComponents.end();

	for(;iter != iterEnd; ++iter)
		if ((*iter) == component)
		{
			mComponents.erase(iter);
			break;
		}
}

