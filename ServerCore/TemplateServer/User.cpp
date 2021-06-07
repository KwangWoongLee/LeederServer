#include "stdafx.h"
#include "User.h"


namespace leeder
{

User::User(std::shared_ptr<Session>& session, std::string& id)
	: mSession(session)
	, mID(id)
{
}

void User::Update()
{
	while (!mInputQueue.Empty())
	{
		auto input = mInputQueue.Pop();

		switch ((input))
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
	}
}




}

