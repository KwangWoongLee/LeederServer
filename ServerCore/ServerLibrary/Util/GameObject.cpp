#include "stdafx.h"
#include "GameObject.h"

namespace leeder
{
GameObject::GameObject()
	: mState(eObjectState::NONE)
	, mType(eObjectType::NONE)
{
	mPosition = { 0.f, 0.f };
}
GameObject::~GameObject()
{
}
}