#include <chucho/host.hpp>
#include <sys/utsname.h>
#include <netdb.h>
#include <cstring>

namespace chucho
{

void host::get_base_impl(std::string& result)
{
    struct utsname info;
    uname(&info);
    result = info.nodename;
}

void host::get_full_impl(std::string& result)
{
    std::string full;
    get_base_impl(full);
    struct addrinfo hints;
    std::memset(&hints, 0, sizeof(hints));
    hints.ai_family = PF_UNSPEC;
    hints.ai_flags = AI_CANONNAME;
    struct addrinfo* info;
    if (getaddrinfo(full.c_str(), nullptr, &hints, &info) == 0)
    {
        full = info->ai_canonname;
        freeaddrinfo(info);
    }
    result = full;
}

}
