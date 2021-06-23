#pragma once
#include "stdafx.h"

class BombServer : public Bomb
{
public:
	BombServer(GameObject* owner);

	void Update(float deltaTime) override;

	void HandleDying() override;

	Collider& GetCollider() { return mCollider; }

	bool	IsFirstContact() { return mFirstContact; }
	void	SetContact() { mFirstContact = false; }



private:
	void createBoom();
	Position searchBoomPosition(int dx, int dy);

	GameObject* mOwner;

	Collider mCollider;
	float				mLifeTime;
	bool				mFirstContact;




};

