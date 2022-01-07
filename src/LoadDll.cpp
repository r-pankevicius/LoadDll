// LoadDll.cpp

#include "pch.h"
#include <iostream>

constexpr int RETCODE_INVALID_ARGS = 1;
constexpr int RETCODE_FAILED_TO_LOAD_DLL = 2;

bool is64Bit();
int printUsage();

// Call LocalFree on returned message.
char* getLastErrorMessage();

int main(const int argc, const char* argv[])
{
    size_t hu = sizeof(int);

    if (argc != 2)
        return printUsage();

    if (is64Bit())
        std::cout << "Running 64 bits version.\n";
    else
        std::cout << "Running 32 bits version.\n";

    const char* pathToDll = argv[1];

    HMODULE hModule = ::LoadLibraryA(pathToDll);
    if (NULL != hModule)
    {
        std::cout << "Succeeded.\n";
        ::FreeLibrary(hModule);
        return 0;
    }

    char* lastErrorMessage = getLastErrorMessage();
    std::cout << "Failed to load `" << pathToDll << "`.\n" << lastErrorMessage << '\n';
    LocalFree(lastErrorMessage);

    return RETCODE_FAILED_TO_LOAD_DLL;
}

int printUsage()
{
    std::cout
        << "LoadDll (LoadDll32 or LoadDll64) loads DLL to test if it can be loaded successfully.\n"
        << "Usage:\n"
        << "\tLoadDll <some.dll>\n";

    return RETCODE_INVALID_ARGS;
}

// https://docs.microsoft.com/en-us/windows/win32/debug/retrieving-the-last-error-code
char* getLastErrorMessage()
{
    char* lpMsgBuf;

    DWORD dw = GetLastError();
    FormatMessageA(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        dw,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPSTR)&lpMsgBuf,
        0, NULL);

    return lpMsgBuf;
}

bool is64Bit()
{
#if defined(_WIN64)
    return true;
#elif defined(_WIN32)
    return false;
#else
    Something went wrong!
#endif
}
