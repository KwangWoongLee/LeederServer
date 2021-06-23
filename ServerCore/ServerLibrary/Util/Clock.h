#pragma once
#include "stdafx.h"

namespace leeder
{
	using namespace std::chrono;
	class Clock : public Singleton<Clock>
	{
	public:
		Clock();
		~Clock();

		float GetDeltaTime() { return mDeltaTime; }

		void Update();

		double		GetSystemTime() { 
			auto now = high_resolution_clock::now();
			auto ms = duration_cast<milliseconds>(now - mServerStartTime).count();

			return static_cast<double>(ms) / 1000;
		}
		
		float			GetSystemTimeFloat() { 
			auto now = high_resolution_clock::now();
			auto ms = duration_cast<milliseconds>(now - mServerStartTime).count();

			return static_cast<float>(ms) / 1000;
		}




		std::wstring	GetTimeNowToWS (const wchar_t* format = L"D%Y-%m-%dT%H-%M-%S");

	private:
		high_resolution_clock::time_point mServerStartTime;
		float		mDeltaTime;

		double		mLastFrameStartTime;
		float		mFrameStartTimef;

	};

}