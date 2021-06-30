#include "stdafx.h"
#include "ItemClient.h"

ItemClient::ItemClient(eObjectType type)
{
	SetType(type);

	mAnimationComponent = new AnimationComponent(this);
	SDL_Texture* itemBomb = TextureManager::GetInstance().GetTexture("ItemBomb");
	SDL_Texture* itemShoe = TextureManager::GetInstance().GetTexture("ItemShoe");

	mAnimationComponent->AddAnimTextures("ItemBomb", itemBomb);
	mAnimationComponent->AddAnimTextures("ItemShoe", itemShoe);



}

ItemClient::~ItemClient()
{
	delete mAnimationComponent;
}

void ItemClient::Update(float deltaTime)
{
	mAnimationComponent->Update(deltaTime);
}
