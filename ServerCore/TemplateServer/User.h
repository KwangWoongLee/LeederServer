#pragma once
#include "stdafx.h"

namespace leeder
{

class User : public GameObject
{
public:
	User(std::shared_ptr<Session>& session, std::string& id);

	const std::shared_ptr<Session>& GetSession() { return mSession; }

	void	PushInput(eInputType type) { mInputQueue.Push(type); }

	void	Update();
	
	
private:
	std::shared_ptr<Session>	mSession;
	std::string					mID;

	ThreadSafeQueue<eInputType>		mInputQueue;
};

}
