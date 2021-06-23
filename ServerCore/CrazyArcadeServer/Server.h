#pragma once
#include "stdafx.h"

class Server
{
public:
	void Init();

	void Run();

private:
	void loadData();

	bool readMapCSV(const std::string& fileName);
	bool setTileMap();
	std::vector<std::vector<eObjectType>>& GetTileMapToType() { return mTileMapToType; }

	std::shared_ptr<leeder::IOCPServer> mCrazyArcadeServer;


	//≈∏¿œ∏ 
	std::vector<std::vector<eObjectType>> mTileMapToType;
	int GetTileSizeX() const { return mTileSizeX; }
	int GetTileSizeY() const { return mTileSizeY; }
	void SetTileSizeXY(int x, int y);


	int	mMapSizeX;
	int mMapSizeY;
	int mTileSizeX;
	int mTileSizeY;
};

