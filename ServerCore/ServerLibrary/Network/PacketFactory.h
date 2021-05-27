#pragma once
#include "stdafx.h"

namespace leeder
{

class PacketFactory : public Singleton<PacketFactory>
{
public:
	std::shared_ptr<Packet> GetPacket(ePacketType type)
	{
		switch (type)
		{
		case ePacketType::TEST:
			return std::make_shared<Packet_Test>();
			break;

		case ePacketType::SC_RES_WELCOME:
			return std::make_shared<PK_SC_RES_WELCOME>();
			break;

		case ePacketType::CS_REQ_PARTICIPATE_CHATTING:
			return std::make_shared<PK_CS_REQ_PARTICIPATE_CHATTING>();
			break;

		case ePacketType::CS_REQ_CHATTING_MSG:
			return std::make_shared<PK_CS_REQ_CHATTING_MSG>();
			break;

		case ePacketType::SC_MSG_NOTIFY_ALL:
			return std::make_shared<PK_SC_MSG_NOTIFY_ALL>();
			break;
		default:
			return nullptr;
			break;
		}
	}
};

}
