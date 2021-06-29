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

enum class eObjectType : int
{
	NONE = -1,

	BLOCK_1 = 1,
	BLOCK_2 = 2,
	BLOCK_3 = 3,
	FLOWER = 4,
	BUSH_1 = 5,
	BUSH_2 = 6,
	HOUSE_1 = 7,
	HOUSE_2 = 8,
	HOUSE_3 = 9,
	BLOCK_4 = 13,
	BLOCK_5 = 14,
	BOX = 15,

	BACKGROUND,
	BOMB,
	BOOM,
	PLAYER,
	ITEM_SHOE,
	ITEM_BOMB,
};


struct Position
{
	float mX;
	float mY;
};

enum class eMoveState;

struct ObjectInfo
{
	float			mScale;
	eObjectState	mState;
	eObjectType		mType;
	eMoveState		mMoveState;
	Position		mPos;
};

class GameObject
{
public:
	GameObject();
	~GameObject();

	virtual void Update(float deltaTime) {};

	virtual void HandleDying() { SetState(eObjectState::DESTROY); }

	uint32_t&	GetNetworkID() { return mNetworkID; }
	void		SetNetworkID(uint32_t	networkID) { mNetworkID = networkID; }

	virtual ObjectInfo& GetObjectInfo() { return mObjectInfo; }

	Position&	GetPosition() { return mPosition; }
	void		SetPosition(Position position) { mPosition = position; mObjectInfo.mPos = position; }

	float& GetWidth() { return mWidth; }
	void	SetWidth(float width) { mWidth = width; }

	float& GetHeight() { return mHeight; }
	void	SetHeight(float height) { mHeight = height; }


	eObjectState& GetState() { return mState; }
	void		SetState(eObjectState state) { mState = state; mObjectInfo.mState = state; }

	eObjectType& GetType() { return mType; }
	void		SetType(eObjectType type) { mType = type;  mObjectInfo.mType = type; }

	float&		GetScale() { return mScale; }
	void		SetScale(float scale) { mScale = scale; mObjectInfo.mScale = scale; }

	void	SetIndex(int index) { mIndex = index; }
	int		GetIndex()		const { return mIndex; }

	bool	IsDie() { return mDie; }
	void	Die() { mDie = true; }


protected:
	uint32_t					mNetworkID;
	Position					mPosition;
	eObjectState				mState;
	eObjectType					mType;
	float						mScale;
	float						mWidth;
	float						mHeight;
	uint32_t					mIndex;
	ObjectInfo					mObjectInfo;
	bool						mDie;

};

}

