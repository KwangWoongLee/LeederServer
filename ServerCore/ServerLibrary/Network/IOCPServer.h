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


private:
	//���� �Լ�
	int				createListenSocket();

	bool			createCompletionPort();
	bool			registCompletionPort(SOCKET socket, ULONG_PTR completionKey);



	bool			bind();
	int				listen(int inBackLog = SOMAXCONN);


private:
	SOCKET				mListenSocket;
	HANDLE				mIOCP;	
	
	std::vector<std::unique_ptr<Thread>>	mWorkThreadPool;


};



}
