#pragma once
#include "stdafx.h"

namespace leeder
{

	constexpr auto BUF_SIZE = 1024 * 10;

	enum class eIOType
	{
		NONE,
		DISCONNECT,
		SEND,
		RECV_ZERO,
		RECV,
		ACCEPT
	};


	enum class eDisconnectReason
	{
		RECV_ZERO,
		ACTIVE,
		ONCONNECT_ERROR,
		COMPLETION_ERROR
	};

	class IOCPSession;

	class Overlapped
	{

	public:
		Overlapped() = delete;
		Overlapped(void* data);



		void*				GetIOData() { return mIOData; }
		void				SetIOData(void* data) { mIOData = data; }

	private:
		OVERLAPPED		mOverlapped;
		void*			mIOData;
	};

	class IOData
	{
	public:
		IOData(eIOType type);
		eIOType				GetType() { return mType; }
		void				SetType(eIOType type) { mType = type; }

		void	SetSession(std::shared_ptr<IOCPSession> session) { mSession = session; }
		std::shared_ptr<IOCPSession>	GetSession() { return mSession; }

		char*	GetBuffer() { return mBuffer; }

	protected:
		eIOType			mType;
		std::shared_ptr<IOCPSession>	mSession;

		char						mBuffer[BUF_SIZE];
	};

	class AcceptIOData : public IOData
	{
	public:
		AcceptIOData()
			: IOData(eIOType::ACCEPT) {};

	};

	class ZeroIOData : public IOData
	{
	public:
		ZeroIOData()
			: IOData(eIOType::RECV_ZERO) {};

	};

	class RWIOData : public IOData
	{
	public:
		RWIOData(eIOType type);
		void Reset();

		void	SetTotalByte();
		bool	IsRemainToIO(size_t transffered);


	private:
		size_t	mTotalByte;
		size_t	mCurrentByte;

	};

}
