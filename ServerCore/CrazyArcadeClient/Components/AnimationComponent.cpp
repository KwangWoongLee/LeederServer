#include "stdafx.h"
#include "AnimationComponent.h"

AnimationComponent::AnimationComponent(GameObject* owner, int drawOrder)
	: SpriteComponent(owner, drawOrder)
	, mCurrFrame(0.0f)
	, mAnimFPS(10.0f)
	, mFrameX(1)
{
}

void AnimationComponent::Update(float deltaTime)
{
	SpriteComponent::Update(deltaTime);

	auto owner = static_cast<MoveGameObject*>(mOwner);

	auto pair = AnimationFactory::GetInstance().GetTextureAndFrame(owner->GetType(),owner->GetMoveState());

	if (pair.first != "")
	{
		auto textureName = pair.first;
		mFrameX = pair.second;

		mAnimTexture = GetAnimTexture(textureName);
	}
	


	if (owner->GetMoveState() == eMoveState::STOP &&owner->GetType() == eObjectType::PLAYER)
	{
		mCurrFrame = 0.f;
	}

	else
	{
		mCurrFrame += mAnimFPS * deltaTime;
	}


	////��� texture animation�� ������ ó�� animation���� ����
	while (mCurrFrame >= mFrameX)
	{
		mCurrFrame -= mFrameX;
	}

	// mCurrFrame�� �´� texture ����
	SetTexture(mAnimTexture);
}

void AnimationComponent::Draw(SDL_Renderer* renderer)
{
	if (mTexture)
	{
		SDL_Rect src;
		src.w = static_cast<int>(mTextureWidth / mFrameX);
		src.h = static_cast<int>(mTextureHeight);
		src.x = static_cast<int>(mTextureWidth / mFrameX) * static_cast<int>(mCurrFrame);
		src.y = static_cast<int>(0);

		//���� actor�� ������ ���� texture ��ġ
		SDL_Rect dsc;
		dsc.w = static_cast<int>(mTextureWidth / mFrameX * mOwner->GetScale());
		dsc.h = static_cast<int>(mTextureHeight * mOwner->GetScale());
		dsc.x = static_cast<int>(mOwner->GetPosition().mX - dsc.w / 2);
		dsc.y = static_cast<int>(mOwner->GetPosition().mY - dsc.h / 2);

		SDL_RenderCopy(renderer,
			mTexture,
			&src,
			&dsc);
	}
}

void AnimationComponent::SetAnimTexture(SDL_Texture* texture, int frameX)
{
	mAnimTexture = texture;

	mFrameX = frameX;

	mCurrFrame = 0.0f;


	SetTexture(mAnimTexture);
}

void AnimationComponent::AddAnimTextures(std::string strTexture, SDL_Texture* animTexture)
{
	mAnimTextures[strTexture] =  animTexture;
}

SDL_Texture* AnimationComponent::GetAnimTexture(std::string strTexture)
{
	auto iter = mAnimTextures.find(strTexture);
	if (iter == mAnimTextures.end())
		return nullptr;
	
	return iter->second;
}

