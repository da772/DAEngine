
#include "network_util.h"


#if defined (DA_PLATFORM_LINUX)
#include <arpa/inet.h>
#endif

#ifdef DA_PLATFORM_MACOSX
#include <arpa/inet.h>
#endif

#if defined(_WIN32)
#include <locale>
#include <codecvt>
#include <WS2tcpip.h>
#include <algorithm>
#include <libloaderapi.h>
#include <windows.h>
#include <errhandlingapi.h>
#include <process.h>
#define MS_xstr(a) MS_str(a)
#define MS_str(a) #a
#endif


namespace da::net
{

	std::string CNetworkUtil::IPV4ToString(uint32_t ip)
	{

#ifdef DA_PLATFORM_WINDOWS
		char ch[255];
		inet_ntop(AF_INET, &ip, ch, 255);
		return std::string(ch);
#else 
		char ch[255];
		inet_ntop(AF_INET, &ip, ch, 255);
		return std::string(ch);
#endif
	}

}