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


class Packet_Test : public Packet
{
public:
	Packet_Test()
		:Packet(ePacketType::TEST) {};

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

	std::string GetID() { return mID; }
	void SetID(std::string id) { mID = id; }

private:
	std::string	mID;

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

private:
	std::string	mID;

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

private:
	std::string	mID;

};


};
