#pragma once
#include "stdafx.h"

namespace leeder
{

using PACKET_TYPE_SIZE = uint32_t;

enum class ePacketType : PACKET_TYPE_SIZE
{
	NONE,
	TEST
};

}

