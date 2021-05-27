#pragma once
#include "stdafx.h"

namespace leeder
{
class IOCPServer : public Server
{
public:
	IOCPServer(std::unique_ptr<ContentsProcess>&& contents);
	~IOCPServer();



	void Run() override; // 순수 가상함수 처리. 

	HANDLE&		GetIOCP() { return mIOCP; }


	bool			RegistCompletionPort(SOCKET socket, ULONG_PTR key);



private:
	bool			createCompletionPort();

	HANDLE				mIOCP;	
	
	std::vector<std::unique_ptr<Thread>>	mWorkThreadPool;


};



}
