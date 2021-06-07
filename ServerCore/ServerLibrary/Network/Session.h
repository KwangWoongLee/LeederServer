#pragma once
#include "stdafx.h"

namespace leeder
{
enum class eSessionType
{
	TERMINAL,
	CLIENT,
};


class Session 
{
public:
	Session();
	virtual ~Session();

	void			UpdateHeartBeat();
	
	std::atomic<bool>	IsConnected() { return mConnected.load(); }

	eSessionType	GetType() const { return mType; }

	uint32_t		GetID() const { return mID; }
	void			SetID(std::atomic<int> id) { mID = id; }

	Socket&			GetSocket() { return mSocket; }

	std::time_t		GetLastHeartBeat() { return mLastHeartBeat; }


protected:
	std::atomic<bool>		mConnected;

	Socket			mSocket;
	eSessionType	mType;

	int				mID;
	

	std::time_t		mLastHeartBeat;


};
}


