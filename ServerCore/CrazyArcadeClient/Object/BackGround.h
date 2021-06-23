#pragma once
#include "stdafx.h"

class BackGround : public GameObject
{
public:
	BackGround();

	void HandleDying() override;
};

