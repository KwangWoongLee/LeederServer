#include "stdafx.h"
#include "Socket.h"

leeder::Socket::Socket()
{
	mSocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, nullptr, 0, WSA_FLAG_OVERLAPPED);
	ZeroMemory(&mAddrInfo, sizeof(mAddrInfo));

	if (mSocket == INVALID_SOCKET)
	{
		//에러처리
	}
}

leeder::Socket::~Socket()
{
	closesocket(mSocket);
}

bool leeder::Socket::ReuseAddr(bool reuse)
{
	return false;
}

bool leeder::Socket::SetLinger(bool lingerOn, int lingerTime)
{
	LINGER lingerOption;
	lingerOption.l_onoff = lingerOn ? 1 : 0;
	lingerOption.l_linger = lingerTime;

	return SOCKET_ERROR != setsockopt(mSocket, SOL_SOCKET, SO_LINGER, (const char*)&lingerOption, sizeof(LINGER));
}

bool leeder::Socket::UpdateAcceptContext(Socket* s)
{
	return SOCKET_ERROR != setsockopt(mSocket, SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT, (char*)&s->mSocket, sizeof(SOCKET));
}

bool leeder::Socket::SetNodelay(bool nodelay)
{
	int opt = nodelay ? 1 : 0;
	return SOCKET_ERROR != setsockopt(mSocket, IPPROTO_TCP, TCP_NODELAY, (const char*)&opt, sizeof(int));
}

int leeder::Socket::Bind(const char* ip, uint16_t port)
{

	mAddrInfo.sin_family = AF_INET;
	mAddrInfo.sin_port = htons(port);
	inet_pton(AF_INET, ip, &(mAddrInfo.sin_addr));

	return SOCKET_ERROR != ::bind(mSocket, (SOCKADDR*)&mAddrInfo, sizeof(mAddrInfo));

}

int leeder::Socket::Listen(int backLog)
{
	return SOCKET_ERROR != ::listen(mSocket, backLog);
}
