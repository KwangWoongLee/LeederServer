#include "stdafx.h"
#include "ItemServer.h"

ItemServer::ItemServer()
	: mCollider(this)
{
	SetScale(1.1f);
	SetMoveState(eMoveState::IDLE);
	mCollider.SetRadius(32.f);

}


void ItemServer::HandleDying()
{
	NetworkManagerServer::GetInstance().RemoveGameObjectToNetwork(this);
}
