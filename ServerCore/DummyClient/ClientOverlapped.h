#pragma once
#include "stdafx.h"

namespace leeder
{
	enum class eClientIOType
	{
		NONE,
		SEND,
		RECV,
		CONNECT
	};


	class Client;
	class ClientIOData;

	class ClientOverlapped
	{

	public:
		ClientOverlapped() = delete;
		ClientOverlapped(std::shared_ptr<ClientIOData> data);


		std::shared_ptr<ClientIOData>			GetIOData() { return mIOData; }
		void							SetIOData(std::shared_ptr<ClientIOData> data) { mIOData = data; }

	private:
		OVERLAPPED		mOverlapped;
		std::shared_ptr<ClientIOData>			mIOData;
	};

	class ClientIOData
	{
	public:
		ClientIOData(eClientIOType type);
		eClientIOType				GetType() { return mType; }
		void				SetType(eClientIOType type) { mType = type; }

		void	SetClient(std::shared_ptr<Client> client) { mClient = client; }
		std::shared_ptr<Client>	GetClient() { return mClient; }


		char* GetBuffer() { return mBuffer; }

	protected:
		eClientIOType			mType;
		std::shared_ptr<Client> mClient;

		char						mBuffer[BUF_SIZE];
	};


	class ConnectIOData : public ClientIOData
	{
	public:
		ConnectIOData()
			: ClientIOData(eClientIOType::CONNECT) {};

	};


	class ClientRWIOData : public ClientIOData
	{
	public:
		ClientRWIOData(eClientIOType type);
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



};

