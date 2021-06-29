#pragma once
#include "stdafx.h"

class TileServer : public Tile
{
public:
	TileServer()
		:Tile()
		, mCollider(this)
	{
		SetScale(2.0f);
		mCollider.SetRadius(16.f * GetScale());
	};

	void HandleDying() override;

	void Update(float deltaTime) override;


	Collider& GetCollider() { return mCollider; }

private:
	Collider mCollider;

	void createItem();
};

