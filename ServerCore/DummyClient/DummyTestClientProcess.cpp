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

		client->RecvStandBy();

	}

	void DummyTestClientProcess::SC_REPLICATION_STATE(std::shared_ptr<Client>& client, std::shared_ptr<Packet>& packet)
	{
		std::shared_ptr<PK_SC_REPLICATION_STATE> typePacket = std::static_pointer_cast<PK_SC_REPLICATION_STATE>(packet);

		auto objects = typePacket->GetGameObjects();

		for (auto object : objects)
		{
			auto netid = object->GetNetworkID();
			printf("id : %d \n", netid);

			//if (object->GetState() == eObjectState::CREATE)
			//{
			//	mNetworkIDToGameObjectMap.insert(std::make_pair(netid, object));
			//}

			//else if (object->GetState() == eObjectState::ACTION)
			//{
			//	mNetworkIDToGameObjectMap[netid] = object;
			//}

			//else if (object->GetState() == eObjectState::DESTROY)
			//{
			//	mNetworkIDToGameObjectMap.erase(netid);
			//}

		}

	}

	void DummyTestClientProcess::SC_RES_EXIT(std::shared_ptr<Client>& client, std::shared_ptr<Packet>& packet)
	{
		client->SetState(eClientState::TERMINATE);
		
	}



};