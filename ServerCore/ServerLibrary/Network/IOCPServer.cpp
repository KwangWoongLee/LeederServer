#include "stdafx.h"
#include "IOCPServer.h"

namespace leeder
{

std::function < void(IOCPServer*)> ioWorkerThreadFunction = [](IOCPServer* server) {

	while (!bShutDown) 
	{
		Overlapped* overlapped = nullptr;
		std::shared_ptr<IOCPSession> session = nullptr;
		DWORD			transferSize;

		BOOL ret = GetQueuedCompletionStatus(server->GetIOCP(), &transferSize, (PULONG_PTR)&session, (LPOVERLAPPED*)&overlapped, INFINITE);

		std::shared_ptr<IOData> ioData = overlapped->GetIOData();

		session = (ioData == nullptr) ? nullptr : ioData->GetSession();

		if (!ret)
			continue;

		if (session == nullptr)
		{
			//俊矾贸府
			return;
		}

		if ( (ioData->GetType()==eIOType::RECV || ioData->GetType() == eIOType::SEND)
			&& transferSize == 0) 
		{
			printf("Session Already Not Exists");

			session->RequestDisconnect(eDisconnectReason::COMPLETION_ERROR);

			delete overlapped;

			continue;
		}

		switch (ioData->GetType())
		{
		case eIOType::DISCONNECT:
			session->OnDisconnect(static_pointer_cast<DisconnectIOData>(ioData)->GetReason());
			break;

		case eIOType::ACCEPT: 
			session->OnAccept(server);
			break;

		case eIOType::RECV: 
		{
			std::shared_ptr<Package> package = session->OnRecv(transferSize);

			if (package)
				server->PutPackage(std::move(package));
			break;
		}

		case eIOType::SEND: 
			session->OnSend(transferSize);
			break;

		case eIOType::NONE:
			printf("IOData NONE");
			continue;

		default:
			printf("Unknown Data Type");
			break;
		}

		delete overlapped;

	}	

};

LPFN_ACCEPTEX		mFnAcceptEx = nullptr;
LPFN_DISCONNECTEX	mFnDisconnectEx = nullptr;
LPFN_CONNECTEX		mFnConnectEx = nullptr;


IOCPServer::IOCPServer(std::unique_ptr<ContentsProcess>&& contents)
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


	GUID guidDisconnectEx = WSAID_DISCONNECTEX;
	if (SOCKET_ERROR == WSAIoctl(mListenSocket->GetHandle(), SIO_GET_EXTENSION_FUNCTION_POINTER,
		&guidDisconnectEx, sizeof(GUID), &mFnDisconnectEx, sizeof(LPFN_DISCONNECTEX), &bytes, NULL, NULL))
		//俊矾贸府
		printf("error");

	GUID guidConnectEx = WSAID_CONNECTEX;
	if (SOCKET_ERROR == WSAIoctl(mListenSocket->GetHandle(), SIO_GET_EXTENSION_FUNCTION_POINTER,
		&guidConnectEx, sizeof(GUID), &mFnConnectEx, sizeof(LPFN_CONNECTEX), &bytes, NULL, NULL))
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


	for (size_t i = 0; i < GetThreadCount(); ++i) {
		mWorkThreadPool.push_back(std::make_unique<Thread>(ioWorkerThreadFunction, this));
	}


	SetState(eServerState::RUN);

	while (!bShutDown)
	{
		SessionManager::GetInstance().AcceptSessions(mListenSocket->GetHandle());
		Sleep(100);
	}
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