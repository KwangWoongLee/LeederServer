#include "stdafx.h"
#include "IOCPServer.h"

namespace leeder
{

std::function < void()> AcceptThreadFunction = []() {

	SessionManager::GetInstance().AcceptSessions();

};

std::function < void()> ioWorkerThreadFunction = []() {


};


IOCPServer::IOCPServer()
	:Server()
	, mIOCP(NULL)
{
	SysLogger::GetInstance().Log(L"Create IOCPServer");
}

IOCPServer::~IOCPServer()
{
	CloseHandle(mIOCP);
}


void IOCPServer::Run()
{
	if (mListenSocket.Bind(GetIP().c_str(), GetPort()))
	{
		//俊矾贸府
	}

	if (mListenSocket.Listen())
	{
		//俊矾贸府
	}

	if (!mListenSocket.ReuseAddr(true))
	{
		//俊矾贸府
	}


	if (!createCompletionPort())
	{
		//俊矾贸府
	}

	if (!registCompletionPort(mListenSocket.GetSocket(), (ULONG_PTR)0))
	{
		//俊矾贸府
	}



	mAcceptThread = std::make_unique<Thread>(AcceptThreadFunction);

	for (size_t i = 0; i < GetThreadCount(); ++i) {
		mWorkThreadPool.push_back(std::make_unique<Thread>(ioWorkerThreadFunction));
	}


	SetState(eServerState::RUN);

	while (true)
	{
		printf("IOCPServer is running");
		Sleep(10000);
	}

}


bool IOCPServer::createCompletionPort()
{
	mIOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);

	if (mIOCP == NULL)
		return false;

	return true;
}


bool IOCPServer::registCompletionPort(SOCKET socket, ULONG_PTR completionKey)
{
	HANDLE handle = CreateIoCompletionPort((HANDLE)socket, mIOCP, completionKey, 0);

	if (handle != mIOCP)
	{
		SysLogger::GetInstance().Log(L"Socket failed to regist for IOCP %d", GetLastError());

		return false;
	}
}


}