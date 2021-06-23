#include "stdafx.h"
#include "TileServer.h"

void TileServer::HandleDying()
{
	NetworkManagerServer::GetInstance().RemoveGameObjectToNetwork(this);
}

void TileServer::Update(float deltaTime)
{
	mCollider.Update(deltaTime);
}
