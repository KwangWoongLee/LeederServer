#pragma once
#include "stdafx.h"

namespace leeder
{

class IOCPServer;

class IOCPSession : public Session , public std::enable_shared_from_this<IOCPSession>
{
public:
	IOCPSession();
	~IOCPSession();


	void	RecvStandBy();
	void	SendPacket(Packet* packet);

	void	Accept(SOCKET listenSocket);
	void	ZeroRecv();


	std::list<IOCPSession*>::iterator	OnDisconnect(eDisconnectReason reason);
	void	OnAccept(IOCPServer* server);
	void	OnZeroRecv();
	
	void						OnSend(DWORD transferSize);
	std::shared_ptr<Package>	OnRecv(DWORD transferSize);


	void Reset();

private:
	void	recv(Overlapped* overlapped, WSABUF wsaBuf);
	void	send(Overlapped* overlapped, WSABUF wsaBuf);

	bool	setSocketOption(IOCPServer* server);


	bool	checkIOError(DWORD error);




	std::mutex		mBufferMutex;

	CircularBuffer	mBuffer;
	char			mSendBuffer[BUF_SIZE];


};
}

