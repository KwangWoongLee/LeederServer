#pragma once
#include "stdafx.h"

namespace leeder
{
	class Clock : public Singleton<Clock>
	{
	public:
		Clock();
		~Clock();

		std::wstring GetTimeNowToWS() const;

	private:
		std::chrono::system_clock::time_point mServerStartTime;

	};

}