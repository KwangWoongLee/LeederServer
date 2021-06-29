#pragma once
#include "stdafx.h"

namespace leeder
{

using func = std::function<void(Session*, std::shared_ptr<Packet>&)>;


class ContentsProcess
{
public:
	ContentsProcess();
	~ContentsProcess();

	void Init(XMLDocument* config);

	ThreadSafeQueue<std::shared_ptr<Package>>& GetPackageQueue() { return mPackageQueue; }

	void	PutPackage(std::shared_ptr<Package>&& package);
	std::shared_ptr<Package> GetPackage();

	void	ProcessPackage(std::shared_ptr<Package>& package);

	size_t	GetThreadCount() { return mThreadCount; }

protected:
	void RegistFunction(ePacketType type, func func);
	
private:
	void	RegistDefaultFunction();
	void	HeartBeatPacketFunction(Session* session, std::shared_ptr<Packet>& packet);


	ThreadSafeQueue<std::shared_ptr<Package>>	mPackageQueue;
	size_t										mThreadCount;
	std::unordered_map<ePacketType, func>		mProcessFunctionMap;

};


}
