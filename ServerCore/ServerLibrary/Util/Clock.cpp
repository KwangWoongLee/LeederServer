#include "stdafx.h"
#include "Clock.h"

namespace leeder
{
    float kDesiredFrameTime = 0.0166f;

    Clock::Clock()
    {
        mServerStartTime = high_resolution_clock::now();
    }


    Clock::~Clock()
    {
    }

    void Clock::Update()
    {
        double currentTime = GetSystemTime();

        mDeltaTime = (float)(currentTime - mLastFrameStartTime);

        //frame lock at 60fps
        while (mDeltaTime < kDesiredFrameTime)
        {
            currentTime = GetSystemTime();

            mDeltaTime = (float)(currentTime - mLastFrameStartTime);
        }

        mLastFrameStartTime = currentTime;
        mFrameStartTimef = static_cast<float> (mLastFrameStartTime);

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