#pragma once
#include "stdafx.h"

class GameProcess : public ContentsProcess
{

public:
	GameProcess();

private:
	void	CS_REQ_HELLO(Session* session, std::shared_ptr<Packet>& packet);
	void	CS_REQ_EXIT(Session* session, std::shared_ptr<Packet>& packet);
	void	CS_SEND_INPUTLIST(Session* session, std::shared_ptr<Packet>& packet);
	void	T_NOTIFY_AUTH(Session* session, std::shared_ptr<Packet>& packet);
};
