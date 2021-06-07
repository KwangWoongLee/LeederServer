#include "stdafx.h"
#include "ClientOverlapped.h"

namespace leeder
{
ClientOverlapped::ClientOverlapped(std::shared_ptr<ClientIOData> data)
{
	SetIOData(data);
	ZeroMemory(&mOverlapped, sizeof(mOverlapped));
}

ClientIOData::ClientIOData(eClientIOType type)
	:mType(type)
{
	ZeroMemory(&mBuffer, sizeof(BUF_SIZE));
}





ClientRWIOData::ClientRWIOData(eClientIOType type)
	:ClientIOData(type)
	, mTotalByte(0)
	, mCurrentByte(0)
{
}

void ClientRWIOData::Reset()
{
	mTotalByte = 0;
	mCurrentByte = 0;
	memset(mBuffer, 0, sizeof(char) * BUF_SIZE);
}

bool ClientRWIOData::SetStreamToIOData(Stream& stream)
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

size_t ClientRWIOData::SetTotalByte()
{
	size_t offset = 0;
	PACKET_SIZE packetSize[1] = { 0 };

	if (mTotalByte == 0) {
		memcpy((void*)packetSize, (void*)mBuffer, sizeof(PACKET_SIZE));

		mTotalByte = (size_t)packetSize[0];
	}

	offset += sizeof(PACKET_SIZE);

	return offset;
}

bool ClientRWIOData::IsRemainToIO(size_t transffered)
{
	mCurrentByte += transffered;
	if (mCurrentByte < mTotalByte) {
		return true;
	}
	return false;
}


}