#include "stdafx.h"
#include "PlayerServer.h"

void PlayerServer::Update()
{
	while (!mInputList.empty())
	{
		auto input = mInputList.front();

		switch (input.GetType())
		{
		case eInputType::A:
			SetPosition({ GetPosition().mX - 2.0f, GetPosition().mY });
			break;
		case eInputType::D:
			SetPosition({ GetPosition().mX + 2.0f, GetPosition().mY });
			break;
		case eInputType::S:
			SetPosition({ GetPosition().mX, GetPosition().mY + 2.0f });
			break;
		case eInputType::W:
			SetPosition({ GetPosition().mX, GetPosition().mY - 2.0f });
			break;
		case eInputType::SPACE:
			SetPosition({ GetPosition().mX, GetPosition().mY });
			break;
		case eInputType::NONE:
			break;
		default:
			break;
		}

		mInputList.pop_front();
	}

	mInputList.clear();
}
