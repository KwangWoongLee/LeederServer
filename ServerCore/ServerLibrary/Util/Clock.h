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

		

		std::time_t		GetSystemTick() { return  system_clock::to_time_t(system_clock::now()); }
		
		float			GetSystemTimeFloat() { 
		
			high_resolution_clock::time_point point = high_resolution_clock::now();
			milliseconds ms = duration_cast<milliseconds>(point.time_since_epoch());

			auto fms = std::chrono::duration<float>(ms);

			return static_cast<float>(fms.count()/1000);
		}



		std::wstring	GetTimeNowToWS (const wchar_t* format = L"D%Y-%m-%dT%H-%M-%S");

	private:
		std::chrono::system_clock::time_point mServerStartTime;

	};

}