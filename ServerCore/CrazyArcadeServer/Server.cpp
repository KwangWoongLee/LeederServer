#include "stdafx.h"
#include "Server.h"


void Server::Init()
{
	mCrazyArcadeServer = std::make_shared<leeder::IOCPServer>(std::make_shared<GameProcess>());

	mCrazyArcadeServer->Run();


	NetworkManagerServer::GetInstance().Init(mCrazyArcadeServer->GetProcess());


	loadData();
}

void Server::Run()
{
	//단일 로직 스레드
	while (!leeder::bShutDown)
	{
		Clock::GetInstance().Update();

		auto deltaTime = Clock::GetInstance().GetDeltaTime();

		NetworkManagerServer::GetInstance().ProcessQueuedPacket();

		World::GetInstance().Update(deltaTime);

		NetworkManagerServer::GetInstance().Replication();


	}
}

void Server::loadData()
{
	auto BackGround = std::make_shared<GameObject>();
	BackGround->SetPosition({ 352.5f, 352.5f });
	BackGround->SetScale(1.0f);
	BackGround->SetType(eObjectType::BACKGROUND);

	NetworkManagerServer::GetInstance().AddGameObjectToNetwork(BackGround);

	World::GetInstance().AddGameObject(BackGround);

	readMapCSV("./Assets/MapLayer.csv");

	setTileMap();
}

bool Server::readMapCSV(const std::string& fileName)
{
	using namespace std;

	ifstream file(fileName);
	if (true == file.fail())
	{
		//파일 읽기실패 TODO:로그 예외처리
		return false;
	}

	string cell;
	string line;
	while (getline(file, line))
	{
		vector<int> rowInt;
		vector<eObjectType> rowType;
		stringstream lineStream(line);
		while (getline(lineStream, cell, ','))
		{
			int cellInt = stoi(cell);
			eObjectType type = static_cast<eObjectType>(cellInt);
			rowInt.emplace_back(cellInt);
			rowType.emplace_back(type);
		}

		mTileMapToType.emplace_back(rowType);
		mMapSizeX = mTileMapToType[0].size();
		mMapSizeY = mTileMapToType.size();
	}
}

bool Server::setTileMap()
{
	SetTileSizeXY(32 * 2.0f, 32 * 2.0f);


	for (int row = 0; row < mMapSizeY; ++row)
	{
		for (int col = 0; col < mMapSizeX; ++col)
		{
			auto tileType = mTileMapToType[row][col];
			if (tileType != eObjectType::NONE)
			{
				auto tile = std::make_shared<TileServer>();
				tile->SetScale(2.0f);
				tile->SetPosition({row * mTileSizeX + mTileSizeX / 2.0f, col * mTileSizeY + mTileSizeY / 2.0f});
				tile->SetType(tileType);

				NetworkManagerServer::GetInstance().AddGameObjectToNetwork(tile);
				World::GetInstance().AddGameObject(tile);
			}
		}
	}

	

	return true;
}

void Server::SetTileSizeXY(int x, int y)
{
	//타일 1개의 가로세로
	mTileSizeX = x;
	mTileSizeY = y;
}
