#include "stdafx.h"
#include "Logger.h"

namespace leeder
{

void LogPrint::Log(const wchar_t* logStr)
{
	printf("%ws", logStr);
}


LogFile::LogFile()
{
	XMLDocument config;

	if (!loadConfig(&config)) {
		printf("Log Config File Not Found\n");
		exit(0);
		return;
	}
	this->Init(&config);

}

LogFile::~LogFile()
{
	mFileStream.close();
	mFileStream.clear();



	// log ������ ���
	std::size_t found = mFilePath.find(L".log");
	if (found == std::wstring::npos)  
	{
		return;
	}

	std::wstring closeFileName = mFilePath.substr(0, found);
	closeFileName += Clock::GetInstance().GetTimeNowToWS();
	closeFileName += L".log";

	// ���� �̸� ����
	_wrename(mFilePath.c_str(), closeFileName.c_str());
}

void LogFile::Init(XMLDocument* config)
{
	XMLElement* root = config->FirstChildElement("App")->FirstChildElement("Log");

	XMLElement* elem = root->FirstChildElement("Path");
	
	mFilePath = CA2W(elem->GetText());


	// Log ���丮�� ���� ��� �ڵ� ���� 
	std::size_t found = mFilePath.find_last_of(L"/");
	if (found == std::wstring::npos)
	{
		return;
	}

	std::wstring foldername = mFilePath.substr(0, found);

	namespace fs = std::filesystem;
	fs::path tmp(foldername);
	if (!fs::exists(tmp))
		fs::create_directory(tmp);

	mFileStream.open(mFilePath, std::ios::out | std::ios::trunc);

	if (mFileStream.bad()) {
		printf("Log File Open is Failed\n");
		assert(false);
	}
}

void LogFile::Log(const wchar_t* logStr)
{
	printf("%ws", logStr);

	mFileStream << logStr;
	mFileStream.flush();
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
	//TODO :: ThreadID , Time�� logStr�� �߰� 

	std::wstring msg = L"";

	msg += mPrefix;


	std::array<WCHAR, 1024> logStr;
	vswprintf_s(logStr.data(), logStr.size(), fmt, args);

	msg += logStr.data();


	mLogBase->Log(msg.c_str());


}

void LogWriter::SetLogger(eLogType eType, std::wstring prefix)
{
	mPrefix.clear();
	mPrefix = prefix;

	if (mLogBase)
		mLogBase.reset();

	switch ((eLogType)eType)
	{
	case eLogType::PRINT:
		mLogBase = std::make_shared<LogPrint>();
		break;
	case eLogType::FILE:
		mLogBase = std::make_shared<LogFile>();
		break;
	}

}

SysLogger::SysLogger()
{
	XMLDocument config;

	if (!loadConfig(&config)) {
		printf("!!! have not config file\n");
		exit(0);
		return;
	}
	this->Init(&config);
}

SysLogger::~SysLogger()
{
}

void SysLogger::Init(XMLDocument* config)
{
	LogBase* base = nullptr;

	XMLElement* root = config->FirstChildElement("App")->FirstChildElement("Log");
	
	eLogType logType{};

	if (!root) {
		printf("System Log Default : Print");
		
		logType = eLogType::PRINT;
		mLogWriter.SetLogger(logType, nullptr);

		return;
	}

	//Prefix �������� 
	XMLElement* elem = root->FirstChildElement("Prefix");
	std::wstring prefix = CA2W(elem->GetText());

	elem = root->FirstChildElement("Type");
	const char* type = elem->GetText();

	if (!strcmp(type, "File")) // file�� ���
	{		
		logType = eLogType::FILE;
	}
	else //file�� �ƴ� ���
	{
		logType = eLogType::PRINT;
	}


	mLogWriter.SetLogger(logType, prefix);

}

void SysLogger::Log(const wchar_t* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	mLogWriter.Log(fmt, args);
	va_end(args);
}


};