#pragma once
#include "stdafx.h"
namespace leeder
{

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

		void	init();
		bool	Connect();

		void	OnConnect();
		void	Disconnect();

		eClientState	GetState() const { return mState; }
		void			SetState(eClientState state) { mState = state; }

		SOCKET& GetSocket() { return mSocket; }

		void					SendHelloPacket();
		std::shared_ptr<Packet> SendInputPacket();
		void SendHeartBeat();
		void SendRequestReplication();

		void Action();
		void SendReqExitPacket();


		void	RecvStandBy();
		bool	isRemainToRecv(size_t transferred);


		void					 OnSend(DWORD transferSize);
		std::shared_ptr<ClientPackage> OnRecv(DWORD transferSize);



		std::atomic<bool> mConnected;

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

		uint16_t		mLastHelloTime;
		uint16_t		mLastPacketSendTime;


		std::shared_ptr<ClientRWIOData>	mReadIO;
		std::shared_ptr<ClientRWIOData>	mWriteIO;

		std::list<eInputType> mInputList;


		

	};

};
