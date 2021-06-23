#include "stdafx.h"
#include "Packet.h"

namespace leeder
{
void PK_SC_RES_WELCOME::Encode(OutputStream& stream)
{
	stream << GetTypeToInt();
	stream << mNetworkIDToState;
}

void PK_SC_RES_WELCOME::Decode(InputStream& stream)
{
	stream >> mNetworkIDToState;
}

void PK_SC_RES_WELCOME::SetState(std::unordered_map<uint32_t, std::shared_ptr<GameObject>>& networkIDToGameObject)
{
	for (auto element : networkIDToGameObject)
	{
		mNetworkIDToState[element.first] = *element.second;
	}
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
	stream << mNetworkIDToInfo;
}

void PK_SC_REPLICATION_STATE::Decode(InputStream& stream)
{
	stream >> mNetworkIDToInfo;
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

void PK_CS_SEND_INPUTLIST::PushInputType(Input input)
{
	mInputList.push_back(input);
}

void PK_CS_REQ_REPLICATION_STATE::Encode(OutputStream& stream)
{
	stream << GetTypeToInt();
}

void PK_T_NOTIFY::Encode(OutputStream& stream)
{
	stream << GetTypeToInt();
}

void PK_T_NOTIFY_AUTH::Encode(OutputStream& stream)
{
	stream << GetTypeToInt();
	stream << mbAuthResult;
	if (IsAuth())
	{
		stream << mName;
		stream << mAuthToken;
	}

}

void PK_T_NOTIFY_AUTH::Decode(InputStream& stream)
{
	stream >> mbAuthResult;
	if (IsAuth())
	{
		stream >> mName;
		stream >> mAuthToken;
	}
}

void PK_CS_REQ_AUTH::Encode(OutputStream& stream)
{
	stream << GetTypeToInt();
	stream << mID;
	stream << mPassWord;
}

void PK_CS_REQ_AUTH::Decode(InputStream& stream)
{
	stream >> mID;
	stream >> mPassWord;
}

void PK_DB_RES_AUTH::Encode(OutputStream& stream)
{
	stream << GetTypeToInt();
	stream << mbAuthResult;
	if (IsAuth())
	{
		stream << mName;
		stream << mAuthToken;
	}
}

void PK_DB_RES_AUTH::Decode(InputStream& stream)
{
	stream >> mbAuthResult;
	if (IsAuth())
	{
		stream >> mName;
		stream >> mAuthToken;
	}
}



};
