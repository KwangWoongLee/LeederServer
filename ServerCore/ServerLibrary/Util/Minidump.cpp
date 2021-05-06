#include "stdafx.h"
#include "MiniDump.h"
namespace leeder
{
    Minidump::Minidump()
    {
        ::SetUnhandledExceptionFilter(exceptionFilter);
        printf("* Dump filter setting complte!\n");
    }

    LONG WINAPI Minidump::exceptionFilter(struct _EXCEPTION_POINTERS* exceptionInfo)
    {
        // 프로그램 시작부터 모두 덤프
        _CrtMemDumpAllObjectsSince(NULL);

        HMODULE dumpDll = nullptr;
        dumpDll = ::LoadLibraryA("DBGHELP.DLL");
        if (!dumpDll) {
            printf("DBGHelp.dll cannot be loaded\n");
            return 0;
        }

        //TODO :: DUMPNAME
        std::wstring name = L"minidump.dmp";

        HANDLE file = ::CreateFile(name.c_str(), GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
        if (file == INVALID_HANDLE_VALUE) {
            printf("! dump file not making\n");
            return 0;
        }

        _MINIDUMP_EXCEPTION_INFORMATION info;
        info.ThreadId = ::GetCurrentThreadId();
        info.ExceptionPointers = exceptionInfo;
        info.ClientPointers = NULL;

        WRITEDUMP dumpFunc = (WRITEDUMP)::GetProcAddress(dumpDll, "MiniDumpWriteDump");
        if (dumpFunc(GetCurrentProcess(), GetCurrentProcessId(), file, MiniDumpNormal, &info, NULL, NULL) == FALSE) {
            printf("! dump file saving error\n");
            return 0;
        }
        ::CloseHandle(file);

        return EXCEPTION_CONTINUE_SEARCH;
    }

    static Minidump minidump;
}
