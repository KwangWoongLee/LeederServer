#pragma once
#include "stdafx.h"

class SpriteComponent;

class PlayerClient : public Player, public std::enable_shared_from_this<PlayerClient>
{
public:
	PlayerClient();
	PlayerClient(GameObject obj);

	void Update() override;

private:
	std::shared_ptr<SpriteComponent>	mSpriteComponent;
};

