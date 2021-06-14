#pragma once
#include "stdafx.h"
namespace leeder
{

	enum class eServerState
	{
		UN_INITIALIZE,
		READY,
		RUN,
		STOP,
	};

	class Server
	{
	public:
		Server() = delete;
		Server(std::shared_ptr<ContentsProcess>&& contents);
		virtual ~Server();

		void Init(XMLDocument* config);

		virtual void Run() = 0; // 순수 가상함수 처리. 

		void					PutPackage(std::shared_ptr<Package>&& package);

		std::shared_ptr<ContentsProcess>& GetProcess() { return mContentsProcess; }

		std::shared_ptr<Socket>	GetSocket() { return mListenSocket; }

		eServerState	GetState() const { return mState; }
		void			SetState(eServerState state) { mState = state; }

		std::string		GetName() const { return mName; }
		void			SetName(std::string name) { mName = name; }

		std::string		GetIP() const { return mIP; }
		void			SetIP(std::string ip) { mIP = ip; }

		uint16_t		GetPort() const { return mPort; }
		void			SetPort(uint16_t port) { mPort = port; }

		size_t			GetThreadCount() const { return mThreadCount; }
		void			SetThreadCount(size_t threadCount) { mThreadCount = threadCount; }


	protected:
		std::shared_ptr<Socket>		mListenSocket;

	private:
		eServerState	mState;
		std::string		mName;
		std::string		mIP;
		uint16_t		mPort;
		size_t			mThreadCount;
		std::shared_ptr<ContentsProcess>	mContentsProcess;

	};

};
