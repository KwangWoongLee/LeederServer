#include "stdafx.h"
#include "ContentsProcess.h"

namespace leeder
{


std::function < void(ContentsProcess*)> processThreadFunction = [](ContentsProcess* process) {

	int i = 0;
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

	if (!loadConfig(&config)) {
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

	for (size_t i = 0; i < GetThreadCount(); ++i) {
		mProcessThreadPool.push_back(std::make_unique<Thread>(processThreadFunction, this));
	}

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
	ePacketType type = packet->GetType();

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
	RegistFunction(ePacketType::TEST, std::bind(&ContentsProcess::TestPacketFunction,this,std::placeholders::_1, std::placeholders::_2));

	//RegistFunction(ePacketType::TEST, [](auto& ,auto&) {
	//	printf("\nTEST PACKET FUNCTION\n");
	//	});
}

void ContentsProcess::TestPacketFunction(std::shared_ptr<Session>& session, std::shared_ptr<Packet>& packet)
{
	printf("\nTEST PACKET FUNCTION\n");
}

}