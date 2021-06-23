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
	// ÇïÆÛÇÔ¼ö
	void processInput();
	void updateGame();
	void generateOutput();


	//SDL À©µµ¿ì
	SDL_Window* mWindow;
	//SDL ·»´õ·¯
	SDL_Renderer* mRenderer;

	uint32_t	mTickCount;




};
