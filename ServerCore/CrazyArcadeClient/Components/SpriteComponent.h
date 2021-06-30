#pragma once
#include "stdafx.h"

class SpriteComponent
{
public:
	SpriteComponent(GameObject* owner, int drawOrder = 300);
	~SpriteComponent();

	virtual void Update(float deltaTime) {};

	virtual void Draw(SDL_Renderer* renderer);

	int			GetDrawOrder() { return mDrawOrder; }

	//Getter, Setter
	virtual void SetTexture(SDL_Texture* texture);

	SDL_Texture* GetTexture() const { return mTexture; }

	int GetTextureHeight() const { return mTextureHeight; }
	void SetTextureHeight(int height) { mTextureHeight = height; }
	
	int GetTextureWidth() const { return mTextureWidth; }
	void SetTextureWidth(int width) { mTextureWidth = width; }


protected:
	//스프라이트에 종속된 Texture 변수
	int				mDrawOrder;

	GameObject*		mOwner;
	SDL_Texture*	mTexture;
	int				mTextureHeight;
	int				mTextureWidth;


};
