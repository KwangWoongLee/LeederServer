#include "stdafx.h"
#include "NetworkManager.h"


namespace leeder
{

std::function<void(NetworkManager*)> RecvThreadFunction = [](NetworkManager* network) {

	while (!bShutDown)
	{
		network->RecvPacket();
	}
};



NetworkManager::NetworkManager()
	: mSocket(INVALID_SOCKET)
	, mServerIP("")
	, mServerPort(0)
	, mState(eClientState::UNREADY)
	, mLastHelloTime(0.f)
{
	ZeroMemory(mBuffer, sizeof(mBuffer));
	ZeroMemory(&mServerInfo, sizeof(SOCKADDR_IN));


}

NetworkManager::~NetworkManager()
{
	closesocket(mSocket);

	WSACleanup();
}


bool NetworkManager::Init()
{
	// 윈속 초기화
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != NO_ERROR)
	{
		printf("WSA Start Error : %d", WSAGetLastError());
		return false;
	}

	const char* path = "./config.xml";

	if (!loadConfig(&mConfig, path))
	{
		printf("Client Config Reading is failed\n");
		exit(0);
		return false;
	}

	XMLElement* app = mConfig.FirstChildElement("App");

	XMLElement* server = app->FirstChildElement("Server");

	if (!server) {
		printf("No setting for server in config");
		return false;
	}

	XMLElement* element = server->FirstChildElement("IP");
	mServerIP = element->GetText();

	element = server->FirstChildElement("Port");
	sscanf_s(element->GetText(), "%d", &mServerPort);


	memset(&mServerInfo, 0, sizeof(mServerInfo));
	mServerInfo.sin_family = AF_INET;
	mServerInfo.sin_port = htons(mServerPort);
	inet_pton(AF_INET, mServerIP.c_str(), &mServerInfo.sin_addr.s_addr);

	if (!createSocket())
		return false;

	int opt = 1;
	setsockopt(mSocket, IPPROTO_TCP, TCP_NODELAY, (const char*)&opt, sizeof(int));


	if (!connect())
		return false;

	registPacketFunction();


	mState = eClientState::READY;

	return true;
}
void NetworkManager::Run()
{
	mRecvThread = std::make_unique<Thread>(RecvThreadFunction, this);

}

void NetworkManager::registPacketFunction()
{
	mPacketToFunctionMap.insert(std::make_pair(ePacketType::SC_RES_WELCOME, [&](const auto& packet) {
		mState = eClientState::WELCOMED;
		}));

	mPacketToFunctionMap.insert(std::make_pair(ePacketType::SC_RES_EXIT, [&](const auto& packet) {
		
		bShutDown = true;
		mState = eClientState::TERMINATE;

		SDL_Quit();

		}));

	mPacketToFunctionMap.insert(std::make_pair(ePacketType::SC_REPLICATION_STATE, [&](const auto& packet) {

		std::shared_ptr<PK_SC_REPLICATION_STATE> typePacket = std::static_pointer_cast<PK_SC_REPLICATION_STATE>(packet);

		auto networkIDToState = typePacket->GetState();

		for (auto object : networkIDToState)
		{
			auto netid = object.first;
			auto state = object.second;

			printf("id : %d - ", netid);

			switch ((state))
			{
			case eObjectState::CREATE:
				printf("CREATE\n");
				break;
			case eObjectState::ACTION:
				printf("ACTION\n");
				break;
			case eObjectState::DESTROY:
				printf("DESTROY\n");
				break;

			default:
				break;
			}

		}

		}));

}

void NetworkManager::RecvPacketProcess()
{
	while (!mRecvPacketQueue.Empty())
	{
		auto packet = GetRecvPacket();

		if (packet == nullptr)
			return;

		auto iter = mPacketToFunctionMap.find(packet->GetType());

		if (iter != mPacketToFunctionMap.end())
			(iter->second)(packet);

	}
}

void NetworkManager::SendPacketProcess()
{
	std::shared_ptr<Packet> packet = nullptr;

	auto time = Clock::GetInstance().GetSystemTimeFloat();

	if (mState == eClientState::READY)
	{

		if (time > mLastHelloTime )
		{
			packet = SendHelloPacket();
			mLastHelloTime = time;
		}
	}

	else if (mState == eClientState::WELCOMED)
	{
		if (time > mLastPacketSendTime + 0.0000003f)
		{
			packet = SendInputPacket();
			mLastPacketSendTime = time;
		}

		//HeartBeat 패킷 전송
		if (time > mLastHeartBeatTime + 0.003f)
		{
			SendHeartBeat();
			mLastHeartBeatTime = time;
		}

	}


	if (packet != nullptr)
		SendPacket(std::move(packet));

}







