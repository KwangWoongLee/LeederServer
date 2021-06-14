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

    std::wstring Clock::GetTimeNowToWS(const wchar_t* format)
    {
        std::chrono::system_clock::time_point currentTimePoint = std::chrono::system_clock::now();

        __time64_t currentTime = std::chrono::system_clock::to_time_t(currentTimePoint);
        tm   tm;

        
        errno_t err = localtime_s(&tm, &currentTime);

        std::array<wchar_t, 128> time;

        wcsftime(time.data(), time.size(), format, &tm);


        return time.data();
    }
}