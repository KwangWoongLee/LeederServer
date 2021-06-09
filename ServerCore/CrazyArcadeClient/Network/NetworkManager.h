#pragma once
#include "stdafx.h"

namespace leeder
{


enum class eClientState
{
	UNREADY,
	READY,
	WELCOMED,
	TERMINATE

};

class NetworkManager : public Singleton<NetworkManager>
{
public:

	std::unordered_map<uint32_t, std::shared_ptr<GameObject>> mNetworkIDToGameObjectMap;

	NetworkManager();
	~NetworkManager();


	bool Init();
	void Run();

	void registPacketFunction();

	void RecvPacketProcess();
	void SendPacketProcess();

	void RecvPacket();
	void SendPacket(std::shared_ptr<Packet> packet);


	void	PutRecvPacket(std::shared_ptr<Packet>&& packet);
	std::shared_ptr<Packet> GetRecvPacket();


	//Getter and Setter
	SOCKET& GetSocket() { return mSocket; }

	eClientState GetState() { return mState; }


	std::shared_ptr<Packet> SendHelloPacket();
	void SendHeartBeat();
	void SendReqExitPacket();


private:
	// ÇïÆÛ ÇÔ¼ö
	bool createSocket();
	bool connect();


	SOCKET mSocket;
	eClientState mState;

	XMLDocument mConfig;

	std::atomic<bool>	mConnected;




	SOCKADDR_IN		mServerInfo;

	std::string		mServerIP;
	uint16_t		mServerPort;

	char			mBuffer[BUF_SIZE];



	uint16_t		mLastHelloTime;
	uint16_t		mLastPacketSendTime;
	uint16_t		mLastHeartBeatTime;




	//
	ThreadSafeQueue<std::shared_ptr<Packet>>	mRecvPacketQueue;

	std::unique_ptr<Thread>	mRecvThread;

	std::unordered_map<ePacketType, std::function<void(std::shared_ptr<Packet>&)>> mPacketToFunctionMap;

};

};