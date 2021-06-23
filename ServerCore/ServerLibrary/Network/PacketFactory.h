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
		case ePacketType::SC_RES_EXIT:
			return std::make_shared<PK_SC_RES_EXIT>();
			break;

		case ePacketType::CS_REQ_EXIT:
			return std::make_shared<PK_CS_REQ_EXIT>();
			break;

		case ePacketType::CS_NOTIFY_HEARTBEAT:
			return std::make_shared<PK_CS_NOTIFY_HEARTBEAT>();
			break;

		case ePacketType::CS_REQ_HELLO:
			return std::make_shared<PK_CS_REQ_HELLO>();
			break;

		case ePacketType::SC_RES_WELCOME:
			return std::make_shared<PK_SC_RES_WELCOME>();
			break;

		case ePacketType::CS_REQ_REPLICATION_STATE:
			return std::make_shared<PK_CS_REQ_REPLICATION_STATE>();
			break;

		case ePacketType::SC_REPLICATION_STATE:
			return std::make_shared<PK_SC_REPLICATION_STATE>();
			break;

		case ePacketType::CS_SEND_INPUTLIST:
			return std::make_shared<PK_CS_SEND_INPUTLIST>();
			break;

		case ePacketType::T_NOTIFY:
			return std::make_shared<PK_T_NOTIFY>();
			break;

		case ePacketType::T_NOTIFY_AUTH:
			return std::make_shared<PK_T_NOTIFY_AUTH>();
			break;

		case ePacketType::CS_REQ_AUTH:
			return std::make_shared<PK_CS_REQ_AUTH>();
			break;

		case ePacketType::DB_RES_AUTH:
			return std::make_shared<PK_DB_RES_AUTH>();
			break;
		default:
			return nullptr;
			break;
		}
	}
};

}
