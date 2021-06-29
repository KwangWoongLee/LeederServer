#pragma once
#include "stdafx.h"

class SpriteComponent;

class ItemClient : public Item
{
public:
	ItemClient(eObjectType type);
	~ItemClient();

	void Update(float deltaTime) override;

private:
	AnimationComponent* mAnimationComponent;
};

