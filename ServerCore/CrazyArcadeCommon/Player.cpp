#include "Common.h"
#include "Player.h"


Player::Player()
{
	SetType(eObjectType::PLAYER);
}


void Player::AddInput(Input input)
{
	mInputList.push_back(input);
}
