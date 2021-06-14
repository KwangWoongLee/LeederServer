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

	mNetworkIDToInfo.clear();
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
	ObjectInfo info(obj->GetState(), obj->GetType(), obj->GetPosition());
	mNetworkIDToInfo[obj->GetNetworkID()] = info;
}

void NetworkManager::SendReplicationPacket(IOCPSession* session)
{
	if (mNetworkIDToInfo.size() == 0)
		return;

	using leeder::PK_SC_REPLICATION_STATE;

	auto iocpSession = session;

	PK_SC_REPLICATION_STATE replicationPacket;

	replicationPacket.SetInfo(mNetworkIDToInfo);



	iocpSession->SendPacket(&replicationPacket);
}

std::shared_ptr<User> NetworkManager::FindUserToSessionID(uint32_t sessionID)
{

	auto iter = mSessionIDToUser.find(sessionID);
	if (iter != mSessionIDToUser.end())
		return iter->second;

	return nullptr;
}
