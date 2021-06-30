#include "stdafx.h"
#include "BoomClient.h"

BoomClient::BoomClient()
{
	mAnimationComponent = new AnimationComponent(this);

	SDL_Texture* boomCenter = TextureManager::GetInstance().GetTexture("BoomCenter");
	SDL_Texture* boomRight = TextureManager::GetInstance().GetTexture("BoomRight");
	SDL_Texture* boomLeft = TextureManager::GetInstance().GetTexture("BoomLeft");
	SDL_Texture* boomUp = TextureManager::GetInstance().GetTexture("BoomUp");
	SDL_Texture* boomDown = TextureManager::GetInstance().GetTexture("BoomDown");

	mAnimationComponent->AddAnimTextures("BoomCenter", boomCenter);
	mAnimationComponent->AddAnimTextures("BoomRight", boomRight);
	mAnimationComponent->AddAnimTextures("BoomLeft", boomLeft);
	mAnimationComponent->AddAnimTextures("BoomUp", boomUp);
	mAnimationComponent->AddAnimTextures("BoomDown", boomDown);

}

BoomClient::~BoomClient()
{
	delete mAnimationComponent;
}

void BoomClient::HandleDying()
{
}





void BoomClient::Update(float deltaTime)
{
	mAnimationComponent->Update(deltaTime);

}
