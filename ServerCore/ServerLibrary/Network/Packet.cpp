#include "stdafx.h"
#include "Packet.h"

namespace leeder
{

void PK_SC_MSG_NOTIFY_ALL::Encode(OutputStream& stream)
{
	stream << GetTypeToInt();
	stream << mID;
	stream << mMSG;
}

void PK_SC_MSG_NOTIFY_ALL::Decode(InputStream& stream)
{
	stream >> mID;
	stream >> mMSG;
}

void PK_CS_REQ_CHATTING_MSG::Encode(OutputStream& stream)
{
	stream << GetTypeToInt();
	stream << mID;
	stream << mMSG;
}

void PK_CS_REQ_CHATTING_MSG::Decode(InputStream& stream)
{
	stream >> mID;
	stream >> mMSG;
}

void PK_CS_REQ_PARTICIPATE_CHATTING::Encode(OutputStream& stream)
{
	stream << GetTypeToInt();
	stream << mID;
}

void PK_CS_REQ_PARTICIPATE_CHATTING::Decode(InputStream& stream)
{
	stream >> mID;
}

void PK_SC_RES_WELCOME::Encode(OutputStream& stream)
{
	stream << GetTypeToInt();
}


void PK_CS_NOTIFY_HEARTBEAT::Encode(OutputStream& stream)
{
	stream << GetTypeToInt();
}

void PK_CS_REQ_EXIT::Encode(OutputStream& stream)
{
	stream << GetTypeToInt();
}

void PK_SC_RES_EXIT::Encode(OutputStream& stream)
{
	stream << GetTypeToInt();
}

void PK_CS_REQ_HELLO::Encode(OutputStream& stream)
{
	stream << GetTypeToInt();
	stream << mID;
}

void PK_CS_REQ_HELLO::Decode(InputStream& stream)
{
	stream >> mID;
}

void PK_SC_REPLICATION_STATE::Encode(OutputStream& stream)
{
	stream << GetTypeToInt();
	stream << mGameObjects;
}

void PK_SC_REPLICATION_STATE::Decode(InputStream& stream)
{
	stream >> mGameObjects;
}

void PK_CS_SEND_INPUTLIST::Encode(OutputStream& stream)
{
	stream << GetTypeToInt();
	stream << mInputList;
}

void PK_CS_SEND_INPUTLIST::Decode(InputStream& stream)
{
	stream >> mInputList;
}

void PK_CS_SEND_INPUTLIST::PushInputType(eInputType type)
{
	mInputList.push_back(type);
}

void PK_CS_REQ_REPLICATION_STATE::Encode(OutputStream& stream)
{
	stream << GetTypeToInt();
}

};
