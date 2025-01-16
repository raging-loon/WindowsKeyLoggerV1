#ifndef ENVIRONMENT_CHECK_H_
#define ENVIRONMENT_CHECK_H_
#include <Windows.h>
#include <winternl.h>

class EnvironmentCheck
{
public:

    static __forceinline bool checkForDebugger()
    {
        return ((PPEB)__readgsqword(0x60))->BeingDebugged != 0;
    }

private:

};

#define KILLSELF_ON_DEBUGGER {if (EnvironmentCheck::checkForDebugger()) exit(0);}


#endif // ENVIRONMENT_CHECK_H_

