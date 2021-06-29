#pragma once
#include "stdafx.h"

class TileServer;

class PlayerServer : public Player
{
public:
	PlayerServer(uint32_t sessionID);

	void Update(float deltaTime) override;

	void HandleDying() override;

	void CreateBomb();

	Collider& GetCollider() { return mCollider; }

	int	GetBombCount() { return mBombCount; }
	void SetBombCount(int count) { mBombCount = count; }

private:
	Position searchBombPosition();

	float BirthPosition[4][2] = { { 160.f, 32.f} ,{96.f , 608.f} , { 672.f , 96.f } , { 672, 608.f } };
	Collider mCollider;
	int		mBombCount;
	int		mMaxBombCount;
	float	mDeathTimer;


};

