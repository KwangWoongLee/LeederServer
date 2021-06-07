#include "stdafx.h"
#include "Overlapped.h"

namespace leeder
{
Overlapped::Overlapped(std::shared_ptr<IOData> ioData)
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

bool RWIOData::SetStreamToIOData(Stream& stream)
{
	this->Reset();

	if (BUF_SIZE < stream.GetLength() + sizeof(PACKET_SIZE))
	{
		//에러 처리
		return false;
	}

	//패킷 총길이
	PACKET_SIZE packetTotalLength[1] = { sizeof(PACKET_SIZE) + stream.GetLength() };


	size_t	offset = sizeof(PACKET_SIZE);

	//패킷 총길이 입력
	memcpy((void*)mBuffer, (void*)packetTotalLength, sizeof(PACKET_SIZE));

	// 스트림 데이터 입력
	memcpy((void*)(mBuffer + offset), (void*)stream.GetBuffer(), stream.GetLength());
	
	mTotalByte = packetTotalLength[0];

	return true;

}

size_t RWIOData::SetTotalByte()
{
	size_t offset = 0;
	PACKET_SIZE packetSize[1] = {0};

	if (mTotalByte == 0) {
		memcpy((void*)packetSize, (void*)mBuffer, sizeof(PACKET_SIZE));

		mTotalByte = (size_t)packetSize[0];
	}

	offset += sizeof(PACKET_SIZE);

	return offset;
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
