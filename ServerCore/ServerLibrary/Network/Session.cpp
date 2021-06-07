#include "stdafx.h"
#include "Session.h"

namespace leeder
{
Session::Session()
	:mType(eSessionType::CLIENT)
	, mID(0)
	, mConnected(false)
{
}
Session::~Session()
{
}

void Session::UpdateHeartBeat()
{
	mLastHeartBeat = Clock::GetInstance().GetSystemTick();
}


}



