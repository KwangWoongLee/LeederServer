#include "stdafx.h"
#include "Overlapped.h"

namespace leeder
{
Overlapped::Overlapped(void* ioData)
{
	SetIOData(ioData);
	ZeroMemory(&mOverlapped, sizeof(mOverlapped));
}


RWIOData::RWIOData(eIOType type)
	:IOData(type)
	, mTotalByte(0)
	, mCurrentByte(0)
{
}

void RWIOData::Reset()
{
	mTotalByte = 0;
	mCurrentByte = 0;
	memset(mBuffer, 0, sizeof(char) * BUF_SIZE);
}

void RWIOData::SetTotalByte()
{
	PACKET_SIZE size = 0; 
	if (mTotalByte == 0) {
		memcpy((void*)size, (void*)mBuffer, sizeof(PACKET_SIZE));

		mTotalByte = (size_t)size;
	}
}

bool RWIOData::IsRemainToIO(size_t transffered)
{
	mCurrentByte += transffered;
	if (mCurrentByte < mTotalByte) {
		return true;
	}
	return false;
}


IOData::IOData(eIOType type)
	:mType(type)
{
}

}
