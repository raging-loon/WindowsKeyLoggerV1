#ifndef WMI_SUBSCRIBER_H_
#define WMI_SUBSCRIBER_H_

#include <windows.h>
#include <WbemCli.h>
#include <WbemProv.h>
#include <string>


class WmiSubscriber
{
public:
    WmiSubscriber();
    ~WmiSubscriber();

    bool subscribe(const std::wstring& location);
private:
    IWbemLocator*       m_loc;
    IWbemServices*      m_svcs;
    IWbemEventSink* m_sink{ nullptr };

};


#endif // WMI_SUBSCRIBER_H_

