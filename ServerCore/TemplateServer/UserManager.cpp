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
	user->SetState(eObjectState::CREATE);

	mNetworkIDToState.insert(std::make_pair(sessionID, eObjectState::CREATE));

	mUserMap.insert(make_pair(sessionID, user));

}

void UserManager::Remove(uint32_t sessionID)
{
	std::unique_lock<std::mutex> lock(mUserManagerMutex);
	
	mNetworkIDToState[sessionID] = eObjectState::DESTROY;
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
		mNetworkIDToState[sessionID] = eObjectState::ACTION;
		for (auto input : inputList)
		{
			(*iter).second->PushInput(input);
		}

	}
}

void UserManager::UpdateUsers()
{
	std::unique_lock<std::mutex> lock(mUserManagerMutex);

	auto iter = mUserMap.begin();
	auto iterEnd = mUserMap.end();

	for (; iter != iterEnd;)
	{	
		auto session = iter->second->GetSession();
		if (!session->IsConnected())
		{
			mNetworkIDToState[session->GetID()] = eObjectState::DESTROY;
			iter = mUserMap.erase(iter);
			continue;
		}

		iter->second->Update();
		++iter;
	}
}


void UserManager::Replication()
{
	std::unique_lock<std::mutex> lock(mUserManagerMutex);

	PK_SC_REPLICATION_STATE resPacket;

	for (auto user : mUserMap)
	{
		auto iocpSession = std::static_pointer_cast<IOCPSession>(user.second->GetSession());

		iocpSession->SendPacket(&resPacket);
	}

}

}