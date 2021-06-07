#include "stdafx.h"
#include "UserManager.h"

namespace leeder
{

void UserManager::Put(std::shared_ptr<User> user)
{
	std::unique_lock<std::mutex> lock(mUserManagerMutex);

	uint32_t sessionID = user->GetSession()->GetID();
	user->SetNetworkID(sessionID);
	user->SetPosition({ GetRandomFloat() * 1024, GetRandomFloat() * 512});


	mUserMap.insert(make_pair(sessionID, user));

}

void UserManager::Remove(uint32_t sessionID)
{
	std::unique_lock<std::mutex> lock(mUserManagerMutex);

	mUserMap.erase(sessionID);
}

std::shared_ptr<User> UserManager::At(uint32_t sessionID)
{
	std::unique_lock<std::mutex> lock(mUserManagerMutex);

	auto iter = mUserMap.find(sessionID);
	if (iter == mUserMap.end()) {
		return nullptr;
	}
	return iter->second;
}

void UserManager::InputUpdate(uint32_t sessionID, std::vector<eInputType>& inputList)
{
	std::unique_lock<std::mutex> lock(mUserManagerMutex);

	auto iter = mUserMap.find(sessionID);
	if (iter != mUserMap.end()) {
		for (auto input : inputList)
		{
			(*iter).second->PushInput(input);
			printf("%d", input);
		}

	}
}

void UserManager::UpdateUsers()
{
	std::unique_lock<std::mutex> lock(mUserManagerMutex);

	for (auto user : mUserMap)
		user.second->Update();
}

std::vector<std::shared_ptr<GameObject>> UserManager::GetUsers()
{
	std::unique_lock<std::mutex> lock(mUserManagerMutex);

	std::vector<std::shared_ptr<GameObject>> userVector;
	userVector.reserve(mUserMap.size());

	for (auto user : mUserMap)
	{
		auto obj = std::static_pointer_cast<GameObject>(user.second);
		userVector.push_back(obj);
	}

	return userVector;
}

void UserManager::Replication()
{
	std::unique_lock<std::mutex> lock(mUserManagerMutex);

	PK_SC_REPLICATION_STATE resPacket;

	std::vector<std::shared_ptr<GameObject>> userVector;
	userVector.reserve(mUserMap.size());

	for (auto user : mUserMap)
	{
		auto obj = std::static_pointer_cast<GameObject>(user.second);
		userVector.push_back(obj);
	}

	resPacket.SetGameObjects(userVector);
	
	for (auto user : mUserMap)
	{
		auto iocpSession = std::static_pointer_cast<IOCPSession>(user.second->GetSession());

		iocpSession->SendPacket(&resPacket);
	}

}

}