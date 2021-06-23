#include "stdafx.h"
#include "BackGround.h"

BackGround::BackGround()
{
	SetScale(1.0f);
	SDL_Texture* bgTexture = TextureManager::GetInstance().GetTexture("BackGround");

	SpriteComponent* bgComponent = new SpriteComponent(this, 0);
	bgComponent->SetTexture(bgTexture);

}

void BackGround::HandleDying()
{
}
