#include "stdafx.h"
#include "TileServer.h"

void TileServer::HandleDying()
{
	NetworkManagerServer::GetInstance().RemoveGameObjectToNetwork(this);
	
	createItem();
}

void TileServer::Update(float deltaTime)
{
	mCollider.Update(deltaTime);
};

void TileServer::createItem()
{
	int rand = leeder::GetRandomInt();
	if (rand % 3 != 0)
		return;


	auto item = std::make_shared<ItemServer>();
	item->SetPosition(GetPosition());

	rand = leeder::GetRandomInt();

	if (rand % 2 == 0)
		item->SetType(eObjectType::ITEM_BOMB);
	else
		item->SetType(eObjectType::ITEM_SHOE);

	NetworkManagerServer::GetInstance().AddGameObjectToNetwork(item);
	World::GetInstance().AddGameObject(item);
};