#pragma once
#include "stdafx.h"

namespace leeder
{

using PACKET_TYPE_SIZE = uint32_t;

enum class ePacketType : PACKET_TYPE_SIZE
{
	NONE,
	CS_REQ_HELLO,
	SC_RES_WELCOME,
	CS_REQ_EXIT,
	SC_RES_EXIT,
	CS_REQ_REPLICATION_STATE,
	SC_REPLICATION_STATE,
	CS_SEND_INPUTLIST,
	CS_NOTIFY_HEARTBEAT,
};

}

