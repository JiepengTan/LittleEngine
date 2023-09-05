#include "Guid.h"

#include <stdio.h>
#include <stdint.h>
#include <string>

#if defined(_WIN32)
#include <objbase.h>
#elif defined(__linux__)
#include <Guid/Guid.h>
#else
#error "Guid unsupport platform"
#endif

#define GUID_LEN 64
std::string OvCore::Guid::ToString()
{
    char buf[GUID_LEN] = {0};
    sprintf(buf,
        "%08x%04x%04x%02x%02x%02x%02x%02x%02x%02x%02x",
        //"%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x",
        Data1, Data2, Data3,
        Data4[0], Data4[1], Data4[2],
        Data4[3], Data4[4], Data4[5],
        Data4[6], Data4[7]);
    return std::move(std::string(buf));
}
OvCore::Guid OvCore::Guid::CreateGuid()
{
    Guid guid;
#ifdef _WIN32
    CoCreateGuid((GUID*)&guid);
#elif __linux__
    char buf[GUID_LEN] = { 0 };
    uuid_t uu;
    uuid_generate(uu);
    guid = *((Guid*)&uu);
#endif // WIN32
    return guid;
}
OvCore::Guid OvCore::Guid::CreateGuid(const std::string& str)
{
    Guid stGuid;
    const char* cstr = str.c_str();
    sscanf(cstr, "%8x%4x%4x%2x%2x%2x%2x%2x%2x%2x%2x",
       &(stGuid.Data1),&(stGuid.Data2),&(stGuid.Data3),
       &(stGuid.Data4[0]),&(stGuid.Data4[1]),&(stGuid.Data4[2]),&(stGuid.Data4[3]),
       &(stGuid.Data4[4]),&(stGuid.Data4[5]),&(stGuid.Data4[6]),&(stGuid.Data4[7]));
    return stGuid;
}
