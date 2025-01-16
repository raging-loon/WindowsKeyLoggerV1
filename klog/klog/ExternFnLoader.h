#ifndef EXTERN_FUNCTION_LOADER_H_
#define EXTERN_FUNCTION_LOADER_H_

#include <Windows.h>
#include <iphlpapi.h>


class ExternFnLdr
{
public:

    bool load();

    static HANDLE(*Kl_IcmpCreateFile)();
    static DWORD (*Kl_IcmpSendEcho)(
            HANDLE                 IcmpHandle,
            IPAddr                 DestinationAddress,
            LPVOID                 RequestData,
            WORD                   RequestSize,
            PIP_OPTION_INFORMATION RequestOptions,
            LPVOID                 ReplyBuffer,
            DWORD                  ReplySize,
            DWORD                  Timeout
        );
    static BOOL (*Kl_IcmpCloseHandle)(HANDLE);

    static HHOOK (*Kl_SetWindowsHookExA)(
        int       idHook,
        HOOKPROC  lpfn,
        HINSTANCE hmod,
        DWORD     dwThreadId
    );

    static BOOL(*Kl_UnhookWindowsHookEx)(HHOOK);

};

#endif // EXTERN_FUNCTION_LOADER_H_