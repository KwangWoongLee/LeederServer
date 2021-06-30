#include "stdafx.h"
#include "ClientContentsProcess.h"


std::function < void(ClientContentsProcess*)> processThreadFunction = [](ClientContentsProcess* process) {

	while (!leeder::bShutDown)
	{
		std::shared_ptr<ClientPackage> package = nullptr;
		package = process->GetPackage();

		if (!package)
			continue;


		process->ProcessPackage(package);
	}
};


ClientContentsProcess::ClientContentsProcess()
{
	XMLDocument config;

	const char* path = "./config.xml";

	if (!loadConfig(&config, path))
	{
		printf("ClientContentsProcess Config Reading is failed\n");
		exit(0);
		return;
	}

	this->Init(&config);
}

ClientContentsProcess::~ClientContentsProcess()
{
	mPackageQueue.Clear();
}

void ClientContentsProcess::Init(XMLDocument* config)
{
	XMLElement* app = config->FirstChildElement("App");

	XMLElement* contents = app->FirstChildElement("Contents");

	if (!contents) {
		printf("No setting for contents in config");
		return;
	}



	RegistDefaultFunction();


}

void ClientContentsProcess::PutPackage(std::shared_ptr<ClientPackage>&& package)
{
	mPackageQueue.Push(package);
}

std::shared_ptr<ClientPackage> ClientContentsProcess::GetPackage()
{
	return mPackageQueue.Pop();
}

void ClientContentsProcess::ProcessPackage(std::shared_ptr<ClientPackage>& package)
{
	std::shared_ptr<Packet> packet = package->GetPacket();
	ePacketType type = packet->GetType();

	auto iter = mProcessFunctionMap.find(type);
	if (iter == mProcessFunctionMap.end()) {
		return;
	}
	auto packetFunction = iter->second;


	packetFunction(package->GetClient(), package->GetPacket());
}

void ClientContentsProcess::RegistFunction(ePacketType type, clientPacketfunc func)
{
	mProcessFunctionMap.insert(std::make_pair(type, std::move(func)));

}

void ClientContentsProcess::RegistDefaultFunction()
{
	
}
