#include "stdafx.h"
#include "PlayerServer.h"

PlayerServer::PlayerServer(uint32_t sessionID)
	:Player()
	, mBombCount(0)
	, mMaxBombCount(1)
	, mCollider(this)
	, mDeathTimer(4.0f)
{
	auto rand = leeder::GetRandomInt() % 4;
	SetPlayerID(sessionID);
	SetPosition({ BirthPosition[rand][0] , BirthPosition[rand][1] });
	SetMoveState(eMoveState::IDLE);
	SetSpeed(150.0f);
	SetScale(1.0f);
	mCollider.SetRadius(32.f);

};



void PlayerServer::Update(float deltaTime)
{
	float deltaX = 0.f;
	float deltaY = 0.f;

	if (GetMoveState() == eMoveState::TEMP_DIE)
	{
		mDeathTimer -= deltaTime;
		if (mDeathTimer <= 0.f)
		{
			mDeathTimer = 1.0f;
			SetState(eObjectState::ACTION);
			SetMoveState(eMoveState::DIE);
			NetworkManagerServer::GetInstance().SetObjectState(this);
		}
	}

	if (GetMoveState() == eMoveState::DIE)
	{
		mDeathTimer -= deltaTime;
		if (mDeathTimer <= 0.f)
		{
			Die();
		}
	}

	if (!mInputList.empty())
	{
		auto input = mInputList.front();


		switch (input.GetType())
		{
		case eInputType::A:
			SetMoveState(eMoveState::LEFT);
			SetPosition({ GetPosition().mX - GetSpeed() * deltaTime , GetPosition().mY });
			break;
		case eInputType::D:
			SetMoveState(eMoveState::RIGHT);
			SetPosition({ GetPosition().mX + GetSpeed() * deltaTime, GetPosition().mY });
			break;
		case eInputType::S:
			SetMoveState(eMoveState::DOWN);
			SetPosition({ GetPosition().mX, GetPosition().mY + GetSpeed() * deltaTime });
			break;
		case eInputType::W:
			SetMoveState(eMoveState::UP);
			SetPosition({ GetPosition().mX, GetPosition().mY - GetSpeed() * deltaTime });
			break;
		case eInputType::SPACE:
			CreateBomb();
			break;
		case eInputType::NONE:
			break;
		default:
			break;
		}

		mInputList.pop_front();

	}


	mCollider.Update(deltaTime);



	auto map = NetworkManagerServer::GetInstance().GetNetworkState();
	for (auto& element : map)
	{
		auto obj = element.second;

		if (IsDie())
			break;

		if (obj->GetNetworkID() == GetNetworkID() || obj->GetType()==eObjectType::BOOM)
			continue;
		
		if (GetMoveState() == eMoveState::TEMP_DIE && obj->GetType() != eObjectType::PLAYER)
		{
			break;
		}

		switch ((obj->GetType()))
		{
		case eObjectType::PLAYER:
		{
			auto other = std::static_pointer_cast<PlayerServer>(obj);

			if (mCollider.IsIntersect(other->GetCollider(), &deltaX, &deltaY))
			{
				if (GetMoveState() == eMoveState::TEMP_DIE)
				{
					SetState(eObjectState::ACTION);
					SetMoveState(eMoveState::DIE);
					NetworkManagerServer::GetInstance().SetObjectState(this);
				}
			}
			break;
		}

		case eObjectType::BOMB:
		{
			auto bomb = std::static_pointer_cast<BombServer>(obj);

			if (mCollider.IsIntersect(bomb->GetCollider(), &deltaX, &deltaY))
			{
				if (bomb->IsFirstContact())
				{
					continue;
				}

				SetPosition({ bomb->GetPosition().mX - deltaX, bomb->GetPosition().mY - deltaY });

			}
			else
			{
				if(bomb->IsOwner(GetNetworkID()))
					bomb->SetContact();
			}
			break;
		}

		case eObjectType::ITEM_BOMB:
		{
			auto item = std::static_pointer_cast<ItemServer>(obj);

			if (mCollider.IsIntersect(item->GetCollider(), &deltaX, &deltaY))
			{
				item->Die();
				mMaxBombCount += 1;

			}
			break;
		}

		case eObjectType::ITEM_SHOE:
		{
			auto item = std::static_pointer_cast<ItemServer>(obj);

			if (mCollider.IsIntersect(item->GetCollider(), &deltaX, &deltaY))
			{
				item->Die();
				SetSpeed(GetSpeed() + 50.0f);
			}
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
		case eObjectType::FLOWER:
		case eObjectType::HOUSE_1:
		case eObjectType::HOUSE_2:
		case eObjectType::HOUSE_3:
		{
			auto other = std::static_pointer_cast<TileServer>(obj);

			if (mCollider.IsIntersect(other->GetCollider(), &deltaX, &deltaY))
			{
				printf("Collision with tile");
				SetPosition({ other->GetPosition().mX - deltaX, other->GetPosition().mY - deltaY });
			}
			break;
		}

		default:


			break;
		}
	}

}

void PlayerServer::HandleDying()
{
	NetworkManagerServer::GetInstance().RemoveGameObjectToNetwork(this);
}

void PlayerServer::CreateBomb()
{
	if (mBombCount >= mMaxBombCount)
		return;



	Position pos = searchBombPosition();
	auto map = NetworkManagerServer::GetInstance().GetNetworkState();

	for (auto& element : map)
	{
		if (GetNetworkID() != element.first)
		{
			auto& otherObj = element.second;
			if (otherObj->GetPosition().mX == pos.mX && otherObj->GetPosition().mY == pos.mY)
			{
				return;
			}
		}
		
	}

	auto bomb = std::make_shared<BombServer>(this);
	bomb->SetPosition(pos);

	NetworkManagerServer::GetInstance().AddGameObjectToNetwork(bomb);
	World::GetInstance().AddGameObject(bomb);

	++mBombCount;
}


Position PlayerServer::searchBombPosition()
{
	int x = static_cast<int>(fabsf(GetPosition().mX) / 64) * 64 + 32 ;
	int y = static_cast<int>(fabsf(GetPosition().mY) / 64) * 64 + 32 ;

	return { x * 1.0f, y * 1.0f };
}
