#pragma once
#include "stdafx.h"

class PlayerServer : public Player
{
public:
	PlayerServer(uint32_t sessionID)
		:Player()
	{
		SetPlayerID(sessionID);
		SetPosition({ leeder::GetRandomFloat() * 1024 , leeder::GetRandomFloat() * 512 });
	};

	void Update() override;

private:

};

