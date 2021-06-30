#include "stdafx.h"
#include "IOCPSession.h"

namespace leeder
{

IOCPSession::IOCPSession()
	: mBuffer(BUF_SIZE)
{
}

IOCPSession::~IOCPSession()
{
}

void IOCPSession::RecvStandBy()
{
	if (!IsConnected())
		return;


	std::unique_lock<std::mutex> lock(mBufferMutex);

	Overlapped* RecvOverlapped = new Overlapped(this);
	RecvOverlapped->SetType(eIOType::RECV);
	RecvOverlapped->SetSession(this);

	auto wsaBuf = RecvOverlapped->GetWSABuf();

	wsaBuf.buf = mBuffer.GetBuffer();
	wsaBuf.len = (ULONG)mBuffer.GetFreeSpaceSize();

	this->recv(RecvOverlapped, wsaBuf);

}


void IOCPSession::SendPacket(Packet* packet)
{
	OutputStream stream;
	packet->Encode(stream);


	if (!IsConnected())
	{
		return;
	}


	//패킷 총길이
	PACKET_SIZE packetTotalLength[1] = { sizeof(PACKET_SIZE) + sizeof(float) + stream.GetLength() };


	size_t	offset = sizeof(PACKET_SIZE);

	//패킷 총길이 입력
	memcpy((void*)mSendBuffer, (void*)packetTotalLength, sizeof(PACKET_SIZE));

	float time = Clock::GetInstance().GetSystemTimeFloat();
	memcpy((void*)(mSendBuffer + offset), &time, sizeof(float));

	offset += sizeof(float);

	// 스트림 데이터 입력
	memcpy((void*)(mSendBuffer + offset), (void*)stream.GetBuffer(), stream.GetLength());



	Overlapped* sendOverlapped = new Overlapped(this);
	sendOverlapped->SetType(eIOType::SEND);
	sendOverlapped->SetSession(this);

	auto wsaBuf = sendOverlapped->GetWSABuf();
	wsaBuf.len = (ULONG)packetTotalLength[0];
	wsaBuf.buf = mSendBuffer;


	this->send(sendOverlapped, wsaBuf);

}

std::list<IOCPSession*>::iterator IOCPSession::OnDisconnect(eDisconnectReason reason)
{
	return SessionManager::GetInstance().ReturnSession(this);
}

void IOCPSession::OnAccept(IOCPServer* server)
{
	if (true == mConnected.exchange(true))
	{
		SysLogger::GetInstance().Log(L"Session is already connected");
		return;
	}

	UpdateHeartBeat();

	if (!this->setSocketOption(server))
	{
		SysLogger::GetInstance().Log(L"Socket Option Settion Error");
		ASSERT(false);
		return;
	}

	int addrLen = sizeof(SOCKADDR_IN);
	int ret = getpeername(mSocket.GetHandle(), (struct sockaddr*)&mSocket.GetSocketInfo(), &addrLen);


	if (!server->RegistCompletionPort(mSocket.GetHandle(), (ULONG_PTR)this))
	{
		SysLogger::GetInstance().Log(L"CompletionPort Registing Error");
		ASSERT(false);
		return;
	}

	this->ZeroRecv();

}

void IOCPSession::OnZeroRecv()
{
	this->RecvStandBy();
}


void IOCPSession::OnSend(DWORD transferSize)
{
	//std::unique_lock<std::mutex> lock(mBufferMutex);

	//mBuffer.Remove(transferSize);
	ZeroMemory(mSendBuffer, sizeof(BUF_SIZE) * sizeof(char));

	//printf("%d\n", transferSize);
}

std::shared_ptr<Package> IOCPSession::OnRecv(DWORD transferSize)
{
	if (!IsConnected())
		return nullptr;

	std::unique_lock<std::mutex> lock(mBufferMutex);

	if (0 == mBuffer.GetFreeSpaceSize())
		return nullptr;

	size_t offset = 0;
	PACKET_SIZE packetSize[1] = { 0 };

	memcpy((void*)packetSize, (void*)mBuffer.GetBuffer(), sizeof(PACKET_SIZE));

	offset += sizeof(PACKET_SIZE);

	float recvTime[1] = { 0 };
	memcpy((void*)recvTime, (void*)(mBuffer.GetBuffer() + offset), sizeof(float));

	offset += sizeof(float);


	std::shared_ptr<Packet> packet = PacketAnalyzer::GetInstance().analyze(mBuffer.GetBuffer() + offset, packetSize[0] - offset);


	mBuffer.Commit(transferSize);

	mBuffer.Remove(transferSize);


	if (packet == nullptr) {

		return nullptr;
	}

	this->ZeroRecv();

	std::shared_ptr<Package> package = std::make_shared<Package>(this, packet);

	return std::move(package);

}

void IOCPSession::Reset()
{
	mConnected.store(false);

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

	if (!mSocket.SetRecvBufferSize(0))
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


	Overlapped* acceptOverlapped = new Overlapped(this);
	acceptOverlapped->SetType(eIOType::ACCEPT);
	auto wsabuf = acceptOverlapped->GetWSABuf();
	wsabuf.len = 0;
	wsabuf.buf = nullptr;

	if (FALSE == mFnAcceptEx(listenSocket, mSocket.GetHandle(), SessionManager::GetInstance().mAcceptBuffer, 0,
		sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16, &bytes, (LPOVERLAPPED)acceptOverlapped))
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
		{
			delete acceptOverlapped;
		}
	}

}

void IOCPSession::ZeroRecv()
{
	if (!IsConnected())
		return ;

	Overlapped* ZeroRecvOverlapped = new Overlapped(this);
	ZeroRecvOverlapped->SetSession(this);
	ZeroRecvOverlapped->SetType(eIOType::RECV_ZERO);
	auto wsabuf = ZeroRecvOverlapped->GetWSABuf();
	wsabuf.len = 0;
	wsabuf.buf = nullptr;

	DWORD recvbytes = 0;
	DWORD flags = 0;

	/// start async recv
	if (SOCKET_ERROR == WSARecv(mSocket.GetHandle(), &wsabuf ,1, &recvbytes, &flags, (LPWSAOVERLAPPED)ZeroRecvOverlapped, NULL))
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
		{
			delete ZeroRecvOverlapped;
		}
	}

}


bool IOCPSession::checkIOError(DWORD error)
{
	if (error == SOCKET_ERROR
		&& (WSAGetLastError() != WSA_IO_PENDING)) {


		if (WSAGetLastError() == WSAECONNRESET)
			return false;

		if (!mConnected.load())
			return false;

		SysLogger::GetInstance().Log(L"! socket error: %d", WSAGetLastError());

		return false;
	}

	return true;
}
}