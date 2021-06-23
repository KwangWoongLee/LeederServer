#pragma once
#include "stdafx.h"

namespace leeder
{

enum class eTermialState
{
	STOP,
	UNCONNECT,
	READY,
};

class Server;


class Terminal
{
public:
	Terminal(Server* server, std::string name);
	~Terminal();

	void Run();

	void SetIP(std::string ip) { mIP = ip; }
	void SetPort(uint16_t port) { mPort = port; }

private:
	Server* mServer;
	std::string mName;

	TerminalSession mSession;

	eTermialState	mState;
	std::string mIP;
	uint16_t	mPort;

	std::unique_ptr<Thread>	mProcessThread;
};
}