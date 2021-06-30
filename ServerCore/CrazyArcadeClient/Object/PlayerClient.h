#pragma once
#include "stdafx.h"

class AnimationComponent;

class PlayerClient : public Player
{
public:
	PlayerClient();

	void HandleDying() override;

	bool IsStop();

	void Update(float deltaTime) override;

private:
	AnimationComponent* mAnimationComponent;

};

