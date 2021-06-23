#include "stdafx.h"
#include "NetworkManagerClient.h"


std::function<void(NetworkManagerClient*)> RecvThreadFunction = [](NetworkManagerClient* network) {

	while (!bShutDown)
	{
		network->RecvPacket();
	}
};



NetworkManagerClient::NetworkManagerClient()
	: mSocket(INVALID_SOCKET)
	, mServerIP("")
	, mServerPort(0)
	, mState(eClientState::UNREADY)
	, mLastHelloTime(0.f)
{
	ZeroMemory(mBuffer, sizeof(mBuffer));
	ZeroMemory(&mServerInfo, sizeof(SOCKADDR_IN));


}

NetworkManagerClient::~NetworkManagerClient()
{
	closesocket(mSocket);

	WSACleanup();
}


bool NetworkManagerClient::Init()
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

	XMLElement* loginServer = app->FirstChildElement("LoginServer");

	if (!loginServer) {
		printf("No setting for login server in config");
		return false;
	}

	XMLElement* element = loginServer->FirstChildElement("IP");
	mServerIP = element->GetText();

	element = loginServer->FirstChildElement("Port");
	std::string strPort = element->GetText();
	mServerPort = std::stoi(strPort);


	memset(&mServerInfo, 0, sizeof(mServerInfo));
	mServerInfo.sin_family = AF_INET;
	mServerInfo.sin_port = htons(mServerPort);
	inet_pton(AF_INET, mServerIP.c_str(), &mServerInfo.sin_addr.s_addr);


	this->auth();

	if (!createSocket())
		return false;

	XMLElement* server = app->FirstChildElement("Server");

	if (!server) {
		printf("No setting for server in config");
		return false;
	}

	element = server->FirstChildElement("IP");
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
void NetworkManagerClient::Run()
{
	mRecvThread = std::make_unique<Thread>(RecvThreadFunction, this);

}

void NetworkManagerClient::registPacketFunction()
{
	mPacketToFunctionMap.insert(std::make_pair(ePacketType::SC_RES_WELCOME, [&](const auto& packet) {
		using leeder::PK_SC_RES_WELCOME;
		std::shared_ptr<PK_SC_RES_WELCOME> typePacket = std::static_pointer_cast<PK_SC_RES_WELCOME>(packet);

		auto map = typePacket->GetState();

		for (auto element : map)
		{
			auto netid = element.first;
			auto obj = element.second;

			std::shared_ptr<GameObject> newObject = ObjectFactory::GetInstance().CreateObject(obj.GetType());

			if (newObject == nullptr)
				continue;

			newObject->SetNetworkID(netid);
			newObject->SetPosition(obj.GetPosition());
			newObject->SetScale(obj.GetScale());


			AddGameObjectToNetwork(newObject);

			World::GetInstance().AddGameObject(newObject);
		}

		mState = eClientState::WELCOMED;
		}));

	mPacketToFunctionMap.insert(std::make_pair(ePacketType::SC_RES_EXIT, [&](const auto& packet) {

		leeder::bShutDown = true;
		mState = eClientState::TERMINATE;

		SDL_Quit();

		}));

	mPacketToFunctionMap.insert(std::make_pair(ePacketType::SC_REPLICATION_STATE, [&](const auto& packet) {
		using leeder::PK_SC_REPLICATION_STATE;

		std::shared_ptr<PK_SC_REPLICATION_STATE> typePacket = std::static_pointer_cast<PK_SC_REPLICATION_STATE>(packet);

		auto networkIDToInfo = typePacket->GetInfo();

		for (auto object : networkIDToInfo)
		{
			auto netid = object.first;
			auto info = object.second;

			printf("id : %d - ", netid);
			auto iter = mNetworkIDToGameObject.find(netid);
			if (iter == mNetworkIDToGameObject.end() && info.mState != eObjectState::CREATE)
				continue;


			switch (info.mState)
			{
			case eObjectState::CREATE:
			{
				printf("CREATE : ");

				std::shared_ptr<GameObject> newObject = ObjectFactory::GetInstance().CreateObject(info.mType);

				if (newObject == nullptr)
					continue;

				if (info.mMoveState != eMoveState::NONE)
				{
					auto object = std::static_pointer_cast<MoveGameObject>(newObject);
					object->SetMoveState(info.mMoveState);
				}
				newObject->SetNetworkID(netid);
				newObject->SetPosition(info.mPos);
				newObject->SetScale(info.mScale);


				AddGameObjectToNetwork(newObject);
				World::GetInstance().AddGameObject(newObject);

				break;
			}

			case eObjectState::ACTION:
			{
				printf("ACTION");
				printf(" : %d\n", info.mMoveState);
				auto obj = std::static_pointer_cast<MoveGameObject>(iter->second);
				obj->SetPosition(info.mPos);
				obj->SetMoveState(info.mMoveState);
				obj->SetScale(info.mScale);
				obj->SetLastActionTime();

				break;
			}
			case eObjectState::DESTROY:
				printf("DESTROY : ");
				iter->second->Die();
				mNetworkIDToGameObject.erase(iter->first);
				break;

			default:
				break;
			}

		}

		}));

}

void NetworkManagerClient::RecvPacketProcess()
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

void NetworkManagerClient::SendPacketProcess()
{
	std::shared_ptr<Packet> packet = nullptr;

	auto time = Clock::GetInstance().GetSystemTimeFloat();

	if (mState == eClientState::READY)
	{

		if (time > mLastHelloTime + 0.4f)
		{
			packet = SendHelloPacket();
			mLastHelloTime = time;
			printf("hello");
		}
	}

	else if (mState == eClientState::WELCOMED)
	{
		if (time > mLastPacketSendTime + 0.03f)
		{
			packet = SendInputPacket();
			mLastPacketSendTime = time;

		}

		//HeartBeat 패킷 전송
		if (time > mLastHeartBeatTime + 3.f)
		{
			SendHeartBeat();
			mLastHeartBeatTime = time;
		}

	}


	if (packet != nullptr)
		SendPacket(std::move(packet));

}







