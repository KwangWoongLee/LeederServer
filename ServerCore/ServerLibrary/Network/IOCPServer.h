#pragma once
#include "stdafx.h"

namespace leeder
{
class IOCPServer : public Server
{
public:
	IOCPServer();
	~IOCPServer();



	void Run() override; // 순수 가상함수 처리. 


private:
	//헬퍼 함수
	bool			createCompletionPort();
	bool			registCompletionPort(SOCKET socket, ULONG_PTR completionKey);


private:
	Socket				mListenSocket;
	HANDLE				mIOCP;	
	
	std::unique_ptr<Thread>					mAcceptThread;
	std::vector<std::unique_ptr<Thread>>	mWorkThreadPool;


};



}
