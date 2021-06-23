#include "stdafx.h"
#include "BombClient.h"

BombClient::BombClient()
{
	mAnimationComponent = new AnimationComponent(this, 1);

	SDL_Texture* bomb = TextureManager::GetInstance().GetTexture("Bomb");

	mAnimationComponent->AddAnimTextures("Bomb", bomb);


}

void BombClient::HandleDying()
{
}





void BombClient::Update(float deltaTime)
{
	mAnimationComponent->Update(deltaTime);

}
