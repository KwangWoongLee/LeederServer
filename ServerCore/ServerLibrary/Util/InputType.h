#pragma once
#include "stdafx.h"

namespace leeder
{
enum class eInputType : int
{
	NONE,
	SPACE,
	A,
	S,
	D,
	W,
};



class Input
{
public:
	Input(eInputType type)
		: mType(type)
	{
		mTimeStamp = Clock::GetInstance().GetSystemTimeFloat();
	};

	Input() 
		: mTimeStamp(0.f)
	
	{};
	~Input() {};


	eInputType	GetType() { return mType; }
	void		SetType(eInputType type) { mType = type; }

	float		GetTimeStamp() { return mTimeStamp; }
	void		SetTimeStamp(float timeStamp) { mTimeStamp = timeStamp; }

private:
	eInputType	mType;
	float		mTimeStamp;
};

};