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

	int	GetPower() { return mPower; }
	void SetPower(int power) { mPower = power; }

private:
	Position searchBombPosition();

	float BirthPosition[4][2] = { { 160.f + 24.f, 32.f + 53.f} ,{96.f + 24.f , 608.f + 53.f} , { 672.f +24.f , 96.f + 53.f } , { 736.f +24.f, 608.f + 53.f } };
	Collider mCollider;
	int		mPower;
	float	mDeathTimer;


};

