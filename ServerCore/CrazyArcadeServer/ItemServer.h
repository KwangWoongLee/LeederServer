#pragma once
#include "stdafx.h"

class ItemServer : public Item
{
public:
	ItemServer();

	void HandleDying() override;

	Collider& GetCollider() { return mCollider; }


private:
	Collider mCollider;

};

