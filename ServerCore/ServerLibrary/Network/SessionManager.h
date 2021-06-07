#pragma once
#include "stdafx.h"

namespace leeder
{
class Server;

constexpr int LifeTime = 15;

class SessionManager : public Singleton< SessionManager >
{
public:


	SessionManager();
	~SessionManager();

	void	PrepareSessionPool();
	void	AcceptSessions(SOCKET listenSocket);


	bool	AddSession(const std::shared_ptr<IOCPSession>& session);
	std::list<std::shared_ptr<IOCPSession>>::iterator	ReturnSession(std::shared_ptr<IOCPSession> returnSession);

	void	CheckHeartBeat();



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
