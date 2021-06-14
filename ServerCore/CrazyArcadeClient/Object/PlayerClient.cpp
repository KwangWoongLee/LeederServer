#include "stdafx.h"
#include "PlayerClient.h"

PlayerClient::PlayerClient()
{
	SetPosition({ 512.f, 256.f });
	SetScale(1.0f);

	mSpriteComponent = std::make_shared<SpriteComponent>(this);
	mSpriteComponent->SetTexture(TextureManager::GetInstance().GetTexture("BazzyIdle"));
}

PlayerClient::PlayerClient(GameObject obj)
{
	SetNetworkID(obj.GetNetworkID());
	SetPosition( obj.GetPosition());
	SetScale(1.0f);

	mSpriteComponent = std::make_shared<SpriteComponent>(this);
	mSpriteComponent->SetTexture(TextureManager::GetInstance().GetTexture("BazzyIdle"));
}

void PlayerClient::Update()
{

}
