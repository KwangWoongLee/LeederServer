#include "stdafx.h"
#include "RenderManager.h"

void RenderManager::Render(SDL_Renderer* renderer)
{
	for (auto component : mComponents)
		component->Draw(renderer);
}

void RenderManager::AddComponent(SpriteComponent* component)
{
	int myDrawOrder = component->GetDrawOrder();
	auto iter = mComponents.begin();
	for (;
		iter != mComponents.end();
		++iter)
	{
		if (myDrawOrder < (*iter)->GetDrawOrder())
		{
			break;
		}
	}

	mComponents.insert(iter, component);
}

void RenderManager::RemoveComponent(SpriteComponent* component)
{
	auto iter = std::find(mComponents.begin(), mComponents.end(), component);
	mComponents.erase(iter);
}



