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

	void Init();

	void	RecvStandBy();
	bool	isRemainToRecv(size_t transferred);

	void	SendPacket(Packet* packet);

	void	Accept(SOCKET listenSocket);


	std::list<std::shared_ptr<IOCPSession>>::iterator	OnDisconnect(eDisconnectReason reason);
	void	OnAccept(IOCPServer* server);
	
	void					 OnSend(DWORD transferSize);
	std::shared_ptr<Package> OnRecv(DWORD transferSize);


	void Reset();

private:
	void	recv(Overlapped* overlapped, WSABUF wsaBuf);
	void	send(Overlapped* overlapped, WSABUF wsaBuf);

	bool	setSocketOption(IOCPServer* server);


	void	checkIOError(DWORD error);




	std::shared_ptr<RWIOData>	mReadIO;
	std::shared_ptr<RWIOData>	mWriteIO;

};
}

