#pragma once
#include "stdafx.h"

class NetworkManager : public Singleton<NetworkManager>
{
public:
	void Init(std::shared_ptr<ContentsProcess> process);

	void ProcessQueuedPacket();
	void Replication();

	void HandleNewClient(std::shared_ptr<User> user);

	void AddGameObjectToNetwork(std::shared_ptr<GameObject> obj);
	void RemoveGameObjectToNetwork(std::shared_ptr<GameObject>& obj);

	void SetObjectState(std::shared_ptr<GameObject> obj);

	void SendReplicationPacket(const std::shared_ptr<Session>& session);

	std::shared_ptr<User>	FindUserToSessionID(uint32_t sessionID);

private:
	std::shared_ptr<ContentsProcess> mProcess;
	std::unordered_map<uint32_t, std::shared_ptr<User>>	mSessionIDToUser;

	uint32_t													mNetworkIDSeed;
	std::unordered_map<uint32_t, std::shared_ptr<GameObject>>	mNetworkIDToGameObject;
	std::unordered_map<uint32_t, eObjectState>					mNetworkIDToState;

};

