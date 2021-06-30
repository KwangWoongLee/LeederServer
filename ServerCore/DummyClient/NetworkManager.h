#pragma once
#include "stdafx.h"

class NetworkManager : public Singleton<NetworkManager>
{
public:
	void Init(std::shared_ptr<ClientContentsProcess> process);

	void ProcessQueuedPacket();

private:
	std::shared_ptr<ClientContentsProcess> mProcess;

	std::unordered_map<uint32_t, std::shared_ptr<GameObject>>	mNetworkIDToGameObject;
	std::unordered_map<uint32_t, ObjectInfo>					mNetworkIDToInfo;

};

