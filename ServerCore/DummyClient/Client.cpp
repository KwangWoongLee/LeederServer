#include "stdafx.h"
#include "Client.h"


namespace leeder
{
	Client::Client(std::string serverIP, uint16_t serverPort)
		: mServerIP(serverIP)
		, mServerPort(serverPort)
		, mLastHelloTime(0.f)
		, mState(eClientState::UNCONNECT)
		, mRandomActionCount(GetRandomInt())
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

		this->init();
	}


	Client::~Client()
	{
		SetState(eClientState::TERMINATE);

	}


	void Client::init()
	{
		mReadIO = std::make_shared<ClientRWIOData>(eClientIOType::RECV);
		mWriteIO = std::make_shared<ClientRWIOData>(eClientIOType::SEND);
	}

	bool Client::Connect()
	{

		SOCKADDR_IN serverInfo;

		memset(&serverInfo, 0, sizeof(serverInfo));
		serverInfo.sin_family = AF_INET;
		serverInfo.sin_port = htons(mServerPort);
		inet_pton(AF_INET, mServerIP.c_str(), &serverInfo.sin_addr.s_addr);

		std::shared_ptr<ClientIOData> data = std::make_shared<ConnectIOData>();
		data->SetClient(shared_from_this());

		ClientOverlapped* connectOverlapped = new ClientOverlapped(data);


		if (FALSE == mFnConnectEx(mSocket, (SOCKADDR*)&serverInfo, sizeof(serverInfo), NULL, 0, NULL, (LPOVERLAPPED)connectOverlapped))
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
		mConnected.store(true);

		mName = GetRandomString();

		mState = eClientState::HELLO;

		this->RecvStandBy();

		SendHelloPacket();

	}

	void Client::Disconnect()
	{
		mState = eClientState::TERMINATE;

		mReadIO->Reset();
		mWriteIO->Reset();


		closesocket(mSocket);

	}

	void Client::SendHelloPacket()
	{
		std::shared_ptr<PK_CS_REQ_HELLO> packet = std::make_shared<PK_CS_REQ_HELLO>();

		packet->SetID(mName);

		SendPacket(packet);

	}


	std::shared_ptr<Packet> Client::SendInputPacket()
	{
		if (mInputList.empty())
			return nullptr;

		std::shared_ptr<PK_CS_SEND_INPUTLIST> packet = std::make_shared<PK_CS_SEND_INPUTLIST>();

		for (auto type : mInputList)
		{
			packet->PushInputType(type);
		}

		mInputList.clear();

		SendPacket(packet);

		return nullptr;
	}

	void Client::SendHeartBeat()
	{
		std::shared_ptr<PK_CS_NOTIFY_HEARTBEAT> packet = std::make_shared<PK_CS_NOTIFY_HEARTBEAT>();
		SendPacket(std::move(packet));
	}

	void Client::SendRequestReplication()
	{
		std::shared_ptr<PK_CS_REQ_REPLICATION_STATE> packet = std::make_shared<PK_CS_REQ_REPLICATION_STATE>();
		SendPacket(std::move(packet));
	}

	void Client::Action()
	{
		////랜덤하게 action 하고 종료
		//if (!mRandomActionCount)
		//{
		//	SendReqExitPacket();
		//	return;
		//}
		for (int i = 0; i < 3; ++i)
		{
			auto type = (eInputType)(GetRandomInt() % 6);

			if (type != eInputType::NONE)
				mInputList.push_back(type);
		}

		SendInputPacket();
		
		--mRandomActionCount;

		
	}

	void Client::SendReqExitPacket()
	{
		std::shared_ptr<PK_CS_REQ_EXIT> packet = std::make_shared<PK_CS_REQ_EXIT>();

		SendPacket(std::move(packet));

		SetState(eClientState::READY_TO_TERMINATE);
	}

	void Client::RecvStandBy()
	{
		mReadIO->Reset();

		if (mReadIO->GetClient() == nullptr)
			mReadIO->SetClient(shared_from_this());


		mReadIO->SetType(eClientIOType::RECV);

		ClientOverlapped* RecvOverlapped = new ClientOverlapped(mReadIO);


		WSABUF wsaBuf;
		wsaBuf.buf = mReadIO->GetBuffer();
		wsaBuf.len = BUF_SIZE;

		this->recv(RecvOverlapped, wsaBuf);
	}

	bool Client::isRemainToRecv(size_t transferred)
	{
		if (mReadIO->IsRemainToIO(transferred)) {

			ClientOverlapped* RecvOverlapped = new ClientOverlapped(mReadIO);


			WSABUF wsaBuf;
			wsaBuf.buf = mReadIO->GetBuffer() + mReadIO->GetCurrentByte();
			wsaBuf.len = (ULONG)(mReadIO->GetTotalByte() - mReadIO->GetCurrentByte());

			this->recv(RecvOverlapped, wsaBuf);

			return true;
		}

		return false;
	}

	void Client::SendPacket(std::shared_ptr<Packet> packet)
	{
		OutputStream stream;
		packet->Encode(stream);

		mWriteIO->SetStreamToIOData(stream);

		if (mWriteIO->GetClient() == nullptr)
			mWriteIO->SetClient(shared_from_this());


		ClientOverlapped* SendOverlapped = new ClientOverlapped(mWriteIO);


		WSABUF wsaBuf;
		wsaBuf.buf = mWriteIO->GetBuffer();
		wsaBuf.len = mWriteIO->GetTotalByte();

		this->send(SendOverlapped, wsaBuf);
	}

	void Client::OnSend(DWORD transferSize)
	{
		if (!mConnected.load())
			return;

		if (mWriteIO->IsRemainToIO(transferSize))
		{
			ClientOverlapped* SendOverlapped = new ClientOverlapped(mWriteIO);


			WSABUF wsaBuf;
			wsaBuf.buf = mWriteIO->GetBuffer() + mWriteIO->GetCurrentByte();
			wsaBuf.len = (ULONG)(mWriteIO->GetTotalByte() - mWriteIO->GetCurrentByte());

			this->send(SendOverlapped, wsaBuf);

		}
	}

	std::shared_ptr<ClientPackage> Client::OnRecv(DWORD transferSize)
	{
		if (!mConnected.load())
			return nullptr;

		//패킷 가장 앞부분. 패킷 총길이를 가져오고 그만큼 offset 증가.
		size_t offset = mReadIO->SetTotalByte();

		if (this->isRemainToRecv(transferSize)) {
			return nullptr;
		}


		std::shared_ptr<Packet> packet = PacketAnalyzer::GetInstance().analyze(mReadIO->GetBuffer() + offset, mReadIO->GetTotalByte() - offset);


		if (packet == nullptr) {

			return nullptr;
		}

		this->RecvStandBy();

		std::shared_ptr<ClientPackage> package = std::make_shared<ClientPackage>(shared_from_this(), packet);

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


}