#include "stdafx.h"
#include "Stream.h"


namespace leeder
{

Stream::Stream() 
	: mBuffer(nullptr)
	, mCapacity(0)
	, mHead(0)
{}

Stream::~Stream()
{
	std::free(mBuffer);
}

void Stream::ReallocBuffer(size_t size)
{
	mBuffer = static_cast<char*>(std::realloc(mBuffer, size));

	//俊矾贸府 
	if (mBuffer == nullptr)
	{
	}

	mCapacity = size;
}

OutputStream::OutputStream()
	:Stream()
{
	ReallocBuffer(32);
}



void OutputStream::write(const void* data, size_t size)
{
	size_t resultHead = mHead + size;

	if (resultHead > mCapacity)
	{
		ReallocBuffer((std::max)(mCapacity * 2, resultHead));
	}

	std::memcpy(mBuffer + mHead, data, size);

	mHead = resultHead;
}




InputStream::InputStream(char* buffer, size_t size)
	:Stream()
{
	mBuffer = buffer;
	mCapacity = size;
	mHead = 0;
}

void InputStream::read(void* data, size_t size)
{
	PACKET_SIZE resultHead = static_cast<PACKET_SIZE>(mHead + size);
	if (resultHead > mCapacity)
	{
		//俊矾贸府
	}

	std::memcpy(data, mBuffer + mHead, size);

	mHead = resultHead;
}

}