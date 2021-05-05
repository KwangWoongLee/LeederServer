#pragma once
#include "stdafx.h"

#include <fstream>
#include <filesystem>

// Log 연관 클래스 정의

namespace leeder
{

enum class eLogType
{
    PRINT,
    FILE
};

class LogBase
{
public:
    LogBase() = default;
    virtual ~LogBase() = default;

    virtual void    Init(XMLDocument* config) {};

    virtual void    Log(const wchar_t* logStr) {};
};


using LogBasePtr = std::shared_ptr< LogBase >;



class LogPrint : public LogBase
{
public:
    LogPrint() = default;
    virtual ~LogPrint() = default;
    
    void            Log(const wchar_t* logStr) override;
};


class LogFile : public LogBase
{
public:
    LogFile();
    virtual ~LogFile();

    void            Init(XMLDocument* config) override;
    void            Log(const wchar_t* logStr) override;


private:
    std::wfstream   mFileStream;
    std::wstring    mFilePath;

 
};




class LogWriter
{

public:
    LogWriter();
    virtual ~LogWriter();
    LogWriter(const LogWriter& copy) = delete;
    LogWriter& operator=(const LogWriter& rhs) = delete;



    void            Log(const wchar_t* fmt, ...);
    void            Log(const wchar_t* fmt, va_list args);


    void            SetLogger(eLogType eLogType, std::wstring prefix);

private:
    LogBasePtr      mLogBase;
    std::wstring	mPrefix;

};




class SysLogger : public Singleton< SysLogger >
{

public:
    SysLogger();
    virtual ~SysLogger();

    void        Init(XMLDocument* config);

    void        Log(const wchar_t* fmt, ...);


private:
    LogWriter   mLogWriter;

};



}
