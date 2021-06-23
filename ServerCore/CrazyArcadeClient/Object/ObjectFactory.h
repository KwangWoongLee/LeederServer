#pragma once
#include "stdafx.h"

class ObjectFactory : public Singleton<ObjectFactory>
{
public:
	std::shared_ptr<GameObject> CreateObject(eObjectType type)
	{
		std::shared_ptr<GameObject> obj = nullptr;

		switch (type)
		{
		case eObjectType::BACKGROUND:
			obj = std::make_shared<BackGround>();
			
			break;

		case eObjectType::PLAYER:
			obj = std::make_shared<PlayerClient>();
			break;

		case eObjectType::BOMB:
			return std::make_shared<BombClient>();
			break;

		case eObjectType::BOOM:
			return std::make_shared<BoomClient>();
			break;


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
			obj = std::make_shared<TileClient>();
			break;
		}
			
		default:
			return nullptr;
			break;
		}

		obj->SetType(type);

		return obj;
	}
};
