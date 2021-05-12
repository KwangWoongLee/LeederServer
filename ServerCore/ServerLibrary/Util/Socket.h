#pragma once
#include "stdafx.h"

namespace leeder
{
class Socket
{
public:
	Socket();
	virtual ~Socket();

	SOCKET& GetSocket() { return mSocket; }

	bool	ReuseAddr(bool reuse);
	bool	SetLinger(bool lingerOn, int lingerTime);
	bool	UpdateAcceptContext(Socket* s);
	bool	SetNodelay(bool nodelay);

	int		Bind(const char* ip, uint16_t port);
	int		Listen(int backLog = SOMAXCONN);


private:
	SOCKET			mSocket;
	SOCKADDR_IN		mAddrInfo;
};

}

