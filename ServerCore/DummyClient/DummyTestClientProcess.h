#pragma once
#include "stdafx.h"


class DummyTestClientProcess : public ClientContentsProcess
{
public:
	DummyTestClientProcess();

private:
	void	SC_RES_WELCOME(Client* client, std::shared_ptr<Packet>& packet);
	void	SC_REPLICATION_STATE(Client* client, std::shared_ptr<Packet>& packet);
	void	SC_RES_EXIT(Client* client, std::shared_ptr<Packet>& packet);
};