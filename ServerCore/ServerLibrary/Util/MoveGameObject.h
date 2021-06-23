#pragma once
#include "stdafx.h"

namespace leeder
{

enum class eMoveState
{
	NONE,
	IDLE,
	LEFT,
	RIGHT,
	UP,
	DOWN,
	STOP,
	TEMP_DIE,
	DIE,
};

class MoveGameObject : public GameObject
{
public:
	eMoveState	GetMoveState() { return mMoveState; }
	void		SetMoveState(eMoveState state) { mMoveState = state; mObjectInfo.mMoveState = state; }

	float	GetSpeed() { return mSpeed; }
	void		SetSpeed(float speed) { mSpeed = speed; }

	void SetLastActionTime() { mLastActionTime = Clock::GetInstance().GetSystemTimeFloat(); }
	float GetLastActionTime() { return mLastActionTime; }
private:
	eMoveState	mMoveState;
	float		mSpeed;
	float mLastActionTime;
};

}

