#pragma once
#include "stdafx.h"

class NetworkManagerServer : public Singleton<NetworkManagerServer> , public NetworkManager
{
public:
	void Init(std::shared_ptr<ContentsProcess> process);

	void ProcessQueuedPacket();
	void Replication();

	void HandleNewClient(std::shared_ptr<User> user);

	void AddGameObjectToNetwork(std::shared_ptr<GameObject> obj) override;
	void RemoveGameObjectToNetwork(GameObject* obj) override;

	void SetObjectState(GameObject* obj);

	void SendReplicationPacket(IOCPSession* session);

	std::unordered_map<uint32_t, std::shared_ptr<GameObject>>& GetNetworkState() { return mNetworkIDToGameObject; }

	std::shared_ptr<User>	FindUserToSessionID(uint32_t sessionID);

private:
	std::shared_ptr<ContentsProcess> mProcess;
	std::unordered_map<uint32_t, std::shared_ptr<User>>	mSessionIDToUser;

	uint32_t													mNetworkIDSeed;

	std::unordered_map<uint32_t, ObjectInfo>					mNetworkIDToInfo;

	float			mLastReplicationTime;

};