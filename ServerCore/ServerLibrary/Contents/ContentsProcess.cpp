#include "stdafx.h"
#include "ContentsProcess.h"

namespace leeder
{


std::function < void(ContentsProcess*)> processThreadFunction = [](ContentsProcess* process) {

	while (!bShutDown)
	{
		std::shared_ptr<Package> package = nullptr;
		package = process->GetPackage();

		if (!package)
			continue;


		process->ProcessPackage(package);
	}
};


ContentsProcess::ContentsProcess()
{
	XMLDocument config;

	const char* path = "./config.xml";

	if (!loadConfig(&config, path))
	{
		printf("ContentsProcess Config Reading is failed\n");
		exit(0);
		return;
	}

	this->Init(&config);
}

ContentsProcess::~ContentsProcess()
{
	mPackageQueue.Clear();
}

void ContentsProcess::Init(XMLDocument* config)
{
	XMLElement* app = config->FirstChildElement("App");

	XMLElement* contents = app->FirstChildElement("Contents");

	if (!contents) {
		SysLogger::GetInstance().Log(L"No setting for contents in config");
		return;
	}

	XMLElement* element = contents->FirstChildElement("ThreadCount");
	sscanf_s(element->GetText(), "%d", &mThreadCount);


	RegistDefaultFunction();

}

void ContentsProcess::PutPackage(std::shared_ptr<Package>&& package)
{
	mPackageQueue.Push(package);
}

std::shared_ptr<Package> ContentsProcess::GetPackage()
{
	return mPackageQueue.Pop();
}

void ContentsProcess::ProcessPackage(std::shared_ptr<Package>& package)
{
	std::shared_ptr<Packet> packet = package->GetPacket();
	ePacketType type = static_cast<ePacketType>(packet->GetTypeToInt());

	auto iter = mProcessFunctionMap.find(type);
	if (iter == mProcessFunctionMap.end()) {
		return;
	}
	func packetFunction = iter->second;


	packetFunction(package->GetSession(), package->GetPacket());
}

void ContentsProcess::RegistFunction(ePacketType type,func func)
{
	mProcessFunctionMap.insert(std::make_pair(type, std::move(func)));

}

void ContentsProcess::RegistDefaultFunction()
{
	RegistFunction(ePacketType::CS_NOTIFY_HEARTBEAT, std::bind(&ContentsProcess::HeartBeatPacketFunction, this, std::placeholders::_1, std::placeholders::_2));
}

void ContentsProcess::HeartBeatPacketFunction(Session* session, std::shared_ptr<Packet>& packet)
{
	session->UpdateHeartBeat();
}

}