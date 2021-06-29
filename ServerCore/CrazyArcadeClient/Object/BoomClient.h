#pragma once
#include "stdafx.h"

class AnimationComponent;

class BoomClient : public Boom
{
public:
	BoomClient();
	~BoomClient();

	void HandleDying() override;

	void Update(float deltaTime) override;

private:
	AnimationComponent* mAnimationComponent;

};

