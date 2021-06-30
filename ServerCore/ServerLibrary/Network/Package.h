#pragma once
#include "stdafx.h"

namespace leeder
{
class Session;

class Package
{

public:
	Package(Session* session, std::shared_ptr<Packet>& packet)
		: mSession(session)
		, mPacket(std::move(packet))
	{};

	Session* GetSession() { return mSession; }
	std::shared_ptr<Packet>&	GetPacket() { return mPacket; }

private:
	Session* mSession;
	std::shared_ptr<Packet>		mPacket;

};

}