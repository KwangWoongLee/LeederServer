#pragma once
#include "stdafx.h"

namespace leeder
{
class IOCPSession;


class Package
{

public:
	Package(IOCPSession* session, std::shared_ptr<Packet>& packet)
		: mSession(session)
		, mPacket(std::move(packet))
	{};

	IOCPSession* GetSession() { return mSession; }
	std::shared_ptr<Packet>&	GetPacket() { return mPacket; }

private:
	IOCPSession* mSession;
	std::shared_ptr<Packet>		mPacket;

};

}