#include "stdafx.h"
#include "SpriteComponent.h"



SpriteComponent::SpriteComponent(std::shared_ptr<GameObject>&& owner)
	: mOwner(std::move(owner))
	,mTexture(nullptr)
	, mTextureHeight(0)
	, mTextureWidth(0)
{
	RenderManager::GetInstance().AddComponent(this);
}

SpriteComponent::~SpriteComponent()
{
	RenderManager::GetInstance().RemoveComponent(this);
}

void SpriteComponent::Draw(SDL_Renderer* renderer)
{
	if (mTexture)
	{
		SDL_Rect r;
		r.w = static_cast<int>(mTextureWidth * mOwner->GetScale());
		r.h = static_cast<int>(mTextureHeight * mOwner->GetScale());
		r.x = static_cast<int>(mOwner->GetPosition().mX - r.w / 2);
		r.y = static_cast<int>(mOwner->GetPosition().mY - r.h / 2);


		SDL_RenderCopy(renderer,
			mTexture,
			nullptr,
			&r);
	}
}

void SpriteComponent::SetTexture(SDL_Texture* texture)
{
	mTexture = texture;

	//해당 texture의 width와 height 정보를 추출
	SDL_QueryTexture(texture, nullptr, nullptr, &mTextureWidth, &mTextureHeight);
}
