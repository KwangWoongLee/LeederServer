#pragma once
#include "stdafx.h"

enum class eClientIOType
{
	NONE,
	SEND,
	RECV_ZERO,
	RECV,
	CONNECT
};


class Client;


class ClientOverlapped : public leeder::ObjectPool<ClientOverlapped>
{

public:
	ClientOverlapped(Client*);
	~ClientOverlapped();

	eClientIOType				GetType() { return mType; }
	void				SetType(eClientIOType type) { mType = type; }

	void	SetClient(Client* client) { mClient = client; }
	Client* GetClient() { return mClient; }

	WSABUF& GetWSABuf() { return mWsaBuf; }


	static std::mutex		mLock;
private:
	OVERLAPPED		mOverlapped;
	eClientIOType	mType;
	Client*			mClient;
	WSABUF			mWsaBuf;
};


