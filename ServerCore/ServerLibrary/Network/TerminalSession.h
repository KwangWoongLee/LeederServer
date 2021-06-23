#pragma once
#include "stdafx.h"

namespace leeder
{

class TerminalSession : public Session
{
public:
	TerminalSession();
	~TerminalSession();

	bool		Connect(std::string ip, uint16_t port);
	void		SendPacket(Packet* packet);

	std::shared_ptr<Package>	OnRecv();

private:
	char			mBuffer[BUF_SIZE];
};


}
