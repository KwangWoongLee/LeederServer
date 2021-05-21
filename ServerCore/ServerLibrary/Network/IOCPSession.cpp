#include "stdafx.h"
#include "IOCPSession.h"

namespace leeder
{

IOCPSession::IOCPSession()
	:mConnected(false)
	, mReadIO(eIOType::RECV)
	, mWriteIO(eIOType::SEND)
{
}

IOCPSession::~IOCPSession()
{
}
void IOCPSession::RecvStandBy()
{
	mReadIO.Reset();

	if (mReadIO.GetSession() == nullptr)
		mReadIO.SetSession(shared_from_this());


	mReadIO.SetType(eIOType::RECV_ZERO);
	
	Overlapped* zeroByteOverlapped = new Overlapped((void*)&mReadIO);


	WSABUF wsaBuf;
	wsaBuf.buf = mReadIO.GetBuffer();
	wsaBuf.len = 0;

	this->recv(zeroByteOverlapped, wsaBuf);

}

bool IOCPSession::isRemainToRecv(size_t transferred)
{
	if (mReadIO.IsRemainToIO(transferred)) {

		mReadIO.SetType(eIOType::RECV);

		Overlapped* RecvOverlapped = new Overlapped((void*)&mReadIO);


		WSABUF wsaBuf;
		wsaBuf.buf = mReadIO.GetBuffer() + mReadIO.GetCurrentByte();
		wsaBuf.len = (ULONG)(mReadIO.GetTotalByte() - mReadIO.GetCurrentByte());

		this->recv(RecvOverlapped, wsaBuf);

		return true;
	}

	return false;
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

void IOCPSession::OnZeroRecv()
{
	if (!mConnected.load())
	{
		return;
	}

	mReadIO.SetType(eIOType::RECV);

	Overlapped* RecvOverlapped = new Overlapped((void*)&mReadIO);


	WSABUF wsaBuf;
	wsaBuf.buf = mReadIO.GetBuffer();
	wsaBuf.len = BUF_SIZE;

	this->recv(RecvOverlapped, wsaBuf);

}

void IOCPSession::OnRecv(DWORD transferSize)
{
	if (!mConnected.load())
	{
		return;
	}

	//패킷 가장 앞부분. 패킷 총길이를 가져오고 그만큼 offset 증가.
	size_t offset = mReadIO.SetTotalByte();

	if (this->isRemainToRecv(transferSize)) {
		return;
	}

	Packet* packet = PacketAnalyzer::GetInstance().analyze(mReadIO.GetBuffer() + offset, mReadIO.GetTotalByte() - offset);

	if (packet == nullptr) {

		return;
	}

	this->RecvStandBy();
}

void IOCPSession::Reset()
{
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
	//DWORD flags = 0;
	//DWORD sendBytes;
	//DWORD error = WSASend(mSocket.GetHandle(), &wsaBuf, 1, &sendBytes, flags, mWriteIO.GetOverlapped(), NULL);

	//this->checkIOError(error);
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
}

void IOCPSession::Accept(SOCKET listenSocket)
{
	DWORD bytes = 0;
	
	IOData* data = new AcceptIOData();
	data->SetSession(shared_from_this());


	Overlapped* acceptOverlapped = new Overlapped((void*) data);
	

	if (FALSE == mFnAcceptEx(listenSocket, mSocket.GetHandle(), data->GetBuffer(), 0,
		sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16, &bytes, (LPOVERLAPPED)acceptOverlapped))
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
		{
			delete acceptOverlapped;
		}
	}
}

void IOCPSession::checkIOError(DWORD error)
{
	if (error == SOCKET_ERROR
		&& (WSAGetLastError() != ERROR_IO_PENDING)) {
		SysLogger::GetInstance().Log(L"! socket error: %d", WSAGetLastError());
	}
}
}