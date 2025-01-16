#ifndef _DISK_H_
#define _DISK_H_

#include <string>
#include <memory>

#include <Windows.h>

namespace dhm 
{

enum DiskFlags : int
{
    NONE = 0,
    NON_EXISTANT = 1 << 0,
    READY_FOR_MONITORING = 1 << 1
};

class Disk
{
public:

    Disk(const std::string& devicePath);
    ~Disk();

    bool exists();
    bool readyForMonitoring();




private:
    ///
    /// PURPOSE
    ///     Internal name of the device, e.g. \\?\Volume{someuid}\
    /// 
    std::string m_devicePath;

    STORAGE_DESCRIPTOR_HEADER   m_header;

    std::unique_ptr<STORAGE_DEVICE_DESCRIPTOR> m_descriptor;

    HANDLE m_deviceHandle;

    uint8_t m_flags;

private:

    bool queryBaseInfo();

};

} // dhm

#endif // _DISK_H_