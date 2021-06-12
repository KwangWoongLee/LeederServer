#pragma once
#include "stdafx.h"

namespace leeder
{
enum class eObjectState
{
	NONE,
	CREATE,
	ACTION,
	WILL_DESTROY,
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

	virtual void Update() {};

	uint32_t&	GetNetworkID() { return mNetworkID; }
	void		SetNetworkID(uint32_t	networkID) { mNetworkID = networkID; }

	Position&	GetPosition() { return mPosition; }
	void		SetPosition(Position position) { mPosition = position; }

	eObjectState& GetState() { return mState; }
	void		SetState(eObjectState state) { mState = state; }

	float		GetScale() { return mScale; }
	void		SetScale(float scale) { mScale = scale; }

	void	SetIndex(int index) { mIndex = index; }
	int		GetIndex()		const { return mIndex; }

private:
	uint32_t					mNetworkID;
	Position					mPosition;
	eObjectState				mState;
	float						mScale;
	uint32_t					mIndex;

};

}

