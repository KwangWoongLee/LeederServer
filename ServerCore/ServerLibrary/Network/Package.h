#pragma once
#include "stdafx.h"

namespace leeder
{
class Session;


class Package
{

public:
	Package(std::shared_ptr<Session> session, std::shared_ptr<Packet>& packet)
		: mSession(session)
		, mPacket(std::move(packet))
	{};

	std::shared_ptr<Session>&	GetSession() { return mSession; }
	std::shared_ptr<Packet>&	GetPacket() { return mPacket; }

private:
	std::shared_ptr<Session>	mSession;
	std::shared_ptr<Packet>		mPacket;

};

}