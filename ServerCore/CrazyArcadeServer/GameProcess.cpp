#include "stdafx.h"
#include "GameProcess.h"


GameProcess::GameProcess()
{
	RegistFunction(ePacketType::CS_REQ_HELLO, std::bind(&GameProcess::CS_REQ_HELLO, this, std::placeholders::_1, std::placeholders::_2));
	RegistFunction(ePacketType::CS_REQ_EXIT, std::bind(&GameProcess::CS_REQ_EXIT, this, std::placeholders::_1, std::placeholders::_2));

}



void GameProcess::CS_REQ_HELLO(std::shared_ptr<Session>& session, std::shared_ptr<Packet>& packet)
{
	auto reqPacket = std::static_pointer_cast<PK_CS_REQ_HELLO>(packet);

	std::string clientID = reqPacket->GetID();

	auto iocpSession = std::static_pointer_cast<IOCPSession>(session);

	PK_SC_RES_WELCOME resPacket;

	iocpSession->SendPacket(&resPacket);


}




void GameProcess::CS_REQ_EXIT(std::shared_ptr<Session>& session, std::shared_ptr<Packet>& packet)
{
	auto iocpSession = std::static_pointer_cast<IOCPSession>(session);


	PK_SC_RES_EXIT resPacket;
	iocpSession->SendPacket(&resPacket);

}

