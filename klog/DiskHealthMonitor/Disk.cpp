#include "Disk.h"

using dhm::Disk;

Disk::Disk(const std::string& devicePath)
    : m_devicePath{ devicePath }, 
      m_header{},
      m_descriptor{nullptr}, 
      m_deviceHandle{INVALID_HANDLE_VALUE},
      m_flags{ DiskFlags::NONE }
{
    m_deviceHandle = CreateFileA(
        m_devicePath.c_str(),
        GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL,
        OPEN_EXISTING,
        0,
        NULL
    );

    if (m_deviceHandle == INVALID_HANDLE_VALUE)
    {
        m_flags |= DiskFlags::NON_EXISTANT;
        return;
    }

    if (!queryBaseInfo())
    {
        CloseHandle(m_deviceHandle);
    }


}

Disk::~Disk()
{
    if (m_deviceHandle != INVALID_HANDLE_VALUE)
    {
        CloseHandle(m_deviceHandle);
        m_deviceHandle = INVALID_HANDLE_VALUE;
    }
}


bool Disk::queryBaseInfo()
{
    STORAGE_PROPERTY_QUERY query = {
        .PropertyId = StorageDeviceProperty,
        .QueryType = PropertyStandardQuery
    };

    DWORD bytesReturned = 0;

    if (!DeviceIoControl(
            m_deviceHandle,
            IOCTL_STORAGE_QUERY_PROPERTY,
            &query,
            sizeof(query),
            &m_header,
            sizeof(m_header),
            &bytesReturned,
            NULL))
    {
        return false;
    }

    std::unique_ptr<BYTE[]> buffer = std::make_unique<BYTE[]>(m_header.Size);
    
    if (!DeviceIoControl(
        m_deviceHandle,
        IOCTL_STORAGE_QUERY_PROPERTY,
        &query,
        sizeof(query),
        buffer.get(),
        m_header.Size,
        &bytesReturned,
        NULL
    ))
    {
        return false;
    }

    m_descriptor = std::unique_ptr<STORAGE_DEVICE_DESCRIPTOR>(
        reinterpret_cast<STORAGE_DEVICE_DESCRIPTOR*>(buffer.release())
    );

    printf("Vendor: %d\n", m_descriptor->DeviceType);

    return true;
}