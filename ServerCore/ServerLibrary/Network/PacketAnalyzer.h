#pragma once
#include "stdafx.h"

namespace leeder
{

class PacketAnalyzer : public Singleton< PacketAnalyzer>
{
public:
	std::shared_ptr<Packet> analyze(const char* buffer, size_t size);
	
};


}
