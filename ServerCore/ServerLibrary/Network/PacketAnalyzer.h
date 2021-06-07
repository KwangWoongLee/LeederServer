#pragma once
#include "stdafx.h"

namespace leeder
{

// read IO buffer���� PacketType�� �о�
// �ش� ��Ŷ�� �����Ͽ� ���� Stream ���� �о����

class PacketAnalyzer : public Singleton< PacketAnalyzer>
{
public:
	std::shared_ptr<Packet> analyze(const char* buffer, size_t size);
	
};


}
