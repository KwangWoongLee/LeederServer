#include "stdafx.h"
#include "BoomServer.h"

BoomServer::BoomServer(GameObject* owner)
	: mOwner(owner)
	, mLifeTime(0.5f)
{
	SetScale(1.6f);
	SetMoveState(eMoveState::IDLE);
	SetState(eObjectState::CREATE);
}

void BoomServer::Update(float deltaTime)
{
	float deltaX = 0.f;
	float deltaY = 0.f;

	mLifeTime -= deltaTime;

	if (mLifeTime < 0.f)
	{
		Die();
	}


}

void BoomServer::HandleDying()
{
	NetworkManagerServer::GetInstance().RemoveGameObjectToNetwork(this);
}
