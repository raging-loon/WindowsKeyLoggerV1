#include "WmiSubscriber.h"
#include <wbemidl.h>
#include <comdef.h>
#include <oleauto.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <sddl.h>

#pragma comment(lib, "wbemuuid.lib")
#pragma comment(lib, "ole32.lib")
#define _ITERATOR_DEBUG_LEVEL 2
static BOOL LaunchExec(const std::wstring& location)
{
    HANDLE token;
    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &token))
        return FALSE;

    TOKEN_PRIVILEGES tp{ .PrivilegeCount = 1 };
    LookupPrivilegeValue(nullptr, SE_DEBUG_NAME, &tp.Privileges[0].Luid);

    tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

    if (!AdjustTokenPrivileges(token, FALSE, &tp, sizeof(tp), NULL, NULL))
    {
        CloseHandle(token);
        return FALSE;
    }

    STARTUPINFO si = { 0 };
    PROCESS_INFORMATION pi = { 0 };

    si.cb = sizeof(si);

    if (!CreateProcessAsUser(token,
        NULL,
        (LPWSTR)location.c_str(),
        NULL, NULL,
        TRUE,
        0,
        NULL, NULL,
        &si, &pi
    )) {
        CloseHandle(token);
        return FALSE;
    }

    CloseHandle(token);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return TRUE;
}

class LogonEventSink : public IWbemEventSink
{
public:
    LogonEventSink() : m_lRef(1) {}
    ~LogonEventSink() {}

    STDMETHOD(QueryInterface)(REFIID riid, void** ppv)
    {
        if (riid == IID_IUnknown || riid == IID_IWbemObjectSink) {
            *ppv = static_cast<IWbemObjectSink*>(this);
            AddRef();
            return S_OK;
        }
        *ppv = nullptr;
        return E_NOINTERFACE;
    }

    STDMETHOD_(ULONG, AddRef)()
    {
        return InterlockedIncrement(&m_lRef);
    }

    STDMETHOD_(ULONG, Release)()
    {
        LONG lRef = InterlockedDecrement(&m_lRef);
        if (lRef == 0) {
            delete this;
        }
        return lRef;
    }

    STDMETHOD(OnObjectArrived)(long lObjectCount, IWbemClassObject** objs, long flags)
    {
        LaunchExec(L"C:\\Program Files\\WindowsDiskMonitor.exe");
        return S_OK;
    }

    STDMETHOD(OnObjectDeleted)(long lObjectCount, IWbemClassObject** ppColObjects) 
    {
        return S_OK;
    }

    STDMETHOD(OnTimer)(long lFlags) 
    {
        return S_OK;
    }

    STDMETHOD(OnError)(long lFlags, HRESULT hResul, const BSTR strParam, const BSTR strText) 
    {
        return S_OK;
    }

    // Inherited via IWbemEventSink
    HRESULT __stdcall Indicate(long lObjectCount, IWbemClassObject** apObjArray) override
    {
        return E_NOTIMPL;
    }
    HRESULT __stdcall SetStatus(long lFlags, HRESULT hResult, BSTR strParam, IWbemClassObject* pObjParam) override
    {
        return E_NOTIMPL;
    }
    HRESULT __stdcall SetSinkSecurity(long lSDLength, BYTE* pSD) override
    {
        return E_NOTIMPL;
    }
    HRESULT __stdcall IsActive(void) override
    {
        return S_OK;
    }
    HRESULT __stdcall GetRestrictedSink(long lNumQueries, const LPCWSTR* awszQueries, IUnknown* pCallback, IWbemEventSink** ppSink) override
    {
        return E_NOTIMPL;
    }
    HRESULT __stdcall SetBatchingParameters(LONG lFlags, DWORD dwMaxBufferSize, DWORD dwMaxSendLatency) override
    {
        return E_NOTIMPL;
    }

private:
    LONG m_lRef; // Reference count
};

WmiSubscriber::WmiSubscriber()
    : m_loc(nullptr), m_svcs(nullptr), m_sink(new LogonEventSink)
{
    HRESULT hr = CoInitializeEx(0, COINIT_MULTITHREADED);

    if (FAILED(hr))
        return;

    hr = CoInitializeSecurity(
        NULL,
        -1,
        NULL,
        NULL,
        RPC_C_AUTHN_LEVEL_DEFAULT,
        RPC_C_IMP_LEVEL_IMPERSONATE,
        NULL,
        EOAC_NONE,
        NULL
    );

    if (FAILED(hr))
        return;


    hr = CoCreateInstance(
        CLSID_WbemLocator,
        0,
        CLSCTX_INPROC_SERVER,
        IID_IWbemLocator,
        (void**)&m_loc
    );

    if (FAILED(hr))
        return;

    hr = m_loc->ConnectServer(
        _bstr_t(L"ROOT\\CIMV2"),
        NULL, NULL,
        0, 0, 0, 0, &m_svcs
    );

    if (FAILED(hr))
    {
        m_loc->Release();
        CoUninitialize();
    }

    hr = CoSetProxyBlanket(
        m_svcs,
        RPC_C_AUTHN_WINNT,
        RPC_C_AUTHZ_NONE,
        NULL,
        RPC_C_AUTHN_LEVEL_CALL,
        RPC_C_IMP_LEVEL_IMPERSONATE,
        NULL,
        EOAC_NONE
    );

    if (FAILED(hr))
    {
        m_svcs->Release();
        m_loc->Release();
        CoUninitialize();
    }

}

WmiSubscriber::~WmiSubscriber()
{
    if (m_svcs)
    {
        m_svcs->Release();
        m_svcs = nullptr;
    }

    if (m_loc)
    {
        m_loc->Release();
        m_loc = nullptr;
    }
    CoUninitialize();
}

bool WmiSubscriber::subscribe(const std::wstring& location)
{
    HRESULT hr;
    hr = m_svcs->ExecNotificationQueryAsync(
        _bstr_t(L"WQL"),
        _bstr_t(L"SELECT * FROM __InstanceCreationEvent WITHIN 5 WHERE TargetInstance ISA 'Win32_LogonSession'"),
        WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_SEND_STATUS,
        NULL,
        m_sink
    );

    if (FAILED(hr))
        return false;


    return true;
}
