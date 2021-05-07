#include "stdafx.h"
#include "Clock.h"

namespace leeder
{

    Clock::Clock()
    {
        mServerStartTime = std::chrono::system_clock::now();
    }


    Clock::~Clock()
    {
    }

    std::wstring Clock::GetTimeNowToWS() const
    {
        std::chrono::system_clock::time_point currentTimePoint = std::chrono::system_clock::now();

        __time64_t currentTime = std::chrono::system_clock::to_time_t(currentTimePoint);
        tm   tm;

        errno_t err = localtime_s(&tm, &currentTime);

        std::array<WCHAR, 128> time;

        wcsftime(time.data(), time.size(), L"D%Y-%m-%dT%H:%M:%S", &tm);


        return time.data();
    }
}