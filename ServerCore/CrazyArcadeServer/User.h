#pragma once
#include "stdafx.h"

class User
{
public:
	User(IOCPSession* session, const std::string& name, std::shared_ptr<PlayerServer>&& player);

	IOCPSession* GetSession()  { return mSession; }

	uint32_t							GetSessionID()	const { return mSessionID; }
	const	std::string& GetName()		const { return mName; }

	std::shared_ptr<PlayerServer> GetGameObject() { return mGameObject; }


	void	AddInput(Input input);


private:
	IOCPSession*				mSession;
	uint32_t					mSessionID;

	std::string					mName;

	std::shared_ptr<PlayerServer>		mGameObject;

};

