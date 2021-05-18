#include "stdafx.h"
#include "SessionManager.h"

namespace leeder
{
std::atomic<int> SessionManager::mSessionIDSeed = 0;

SessionManager::SessionManager()
	: mMaxSessionCount(5)
{
	PrepareSessionPool();

	//printSessionPool();
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
		mSessionPool.push_back(std::make_shared<IOCPSession>());
		mSessionCount++;
	}
}

void SessionManager::AcceptSessions(SOCKET listenSocket)
{
	{
		std::unique_lock<std::mutex> lock(mSessionPoolMutex);

		while (mIssueCount - mReturnCount < mMaxSessionCount)
		{
			std::shared_ptr<IOCPSession> session = mSessionPool.back();


			session->SetID(mSessionIDSeed++);
			mSessionList.push_back(session);

			mSessionPool.pop_back();

			++mIssueCount;

			session->Accept(listenSocket);
		}
	}
}

void SessionManager::AllocSession(std::shared_ptr<IOCPSession>& session)
{
	{
		std::unique_lock<std::mutex> lock(mSessionPoolMutex);

		session = mSessionPool.back();

		session->SetID(mSessionIDSeed++);
		mSessionList.push_back(session);

		mSessionPool.pop_back();

	}
}



void SessionManager::ReturnSession(std::shared_ptr<IOCPSession>& returnSession)
{
	{
		std::unique_lock<std::mutex> lock(mSessionPoolMutex);
		
		returnSession->Reset();

		auto iter = std::find(mSessionList.begin(), mSessionList.end(), returnSession);
		if (iter != mSessionList.end())
		{
			mSessionList.erase(iter);
		}

		mSessionPool.push_back(std::move(returnSession));
		
		++mReturnCount;

	}
}


void SessionManager::printSessionPool()
{
	printf("Session Pool Size : %d\n", mSessionPool.size());
	int i = 0;
	for (auto& session : mSessionPool)
	{
		printf("Session Pool count %d's use count : %d \n" , i, session.use_count() );
		++i;
	}
}


void SessionManager::printSessionList()
{
	printf("Session Pool Size : %d\n", mSessionList.size());
	for (auto& session : mSessionList)
	{
		printf("Session List count %d's use count : %d \n", session->GetID(), session.use_count());
	}
}

bool SessionManager::AddSession(const std::shared_ptr<IOCPSession>& session)
{
	{
		std::unique_lock<std::mutex> lock(mSessionPoolMutex);

		mSessionPool.push_back(session);
		mSessionCount++;
	}

	return true;
}


}