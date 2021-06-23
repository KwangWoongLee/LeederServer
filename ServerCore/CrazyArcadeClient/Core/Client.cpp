#include "stdafx.h"
#include "Client.h"

bool bShutDown = false;

Client::Client()
{

}

Client::~Client()
{
	if (mRenderer != nullptr)
	{
		SDL_DestroyRenderer(mRenderer);
	}
}


bool Client::Init()
{
	//NetworkManagerClient �ʱ�ȭ
	if (!NetworkManagerClient::GetInstance().Init())
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

	if (IMG_Init(IMG_INIT_PNG) == 0)
	{
		SDL_Log("Unable to initialize SDL_image: %s", SDL_GetError());
		return false;
	}

	// SDL ������ ����
	mWindow = SDL_CreateWindow("2d Game Client", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1024, 820, 0);
	//384,192
	if (!mWindow)
	{
		printf("SDL CreateWindow Error");
		return false;
	}

	//SDL ������ ���� , ��������ȭ �� �׷��� �ϵ���� ���
	mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!mRenderer)
	{
		printf("SDL Renderer init Error");
		return false;
	}

	//�׸��� ���� ����
	SDL_SetRenderDrawColor(mRenderer, 220, 220, 220, 255);


	TextureManager::GetInstance().Init(mRenderer);

	mTickCount = SDL_GetTicks();


	return true;
}

void Client::Run()
{

	////Network Thread on
	NetworkManagerClient::GetInstance().Run();


	while (!bShutDown)
	{
		processInput();

		updateGame();

		NetworkManagerClient::GetInstance().RecvPacketProcess();

		generateOutput();

		NetworkManagerClient::GetInstance().SendPacketProcess();

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
			NetworkManagerClient::GetInstance().SendReqExitPacket();
			break;
		}
	}

	KeyboardState keyState = InputManager::GetInstance().GetKeyboardState();

	if (keyState.GetKeyState(SDL_SCANCODE_ESCAPE)
		== eButtonState::RELEASED)
	{
		NetworkManagerClient::GetInstance().SendReqExitPacket();
	}

	InputManager::GetInstance().HandleInput(keyState);

}

void Client::updateGame()
{
	Clock::GetInstance().Update();

	auto deltaTime = Clock::GetInstance().GetDeltaTime();

	World::GetInstance().Update(deltaTime);


}

void Client::generateOutput()
{
	//�ĸ����(����) �ʱ�ȭ

	//���� �׸��� �������� �ʱ�ȭ
	SDL_RenderClear(mRenderer);

	RenderManager::GetInstance().Render(mRenderer);


	//�������(���÷���)�� �ĸ���� ��ü
	SDL_RenderPresent(mRenderer);

}
