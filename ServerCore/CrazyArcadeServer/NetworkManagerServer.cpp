#include "stdafx.h"
#include "NetworkManagerServer.h"

void NetworkManagerServer::Init(std::shared_ptr<ContentsProcess> process)
{
	mProcess = process;
}

void NetworkManagerServer::ProcessQueuedPacket()
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

void NetworkManagerServer::Replication()
{
	auto time = Clock::GetInstance().GetSystemTimeFloat();

	if (time > mLastReplicationTime + 0.03f)
	{

		for (auto element : mSessionIDToUser)
		{
			auto user = element.second;

			SendReplicationPacket(user->GetSession());
		}

		mNetworkIDToInfo.clear();

		mLastReplicationTime = time;
	}

}

void NetworkManagerServer::HandleNewClient(std::shared_ptr<User> user)
{
	mSessionIDToUser[user->GetSessionID()] = user;

	auto obj = user->GetGameObject();
	AddGameObjectToNetwork(obj);
	World::GetInstance().AddGameObject(obj);



}

void NetworkManagerServer::AddGameObjectToNetwork(std::shared_ptr<GameObject> obj)
{
	obj->SetNetworkID(mNetworkIDSeed++);
	mNetworkIDToGameObject[obj->GetNetworkID()] = obj;

	obj->SetState(eObjectState::CREATE);
	SetObjectState(obj.get());

}

void NetworkManagerServer::RemoveGameObjectToNetwork(GameObject* obj)
{
	mNetworkIDToGameObject.erase(obj->GetNetworkID());
	obj->SetState(eObjectState::DESTROY);
	SetObjectState(obj);
}

void NetworkManagerServer::SetObjectState(GameObject* obj)
{
	mNetworkIDToInfo[obj->GetNetworkID()] = obj->GetObjectInfo();
}

void NetworkManagerServer::SendReplicationPacket(IOCPSession* session)
{
	if (mNetworkIDToInfo.size() == 0)
		return;

	using leeder::PK_SC_REPLICATION_STATE;

	auto iocpSession = session;

	PK_SC_REPLICATION_STATE replicationPacket;

	replicationPacket.SetInfo(mNetworkIDToInfo);

	iocpSession->SendPacket(&replicationPacket);
}

std::shared_ptr<User> NetworkManagerServer::FindUserToSessionID(uint32_t sessionID)
{

	auto iter = mSessionIDToUser.find(sessionID);
	if (iter != mSessionIDToUser.end())
		return iter->second;

	return nullptr;
}
