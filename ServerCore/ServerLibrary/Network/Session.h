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

	eSessionType	GetType() const { return mType; }

	int				GetID() const { return mID; }
	void			SetID(std::atomic<int> id) { mID = id; }

	Socket&			GetSocket() { return mSocket; }



protected:
	Socket			mSocket;
	eSessionType	mType;

	int				mID;


};
}


