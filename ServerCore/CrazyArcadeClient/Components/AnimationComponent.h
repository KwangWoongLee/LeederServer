#pragma once
#include "SpriteComponent.h"

class AnimationComponent : public SpriteComponent
{
public:
	AnimationComponent(GameObject* owner, int drawOrder = 100);

	virtual void Update(float deltaTime) override;

	virtual void Draw(SDL_Renderer* renderer) override;

	// Getter, Setter
	void SetAnimTexture(SDL_Texture* texture, int frameX);

	void AddAnimTextures(std::string strTexture, SDL_Texture* animTexture);
	SDL_Texture* GetAnimTexture(std::string strTexture);

	float GetCurrFrame() const { return mCurrFrame; }
	void SetCurrFrame(float frame) { mCurrFrame = frame; }

	float GetAnimFPS() const { return mAnimFPS; }
	void SetAnimFPS(float fps) { mAnimFPS = fps; }



protected:
	// Texture Vector
	SDL_Texture* mAnimTexture;
	// 행동별 texture를 담을 map
	std::unordered_map<std::string, SDL_Texture*> mAnimTextures;
	// 현재 프레임
	float mCurrFrame;
	// 애니메이션 프레임수
	int   mFrameX;
	// FPS
	float mAnimFPS;

	float mLastActionTime;

};


class AnimationFactory : public Singleton<AnimationFactory>
{
public:
	std::pair<std::string, int> GetTextureAndFrame(eObjectType type, eMoveState state)
	{
		std::string textureName = "";
		int			frameX = 0;

		switch (type)
		{
		case eObjectType::PLAYER:
			if (state == eMoveState::IDLE)
			{
				textureName = "idle";
				frameX = 1;
			}
			else if (state == eMoveState::DOWN)
			{
				textureName = "down";
				frameX = 8;

			}
			else if (state == eMoveState::UP)
			{
				textureName = "up";
				frameX = 8;
			}
			else if (state == eMoveState::RIGHT)
			{
				textureName = "right";
				frameX = 6;
			}
			else if (state == eMoveState::LEFT)
			{
				textureName = "left";
				frameX = 6;

			}

			else if (state == eMoveState::TEMP_DIE)
			{
				textureName = "tempDie";
				frameX = 16;

			}

			else if (state == eMoveState::DIE)
			{
				textureName = "die";
				frameX = 13;

			}
			break;


		//case eObjectType::BOMB:
		//{
		//	textureName = "Bomb";
		//	frameX = 4;
		//}
		//break;

		case eObjectType::BOOM:
		{
			if (state == eMoveState::IDLE)
			{
				textureName = "BoomCenter";
				frameX = 14;
			}
			else if (state == eMoveState::DOWN)
			{
				textureName = "BoomDown";
				frameX = 9;
			}
			else if (state == eMoveState::UP)
			{
				textureName = "BoomUp";
				frameX = 9;
			}
			else if (state == eMoveState::RIGHT)
			{
				textureName = "BoomRight";
				frameX = 9;
			}
			else if (state == eMoveState::LEFT)
			{
				textureName = "BoomLeft";
				frameX = 9;
			}
		}
		break;

		case eObjectType::ITEM_SHOE:
		case eObjectType::ITEM_BOMB:
		{
			if (state == eMoveState::IDLE)
			{
				if (type == eObjectType::ITEM_SHOE)
					textureName = "ItemShoe";
				else
					textureName = "ItemBomb";

				frameX = 2;
			}
		}
		break;

		}
		return std::make_pair(textureName , frameX);
	}
};