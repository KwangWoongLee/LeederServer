#pragma once
#include "stdafx.h"

class AnimationComponent;

class BombClient : public Bomb
{
public:
	BombClient();
	~BombClient();

	void HandleDying() override;

	void Update(float deltaTime) override;

private:
	AnimationComponent* mAnimationComponent;

};

