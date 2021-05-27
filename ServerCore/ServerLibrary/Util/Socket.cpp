#include "stdafx.h"
#include "Socket.h"

namespace leeder
{
Socket::Socket()
{
	mSocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, nullptr, 0, WSA_FLAG_OVERLAPPED);
	ZeroMemory(&mAddrInfo, sizeof(mAddrInfo));

	if (mSocket == INVALID_SOCKET)
	{
		printf("invalid socket %d" , WSAGetLastError());
		//¿¡·¯Ã³;
	}
}

Socket::~Socket()
{
	closesocket(mSocket);
}

void Socket::Reset()
{
	SetLinger(true,0);

	closesocket(mSocket);

	mSocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, nullptr, 0, WSA_FLAG_OVERLAPPED);
	ZeroMemory(&mAddrInfo, sizeof(mAddrInfo));

}

std::wstring Socket::GetClientAddress()
{
	std::array<char, 64> ip;
	inet_ntop(AF_INET, &(mAddrInfo.sin_addr), ip.data(), ip.size());


	return CA2W(ip.data());
}

bool Socket::ReuseAddr(bool reuse)
{
	return false;
}

bool Socket::SetLinger(bool lingerOn, int lingerTime = 0)
{
	LINGER lingerOption;
	lingerOption.l_onoff = lingerOn ? 1 : 0;
	lingerOption.l_linger = lingerTime;

	return SOCKET_ERROR != setsockopt(mSocket, SOL_SOCKET, SO_LINGER, (const char*)&lingerOption, sizeof(LINGER));
}

bool Socket::UpdateAcceptContext(SOCKET& listenSocket)
{
	return SOCKET_ERROR != setsockopt(mSocket, SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT, (char*)&listenSocket, sizeof(SOCKET));
}


bool Socket::SetRecvBufferSize(int size)
{
	return SOCKET_ERROR != setsockopt(mSocket, SOL_SOCKET, SO_RCVBUF, (const char*)&size, sizeof(int));
}

bool Socket::SetNodelay(bool nodelay)
{
	int opt = nodelay ? 1 : 0;
	return SOCKET_ERROR != setsockopt(mSocket, IPPROTO_TCP, TCP_NODELAY, (const char*)&opt, sizeof(int));
}

bool Socket::Bind(const char* ip, uint16_t port)
{

	mAddrInfo.sin_family = AF_INET;
	mAddrInfo.sin_port = htons(port);
	inet_pton(AF_INET, ip, &(mAddrInfo.sin_addr));

	return SOCKET_ERROR != ::bind(mSocket, (SOCKADDR*)&mAddrInfo, sizeof(mAddrInfo));

}

bool Socket::Listen(int backLog)
{
	return SOCKET_ERROR != ::listen(mSocket, backLog);
}

}
