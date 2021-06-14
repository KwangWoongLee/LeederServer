#include "stdafx.h"
#include "DummyTestClientProcess.h"

namespace leeder
{

	DummyTestClientProcess::DummyTestClientProcess()
	{
		RegistFunction(ePacketType::SC_RES_WELCOME, std::bind(&DummyTestClientProcess::SC_RES_WELCOME, this, std::placeholders::_1, std::placeholders::_2));
		RegistFunction(ePacketType::SC_RES_EXIT, std::bind(&DummyTestClientProcess::SC_RES_EXIT, this, std::placeholders::_1, std::placeholders::_2));
		RegistFunction(ePacketType::SC_REPLICATION_STATE, std::bind(&DummyTestClientProcess::SC_REPLICATION_STATE, this, std::placeholders::_1, std::placeholders::_2));

	}

	void DummyTestClientProcess::SC_RES_WELCOME(std::shared_ptr<Client>& client, std::shared_ptr<Packet>& packet)
	{
		client->SetState(eClientState::WELCOMED);


	}

	void DummyTestClientProcess::SC_REPLICATION_STATE(std::shared_ptr<Client>& client, std::shared_ptr<Packet>& packet)
	{

		std::shared_ptr<PK_SC_REPLICATION_STATE> typePacket = std::static_pointer_cast<PK_SC_REPLICATION_STATE>(packet);

		auto networkIDToState = typePacket->GetState();

		printf("Repl");
		for (auto object : networkIDToState)
		{
			auto netid = object.first;
			auto state = object.second;

			printf("id : %d - ", netid);

			switch ((state))
			{
			case eObjectState::CREATE:
				printf("CREATE\n");
				break;
			case eObjectState::ACTION:
				printf("ACTION\n");
				break;
			case eObjectState::DESTROY:
				printf("DESTROY\n");
				break;

			default:
				break;
			}

		}


	}

	void DummyTestClientProcess::SC_RES_EXIT(std::shared_ptr<Client>& client, std::shared_ptr<Packet>& packet)
	{
		client->SetState(eClientState::TERMINATE);
		
	}



};