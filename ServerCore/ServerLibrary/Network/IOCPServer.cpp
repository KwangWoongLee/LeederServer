#include "stdafx.h"
#include "IOCPServer.h"

namespace leeder
{

std::function < void(IOCPServer*)> ioWorkerThreadFunction = [](IOCPServer* server) {

	while (!bShutDown) 
	{
		Overlapped* overlapped = nullptr;
		IOCPSession* session = nullptr;
		DWORD			transferSize;

		BOOL ret = GetQueuedCompletionStatus(server->GetIOCP(), &transferSize, (PULONG_PTR)&session, (LPOVERLAPPED*)&overlapped, INFINITE);

		if (!ret)
		{
			delete overlapped;
			continue;
		}


		session = (overlapped == nullptr) ? nullptr : overlapped->GetSession();


		if (session == nullptr)
		{
			SysLogger::GetInstance().Log(L"Invalid IOData");
			ASSERT(false);
			return;
		}

		switch (overlapped->GetType())
		{
		case eIOType::ACCEPT: 
			session->OnAccept(server);
			break;

		case eIOType::RECV_ZERO:
		{
			printf("recvzero\n");
			session->OnZeroRecv();
		}


		case eIOType::RECV: 
		{
			
			if (transferSize == 0)
			{
				printf("recv0\n");
				delete overlapped;
				continue;
			}

			std::shared_ptr<Package> package = session->OnRecv(transferSize);
			printf("recv%d\n" , transferSize);
			if (package)
				server->PutPackage(std::move(package));
			break;
		}

		case eIOType::SEND: 
			if (transferSize == 0)
			{
				delete overlapped;
				continue;
			}

			session->OnSend(transferSize);
			break;

		case eIOType::NONE:
			SysLogger::GetInstance().Log(L"Client Close by Client Error ");
			session->OnDisconnect(eDisconnectReason::COMPLETION_ERROR);
			continue;

		default:
			printf("Unknown Data Type");
			break;
		}

		delete overlapped;

	}	

};

LPFN_ACCEPTEX		mFnAcceptEx = nullptr;


IOCPServer::IOCPServer(std::shared_ptr<ContentsProcess>&& contents)
	:Server(std::move(contents))
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

	if (!mListenSocket->Bind(GetIP().c_str(), GetPort()))
	{
		printf("bind error %d" , WSAGetLastError());
		//俊矾贸府;
	}

	if (!mListenSocket->Listen())
	{
		printf("listen error");

		//俊矾贸府
	}

	if (!mListenSocket->ReuseAddr(true))
	{
		//俊矾贸府
	}


	DWORD bytes = 0;

	GUID guidAcceptEx = WSAID_ACCEPTEX;
	if (SOCKET_ERROR == WSAIoctl(mListenSocket->GetHandle(), SIO_GET_EXTENSION_FUNCTION_POINTER,
		&guidAcceptEx, sizeof(GUID), &mFnAcceptEx, sizeof(LPFN_ACCEPTEX), &bytes, NULL, NULL))
		//俊矾贸府
		printf("error");


	if (!createCompletionPort())
	{
		//俊矾贸府
	}
	if (!RegistCompletionPort(mListenSocket->GetHandle(), (ULONG_PTR)0))
	{
		//俊矾贸府
	}

	mAcceptThread = std::make_unique<Thread>([&]() { 
		
		while (!bShutDown)
		{
			SessionManager::GetInstance().AcceptSessions(mListenSocket->GetHandle());

			SessionManager::GetInstance().CheckHeartBeat();

			Sleep(100);

		}
		
		});

	for (size_t i = 0; i < GetThreadCount(); ++i) {
		mWorkThreadPool.push_back(std::make_unique<Thread>(ioWorkerThreadFunction, this));
	}


	SetState(eServerState::RUN);

}


bool IOCPServer::createCompletionPort()
{
	mIOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);

	if (mIOCP == NULL)
		return false;

	return true;
}


bool IOCPServer::RegistCompletionPort(SOCKET socket, ULONG_PTR key)
{
	HANDLE handle = CreateIoCompletionPort((HANDLE)socket, mIOCP, key, 0);

	if (handle != mIOCP)
	{
		SysLogger::GetInstance().Log(L"Socket failed to regist for IOCP %d", GetLastError());

		return false;
	}
}


}