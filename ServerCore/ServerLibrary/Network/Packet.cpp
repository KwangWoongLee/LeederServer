#include "stdafx.h"
#include "Packet.h"

namespace leeder
{

void Packet_Test::Encode(OutputStream& stream)
{
	stream << stream.GetLength();
	stream << GetTypeToInt();
	stream << mID;

}

void Packet_Test::Decode(InputStream& stream)
{
	stream >> mID;

}

void PK_SC_MSG_NOTIFY_ALL::Encode(OutputStream& stream)
{
}

void PK_SC_MSG_NOTIFY_ALL::Decode(InputStream& stream)
{
}

void PK_CS_REQ_CHATTING_MSG::Encode(OutputStream& stream)
{
	stream << stream.GetLength();
	stream << GetTypeToInt();
	stream << mID;
}

void PK_CS_REQ_CHATTING_MSG::Decode(InputStream& stream)
{
	stream >> mID;
}

void PK_CS_REQ_PARTICIPATE_CHATTING::Encode(OutputStream& stream)
{
	stream << stream.GetLength();
	stream << GetTypeToInt();
	stream << mID;
}

void PK_CS_REQ_PARTICIPATE_CHATTING::Decode(InputStream& stream)
{
	stream >> mID;
}

void PK_SC_RES_WELCOME::Encode(OutputStream& stream)
{
}

void PK_SC_RES_WELCOME::Decode(InputStream& stream)
{
}

};
