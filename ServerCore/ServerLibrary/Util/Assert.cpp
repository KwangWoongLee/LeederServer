#include "stdafx.h"
#include "Assert.h"


void leeder::Assert(bool condition, LPCWSTR conditionStr, LPCWSTR fileName, int lineNo)
{
    if (condition) {
        return;
    }
    //shutdownServer();

    std::wstring assertMsg;
    assertMsg = L" Assert: ";
    assertMsg += conditionStr;
    assertMsg += L", file : ";
    assertMsg += fileName;
    assertMsg += L", line : ";

    WCHAR buf[16];
    _itow_s(lineNo, buf, 10);
    assertMsg += buf;

    printf("%ws", assertMsg.c_str());


    Minidump::GetInstance().exceptionFilter(NULL);

}
