#include "stdafx.h"
#include "Logger.h"

namespace leeder
{

void LogPrint::Log(const wchar_t* logStr)
{
	printf("%ws", logStr);
}



LogWriter::LogWriter()
	: mLogBase(nullptr)
{
}

LogWriter::~LogWriter()
{
}

void LogWriter::Log(const wchar_t* fmt, ...)
{
	va_list args;
	va_start(args, fmt);

	this->Log(fmt, args);

	va_end(args);
}

void LogWriter::Log(const wchar_t* fmt, va_list args)
{

	std::array<WCHAR, 1024> logStr;

	vswprintf_s(logStr.data(), logStr.size(), fmt, args);

	std::wstring msg = L"";
	msg += logStr.data();

	mLogBase->Log((WCHAR*)msg.c_str());


}

void LogWriter::SetLogger(LogBase* base)
{

	mLogBase = std::shared_ptr<LogBase>(base);
}

Logger::Logger()
{
	mLogWriter.SetLogger(new LogPrint());
}

Logger::~Logger()
{
}

void Logger::Log(const wchar_t* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	mLogWriter.Log(fmt, args);
	va_end(args);
}


};