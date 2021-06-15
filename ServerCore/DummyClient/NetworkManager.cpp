#include "stdafx.h"
#include "NetworkManager.h"

void NetworkManager::Init(std::shared_ptr<ClientContentsProcess> process)
{
	mProcess = process;
}

void NetworkManager::ProcessQueuedPacket()
{
	while (!mProcess->GetPackageQueue().Empty())
	{
		std::shared_ptr<ClientPackage> package = nullptr;
		package = mProcess->GetPackage();

		if (!package)
			continue;

		mProcess->ProcessPackage(package);
	}
}
