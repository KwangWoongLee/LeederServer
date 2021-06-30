#include "stdafx.h"
#include "DummyTestProcess.h"

namespace leeder
{

DummyTestProcess::DummyTestProcess()
{
	RegistFunction(ePacketType::CS_REQ_HELLO, std::bind(&DummyTestProcess::CS_REQ_HELLO, this, std::placeholders::_1, std::placeholders::_2));
	RegistFunction(ePacketType::CS_SEND_INPUTLIST, std::bind(&DummyTestProcess::CS_SEND_INPUTLIST, this, std::placeholders::_1, std::placeholders::_2));
	RegistFunction(ePacketType::CS_REQ_EXIT, std::bind(&DummyTestProcess::CS_REQ_EXIT, this, std::placeholders::_1, std::placeholders::_2));
	
}



void DummyTestProcess::CS_REQ_HELLO(Session* session, std::shared_ptr<Packet>& packet)
{
	using leeder::PK_CS_REQ_HELLO;
	using leeder::PK_SC_RES_WELCOME;

	auto reqPacket = std::static_pointer_cast<PK_CS_REQ_HELLO>(packet);

	std::string clientName = reqPacket->GetID();

	auto iocpSession = static_cast<IOCPSession*>(session);

	PK_SC_RES_WELCOME resPacket;

	resPacket.SetState(NetworkManager::GetInstance().GetNetworkState());

	iocpSession->SendPacket(&resPacket);


	NetworkManager::GetInstance().HandleNewClient(std::make_shared<User>(iocpSession, clientName));

}

void DummyTestProcess::CS_SEND_INPUTLIST(Session* session, std::shared_ptr<Packet>& packet)
{
	using leeder::PK_CS_SEND_INPUTLIST;

	auto inputPacket = std::static_pointer_cast<PK_CS_SEND_INPUTLIST>(packet);

	std::vector<Input>& list = inputPacket->GetInputList();

	if (list.empty())
		return;

	auto user = NetworkManager::GetInstance().FindUserToSessionID(session->GetID());

	user->SetState(eObjectState::ACTION);
	NetworkManager::GetInstance().SetObjectState(user);

	for (auto input : list)
	{
		user->AddInput(input);
	}


}


void DummyTestProcess::CS_REQ_EXIT(Session* session, std::shared_ptr<Packet>& packet)
{

	PK_SC_RES_EXIT resPacket;
	auto iocpSession = static_cast<IOCPSession*>(session);

	iocpSession->SendPacket(&resPacket);

}


};