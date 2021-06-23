#include "stdafx.h"
#include "PlayerClient.h"

PlayerClient::PlayerClient()
{
	SetScale(1.0f);
	SetMoveState(eMoveState::DOWN);

	mAnimationComponent = new AnimationComponent(this);

	SDL_Texture* idle = TextureManager::GetInstance().GetTexture("BazzyIdle");
	SDL_Texture* down = TextureManager::GetInstance().GetTexture("BazzyDown");
	SDL_Texture* left = TextureManager::GetInstance().GetTexture("BazzyLeft");
	SDL_Texture* right = TextureManager::GetInstance().GetTexture("BazzyRight");
	SDL_Texture* up = TextureManager::GetInstance().GetTexture("BazzyUp");
	SDL_Texture* tempDie = TextureManager::GetInstance().GetTexture("BazzyBubble");
	SDL_Texture* die = TextureManager::GetInstance().GetTexture("BazzyDie");


	mAnimationComponent->AddAnimTextures("idle",  idle);
	mAnimationComponent->AddAnimTextures("down", down);
	mAnimationComponent->AddAnimTextures("left", left);
	mAnimationComponent->AddAnimTextures("right",right);
	mAnimationComponent->AddAnimTextures("up", up);
	mAnimationComponent->AddAnimTextures("tempDie", tempDie);
	mAnimationComponent->AddAnimTextures("die", die);


}

void PlayerClient::HandleDying()
{
}




bool PlayerClient::IsStop()
{
	auto time = Clock::GetInstance().GetSystemTimeFloat();

	if (time > GetLastActionTime() + 0.02f)
		return true;

	return false;
}


void PlayerClient::Update(float deltaTime)
{
	mAnimationComponent->Update(deltaTime);

	if (IsStop() && GetMoveState() != eMoveState::TEMP_DIE && GetMoveState() != eMoveState::DIE)
		SetMoveState(eMoveState::STOP);	

}
