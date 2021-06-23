#pragma once
#include "stdafx.h"

class LoginProcess : public ContentsProcess
{
public:
	LoginProcess();
private:
	void CS_REQ_AUTH(Session* session, std::shared_ptr<Packet>& packet);
	void DB_RES_AUTH(Session* session, std::shared_ptr<Packet>& packet);
};