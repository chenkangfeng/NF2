
#include "cfprecompiled.h"
#include "evutil.h"
#include "cfcnetdns.h"
#include "include/cfplatform.h"
#include "kernel/cfsharedptr.hpp"

NS_CF_BEGIN

CFCNetDNS::CFCNetDNS(void)
{
#if CF_PLATFORM(CF_PLATFORM_WIN)
    WSADATA wsa_data;
    WSAStartup(MAKEWORD(2, 2), &wsa_data);
#endif
}

CFCNetDNS::~CFCNetDNS(void)
{
}

CFBool CFCNetDNS::parse(Protocol protocol, CFStrPtr domain, Callback callback)
{
    evutil_addrinfo hints, *result = nullptr;

    // init hints
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_flags = EVUTIL_AI_CANONNAME;
    switch (protocol)
    {
    case kTCP:
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_protocol = IPPROTO_TCP;
        break;
    case kUDP:
        hints.ai_socktype = SOCK_DGRAM;
        hints.ai_protocol = IPPROTO_UDP;
        break;
    default:
        break;
    }

    CFBool ret = false;
    CFINetAddrInfo::SharedPtr addr_info = CFINetAddrInfo::createComponent();
    if (addr_info) {
        // parse domain
        ret = (evutil_getaddrinfo(domain, nullptr, &hints, &result) == 0);
        if (ret){
            addr_info->addAddrInfo(result);
            evutil_freeaddrinfo(result);
        }
    }
    if (callback) {
        callback(std::move(addr_info));
    }
    return ret;
}

NS_CF_END
