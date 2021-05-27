#pragma once
#include "stdafx.h"

namespace leeder
{

// read IO buffer에서 PacketType을 읽어
// 해당 패킷을 생성하여 리턴 Stream 값을 읽어오고

class PacketAnalyzer : public Singleton< PacketAnalyzer>
{
public:
	std::shared_ptr<Packet> analyze(const char* buffer, size_t size);
	
};


}
