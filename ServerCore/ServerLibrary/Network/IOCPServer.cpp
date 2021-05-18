#include "stdafx.h"
#include "IOCPServer.h"

namespace leeder
{

std::function < void(IOCPServer*)> ioWorkerThreadFunction = [](IOCPServer* server) {

	while (true) 
	{
		Overlapped* overlapped = nullptr;
		std::shared_ptr<IOCPSession> session = nullptr;
		DWORD			transferSize;

		BOOL ret = GetQueuedCompletionStatus(server->GetIOCP(), &transferSize, (PULONG_PTR)&session, (LPOVERLAPPED*)&overlapped, INFINITE);

		IOData* ioData = reinterpret_cast<IOData*>(overlapped->GetIOData());
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
			//技记 辆丰
			continue;
		}
		
		switch (ioData->GetType())
		{
		case eIOType::ACCEPT: 
			session->OnAccept(server);
			break;

		case eIOType::RECV_ZERO:
			session->OnZeroRecv();
			break;

		case eIOType::RECV: 
			session->OnRecv(transferSize);
			break;

		case eIOType::SEND: 
			//context->_event_type = 0;
			//event->RemoveType(ET_WRITE);
			//std::shared_ptr<RWSocket> rw_socket = std::dynamic_pointer_cast<RWSocket>(sock);
			//if (bytes == 0) {
			//	rw_socket->OnDisConnect(CEC_CLOSED);
			//}
			//else {
			//	rw_socket->OnWrite(bytes);
			//}

			break;

		default:

			break;
		}

		

		delete ioData;
	}	

};

LPFN_ACCEPTEX		mFnAcceptEx = nullptr;
LPFN_DISCONNECTEX	mFnDisconnectEx = nullptr;
LPFN_CONNECTEX		mFnConnectEx = nullptr;


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

	while (true)
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