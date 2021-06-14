#include "stdafx.h"
#include "User.h"

User::User(IOCPSession* session, const std::string& name, std::shared_ptr<PlayerServer>&& player)
	: mSession(session)
	, mSessionID(session->GetID())
	, mName(name)
	, mGameObject(std::move(player))
{
}

void User::AddInput(Input input)
{
	mGameObject->AddInput(input);
}
