#pragma once

namespace leeder
{
class UserManager : public Singleton<UserManager>
{
public:
	void Put(std::shared_ptr<User> user);
	void Remove(uint32_t sessionID);
	std::shared_ptr<User> At(uint32_t sessionID);

	void InputUpdate(uint32_t sessionID, std::vector<eInputType>& inputList);
	size_t GetSize() { return mUserMap.size(); }

	void UpdateUsers();

	std::atomic<int>			mNetworkIDSeed;

	std::vector<std::shared_ptr<GameObject>>	GetUsers();
	void Replication();


private:
	std::unordered_map<uint32_t , std::shared_ptr<User>> mUserMap;
	std::mutex		mUserManagerMutex;

};

}
