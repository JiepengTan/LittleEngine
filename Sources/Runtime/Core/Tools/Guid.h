#pragma once
#include <string>

namespace OvCore
{
    class Guid
    {
    public :
        unsigned long  Data1;
        unsigned short Data2;
        unsigned short Data3;
        unsigned char  Data4[ 8 ];
    public:
        static Guid CreateGuid();
        static Guid CreateGuid(const std::string& str);
        
        std::string ToString();
    };
}
