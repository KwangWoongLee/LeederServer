#include "stdafx.h"
#include "User.h"

User::User(IOCPSession* session, const std::string& name)
	: mSession(session)
	, mSessionID(session->GetID())
	, mName(name)
{
	SetPosition({ leeder::GetRandomFloat() * 512 + 100, leeder::GetRandomFloat() * 512 + 100 });
}

void User::Update()
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

void User::AddInput(Input input)
{
	mInputList.push_back(input);
}
