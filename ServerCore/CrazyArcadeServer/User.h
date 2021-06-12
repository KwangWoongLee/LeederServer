#pragma once
#include "stdafx.h"

class User
{
public:
	User(std::shared_ptr<Session> session, const std::string& name, std::shared_ptr<Player>&& player);

	const std::shared_ptr<Session>& GetSession() const { return mSession; }

	uint32_t							GetSessionID()	const { return mSessionID; }
	const	std::string& GetName()		const { return mName; }

	std::shared_ptr<Player> GetGameObject() { return mGameObject; }


	void	AddInput(Input input);


private:
	std::shared_ptr<Session>	mSession;
	uint32_t					mSessionID;

	std::string					mName;

	std::shared_ptr<Player>		mGameObject;

};

