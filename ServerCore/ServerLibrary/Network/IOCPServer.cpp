#include "stdafx.h"
#include "IOCPServer.h"

namespace leeder
{

std::function < void()> ioThreadFunction = []() {
};


IOCPServer::IOCPServer()
	:Server()
	, mIOCP(NULL)
	, mListenSocket(NULL)
{
	SysLogger::GetInstance().Log(L"Create IOCPServer");
}

IOCPServer::~IOCPServer()
{
	closesocket(mListenSocket);
	CloseHandle(mIOCP);
}


void IOCPServer::Run()
{
	if (createListenSocket() == SOCKET_ERROR)
	{
		//俊矾贸府
	}

	if (!createCompletionPort())
	{
		//俊矾贸府
	}

	if (!registCompletionPort(mListenSocket, (ULONG_PTR)0))
	{
		//俊矾贸府
	}



	for (size_t i = 0; i < GetThreadCount(); ++i) {
		mWorkThreadPool.push_back(std::make_unique<Thread>(ioThreadFunction));
	}


	SetState(eServerState::RUN);

}

int IOCPServer::createListenSocket()
{
	mListenSocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);

	if (mListenSocket == INVALID_SOCKET)
	{
		//俊矾贸府
	}

	this->bind();
	// 俊矾贸府

	this->listen();
	// 俊矾贸府


	return NO_ERROR;

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

bool IOCPServer::bind()
{
	SOCKADDR_IN serveraddr = { 0, };

	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(GetPort());
	inet_pton(AF_INET, GetIP().c_str(), &(serveraddr.sin_addr));

	return SOCKET_ERROR != ::bind(mListenSocket, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
}

int IOCPServer::listen(int inBackLog)
{
	return SOCKET_ERROR != ::listen(mListenSocket, SOMAXCONN);
}

}