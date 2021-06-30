#include "stdafx.h"
#include "TileComponent.h"


TileComponent::TileComponent(GameObject* owner, int drawOrder)
	: AnimationComponent(owner, drawOrder)
{
	mCurrFrame = 1.0f;
}

void TileComponent::Update(float deltaTime)
{

	auto owner = static_cast<MoveGameObject*>(mOwner);

	if (owner->GetMoveState() == eMoveState::IDLE)
	{
		mAnimTexture = GetAnimTexture("idle");
		mFrameX = 1;
	}
	

	// mCurrFrame에 맞는 texture 적용
	SetTexture(mAnimTexture);
}

void TileComponent::Draw(SDL_Renderer* renderer)
{
	int mTileRowSize = mTextureWidth / 32;
	int mTileColSize = mTextureHeight / 32;


	if (mTexture)
	{
		SDL_Rect src;
		src.w = static_cast<int>(32);
		src.h = static_cast<int>(32);
		src.x = static_cast<int>(((static_cast<int>(mOwner->GetType()) % mTileRowSize -1) * 32) / mFrameX * static_cast<int>(mCurrFrame));
		src.y = static_cast<int>((static_cast<int>(mOwner->GetType()) / mTileRowSize * 32));

		//소유 actor의 정보에 따라 texture 배치
		SDL_Rect dsc;
		dsc.w = static_cast<int>(32 * mOwner->GetScale());
		dsc.h = static_cast<int>(32 * mOwner->GetScale());
		dsc.x = static_cast<int>(mOwner->GetPosition().mX - dsc.w / 2);
		dsc.y = static_cast<int>(mOwner->GetPosition().mY - dsc.h / 2);

		SDL_RenderCopy(renderer,
			mTexture,
			&src,
			&dsc);
	}
}