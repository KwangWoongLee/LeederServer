#pragma once
#include "stdafx.h"

namespace leeder
{
class IOCPServer : public Server
{
public:
	IOCPServer();
	~IOCPServer();



	void Run() override; // ���� �����Լ� ó��. 

	HANDLE&		GetIOCP() { return mIOCP; }


	bool			RegistCompletionPort(SOCKET socket, ULONG_PTR key);



private:
	bool			createCompletionPort();

	HANDLE				mIOCP;	
	
	std::vector<std::unique_ptr<Thread>>	mWorkThreadPool;


};



}
