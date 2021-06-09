#include "stdafx.h"
#include "DummyTestProcess.h"

namespace leeder
{

DummyTestProcess::DummyTestProcess()
{
	RegistFunction(ePacketType::CS_REQ_HELLO, std::bind(&DummyTestProcess::CS_REQ_HELLO, this, std::placeholders::_1, std::placeholders::_2));
	RegistFunction(ePacketType::CS_SEND_INPUTLIST, std::bind(&DummyTestProcess::CS_SEND_INPUTLIST, this, std::placeholders::_1, std::placeholders::_2));
	RegistFunction(ePacketType::CS_REQ_EXIT, std::bind(&DummyTestProcess::CS_REQ_EXIT, this, std::placeholders::_1, std::placeholders::_2));
	
	UserManager::GetInstance().mNetworkIDSeed = 0;
}



void DummyTestProcess::CS_REQ_HELLO(std::shared_ptr<Session>& session, std::shared_ptr<Packet>& packet)
{
	auto reqPacket = std::static_pointer_cast<PK_CS_REQ_HELLO>(packet);

	std::string clientID =reqPacket->GetID();

	auto iocpSession = std::static_pointer_cast<IOCPSession>(session);

	auto user = UserManager::GetInstance().At(session->GetID());

	if (user != nullptr) {
		SysLogger::GetInstance().Log(L"Already Registed User");
		iocpSession->OnDisconnect(eDisconnectReason::ALREADY_REGISTED);
		return;
	}
	

	UserManager::GetInstance().Put(std::make_shared<User>(session, clientID));

	PK_SC_RES_WELCOME resPacket;

	iocpSession->SendPacket(&resPacket);


}


void DummyTestProcess::CS_SEND_INPUTLIST(std::shared_ptr<Session>& session, std::shared_ptr<Packet>& packet)
{
	auto inputPacket = std::static_pointer_cast<PK_CS_SEND_INPUTLIST>(packet);
	
	auto& inputList = inputPacket->GetInputList();


	UserManager::GetInstance().InputUpdate(session->GetID(), inputList);

}


void DummyTestProcess::CS_REQ_EXIT(std::shared_ptr<Session>& session, std::shared_ptr<Packet>& packet)
{
	auto iocpSession = std::static_pointer_cast<IOCPSession>(session);

	auto user = UserManager::GetInstance().At(session->GetID());
	if (user == nullptr) {
		SysLogger::GetInstance().Log(L"This user is not registed ");
		iocpSession->OnDisconnect(eDisconnectReason::UNREGISTED);
		return;
	}

	UserManager::GetInstance().Remove(session->GetID());


	PK_SC_RES_EXIT resPacket;
	iocpSession->SendPacket(&resPacket);

}


};