// Helper ---------------------------------------------------------------//


std::shared_ptr<Packet> NetworkManagerClient::SendHelloPacket()
{
	using leeder::PK_CS_REQ_HELLO;
	std::shared_ptr<PK_CS_REQ_HELLO> packet = std::make_shared<PK_CS_REQ_HELLO>();

	packet->SetID(leeder::GetRandomString());

	return packet;
}


std::shared_ptr<Packet> NetworkManagerClient::SendInputPacket()
{
	using leeder::PK_CS_SEND_INPUTLIST;
	std::deque<Input>& inputList = ::InputManager::GetInstance().GetInputList();

	if (inputList.empty())
		return nullptr;

	std::shared_ptr<PK_CS_SEND_INPUTLIST> packet = std::make_shared<PK_CS_SEND_INPUTLIST>();

	int loopSize = std::min((int)inputList.size(), 3);

	int startIndex = loopSize < 3 ? 0 : (int)inputList.size() - 3;

	for (int i = startIndex; i < inputList.size(); ++i)
	{
		packet->PushInputType(inputList[i]);
	}

	inputList.clear();

	return packet;
}



void NetworkManagerClient::SendHeartBeat()
{
	using leeder::PK_CS_NOTIFY_HEARTBEAT;
	std::shared_ptr<PK_CS_NOTIFY_HEARTBEAT> packet = std::make_shared<PK_CS_NOTIFY_HEARTBEAT>();
	SendPacket(std::move(packet));

	printf("heartbeat\n");
}

void NetworkManagerClient::SendReqExitPacket()
{
	using leeder::PK_CS_REQ_EXIT;
	std::shared_ptr<PK_CS_REQ_EXIT> packet = std::make_shared<PK_CS_REQ_EXIT>();

	SendPacket(std::move(packet));
}


bool NetworkManagerClient::createSocket()
{
	mSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (mSocket == INVALID_SOCKET)
	{
		printf("Socket Create Failed %d", GetLastError());
		return false;
	}

	return true;
}

bool NetworkManagerClient::connect()
{
	if (::connect(mSocket, (SOCKADDR*)&mServerInfo, sizeof(mServerInfo)) == SOCKET_ERROR)
	{
		printf("Connect Error : %d", GetLastError());
		return false;
	}

	mConnected = true;


	return true;
}

void NetworkManagerClient::auth()
{
	if (!createSocket())
		return ;

	while (true)
	{
		if (!connect())
		{
			Sleep(100);
			continue;
		}

		std::string id;
		std::string password;

		printf("id를 입력하세요 : ");
		std::cin >> id;

		printf("password를 입력하세요 : ");
		std::cin >> password;

		using leeder::PK_CS_REQ_AUTH;
		std::shared_ptr<PK_CS_REQ_AUTH> packet = std::make_shared<PK_CS_REQ_AUTH>();

		packet->SetID(id);
		packet->SetPassWord(password);

		SendPacket(packet);

		char buf[10240];

		recv(mSocket, buf, 10240, 0);
		size_t offset = 0;
		size_t size[1] = { 0 };
		memcpy(size, buf, sizeof(leeder::PACKET_SIZE));

		offset += sizeof(leeder::PACKET_SIZE);

		std::shared_ptr<leeder::Packet> packet = leeder::PacketAnalyzer::GetInstance().analyze(buf + offset, size[0] - offset);

		if (packet == nullptr) {
			continue;
		}


	}

	closesocket(mSocket);
}

void NetworkManagerClient::RecvPacket()
{
	char buf[10240];

	recv(mSocket, buf, 10240, 0);
	size_t offset = 0;
	size_t size[1] = { 0 };
	memcpy(size, buf, sizeof(leeder::PACKET_SIZE));

	offset += sizeof(leeder::PACKET_SIZE);

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
		leeder::ASSERT(false);
		return;
	}

	PutRecvPacket(std::move(packet));
}

void NetworkManagerClient::SendPacket(std::shared_ptr<Packet> packet)
{
	OutputStream stream;
	packet->Encode(stream);

	size_t offset = 0;

	leeder::PACKET_SIZE packetLength[1] = { sizeof(leeder::PACKET_SIZE) + sizeof(float) + stream.GetLength() };

	memcpy(mBuffer, packetLength, sizeof(leeder::PACKET_SIZE));

	offset += sizeof(leeder::PACKET_SIZE);

	float packetSendTime[1] = { Clock::GetInstance().GetSystemTimeFloat() };

	memcpy(mBuffer + offset, packetSendTime, sizeof(float));

	offset += sizeof(float);


	memcpy(mBuffer + offset, stream.GetBuffer(), stream.GetLength());

	offset += stream.GetLength();

	send(mSocket, mBuffer, offset, 0);
}



void NetworkManagerClient::PutRecvPacket(std::shared_ptr<Packet>&& packet)
{
	mRecvPacketQueue.Push(packet);
}

std::shared_ptr<Packet> NetworkManagerClient::GetRecvPacket()
{
	return mRecvPacketQueue.Pop();
}



void NetworkManagerClient::AddGameObjectToNetwork(std::shared_ptr<GameObject> obj)
{
	mNetworkIDToGameObject[obj->GetNetworkID()] = obj;
	printf("type : %d \n" , obj->GetType());

}