#pragma once
#include "stdafx.h"

namespace leeder
{
class Server;

constexpr int LifeTime = 15;

class SessionManager : public Singleton< SessionManager >
{
public:
	char								mAcceptBuffer[64];

	SessionManager();
	~SessionManager();

	void								PrepareSessionPool();
	void								AcceptSessions(SOCKET listenSocket);


	bool								AddSession(IOCPSession* session);
	std::list<IOCPSession*>::iterator	ReturnSession(IOCPSession* returnSession);

	void								CheckHeartBeat();


private:
	std::list<IOCPSession*>		mSessionPool;
	std::list<IOCPSession*>		mSessionList;

	std::mutex					mSessionPoolMutex;

	int							mMaxSessionCount;
	std::atomic<int>			mSessionCount;

	int							mIssueCount;
	int							mReturnCount;


	static std::atomic<int>		mSessionIDSeed;


	
};


}
