#pragma once
#include "stdafx.h"

namespace leeder
{

class ChattingProcess : public ContentsProcess
{
public:
	ChattingProcess();

private:
	void	SC_RES_WELCOME(std::shared_ptr<Session>& session, std::shared_ptr<Packet>& packet);
	void	CS_REQ_PARTICIPATE_CHATTING(std::shared_ptr<Session>& session, std::shared_ptr<Packet>& packet);
	void	CS_REQ_CHATTING_MSG(std::shared_ptr<Session>& session, std::shared_ptr<Packet>& packet);
	void	SC_MSG_NOTIFY_ALL(std::shared_ptr<Session>& session, std::shared_ptr<Packet>& packet);
};

}

