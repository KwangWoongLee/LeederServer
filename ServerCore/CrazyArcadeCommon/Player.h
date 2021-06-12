#pragma once
#include "Common.h"

class Player : public leeder::GameObject
{
public:
	Player(uint32_t sessionID)
		: mPlayerId(sessionID)
	{
		SetPosition({leeder::GetRandomFloat() * 1024 , leeder::GetRandomFloat() * 512 });
	};

	void Update() override;

	void	AddInput(Input input);

	uint32_t	GetPlayerId()			const { return mPlayerId; }

private:
	uint32_t			mPlayerId;

	std::deque<Input>			mInputList;
};

