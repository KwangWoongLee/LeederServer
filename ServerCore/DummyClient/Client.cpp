#include "stdafx.h"
#include "Client.h"


Client::Client(std::string serverIP, uint16_t serverPort)
	: mServerIP(serverIP)
	, mServerPort(serverPort)
	, mLastHelloTime(0.f)
	, mLastPacketSendTime(0.f)
	, mLastHeartBeatTime(0.f)
	, mState(eClientState::UNCONNECT)
	, mRandomActionCount(leeder::GetRandomInt())
	, mBuffer(BUF_SIZE)
{
	mSocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, nullptr, 0, WSA_FLAG_OVERLAPPED);
	if (mSocket == INVALID_SOCKET) {
		printf("socket error %d\n", WSAGetLastError());
		//에러처리
	}

	SOCKADDR_IN addr;
	ZeroMemory(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = 0;

	if (::bind(mSocket, (SOCKADDR*)&addr, sizeof(addr)) == SOCKET_ERROR)
	{
		printf("bind error %d\n", WSAGetLastError());
		//에러처리
	}

}


Client::~Client()
{
	SetState(eClientState::TERMINATE);

}


bool Client::Connect()
{

	SOCKADDR_IN serverInfo;

	memset(&serverInfo, 0, sizeof(serverInfo));
	serverInfo.sin_family = AF_INET;
	serverInfo.sin_port = htons(mServerPort);
	inet_pton(AF_INET, mServerIP.c_str(), &serverInfo.sin_addr.s_addr);

	ClientOverlapped* connectOverlapped = new ClientOverlapped(this);
	connectOverlapped->SetType(eClientIOType::CONNECT);


	if (FALSE == leeder::mFnConnectEx(mSocket, (SOCKADDR*)&serverInfo, sizeof(serverInfo), NULL, 0, NULL, (LPOVERLAPPED)connectOverlapped))
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
		{
			delete connectOverlapped;
			return false;
		}
	}

	setsockopt(mSocket, SOL_SOCKET, SO_UPDATE_CONNECT_CONTEXT, NULL, 0);
		
	return true;
}

void Client::OnConnect()
{
	if (true == mConnected.exchange(true))
	{
		printf("Session is already connected");
		return;
	}

	mName = leeder::GetRandomString();

	mState = eClientState::HELLO;

	SendHelloPacket();

}

void Client::Disconnect()
{
	mState = eClientState::TERMINATE;

	closesocket(mSocket);

}

void Client::ZeroRecv()
{
	if (!mConnected.load())
		return;

	ClientOverlapped* ZeroRecvOverlapped = new ClientOverlapped(this);
	ZeroRecvOverlapped->SetClient(this);
	ZeroRecvOverlapped->SetType(eClientIOType::RECV_ZERO);
	auto wsabuf = ZeroRecvOverlapped->GetWSABuf();
	wsabuf.len = 0;
	wsabuf.buf = nullptr;

	DWORD recvbytes = 0;
	DWORD flags = 0;

	/// start async recv
	if (SOCKET_ERROR == WSARecv(mSocket, &wsabuf, 1, &recvbytes, &flags, (LPWSAOVERLAPPED)ZeroRecvOverlapped, NULL))
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
		{
			delete ZeroRecvOverlapped;
		}
	}
}

void Client::OnZeroRecv()
{
	this->RecvStandBy();
}

void Client::SendHelloPacket()
{
	using leeder::PK_CS_REQ_HELLO;

	std::shared_ptr<PK_CS_REQ_HELLO> packet = std::make_shared<PK_CS_REQ_HELLO>();

	packet->SetID(mName);

	SendPacket(packet);

	this->ZeroRecv();

}


std::shared_ptr<Packet> Client::SendInputPacket()
{
	using leeder::PK_CS_SEND_INPUTLIST;

	//if (mInputList.empty())
	//	return nullptr;

	std::shared_ptr<PK_CS_SEND_INPUTLIST> packet = std::make_shared<PK_CS_SEND_INPUTLIST>();

	for (int i = 0; i < 10; ++i)
	{
		int a = leeder::GetRandomInt() % 6;

		Input input = (eInputType)a;
		mInputList.push_back(input);
	}

	for (auto type : mInputList)
	{
		packet->PushInputType(type);
	}


	SendPacket(packet);

	mInputList.clear();

	return nullptr;
}

void Client::SendHeartBeat()
{
	using leeder::PK_CS_NOTIFY_HEARTBEAT;

	std::shared_ptr<PK_CS_NOTIFY_HEARTBEAT> packet = std::make_shared<PK_CS_NOTIFY_HEARTBEAT>();
	SendPacket(std::move(packet));


}


