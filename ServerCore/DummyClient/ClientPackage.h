#pragma once
#include "stdafx.h"

namespace leeder
{
	class Client;


	class ClientPackage
	{

	public:
		ClientPackage(std::shared_ptr<Client> client, std::shared_ptr<Packet>& packet)
			: mClient(client)
			, mPacket(std::move(packet))
		{};

		std::shared_ptr<Client>& GetClient() { return mClient; }
		std::shared_ptr<Packet>& GetPacket() { return mPacket; }

	private:
		std::shared_ptr<Client>	mClient;
		std::shared_ptr<Packet>		mPacket;

	};

}