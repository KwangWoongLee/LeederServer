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


};
