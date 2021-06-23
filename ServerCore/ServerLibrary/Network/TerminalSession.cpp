#include "stdafx.h"
#include "TerminalSession.h"

namespace leeder
{
TerminalSession::TerminalSession()
{
	mType = eSessionType::TERMINAL;
}

TerminalSession::~TerminalSession()
{
}

	
bool TerminalSession::Connect(std::string ip, uint16_t port)
{

	auto addrinfo = mSocket.GetSocketInfo();
	ZeroMemory(&addrinfo, sizeof(addrinfo));
	addrinfo.sin_family = AF_INET;
	addrinfo.sin_port = htons(port);
	inet_pton(AF_INET, ip.c_str(), &(addrinfo.sin_addr));


	if (!mSocket.SetNodelay(true))
	{
		return false;
	}

	if (!mSocket.SetRecvBufferSize(0))
	{
		return false;
	}

	if (::connect(mSocket.GetHandle(), (SOCKADDR*)&addrinfo, sizeof(addrinfo)) == SOCKET_ERROR)
	{
		printf("Connect Error : %d", GetLastError());
		return false;
	}

	mConnected = true;

	return true;
}


void TerminalSession::SendPacket(Packet* packet)
{
	OutputStream stream;
	packet->Encode(stream);

	size_t offset = 0;

	PACKET_SIZE packetLength[1] = { sizeof(PACKET_SIZE) + sizeof(float) + stream.GetLength() };

	memcpy(mBuffer, packetLength, sizeof(PACKET_SIZE));

	offset += sizeof(PACKET_SIZE);


	memcpy(mBuffer + offset, stream.GetBuffer(), stream.GetLength());

	offset += stream.GetLength();

	send(mSocket.GetHandle(), mBuffer, offset, 0);
}

std::shared_ptr<Package> TerminalSession::OnRecv()
{
	char buf[10240];

	recv(mSocket.GetHandle(), buf, 10240, 0);
	size_t offset = 0;
	size_t size[1] = { 0 };
	memcpy(size, buf, sizeof(PACKET_SIZE));

	offset += sizeof(PACKET_SIZE);

	std::shared_ptr<Packet> packet = PacketAnalyzer::GetInstance().analyze(buf + offset, size[0] - offset);

	if (packet == nullptr) {
		return nullptr;
	}

	std::shared_ptr<Package> package = std::make_shared<Package>(this, packet);

	return std::move(package);
}

}