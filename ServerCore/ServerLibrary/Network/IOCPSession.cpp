#include "stdafx.h"
#include "IOCPSession.h"

namespace leeder
{

IOCPSession::IOCPSession()
	:mConnected(false)
{
}

IOCPSession::~IOCPSession()
{
}
void IOCPSession::Init()
{
	mReadIO = std::make_shared<RWIOData>(eIOType::RECV);
	mWriteIO = std::make_shared<RWIOData>(eIOType::SEND);
}

void IOCPSession::RecvStandBy()
{
	mReadIO->Reset();

	if (mReadIO->GetSession() == nullptr)
		mReadIO->SetSession(shared_from_this());


	mReadIO->SetType(eIOType::RECV);
	
	Overlapped* RecvOverlapped = new Overlapped(mReadIO);


	WSABUF wsaBuf;
	wsaBuf.buf = mReadIO->GetBuffer();
	wsaBuf.len = BUF_SIZE;

	this->recv(RecvOverlapped, wsaBuf);

}

bool IOCPSession::isRemainToRecv(size_t transferred)
{
	if (mReadIO->IsRemainToIO(transferred)) {

		Overlapped* RecvOverlapped = new Overlapped(mReadIO);


		WSABUF wsaBuf;
		wsaBuf.buf = mReadIO->GetBuffer() + mReadIO->GetCurrentByte();
		wsaBuf.len = (ULONG)(mReadIO->GetTotalByte() - mReadIO->GetCurrentByte());

		this->recv(RecvOverlapped, wsaBuf);

		return true;
	}

	return false;
}

void IOCPSession::OnDisconnect(eDisconnectReason reason)
{
	SessionManager::GetInstance().ReturnSession(shared_from_this());
}

void IOCPSession::OnAccept(IOCPServer* server)
{
	if (true == mConnected.exchange(true))
	{
		//에러처리
		return;
	}


	if (!this->setSocketOption(server))
	{
		//에러처리
		return;
	}

	int addrLen = sizeof(SOCKADDR_IN);
	int ret = getpeername(mSocket.GetHandle(), (struct sockaddr*)&mSocket.GetSocketInfo(), &addrLen);
	if (ret == -1)
		printf("%d", WSAGetLastError());


	if (!server->RegistCompletionPort(mSocket.GetHandle(), (ULONG_PTR)this))
	{
		//에러처리
		return;
	}

	this->RecvStandBy();

}


void IOCPSession::OnSend(DWORD transferSize)
{
	if (!mConnected.load())
	{
		return;
	}

	if (mWriteIO->IsRemainToIO(transferSize))
	{
		Overlapped* SendOverlapped = new Overlapped(mWriteIO);


		WSABUF wsaBuf;
		wsaBuf.buf = mWriteIO->GetBuffer() + mWriteIO->GetCurrentByte();
		wsaBuf.len = (ULONG)(mWriteIO->GetTotalByte() - mWriteIO->GetCurrentByte());

		this->send(SendOverlapped, wsaBuf);

	}
}

std::shared_ptr<Package> IOCPSession::OnRecv(DWORD transferSize)
{
	if (!mConnected.load())
	{
		return nullptr;
	}

	//패킷 가장 앞부분. 패킷 총길이를 가져오고 그만큼 offset 증가.
	size_t offset = mReadIO->SetTotalByte();

	if (this->isRemainToRecv(transferSize)) {
		return nullptr;
	}


	std::shared_ptr<Packet> packet = PacketAnalyzer::GetInstance().analyze(mReadIO->GetBuffer() + offset, mReadIO->GetTotalByte() - offset);


	if (packet == nullptr) {
		//에러처리
		return nullptr;
	}

	this->RecvStandBy();

	std::shared_ptr<Package> package = std::make_shared<Package>(shared_from_this(), packet);

	return std::move(package);

}

void IOCPSession::Reset()
{
	mConnected.store(false);

	mReadIO->Reset();
	mWriteIO->Reset();

	mSocket.Reset();
}



void IOCPSession::recv(Overlapped* overlapped , WSABUF wsaBuf)
{
	DWORD flags = 0;
	DWORD recvBytes;
	
	DWORD error = WSARecv(mSocket.GetHandle(),  &wsaBuf, 1, &recvBytes, &flags, (LPOVERLAPPED) overlapped, NULL);

	this->checkIOError(error);
}

void IOCPSession::send(Overlapped* overlapped, WSABUF wsaBuf)
{
	DWORD flags = 0;
	DWORD sendBytes;
	DWORD error = WSASend(mSocket.GetHandle(), &wsaBuf, 1, &sendBytes, flags, (LPOVERLAPPED)overlapped, NULL);

	this->checkIOError(error);
}

bool IOCPSession::setSocketOption(IOCPServer* server)
{

	SOCKET s = server->GetSocket()->GetHandle();


	if (!mSocket.UpdateAcceptContext(s))
	{
		return false;
	}

	if (!mSocket.SetNodelay(true))
	{
		return false;
	}

	tcp_keepalive keepAliveSet = { 0 }, returned = { 0 };
	keepAliveSet.onoff = 1;
	keepAliveSet.keepalivetime = 3000;        // Keep Alive in 3 sec.
	keepAliveSet.keepaliveinterval = 3000;    // Resend if No-Reply

	DWORD dwBytes;
	if (WSAIoctl(mSocket.GetHandle(), SIO_KEEPALIVE_VALS, &keepAliveSet, sizeof(keepAliveSet), &returned, sizeof(returned), &dwBytes, NULL, NULL) != 0) {
		return false;
	}

	return true;

}

void IOCPSession::Accept(SOCKET listenSocket)
{
	DWORD bytes = 0;
	
	std::shared_ptr<IOData> data = std::make_shared<AcceptIOData>();
	data->SetSession(shared_from_this());


	Overlapped* acceptOverlapped = new Overlapped(data);


	if (FALSE == mFnAcceptEx(listenSocket, mSocket.GetHandle(), data->GetBuffer(), 0,
		sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16, &bytes, (LPOVERLAPPED)acceptOverlapped))
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
		{
			delete acceptOverlapped;
		}
	}

}

void IOCPSession::RequestDisconnect(eDisconnectReason reason)
{
	if (!mConnected.load())
		return;

	DWORD bytes = 0;

	std::shared_ptr<IOData> data = std::make_shared<DisconnectIOData>(reason);
	data->SetSession(shared_from_this());


	Overlapped* disconnectOverlapped = new Overlapped(data);

	if (FALSE == mFnDisconnectEx(mSocket.GetHandle(), (LPOVERLAPPED)disconnectOverlapped, TF_REUSE_SOCKET,0))
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
		{
			delete disconnectOverlapped;
		}
	}

}


void IOCPSession::checkIOError(DWORD error)
{
	if (error == SOCKET_ERROR
		&& (WSAGetLastError() != WSA_IO_PENDING)) {
		SysLogger::GetInstance().Log(L"! socket error: %d", WSAGetLastError());
	}
}
}