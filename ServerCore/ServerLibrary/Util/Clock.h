#pragma once
#include "stdafx.h"

namespace leeder
{
	class Clock : public Singleton<Clock>
	{
	public:
		Clock();
		~Clock();

		std::time_t		GetSystemTick() { return std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()); };

		std::wstring	GetTimeNowToWS (const wchar_t* format = L"D%Y-%m-%dT%H-%M-%S");

	private:
		std::chrono::system_clock::time_point mServerStartTime;

	};

}