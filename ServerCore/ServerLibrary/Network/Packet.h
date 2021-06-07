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

	std::vector<std::shared_ptr<GameObject>>&	GetGameObjects() { return mGameObjects; };
	void						SetGameObjects(std::vector<std::shared_ptr<GameObject>>& gameObjects) { mGameObjects = gameObjects; }


private:
	std::vector<std::shared_ptr<GameObject>>	mGameObjects;
};

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

	std::vector<eInputType>&	GetInputList() { return mInputList; };
	void						PushInputType(eInputType type);


private:
	std::vector<eInputType>	mInputList;


};

class PK_CS_REQ_PARTICIPATE_CHATTING : public Packet
{
public:
	PK_CS_REQ_PARTICIPATE_CHATTING()
		:Packet(ePacketType::CS_REQ_PARTICIPATE_CHATTING) {};

	void Encode(OutputStream& stream) override;
	void Decode(InputStream& stream) override;

	std::string GetID() { return mID; }
	void SetID(std::string id) { mID = id; }

private:
	std::string	mID;

};

class PK_CS_REQ_CHATTING_MSG : public Packet
{
public:
	PK_CS_REQ_CHATTING_MSG()
		:Packet(ePacketType::CS_REQ_CHATTING_MSG) {};

	void Encode(OutputStream& stream) override;
	void Decode(InputStream& stream) override;

	std::string GetID() { return mID; }
	void SetID(std::string id) { mID = id; }

	std::string	GetMSG() { return mMSG; }
	void	SetMSG(std::string msg) { mMSG = msg; }


private:
	std::string	mID;
	std::string	mMSG;

};

class PK_SC_MSG_NOTIFY_ALL : public Packet
{
public:
	PK_SC_MSG_NOTIFY_ALL()
		:Packet(ePacketType::SC_MSG_NOTIFY_ALL) {};

	void Encode(OutputStream& stream) override;
	void Decode(InputStream& stream) override;

	std::string GetID() { return mID; }
	void SetID(std::string id) { mID = id; }

	std::string	GetMSG() { return mMSG; }
	void	SetMSG(std::string msg) { mMSG = msg; }

private:
	std::string	mID;
	std::string	mMSG;

};


};
