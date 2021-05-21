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

};
