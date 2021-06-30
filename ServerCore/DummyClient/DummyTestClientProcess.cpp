#include "stdafx.h"
#include "DummyTestClientProcess.h"


DummyTestClientProcess::DummyTestClientProcess()
{
	RegistFunction(ePacketType::SC_RES_WELCOME, std::bind(&DummyTestClientProcess::SC_RES_WELCOME, this, std::placeholders::_1, std::placeholders::_2));
	RegistFunction(ePacketType::SC_RES_EXIT, std::bind(&DummyTestClientProcess::SC_RES_EXIT, this, std::placeholders::_1, std::placeholders::_2));
	RegistFunction(ePacketType::SC_REPLICATION_STATE, std::bind(&DummyTestClientProcess::SC_REPLICATION_STATE, this, std::placeholders::_1, std::placeholders::_2));

}

void DummyTestClientProcess::SC_RES_WELCOME(Client* client, std::shared_ptr<Packet>& packet)
{
	client->SetState(eClientState::WELCOMED);
}

void DummyTestClientProcess::SC_REPLICATION_STATE(Client* client, std::shared_ptr<Packet>& packet)
{
}

void DummyTestClientProcess::SC_RES_EXIT(Client* client, std::shared_ptr<Packet>& packet)
{
	using leeder::PK_SC_RES_EXIT;

	client->SetState(eClientState::TERMINATE);
		
}

