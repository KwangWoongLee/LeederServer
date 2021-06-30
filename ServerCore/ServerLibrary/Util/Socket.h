#pragma once
#include "stdafx.h"

namespace leeder
{
extern LPFN_ACCEPTEX		mFnAcceptEx;
extern LPFN_CONNECTEX		mFnConnectEx;


class Socket
{
public:
	Socket();
	virtual ~Socket();

	void	Reset();

	SOCKET& GetHandle() { return mSocket; }
	std::wstring	GetClientAddress();
	SOCKADDR_IN& GetSocketInfo() { return mAddrInfo; }

	bool	ReuseAddr(bool reuse);
	bool	SetLinger(bool lingerOn, int lingerTime);
	bool	UpdateAcceptContext(SOCKET& listenSocket);
	bool	SetNodelay(bool nodelay);
	bool	SetRecvBufferSize(int size);

	bool	Bind(const char* ip, uint16_t port);
	bool	Listen(int backLog = SOMAXCONN);

	// for terminal



private:
	SOCKET			mSocket;
	SOCKADDR_IN		mAddrInfo;
};

}

