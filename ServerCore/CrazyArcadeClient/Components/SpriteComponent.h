#pragma once
#include "stdafx.h"

class SpriteComponent : public std::enable_shared_from_this<SpriteComponent>
{
public:
	SpriteComponent(std::shared_ptr<GameObject>&& owner);
	~SpriteComponent();

	virtual void Draw(SDL_Renderer* renderer);

	//Getter, Setter
	virtual void SetTexture(SDL_Texture* texture);

	SDL_Texture* GetTexture() const { return mTexture; }
	int GetTextureHeight() const { return mTextureHeight; }
	int GetTextureWidth() const { return mTextureWidth; }


protected:
	//��������Ʈ�� ���ӵ� Texture ����

	std::shared_ptr<GameObject> mOwner;
	SDL_Texture*	mTexture;
	int				mTextureHeight;
	int				mTextureWidth;


};
