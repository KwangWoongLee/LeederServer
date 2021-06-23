#pragma once
#include "stdafx.h"

class TileClient : public Tile
{
public:
	TileClient();

	void HandleDying() override;

	void Update(float deltaTime) override;

private:
	TileComponent* mTileComponent;
};

