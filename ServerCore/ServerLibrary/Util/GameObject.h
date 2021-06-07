#pragma once
#include "stdafx.h"

namespace leeder
{
enum class eObjectState
{
	NONE,
	CREATE,
	ACTION,
	DESTROY
};

struct Position
{
	float mX;
	float mY;
};


class GameObject
{
public:
	GameObject();
	~GameObject();


	uint32_t&	GetNetworkID() { return mNetworkID; }
	void		SetNetworkID(uint32_t	networkID) { mNetworkID = networkID; }

	Position&	GetPosition() { return mPosition; }
	void		SetPosition(Position position) { mPosition = position; }

	eObjectState& GetState() { return mState; }
	void		SetState(eObjectState state) { mState = state; }

private:
	uint32_t					mNetworkID;
	Position					mPosition;
	eObjectState				mState;
};

}

