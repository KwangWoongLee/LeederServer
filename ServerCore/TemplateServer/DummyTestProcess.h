#pragma once
#include "stdafx.h"

namespace leeder
{

	class DummyTestProcess : public ContentsProcess
	{
	public:
		DummyTestProcess();

	private:
		void	CS_REQ_HELLO(std::shared_ptr<Session>& session, std::shared_ptr<Packet>& packet);
		void	CS_SEND_INPUTLIST(std::shared_ptr<Session>& session, std::shared_ptr<Packet>& packet);
		void	CS_REQ_EXIT(std::shared_ptr<Session>& session, std::shared_ptr<Packet>& packet);
	};

}

