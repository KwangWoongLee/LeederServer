#include "stdafx.h"
#include "GameProcess.h"



GameProcess::GameProcess()
{
	RegistFunction(ePacketType::CS_REQ_HELLO, std::bind(&GameProcess::CS_REQ_HELLO, this, std::placeholders::_1, std::placeholders::_2));
	RegistFunction(ePacketType::CS_SEND_INPUTLIST, std::bind(&GameProcess::CS_SEND_INPUTLIST, this, std::placeholders::_1, std::placeholders::_2));
	RegistFunction(ePacketType::CS_REQ_EXIT, std::bind(&GameProcess::CS_REQ_EXIT, this, std::placeholders::_1, std::placeholders::_2));

}



void GameProcess::CS_REQ_HELLO(std::shared_ptr<Session>& session, std::shared_ptr<Packet>& packet)
{
	using leeder::PK_CS_REQ_HELLO;
	using leeder::PK_SC_RES_WELCOME;

	auto reqPacket = std::static_pointer_cast<PK_CS_REQ_HELLO>(packet);

	std::string clientName = reqPacket->GetID();

	NetworkManager::GetInstance().HandleNewClient(std::make_shared<User>(session, clientName, std::make_shared<Player>(session->GetID())));

	auto iocpSession = std::static_pointer_cast<IOCPSession>(session);

	PK_SC_RES_WELCOME resPacket;

	iocpSession->SendPacket(&resPacket);

}




void GameProcess::CS_REQ_EXIT(std::shared_ptr<Session>& session, std::shared_ptr<Packet>& packet)
{
	using leeder::PK_SC_RES_EXIT;

	auto iocpSession = std::static_pointer_cast<IOCPSession>(session);

	//UserManager::GetInstance().RemoveUser(session->GetID());
	
	PK_SC_RES_EXIT resPacket;
	iocpSession->SendPacket(&resPacket);

}

void GameProcess::CS_SEND_INPUTLIST(std::shared_ptr<Session>& session, std::shared_ptr<Packet>& packet)
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
