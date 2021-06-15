#pragma once
#include "stdafx.h"

enum class eClientState
{
	UNCONNECT,
	HELLO,
	WELCOMED,
	READY_TO_TERMINATE,
	TERMINATE,

};

class Client :  public std::enable_shared_from_this<Client>
{
public:
	Client(std::string serverIP, uint16_t serverPort);
	virtual ~Client();

	bool	Connect();

	void	OnConnect();
	void	Disconnect();
	void	ZeroRecv();
	void	OnZeroRecv();

	eClientState	GetState() const { return mState; }
	void			SetState(eClientState state) { mState = state; }

	SOCKET& GetSocket() { return mSocket; }

	void					SendHelloPacket();
	std::shared_ptr<Packet> SendInputPacket();
	void SendHeartBeat();

	void SendReqExitPacket();


	void	RecvStandBy();

	void					 OnSend(DWORD transferSize);
	std::shared_ptr<ClientPackage> OnRecv(DWORD transferSize);



	std::atomic<bool> mConnected;

	float			mLastHelloTime;
	float			mLastPacketSendTime;
	float			mLastHeartBeatTime;

private:

	void SendPacket(std::shared_ptr<Packet> packet);

	void	recv(ClientOverlapped* overlapped, WSABUF wsaBuf);
	void	send(ClientOverlapped* overlapped, WSABUF wsaBuf);
	void	checkIOError(DWORD error);



	SOCKET			mSocket;

	std::string		mServerIP;
	uint16_t		mServerPort;

	eClientState	mState;
	std::string		mName;

	uint16_t		mRandomActionCount;




	std::mutex		mBufferMutex;

	CircularBuffer	mBuffer;
	char			mSendBuffer[BUF_SIZE];

	std::list<Input> mInputList;


		

};

