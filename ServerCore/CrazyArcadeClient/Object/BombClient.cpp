#include "stdafx.h"
#include "BombClient.h"

BombClient::BombClient()
{
	mAnimationComponent = new AnimationComponent(this);
	SDL_Texture* bomb = TextureManager::GetInstance().GetTexture("Bomb");

	mAnimationComponent->AddAnimTextures("Bomb", bomb);

	mAnimationComponent->SetAnimTexture(bomb, 4);

}

BombClient::~BombClient()
{
	delete mAnimationComponent;
}


void BombClient::HandleDying()
{
}





void BombClient::Update(float deltaTime)
{
	mAnimationComponent->Update(deltaTime);

}
