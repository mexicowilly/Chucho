#include <chucho/host.hpp>
#include <sys/utsname.h>
#include <netdb.h>
#include <cstring>

namespace chucho
{

namespace host
{

std::string get_base_name()
{
    struct utsname info;
    uname(&info);
    return info.nodename;
}

std::string get_full_name()
{
    std::string result = get_base_name();
    struct addrinfo hints;
    std::memset(&hints, 0, sizeof(hints));
    hints.ai_family = PF_UNSPEC;
    hints.ai_flags = AI_CANONNAME;
    struct addrinfo* info;
    if (getaddrinfo(result.c_str(), nullptr, &hints, &info) == 0)
    {
        result = info->ai_canonname;
        freeaddrinfo(info);
    }
    return result;
}

}

}
