#include "stdafx.h"
#include "BombServer.h"

BombServer::BombServer(GameObject* owner)
	: mOwner(owner)
	, mLifeTime(3.f)
	, mCollider(this)
{
	SetScale(1.6f);
	SetMoveState(eMoveState::IDLE);

	mFirstContact = true;
	mCollider.SetRadius(32.f);
}

void BombServer::Update(float deltaTime)
{
	mLifeTime -= deltaTime;

	if (mLifeTime < 0.f)
	{
		Die();
	}


	mCollider.Update(deltaTime);
	
}

void BombServer::HandleDying()
{
	auto player = static_cast<PlayerServer*>(mOwner);
	player->SetBombCount(player->GetBombCount() -1);

	NetworkManagerServer::GetInstance().RemoveGameObjectToNetwork(this);

	createBoom();
}

void BombServer::createBoom()
{
	auto boomCenter = std::make_shared<BoomServer>(mOwner);
	boomCenter->SetPosition(GetPosition());

	NetworkManagerServer::GetInstance().AddGameObjectToNetwork(boomCenter);
	World::GetInstance().AddGameObject(boomCenter);

	auto player = static_cast<PlayerServer*>(mOwner);


	auto pos = searchBoomPosition(1 , 0);

	if (pos.mX != -1.f && pos.mY != -1.f)
	{
		auto boomRight = std::make_shared<BoomServer>(mOwner);

		boomRight->SetPosition(pos);
		boomRight->SetMoveState(eMoveState::RIGHT);

		NetworkManagerServer::GetInstance().AddGameObjectToNetwork(boomRight);
		World::GetInstance().AddGameObject(boomRight);
	}






	pos = searchBoomPosition(-1 , 0);

	if (pos.mX != -1.f && pos.mY != -1.f)
	{
		auto boomLeft = std::make_shared<BoomServer>(mOwner);
		boomLeft->SetPosition(pos);
		boomLeft->SetMoveState(eMoveState::LEFT);

		NetworkManagerServer::GetInstance().AddGameObjectToNetwork(boomLeft);
		World::GetInstance().AddGameObject(boomLeft);
	}


	pos = searchBoomPosition(0, -1);

	if (pos.mX != -1.f && pos.mY != -1.f)
	{

		auto boomUp = std::make_shared<BoomServer>(mOwner);
		boomUp->SetPosition(pos);
		boomUp->SetMoveState(eMoveState::UP);

		NetworkManagerServer::GetInstance().AddGameObjectToNetwork(boomUp);
		World::GetInstance().AddGameObject(boomUp);
	}


	pos = searchBoomPosition(0, 1 );

	if (pos.mX != -1.f && pos.mY != -1.f)
	{

		auto boomDown = std::make_shared<BoomServer>(mOwner);
		boomDown->SetPosition(pos);
		boomDown->SetMoveState(eMoveState::DOWN);

		NetworkManagerServer::GetInstance().AddGameObjectToNetwork(boomDown);
		World::GetInstance().AddGameObject(boomDown);
	}


}

Position BombServer::searchBoomPosition(int dx , int dy)
{
	int x = static_cast<int>(fabsf(GetPosition().mX) / 64) * 64 + (64 * dx) + 32;
	int y = static_cast<int>(fabsf(GetPosition().mY) / 64) * 64 + (64 * dy) + 32;

	float realX = x * 1.0f;
	float realY = y * 1.0f;

	auto map = NetworkManagerServer::GetInstance().GetNetworkState();

	for (auto& element : map)
	{
		auto& obj = element.second;
		if (obj->GetPosition().mX == realX
			&& obj->GetPosition().mY == realY)
		{
			switch (obj->GetType())
			{
			case eObjectType::PLAYER:
			{
				auto player = std::static_pointer_cast<PlayerServer>(obj);
				player->SetState(eObjectState::ACTION);
				player->SetMoveState(eMoveState::TEMP_DIE);
				NetworkManagerServer::GetInstance().SetObjectState(player.get());

				break;
			}

			case eObjectType::BOMB:
			{
				obj->Die();
				break;
			}

			case eObjectType::BLOCK_1:
			case eObjectType::BLOCK_2:
			case eObjectType::BLOCK_3:
			case eObjectType::BLOCK_4:
			case eObjectType::BLOCK_5:

			case eObjectType::BOX:
			case eObjectType::BUSH_1:
			case eObjectType::BUSH_2:
			{	
				obj->Die();
				break;
			}

			case eObjectType::FLOWER:
			case eObjectType::HOUSE_1:
			case eObjectType::HOUSE_2:
			case eObjectType::HOUSE_3:
			{
				break;
			}
			case eObjectType::ITEM_BOMB:
			case eObjectType::ITEM_SHOE:
			{
				obj->Die();
				break;
			}
			default:
				break;
			}


			return { -1.f, -1.f };
		}
		
	}

	return { realX, realY };
}
