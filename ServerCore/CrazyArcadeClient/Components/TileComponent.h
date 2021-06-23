#pragma once
#include "stdafx.h"
class TileComponent : public AnimationComponent
{
public:
	TileComponent(GameObject* owner, int drawOrder = 200);
	
	void Update(float deltaTime) override;

	void Draw(SDL_Renderer* renderer) override;
};