void Client::SendReqExitPacket()
{
	using leeder::PK_CS_REQ_EXIT;

	std::shared_ptr<PK_CS_REQ_EXIT> packet = std::make_shared<PK_CS_REQ_EXIT>();

	SendPacket(std::move(packet));

	SetState(eClientState::TERMINATE);
}

void Client::RecvStandBy()
{
	if (!mConnected.load())
		return;

	std::unique_lock<std::mutex> lock(mBufferMutex);

	ClientOverlapped* RecvOverlapped = new ClientOverlapped(this);
	RecvOverlapped->SetType(eClientIOType::RECV);
	RecvOverlapped->SetClient(this);

	auto wsaBuf = RecvOverlapped->GetWSABuf();

	wsaBuf.buf = mBuffer.GetBuffer();
	wsaBuf.len = (ULONG)mBuffer.GetFreeSpaceSize();

	this->recv(RecvOverlapped, wsaBuf);
}

void Client::SendPacket(std::shared_ptr<Packet> packet)
{
	OutputStream stream;
	packet->Encode(stream);


	if (!mConnected.load())
	{
		return;
	}

	//패킷 총길이
	leeder::PACKET_SIZE packetTotalLength[1] = { sizeof(leeder::PACKET_SIZE) + sizeof(float) + stream.GetLength() };


	size_t	offset = sizeof(leeder::PACKET_SIZE);

	//패킷 총길이 입력
	memcpy((void*)mSendBuffer, (void*)packetTotalLength, sizeof(leeder::PACKET_SIZE));

	float time = Clock::GetInstance().GetSystemTimeFloat();
	memcpy((void*)(mSendBuffer + offset), &time, sizeof(float));

	offset += sizeof(float);

	// 스트림 데이터 입력
	memcpy((void*)(mSendBuffer + offset), (void*)stream.GetBuffer(), stream.GetLength());



	ClientOverlapped* sendOverlapped = new ClientOverlapped(this);
	sendOverlapped->SetType(eClientIOType::SEND);
	sendOverlapped->SetClient(this);


	auto wsaBuf = sendOverlapped->GetWSABuf();
	wsaBuf.len = (ULONG)packetTotalLength[0];
	wsaBuf.buf = mSendBuffer;


	this->send(sendOverlapped, wsaBuf);
}

void Client::OnSend(DWORD transferSize)
{
	if (!mConnected.load())
		return;

	ZeroMemory(mSendBuffer, sizeof(BUF_SIZE) * sizeof(char));
}

std::shared_ptr<ClientPackage> Client::OnRecv(DWORD transferSize)
{
	if (!mConnected.load())
		return nullptr;

	std::unique_lock<std::mutex> lock(mBufferMutex);

	if (0 == mBuffer.GetFreeSpaceSize())
		return nullptr;

	size_t offset = 0;
	leeder::PACKET_SIZE packetSize[1] = { 0 };

	memcpy((void*)packetSize, (void*)mBuffer.GetBuffer(), sizeof(leeder::PACKET_SIZE));

	offset += sizeof(leeder::PACKET_SIZE);

	float recvTime[1] = { 0 };
	memcpy((void*)recvTime, (void*)(mBuffer.GetBuffer() + offset), sizeof(float));

	offset += sizeof(float);


	std::shared_ptr<Packet> packet = leeder::PacketAnalyzer::GetInstance().analyze(mBuffer.GetBuffer() + offset, packetSize[0] - offset);


	mBuffer.Commit(transferSize);

	mBuffer.Remove(transferSize);


	if (packet == nullptr) {

		return nullptr;
	}

	this->ZeroRecv();

	std::shared_ptr<ClientPackage> package = std::make_shared<ClientPackage>(this, packet);

	return std::move(package);
}

void Client::recv(ClientOverlapped* overlapped, WSABUF wsaBuf)
{
	if (mState == eClientState::UNCONNECT || mState == eClientState::TERMINATE)
		return;

	DWORD flags = 0;
	DWORD recvBytes;

	DWORD error = WSARecv(mSocket, &wsaBuf, 1, &recvBytes, &flags, (LPOVERLAPPED)overlapped, NULL);

	this->checkIOError(error);
}

void Client::send(ClientOverlapped* overlapped, WSABUF wsaBuf)
{
	if (mState == eClientState::UNCONNECT || mState == eClientState::TERMINATE )
		return;

	DWORD flags = 0;
	DWORD sendBytes;
	DWORD error = WSASend(mSocket, &wsaBuf, 1, &sendBytes, flags, (LPOVERLAPPED)overlapped, NULL);

	this->checkIOError(error);
}

void Client::checkIOError(DWORD error)
{
	if (error == SOCKET_ERROR
		&& (WSAGetLastError() != WSA_IO_PENDING)) {

		if (WSAGetLastError() == WSAECONNRESET)
		{
			// 서버소켓 끊김
			this->Disconnect();
			return;
		}

		printf("! socket error: %d", WSAGetLastError());
	}
}

