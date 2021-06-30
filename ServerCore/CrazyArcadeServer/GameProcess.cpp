#include "stdafx.h"
#include "GameProcess.h"



GameProcess::GameProcess()
{
	RegistFunction(ePacketType::CS_REQ_HELLO, std::bind(&GameProcess::CS_REQ_HELLO, this, std::placeholders::_1, std::placeholders::_2));
	RegistFunction(ePacketType::CS_SEND_INPUTLIST, std::bind(&GameProcess::CS_SEND_INPUTLIST, this, std::placeholders::_1, std::placeholders::_2));
	RegistFunction(ePacketType::CS_REQ_EXIT, std::bind(&GameProcess::CS_REQ_EXIT, this, std::placeholders::_1, std::placeholders::_2));

}



void GameProcess::CS_REQ_HELLO(Session* session, std::shared_ptr<Packet>& packet)
{
	using leeder::PK_CS_REQ_HELLO;
	using leeder::PK_SC_RES_WELCOME;

	auto reqPacket = std::static_pointer_cast<PK_CS_REQ_HELLO>(packet);

	std::string clientName = reqPacket->GetID();

	auto iocpSession = static_cast<IOCPSession*>(session);

	PK_SC_RES_WELCOME resPacket;

	resPacket.SetState(NetworkManagerServer::GetInstance().GetNetworkState());


	iocpSession->SendPacket(&resPacket);

	NetworkManagerServer::GetInstance().HandleNewClient(std::make_shared<User>(iocpSession, clientName, std::make_shared<PlayerServer>(session->GetID())));


}




void GameProcess::CS_REQ_EXIT(Session* session, std::shared_ptr<Packet>& packet)
{
	using leeder::PK_SC_RES_EXIT;

	auto iocpSession = static_cast<IOCPSession*>(session);

	//UserManager::GetInstance().RemoveUser(session->GetID());
	
	PK_SC_RES_EXIT resPacket;
	iocpSession->SendPacket(&resPacket);

}

void GameProcess::CS_SEND_INPUTLIST(Session* session, std::shared_ptr<Packet>& packet)
{
	using leeder::PK_CS_SEND_INPUTLIST;

	auto inputPacket = std::static_pointer_cast<PK_CS_SEND_INPUTLIST>(packet);

	std::vector<Input>& list = inputPacket->GetInputList();

	auto user = NetworkManagerServer::GetInstance().FindUserToSessionID(session->GetID());

	if (list.empty())
	{
		return;
	}

	if (user->GetGameObject()->IsDie() || user->GetGameObject()->GetMoveState() == eMoveState::TEMP_DIE || user->GetGameObject()->GetMoveState() == eMoveState::DIE)
	{
		return;
	}

	user->GetGameObject()->SetState(eObjectState::ACTION);
	NetworkManagerServer::GetInstance().SetObjectState(user->GetGameObject().get());

	for (auto input : list)
	{
		user->AddInput(input);
	}
	


}

void GameProcess::T_NOTIFY_AUTH(Session* session, std::shared_ptr<Packet>& packet)
{
}
