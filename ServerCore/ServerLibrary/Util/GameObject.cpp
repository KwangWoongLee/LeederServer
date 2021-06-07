#include "stdafx.h"
#include "GameObject.h"

namespace leeder
{
GameObject::GameObject()
	:mState(eObjectState::NONE)
{
	mPosition = { 0.f, 0.f };
}
GameObject::~GameObject()
{
}
}