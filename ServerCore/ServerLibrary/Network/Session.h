#pragma once
#include "stdafx.h"

namespace leeder
{
enum class eSessionType
{
	TERMINAL,
	CLIENT,
};


class Session : public std::enable_shared_from_this<Session>
{
public:
	Session();
	virtual ~Session();

	eSessionType	GetType() const { return mType; }

	int				GetID() const { return mID; }
	void			SetID(std::atomic<int> id) { mID = id; }

	void			Reset();

	void			OnAccept();

	void			OnClose();

	std::shared_ptr<Session> GetThisSharedPtr() { return shared_from_this(); }


private:
	Socket			mSocket;
	eSessionType	mType;

	int				mID;


};
}


