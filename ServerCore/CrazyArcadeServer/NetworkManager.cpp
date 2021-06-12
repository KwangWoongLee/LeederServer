#include "stdafx.h"
#include "NetworkManager.h"

void NetworkManager::Init(std::shared_ptr<ContentsProcess> process)
{
	mProcess = process;
}

void NetworkManager::ProcessQueuedPacket()
{
	while (!mProcess->GetPackageQueue().Empty())
	{
		std::shared_ptr<Package> package = nullptr;
		package = mProcess->GetPackage();

		if (!package)
			continue;

		mProcess->ProcessPackage(package);
	}
}

void NetworkManager::Replication()
{
	for (auto element : mSessionIDToUser)
	{
		auto user = element.second;

		SendReplicationPacket(user->GetSession());
	}

	mNetworkIDToState.clear();
}

void NetworkManager::HandleNewClient(std::shared_ptr<User> user)
{
	mSessionIDToUser[user->GetSessionID()] = user;

	auto obj = user->GetGameObject();
	AddGameObjectToNetwork(obj);
	World::GetInstance().AddGameObject(obj);



}

void NetworkManager::AddGameObjectToNetwork(std::shared_ptr<GameObject> obj)
{
	obj->SetNetworkID(mNetworkIDSeed++);
	mNetworkIDToGameObject[obj->GetNetworkID()] = obj;

	obj->SetState(eObjectState::CREATE);
	SetObjectState(obj);

}

void NetworkManager::RemoveGameObjectToNetwork(std::shared_ptr<GameObject>& obj)
{
	mNetworkIDToGameObject.erase(obj->GetNetworkID());
	obj->SetState(eObjectState::DESTROY);
	SetObjectState(obj);
}

void NetworkManager::SetObjectState(std::shared_ptr<GameObject> obj)
{
	mNetworkIDToState[obj->GetNetworkID()] = obj->GetState();
}

void NetworkManager::SendReplicationPacket(const std::shared_ptr<Session>& session)
{
	using leeder::PK_SC_REPLICATION_STATE;

	auto iocpSession = std::static_pointer_cast<IOCPSession>(session);

	PK_SC_REPLICATION_STATE replicationPacket;

	replicationPacket.SetState(mNetworkIDToState);
	
	iocpSession->SendPacket(&replicationPacket);
}

std::shared_ptr<User> NetworkManager::FindUserToSessionID(uint32_t sessionID)
{

	auto iter = mSessionIDToUser.find(sessionID);
	if (iter != mSessionIDToUser.end())
		return iter->second;

	return nullptr;
}
