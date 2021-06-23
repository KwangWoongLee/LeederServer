#pragma once
#include "Common.h"

class Player : public MoveGameObject
{
public:
	Player();

	virtual void Update(float deltaTime) {};

	void	AddInput(Input input);

	uint32_t	GetPlayerId()			const { return mPlayerId; }
	void		SetPlayerID(uint32_t playerID) { mPlayerId = playerID; }


protected:
	std::deque<Input>			mInputList;

private:
	uint32_t			mPlayerId;
};

