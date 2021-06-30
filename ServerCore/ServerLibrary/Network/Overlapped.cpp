#include "stdafx.h"
#include "Overlapped.h"

namespace leeder
{
Overlapped::Overlapped(IOCPSession* session)
{
	mSession = session;
	ZeroMemory(&mOverlapped, sizeof(mOverlapped));
}

Overlapped::~Overlapped()
{
}

std::mutex Overlapped::mLock;
}
