#include "stdafx.h"
#include "GameObject.h"

namespace leeder
{
GameObject::GameObject()
	: mState(eObjectState::NONE)
	, mType(eObjectType::NONE)
	, mDie(false)
	, mWidth(0.f)
	, mHeight(0.f)
{
	mPosition = { 0.f, 0.f };
	mObjectInfo.mMoveState = eMoveState::NONE;
}
GameObject::~GameObject()
{
}
}