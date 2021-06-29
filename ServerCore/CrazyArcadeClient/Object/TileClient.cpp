#include "stdafx.h"
#include "TileClient.h"

TileClient::TileClient()
{
	SetMoveState(eMoveState::IDLE);
	SetScale(1.0f);
	SDL_Texture* tileTexture = TextureManager::GetInstance().GetTexture("Tile");

	mTileComponent = new TileComponent(this,2);

	mTileComponent->SetTexture(tileTexture);

	mTileComponent->AddAnimTextures("idle", tileTexture);

}

TileClient::~TileClient()
{
	delete mTileComponent;
}

void TileClient::HandleDying()
{
}

void TileClient::Update(float deltaTime)
{
	mTileComponent->Update(deltaTime);

}
