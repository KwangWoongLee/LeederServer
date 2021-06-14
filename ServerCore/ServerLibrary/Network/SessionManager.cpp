#include "stdafx.h"
#include "SessionManager.h"

namespace leeder
{
std::atomic<int> SessionManager::mSessionIDSeed = 0;

SessionManager::SessionManager()
	: mMaxSessionCount(5000)
{
	PrepareSessionPool();

}

SessionManager::~SessionManager()
{
	mSessionPool.clear();
	mSessionList.clear();
}

void SessionManager::PrepareSessionPool()
{
	for (int i = 0; i < mMaxSessionCount; ++i)
	{
		mSessionPool.push_back(new IOCPSession());
		mSessionCount++;
	}
}

void SessionManager::AcceptSessions(SOCKET listenSocket)
{
	{
		std::unique_lock<std::mutex> lock(mSessionPoolMutex);

		while (mIssueCount - mReturnCount < mMaxSessionCount)
		{
			auto session = mSessionPool.back();

			session->SetID(mSessionIDSeed++);

			mSessionList.push_back(session);

			mSessionPool.pop_back();

			++mIssueCount;

			session->Accept(listenSocket);
		}
	}
}


std::list<IOCPSession*>::iterator SessionManager::ReturnSession(IOCPSession* returnSession)
{
	{
		std::unique_lock<std::mutex> lock(mSessionPoolMutex);
		
		returnSession->Reset();

		auto iter = std::find(mSessionList.begin(), mSessionList.end(), returnSession);
		if (iter != mSessionList.end())
		{
			iter = mSessionList.erase(iter);
		}

		mSessionPool.push_back(std::move(returnSession));
		
		++mReturnCount;

		return iter;

	}
}

void SessionManager::CheckHeartBeat()
{
	std::time_t now = Clock::GetInstance().GetSystemTick();

	auto iter = mSessionList.begin();
	auto iterEnd = mSessionList.end();
	
	for (; iter != iterEnd;) {
		if ((*iter)->IsConnected())
		{
			std::time_t lastTick = (*iter)->GetLastHeartBeat();
			if (now - lastTick > LifeTime) {
				SysLogger::GetInstance().Log(L"Session %d is Not HeartBeat", (*iter)->GetID());
				iter = (*iter)->OnDisconnect(eDisconnectReason::DIE);
				continue;
			}
		}
		++iter;
	}
}


bool SessionManager::AddSession(IOCPSession* session)
{
	{
		std::unique_lock<std::mutex> lock(mSessionPoolMutex);

		mSessionPool.push_back(session);
		mSessionCount++;
	}

	return true;
}


}