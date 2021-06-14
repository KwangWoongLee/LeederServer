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

enum class eObjectType
{
	NONE,
	PLAYER,
};

struct Position
{
	float mX;
	float mY;
};

struct ObjectInfo
{
	eObjectState mState;
	eObjectType mType;
	Position mPos;
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

	eObjectType& GetType() { return mType; }
	void		SetType(eObjectType type) { mType = type; }

	float		GetScale() { return mScale; }
	void		SetScale(float scale) { mScale = scale; }

	void	SetIndex(int index) { mIndex = index; }
	int		GetIndex()		const { return mIndex; }

protected:
	uint32_t					mNetworkID;
	Position					mPosition;
	eObjectState				mState;
	eObjectType					mType;
	float						mScale;
	uint32_t					mIndex;
	

};

}

