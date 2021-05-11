#include "stdafx.h"
#include "Thread.h"

namespace leeder
{
	std::atomic<int> Thread::mThreadIDSeed = 0;
	thread_local Thread* thisThread = nullptr;


	Thread::Thread(Thread&& rhs)
		: mId(mThreadIDSeed++)
		, mThread(std::move(rhs.mThread))
	{
		thisThread = this;
	}

	Thread& Thread::operator=(Thread&& rhs)
	{
		if (mThread.joinable())
			mThread.join();

		mThread = std::move(rhs.mThread);
		return *this;
	}

	Thread::~Thread()
	{
		if (mThread.joinable())
			mThread.join();
	}


}
