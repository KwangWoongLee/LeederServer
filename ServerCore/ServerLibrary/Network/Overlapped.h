#pragma once
#include "stdafx.h"

namespace leeder
{

	constexpr auto BUF_SIZE = 1024 * 10;


	enum class eIOType
	{
		NONE,
		SEND,
		RECV_ZERO,
		RECV,
		ACCEPT,
		CONNECT
	};


	enum class eDisconnectReason
	{
		RECV_ZERO,
		ACTIVE,
		ALREADY_REGISTED,
		UNREGISTED,
		ONCONNECT_ERROR,
		COMPLETION_ERROR,
		DIE,
	};

	class IOCPSession;

	class Overlapped : public ObjectPool<Overlapped>
	{

	public:
		Overlapped(IOCPSession*);
		~Overlapped();

		eIOType				GetType() { return mType; }
		void				SetType(eIOType type) { mType = type; }

		void	SetSession(IOCPSession* session) { mSession = session; }
		IOCPSession* GetSession() { return mSession; }

		WSABUF& GetWSABuf() { return mWsaBuf; }


		static std::mutex		mLock;
	private:
		OVERLAPPED		mOverlapped;
		eIOType			mType;
		IOCPSession*	mSession;
		WSABUF			mWsaBuf;
	};



}
