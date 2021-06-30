#include "stdafx.h"
#include "LoginProcess.h"

LoginProcess::LoginProcess()
{
	RegistFunction(ePacketType::CS_REQ_AUTH, std::bind(&LoginProcess::CS_REQ_AUTH, this, std::placeholders::_1, std::placeholders::_2));
	RegistFunction(ePacketType::DB_RES_AUTH, std::bind(&LoginProcess::DB_RES_AUTH, this, std::placeholders::_1, std::placeholders::_2));
}


//---------------------------------------------------------------//
//패킷 처리 정의
void LoginProcess::CS_REQ_AUTH(Session* session, std::shared_ptr<Packet>& packet)
{
	//PK_C_REQ_ID_PW* packet = (PK_C_REQ_ID_PW*)rowPacket;

	//PK_I_DB_REQ_ID_PW dbPacket;
	//dbPacket.clientId_ = (UInt64)session->id();
	//dbPacket.id_ = packet->id_;
	//dbPacket.password_ = packet->password_;

	//Terminal* terminal = _terminal.get(L"DBAgent");
	//terminal->sendPacket(&dbPacket);
}

void LoginProcess::DB_RES_AUTH(Session* session, std::shared_ptr<Packet>& packet)
{
	//PK_I_DB_ANS_ID_PW* packet = (PK_I_DB_ANS_ID_PW*)rowPacket;
	//SLog(L"* id/ pw result = %d", packet->result_);

	//Session* clientSession = _session_manager.session(packet->clientId_);
	//if (clientSession == nullptr) {
	//	return;
	//}

	//const int authFail = 0;
	//if (packet->result_ == authFail) {
	//	PK_S_ANS_ID_PW_FAIL ansPacket;
	//	clientSession->sendPacket(&ansPacket);
	//	return;
	//}

	//PK_I_CHTTING_NOTIFY_ID iPacket;
	//iPacket.oidAccountId_ = packet->oidAccountId_;
	//iPacket.clientId_ = packet->clientId_;
	//Terminal* terminal = _terminal.get(L"ChattingServer");
	//if (terminal == nullptr) {
	//	SLog(L"! Chatting Server terminal is not connected");
	//}
	//terminal->sendPacket(&iPacket);
}
