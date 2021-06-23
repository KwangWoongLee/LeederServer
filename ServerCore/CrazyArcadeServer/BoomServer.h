#pragma once
#include "stdafx.h"

class BoomServer : public Boom
{
public:
	BoomServer(GameObject* owner);

	void Update(float deltaTime) override;

	void HandleDying() override;

	void SetLifeTime(float time) { mLifeTime = time; }


private:
	GameObject* mOwner;

	float				mLifeTime;

};

