#pragma once
#include "stdafx.h"

namespace leeder
{

using PACKET_TYPE_SIZE = uint32_t;

enum class ePacketType : PACKET_TYPE_SIZE
{
	NONE,
	TEST,
	SC_RES_WELCOME,
	CS_REQ_PARTICIPATE_CHATTING,
	CS_REQ_CHATTING_MSG,
	SC_MSG_NOTIFY_ALL
};

}

