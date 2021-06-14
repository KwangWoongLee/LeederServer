#include "stdafx.h"
#include "GameProcess.h"



GameProcess::GameProcess()
{
	RegistFunction(ePacketType::CS_REQ_HELLO, std::bind(&GameProcess::CS_REQ_HELLO, this, std::placeholders::_1, std::placeholders::_2));
	RegistFunction(ePacketType::CS_SEND_INPUTLIST, std::bind(&GameProcess::CS_SEND_INPUTLIST, this, std::placeholders::_1, std::placeholders::_2));
	RegistFunction(ePacketType::CS_REQ_EXIT, std::bind(&GameProcess::CS_REQ_EXIT, this, std::placeholders::_1, std::placeholders::_2));

}



void GameProcess::CS_REQ_HELLO(IOCPSession* session, std::shared_ptr<Packet>& packet)
{
	using leeder::PK_CS_REQ_HELLO;
	using leeder::PK_SC_RES_WELCOME;

	auto reqPacket = std::static_pointer_cast<PK_CS_REQ_HELLO>(packet);

	std::string clientName = reqPacket->GetID();

	auto iocpSession = session;

	PK_SC_RES_WELCOME resPacket;

	resPacket.SetState(NetworkManager::GetInstance().GetNetworkState());


	iocpSession->SendPacket(&resPacket);

	NetworkManager::GetInstance().HandleNewClient(std::make_shared<User>(session, clientName, std::make_shared<PlayerServer>(session->GetID())));


}




void GameProcess::CS_REQ_EXIT(IOCPSession* session, std::shared_ptr<Packet>& packet)
{
	using leeder::PK_SC_RES_EXIT;

	auto iocpSession = session;

	//UserManager::GetInstance().RemoveUser(session->GetID());
	
	PK_SC_RES_EXIT resPacket;
	iocpSession->SendPacket(&resPacket);

}

void GameProcess::CS_SEND_INPUTLIST(IOCPSession* session, std::shared_ptr<Packet>& packet)
{
	using leeder::PK_CS_SEND_INPUTLIST;

	auto inputPacket = std::static_pointer_cast<PK_CS_SEND_INPUTLIST>(packet);

	std::vector<Input>& list = inputPacket->GetInputList();

	if (list.empty())
		return;

	auto user = NetworkManager::GetInstance().FindUserToSessionID(session->GetID());

	user->GetGameObject()->SetState(eObjectState::ACTION);
	NetworkManager::GetInstance().SetObjectState(user->GetGameObject());

	for (auto input : list)
	{
		user->AddInput(input);
	}
	


}
