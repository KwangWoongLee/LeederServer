#pragma once
#include "stdafx.h"

namespace leeder
{

	class DummyTestClientProcess : public ClientContentsProcess
	{
	public:
		DummyTestClientProcess();

	private:
		void	SC_RES_WELCOME(std::shared_ptr<Client>& client, std::shared_ptr<Packet>& packet);
		void	SC_REPLICATION_STATE(std::shared_ptr<Client>& client, std::shared_ptr<Packet>& packet);
		void	SC_RES_EXIT(std::shared_ptr<Client>& client, std::shared_ptr<Packet>& packet);
	};

}

