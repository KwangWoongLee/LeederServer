#include "stdafx.h"
#include "ChattingProcess.h"

namespace leeder
{
ChattingProcess::ChattingProcess()
{
	RegistFunction(ePacketType::SC_RES_WELCOME, std::bind(&ChattingProcess::SC_RES_WELCOME, this, std::placeholders::_1, std::placeholders::_2));
	RegistFunction(ePacketType::CS_REQ_PARTICIPATE_CHATTING, std::bind(&ChattingProcess::CS_REQ_PARTICIPATE_CHATTING, this, std::placeholders::_1, std::placeholders::_2));
	RegistFunction(ePacketType::CS_REQ_CHATTING_MSG, std::bind(&ChattingProcess::CS_REQ_CHATTING_MSG, this, std::placeholders::_1, std::placeholders::_2));
	RegistFunction(ePacketType::SC_MSG_NOTIFY_ALL, std::bind(&ChattingProcess::SC_MSG_NOTIFY_ALL, this, std::placeholders::_1, std::placeholders::_2));
}

void ChattingProcess::SC_RES_WELCOME(std::shared_ptr<Session>& session, std::shared_ptr<Packet>& packet)
{

}

void ChattingProcess::CS_REQ_PARTICIPATE_CHATTING(std::shared_ptr<Session>& session, std::shared_ptr<Packet>& packet)
{
	auto p = std::static_pointer_cast<PK_CS_REQ_PARTICIPATE_CHATTING>(packet);

	printf("\nHello, Can I Participated in your chat ? my ID is %s \n", p->GetID().c_str());

}

void ChattingProcess::CS_REQ_CHATTING_MSG(std::shared_ptr<Session>& session, std::shared_ptr<Packet>& packet)
{
}

void ChattingProcess::SC_MSG_NOTIFY_ALL(std::shared_ptr<Session>& session, std::shared_ptr<Packet>& packet)
{
	printf("\nNOTIFY ALL\n");
}


}