#include "stdafx.h"
#include "Terminal.h"

namespace leeder
{
Terminal::Terminal(Server* server, std::string name)
	: mServer(server)
	, mName(name)
	, mState(eTermialState::UNCONNECT)

{
}

Terminal::~Terminal()
{
}

void Terminal::Run()
{
	mProcessThread = std::make_unique<Thread>([&]() {
		while (!bShutDown)
		{
			while (mState == eTermialState::UNCONNECT)
			{
				if (mSession.Connect(mIP, mPort))
				{
					mState = eTermialState::READY;
					break;
				}

				printf("connect");
				Sleep(1000);        // 1초마다 연결 시도
			}

			PK_T_NOTIFY terminalPacket;
			mSession.SendPacket(&terminalPacket);

			while (!bShutDown && mState != eTermialState::UNCONNECT) {
				std::shared_ptr<Package> package = mSession.OnRecv();

				if (package)
				{
					mServer->PutPackage(std::move(package));
					continue;
				}

				//package == nullptr일 경우
				mState = eTermialState::UNCONNECT;
			}
		}
		
		});

	
}

}