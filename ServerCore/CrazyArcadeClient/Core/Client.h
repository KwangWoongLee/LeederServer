#pragma once
#include "stdafx.h"

extern bool bShutDown ;


class Client
{
public:
	Client();
	~Client();

	bool Init();

	void Run();


private:
	// �����Լ�
	void processInput();
	void updateGame();
	void generateOutput();

	void loadData();

	bool readTileMap(const std::string& fileName);

	//SDL ������
	SDL_Window* mWindow;
	//SDL ������
	SDL_Renderer* mRenderer;

	uint32_t	mTickCount;




};
