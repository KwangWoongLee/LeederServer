#include "stdafx.h"
#include "Client.h"

bool bShutDown = false;

Client::Client()
{

}

Client::~Client()
{

}


bool Client::Init()
{
	//NetworkManager �ʱ�ȭ
	if (!NetworkManager::GetInstance().Init())
	{
		printf("Network Init Error");
		return false;
	}

	if (!InputManager::GetInstance().Init())
	{
		printf("InputManager Init Error");
		return false;
	}


	// SDL �ʱ�ȭ
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
	{
		printf("SDL Init Error");
		return false;
	}

	// SDL ������ ����
	mWindow = SDL_CreateWindow("2d Game Client", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1024, 512, 0);
	//384,192
	if (!mWindow)
	{
		printf("SDL CreateWindow Error");
		return false;
	}

	//SDL ������ ���� , ��������ȭ �� �׷��� �ϵ���� ���
	mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED);
	if (!mRenderer)
	{
		printf("SDL Renderer init Error");
		return false;
	}

	mTickCount = SDL_GetTicks();

	loadData();


	return true;
}

void Client::Run()
{
	////Network Thread on
	NetworkManager::GetInstance().Run();


	while (!bShutDown)
	{
		processInput();

		updateGame();

		NetworkManager::GetInstance().RecvPacketProcess();

		generateOutput();

		NetworkManager::GetInstance().SendPacketProcess();

	}

}

void Client::processInput()
{
	InputManager::GetInstance().PrepareForUpdate();

	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			NetworkManager::GetInstance().SendReqExitPacket();
			break;
		}
	}

	KeyboardState keyState = InputManager::GetInstance().GetKeyboardState();

	if (keyState.GetKeyState(SDL_SCANCODE_ESCAPE)
		== eButtonState::RELEASED)
	{
		NetworkManager::GetInstance().SendReqExitPacket();
	}

	InputManager::GetInstance().HandleInput(keyState);

}

void Client::updateGame()
{
	//while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTickCount + 16))
	//	;

	//float deltaTime = (SDL_GetTicks() - mTickCount) / 1000.f;

	//mTickCount = SDL_GetTicks();

	////��Ÿ �ִ� �ð����� ����
	//if (deltaTime > 0.05f)
	//{
	//	deltaTime = 0.05f;
	//}

}

void Client::generateOutput()
{
	SDL_Rect rect;
	rect.h = 2;
	rect.w = 2;



	//�׸��� ���� ����
	SDL_SetRenderDrawColor(mRenderer, 220, 220, 220, 255);
	//���� �׸��� �������� �ʱ�ȭ
	SDL_RenderClear(mRenderer);


	for (auto object : NetworkManager::GetInstance().mNetworkIDToGameObjectMap)
	{
		SDL_SetRenderDrawColor(mRenderer, (object.first + 1) * 40, (object.first + 1) * 40, 0, 255);

		rect.x = object.second->GetPosition().mX;
		rect.y = object.second->GetPosition().mY;

		int re = SDL_RenderFillRect(mRenderer, &rect);


	}

	//�������(���÷���)�� �ĸ���� ��ü
	SDL_RenderPresent(mRenderer);

}

void Client::loadData()
{
	readTileMap("");
}

bool Client::readTileMap(const std::string& fileName)
{
	return false;
}

