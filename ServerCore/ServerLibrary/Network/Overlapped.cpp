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


//
//RWIOData::RWIOData(eIOType type)
//	: mTotalByte(0)
//	, mCurrentByte(0)
//{
//}
//
//void RWIOData::Reset()
//{
//	mTotalByte = 0;
//	mCurrentByte = 0;
//	memset(mBuffer, 0, sizeof(char) * BUF_SIZE);
//}
//
//bool RWIOData::SetStreamToIOData(Stream& stream)
//{
//	this->Reset();
//
//	if (BUF_SIZE < stream.GetLength() + sizeof(PACKET_SIZE))
//	{
//		//에러 처리
//		return false;
//	}
//
//	//패킷 총길이
//	PACKET_SIZE packetTotalLength[1] = { sizeof(PACKET_SIZE) + sizeof(float) + stream.GetLength() };
//
//
//	size_t	offset = sizeof(PACKET_SIZE);
//
//	//패킷 총길이 입력
//	memcpy((void*)mBuffer, (void*)packetTotalLength, sizeof(PACKET_SIZE));
//
//	float time = Clock::GetInstance().GetSystemTimeFloat();
//	memcpy((void*)(mBuffer + offset), &time, sizeof(float));
//
//	offset += sizeof(float);
//
//	// 스트림 데이터 입력
//	memcpy((void*)(mBuffer + offset), (void*)stream.GetBuffer(), stream.GetLength());
//	
//	mTotalByte = packetTotalLength[0];
//
//
//	return true;
//
//}
//
//size_t RWIOData::SetTotalByte()
//{
//	size_t offset = 0;
//	PACKET_SIZE packetSize[1] = {0};
//
//	if (mTotalByte == 0) {
//		memcpy((void*)packetSize, (void*)mBuffer, sizeof(PACKET_SIZE));
//
//		mTotalByte = (size_t)packetSize[0];
//	}
//
//	offset += sizeof(PACKET_SIZE);
//
//	float recvTime[1] = { 0 };
//	memcpy((void*)recvTime, mBuffer + offset, sizeof(float));
//	mRecvTime = recvTime[0];
//
//	offset += sizeof(float);
//
//
//	return offset;
//}
//
//bool RWIOData::IsRemainToIO(size_t transffered)
//{
//	mCurrentByte += transffered;
//	if (mCurrentByte < mTotalByte) {
//		return true;
//	}
//	return false;
//}


std::mutex Overlapped::mLock;
}
