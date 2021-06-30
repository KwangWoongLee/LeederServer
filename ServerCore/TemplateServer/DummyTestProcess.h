#pragma once
#include "stdafx.h"

namespace leeder
{

	class DummyTestProcess : public ContentsProcess
	{
	public:
		DummyTestProcess();

	private:
		void	CS_REQ_HELLO(Session* session, std::shared_ptr<Packet>& packet);
		void	CS_SEND_INPUTLIST(Session* session, std::shared_ptr<Packet>& packet);
		void	CS_REQ_EXIT(Session* session, std::shared_ptr<Packet>& packet);
	};

}

