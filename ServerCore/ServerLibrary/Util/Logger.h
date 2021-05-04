#pragma once
#include "stdafx.h"

#include <memory>
#include <fstream>

#include "Singleton.h"

namespace leeder
{
class LogBase
{
public:
    LogBase() = default;
    virtual ~LogBase()
    {
        printf("\n~LogBase();");
    };

    virtual void    Log(const wchar_t* logStr) {};
};


using LogBasePtr = std::shared_ptr< LogBase >;



class LogPrint : public LogBase
{
public:
    LogPrint() {};
    virtual ~LogPrint() {
        printf("\n~LogPrint();");
    };

    void            Log(const wchar_t* logStr) override;
};



class LogWriter
{

public:
    LogWriter();
    virtual ~LogWriter();


    void            Log(const wchar_t* fmt, ...);
    void            Log(const wchar_t* fmt, va_list args);



    LogBasePtr      GetLogger() const { return mLogBase; };
    void            SetLogger(LogBase* base);

private:
    LogBasePtr      mLogBase;
    std::wstring	mPrefix;

};




class Logger : public Singleton< Logger >
{

public:
    Logger();
    virtual ~Logger();

    void Log(const wchar_t* fmt, ...);


private:
    LogWriter   mLogWriter;

};



}
