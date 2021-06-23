#pragma once
#include "stdafx.h"

namespace leeder
{
class Packet
{
public:
	Packet(ePacketType type)
		: mType(type) {};

	ePacketType				GetType() { return mType; }
	PACKET_TYPE_SIZE		GetTypeToInt() { return static_cast<PACKET_TYPE_SIZE>(mType); }

	virtual void Encode(OutputStream& stream) {};
	virtual void Decode(InputStream& stream) {};


private:
	ePacketType mType;

};


class PK_CS_REQ_EXIT : public Packet
{
public:
	PK_CS_REQ_EXIT()
		:Packet(ePacketType::CS_REQ_EXIT) {};

	void Encode(OutputStream& stream) override;



};


class PK_SC_RES_EXIT : public Packet
{
public:
	PK_SC_RES_EXIT()
		:Packet(ePacketType::SC_RES_EXIT) {};

	void Encode(OutputStream& stream) override;



};


class PK_CS_NOTIFY_HEARTBEAT : public Packet
{
public:
	PK_CS_NOTIFY_HEARTBEAT()
		:Packet(ePacketType::CS_NOTIFY_HEARTBEAT) {};

	void Encode(OutputStream& stream) override;

};

class PK_CS_REQ_HELLO : public Packet
{
public:
	PK_CS_REQ_HELLO()
		:Packet(ePacketType::CS_REQ_HELLO) {};

	void Encode(OutputStream& stream) override;
	void Decode(InputStream& stream) override;

	std::string GetID() { return mID; }
	void SetID(std::string id) { mID = id; }

private:
	std::string	mID;


};

class PK_SC_RES_WELCOME : public Packet
{
public:
	PK_SC_RES_WELCOME()
		:Packet(ePacketType::SC_RES_WELCOME) {};

	void Encode(OutputStream& stream) override;
	void Decode(InputStream& stream) override;

	std::unordered_map<uint32_t, GameObject>& GetState() { return mNetworkIDToState; };
	void						SetState(std::unordered_map<uint32_t, std::shared_ptr<GameObject>>& networkIDToGameObject);


private:
	std::unordered_map<uint32_t, GameObject> mNetworkIDToState;

};

class PK_CS_REQ_REPLICATION_STATE : public Packet
{
public:
	PK_CS_REQ_REPLICATION_STATE()
		:Packet(ePacketType::CS_REQ_REPLICATION_STATE) {};

	void Encode(OutputStream& stream) override;
};


class PK_SC_REPLICATION_STATE : public Packet
{
public:
	PK_SC_REPLICATION_STATE()
		:Packet(ePacketType::SC_REPLICATION_STATE) {};

	void Encode(OutputStream& stream) override;
	void Decode(InputStream& stream) override;

	std::unordered_map<uint32_t, ObjectInfo>&	GetInfo() { return mNetworkIDToInfo; };
	void						SetInfo(std::unordered_map<uint32_t, ObjectInfo>& networkIDToInfo) { mNetworkIDToInfo = networkIDToInfo; }


private:
	std::unordered_map<uint32_t, ObjectInfo> mNetworkIDToInfo;
};

class Input;

class PK_CS_SEND_INPUTLIST : public Packet
{
public:
	PK_CS_SEND_INPUTLIST()
		:Packet(ePacketType::CS_SEND_INPUTLIST) 
	{
		mInputList.reserve(3);
	};

	void Encode(OutputStream& stream) override;
	void Decode(InputStream& stream) override;

	std::vector<Input>&	GetInputList() { return mInputList; };
	void				PushInputType(Input input);


private:
	std::vector<Input>	mInputList;


};

class PK_T_NOTIFY : public Packet
{
public:
	PK_T_NOTIFY()
		:Packet(ePacketType::T_NOTIFY)
	{
	};

	void Encode(OutputStream& stream) override;
};


class PK_T_NOTIFY_AUTH : public Packet
{
public:
	PK_T_NOTIFY_AUTH()
		:Packet(ePacketType::T_NOTIFY_AUTH)
	{
	};

	void Encode(OutputStream& stream) override;
	void Decode(InputStream& stream) override;

	bool		IsAuth() { return mbAuthResult; }
	void		SetAuth(bool auth) { mAuthToken = auth; }

	std::string GetName() { return mName; }
	void SetName(std::string name) { mName = name; }

	int			GetToken() { return mAuthToken; }
	void		SetToken(int token) { mAuthToken = token; }

private:
	bool		mbAuthResult;
	std::string	mName;
	int			mAuthToken;
};

class PK_CS_REQ_AUTH : public Packet
{
public:
	PK_CS_REQ_AUTH()
		:Packet(ePacketType::CS_REQ_AUTH)
	{
	};

	void Encode(OutputStream& stream) override;
	void Decode(InputStream& stream) override;


	std::string GetID() { return mID; }
	void SetID(std::string id) { mID = id; }

	std::string GetPassWord() { return mPassWord; }
	void SetPassWord(std::string pw) { mPassWord = pw; }

private:
	std::string	mID;
	std::string mPassWord;

};

class PK_DB_RES_AUTH : public Packet
{
public:
	PK_DB_RES_AUTH()
		:Packet(ePacketType::DB_RES_AUTH)
	{
	};

	void Encode(OutputStream& stream) override;
	void Decode(InputStream& stream) override;

	bool		IsAuth() { return mbAuthResult; }
	void		SetAuth(bool auth) { mAuthToken = auth; }

	std::string GetName() { return mName; }
	void SetName(std::string name) { mName = name; }

	int			GetToken() { return mAuthToken; }
	void		SetToken(int token) { mAuthToken = token; }

private:
	bool		mbAuthResult;
	std::string	mName;
	int			mAuthToken;

};

};
