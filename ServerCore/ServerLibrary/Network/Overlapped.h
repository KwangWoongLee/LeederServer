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
	class IOData;

	class Overlapped
	{

	public:
		Overlapped() = delete;
		Overlapped(std::shared_ptr<IOData> data);



		std::shared_ptr<IOData>			GetIOData() { return mIOData; }
		void							SetIOData(std::shared_ptr<IOData> data) { mIOData = data; }

	private:
		OVERLAPPED		mOverlapped;
		std::shared_ptr<IOData>			mIOData;
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

	class DisconnectIOData : public IOData
	{
	public:
		DisconnectIOData(eDisconnectReason& reason)
			: IOData(eIOType::DISCONNECT)
			, mReason(reason)
		{};

		eDisconnectReason&	GetReason() { return mReason; };

	private:
		eDisconnectReason mReason;
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

		bool	SetStreamToIOData(Stream& stream);

		size_t	SetTotalByte();
		bool	IsRemainToIO(size_t transffered);

		size_t	GetTotalByte() { return mTotalByte; }
		size_t	GetCurrentByte() { return mCurrentByte; }


	private:
		size_t	mTotalByte;
		size_t	mCurrentByte;

	};

}