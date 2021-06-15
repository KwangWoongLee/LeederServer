#pragma once
#include "stdafx.h"

class User : public GameObject
{
public:
	User(IOCPSession* session, const std::string& name);

	IOCPSession* GetSession() { return mSession; }

	void Update() override;

	uint32_t							GetSessionID()	const { return mSessionID; }
	const	std::string& GetName()		const { return mName; }


	void	AddInput(Input input);


private:
	IOCPSession* mSession;
	uint32_t					mSessionID;
	std::string					mName;

	std::deque<Input>			mInputList;

};

