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
	bool	isRemainToRecv(size_t transferred);
	void	Accept(SOCKET listenSocket);


	void	OnAccept(IOCPServer* server);
	void	OnZeroRecv();
	void    OnRecv(DWORD transferSize);

	void Reset();

private:
	void	recv(Overlapped* overlapped, WSABUF wsaBuf);
	void	send(Overlapped* overlapped, WSABUF wsaBuf);

	bool	setSocketOption(IOCPServer* server);


	void	checkIOError(DWORD error);


	std::atomic<bool>		mConnected;

	RWIOData	mReadIO;
	RWIOData	mWriteIO;

};
}

