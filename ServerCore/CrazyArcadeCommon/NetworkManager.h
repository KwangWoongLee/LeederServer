#pragma once
#include "Common.h"

class NetworkManager
{
public:
	virtual void AddGameObjectToNetwork(std::shared_ptr<GameObject> obj) {};
	virtual void RemoveGameObjectToNetwork(GameObject* obj) {};



protected:
	std::unordered_map<uint32_t, std::shared_ptr<GameObject>>	mNetworkIDToGameObject;

};

