#include "FileWriter.h"

#include <windows.h>

bool writeToFile(const std::string& location, const unsigned char* bytes, unsigned int length)
{
    if (!bytes)
        return false;

    HANDLE file = CreateFileA(location.c_str(), GENERIC_WRITE, 0, 0, CREATE_NEW, 0, NULL);
    if (file == INVALID_HANDLE_VALUE)
        return false;

    WriteFile(file, bytes, length, NULL, NULL);
    CloseHandle(file);
    return true;
}