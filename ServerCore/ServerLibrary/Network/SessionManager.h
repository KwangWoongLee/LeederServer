#pragma once
#include "stdafx.h"

namespace leeder
{
class SessionManager : public Singleton< SessionManager >
{
public:
	SessionManager();
	~SessionManager();

	void	PrepareSessionPool();
	void	AcceptSessions();

	void	AllocSession(std::shared_ptr<Session>& session);
	void	ReturnSession(std::shared_ptr<Session>& returnSession);

	void	printSessionPool();
	void	printSessionList();

	bool	AddSession(const std::shared_ptr<Session>& session);



private:
	std::list<std::shared_ptr<Session>>	mSessionPool;
	std::list<std::shared_ptr<Session>>		mSessionList;

	std::mutex								mSessionPoolMutex;

	int										mMaxSessionCount;
	std::atomic<int>						mSessionCount;

	int										mIssueCount;
	int										mReturnCount;


	static std::atomic<int>					mSessionIDSeed;


	
};


}
