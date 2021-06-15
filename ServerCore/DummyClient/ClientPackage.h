#pragma once
#include "stdafx.h"

class Client;


class ClientPackage
{

public:
	ClientPackage(Client* client, std::shared_ptr<Packet>& packet)
		: mClient(client)
		, mPacket(std::move(packet))
	{};

	Client* GetClient() { return mClient; }
	std::shared_ptr<Packet>& GetPacket() { return mPacket; }

private:
	Client*	mClient;
	std::shared_ptr<Packet>		mPacket;

};
