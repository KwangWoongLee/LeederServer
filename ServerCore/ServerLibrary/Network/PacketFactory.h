#pragma once
#include "stdafx.h"

namespace leeder
{

class PacketFactory : public Singleton<PacketFactory>
{
public:
	Packet* GetPacket(ePacketType type)
	{
		switch (type)
		{
		case ePacketType::TEST:
			return new Packet_Test();
			break;
		default:
			break;
		}
	}
};

}