// Helper ---------------------------------------------------------------//


std::shared_ptr<Packet> NetworkManager::SendHelloPacket()
{
	std::shared_ptr<PK_CS_REQ_HELLO> packet = std::make_shared<PK_CS_REQ_HELLO>();

	packet->SetID(GetRandomString());

	return packet;
}


std::shared_ptr<Packet> NetworkManager::SendInputPacket()
{
	std::deque<Input>& inputList = ::InputManager::GetInstance().GetInputList();

	if (inputList.empty())
		return nullptr;

	std::shared_ptr<PK_CS_SEND_INPUTLIST> packet = std::make_shared<PK_CS_SEND_INPUTLIST>();

	int loopSize = std::min((int)inputList.size(), 3);

	int startIndex = loopSize < 3 ?  0 : (int)inputList.size() - 3;

	for (int i = startIndex; i < inputList.size(); ++i)
	{
		packet->PushInputType(inputList[i]);
	}

	inputList.clear();

	return packet;
}



void NetworkManager::SendHeartBeat()
{
	std::shared_ptr<PK_CS_NOTIFY_HEARTBEAT> packet = std::make_shared<PK_CS_NOTIFY_HEARTBEAT>();
	SendPacket(std::move(packet));

	printf("heartbeat\n");
}

void NetworkManager::SendReqExitPacket()
{
	std::shared_ptr<PK_CS_REQ_EXIT> packet = std::make_shared<PK_CS_REQ_EXIT>();

	SendPacket(std::move(packet));
}


bool NetworkManager::createSocket()
{
	mSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (mSocket == INVALID_SOCKET)
	{
		printf("Socket Create Failed %d", GetLastError());
		return false;
	}

	return true;
}

bool NetworkManager::connect()
{
	if (::connect(mSocket, (SOCKADDR*)&mServerInfo, sizeof(mServerInfo)) == SOCKET_ERROR)
	{
		printf("Connect Error : %d", GetLastError());
		return false;
	}

	mConnected = true;


	return true;
}

void NetworkManager::RecvPacket()
{
	char buf[10240];

	recv(mSocket, buf, 10240, 0);
	size_t offset = 0;
	size_t size[1] = { 0 };
	memcpy(size, buf, sizeof(PACKET_SIZE));

	offset += sizeof(PACKET_SIZE);

	float packetRecvTime[1] = { 0 };
	memcpy(packetRecvTime, buf + offset, sizeof(float));

	offset += sizeof(float);

	std::shared_ptr<Packet> packet = PacketAnalyzer::GetInstance().analyze(buf + offset, size[0] - offset);

	if (packet == nullptr) {
		if (mState == eClientState::TERMINATE)
			return;

		if (WSAECONNRESET == GetLastError())
		{
			mState = eClientState::TERMINATE;
			//bClientShutDown = true;
			return;
		}


		printf("Packet is not analyzed by Analyzer, Error : %d \n", GetLastError());
		ASSERT(false);
		return;
	}

	PutRecvPacket(std::move(packet));
}

void NetworkManager::SendPacket(std::shared_ptr<Packet> packet)
{
	OutputStream stream;
	packet->Encode(stream);

	size_t offset = 0;

	PACKET_SIZE packetLength[1] = { sizeof(PACKET_SIZE) + sizeof(float) +  stream.GetLength() };

	memcpy(mBuffer, packetLength, sizeof(PACKET_SIZE));

	offset += sizeof(PACKET_SIZE);

	float packetSendTime[1] = { Clock::GetInstance().GetSystemTimeFloat() };

	memcpy(mBuffer + offset, packetSendTime, sizeof(float));

	offset += sizeof(float);


	memcpy(mBuffer + offset, stream.GetBuffer(), stream.GetLength());

	offset += stream.GetLength();

	send(mSocket, mBuffer, offset, 0);
}



void NetworkManager::PutRecvPacket(std::shared_ptr<Packet>&& packet)
{
	mRecvPacketQueue.Push(packet);
}

std::shared_ptr<Packet> NetworkManager::GetRecvPacket()
{
	return mRecvPacketQueue.Pop();
}

}