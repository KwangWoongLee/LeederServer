#pragma once
#include "stdafx.h"

namespace leeder
{
class Server;

class SessionManager : public Singleton< SessionManager >
{
public:
	SessionManager();
	~SessionManager();

	void	PrepareSessionPool();
	void	AcceptSessions(SOCKET listenSocket);

	void	AllocSession(std::shared_ptr<IOCPSession>& session);
	void	ReturnSession(std::shared_ptr<IOCPSession>& returnSession);

	void	printSessionPool();
	void	printSessionList();

	bool	AddSession(const std::shared_ptr<IOCPSession>& session);



private:
	std::list<std::shared_ptr<IOCPSession>>		mSessionPool;
	std::list<std::shared_ptr<IOCPSession>>		mSessionList;

	std::mutex								mSessionPoolMutex;

	int										mMaxSessionCount;
	std::atomic<int>						mSessionCount;

	int										mIssueCount;
	int										mReturnCount;


	static std::atomic<int>					mSessionIDSeed;


	
};


}
