#include "stdafx.h"
#include "ClientOverlapped.h"

	ClientOverlapped::ClientOverlapped(Client* client)
	{
		mClient = client;
		ZeroMemory(&mOverlapped, sizeof(mOverlapped));
	}

	ClientOverlapped::~ClientOverlapped()
	{
	}

	std::mutex ClientOverlapped::mLock;
