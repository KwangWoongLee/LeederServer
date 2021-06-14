#pragma once
#include "stdafx.h"

namespace leeder
{

	class DummyTestProcess : public ContentsProcess
	{
	public:
		DummyTestProcess();

	private:
		void	CS_REQ_HELLO(IOCPSession* session, std::shared_ptr<Packet>& packet);
		void	CS_SEND_INPUTLIST(IOCPSession* session, std::shared_ptr<Packet>& packet);
		void	CS_REQ_EXIT(IOCPSession* session, std::shared_ptr<Packet>& packet);
	};

}

