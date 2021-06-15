#pragma once
#include "stdafx.h"

using clientPacketfunc = std::function<void(Client*, std::shared_ptr<Packet>&)>;


class ClientContentsProcess
{
public:
	ClientContentsProcess();
	~ClientContentsProcess();

	void Init(XMLDocument* config);

	ThreadSafeQueue<std::shared_ptr<ClientPackage>>& GetPackageQueue() { return mPackageQueue; }
	
	void	PutPackage(std::shared_ptr<ClientPackage>&& package);
	std::shared_ptr<ClientPackage> GetPackage();

	void	ProcessPackage(std::shared_ptr<ClientPackage>& package);


protected:
	void RegistFunction(ePacketType type, clientPacketfunc func);

private:
	void	RegistDefaultFunction();


	ThreadSafeQueue<std::shared_ptr<ClientPackage>>	mPackageQueue;
	std::unordered_map<ePacketType, clientPacketfunc>		mProcessFunctionMap;

}
;