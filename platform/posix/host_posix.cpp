#include <chucho/host.hpp>
#include <sys/utsname.h>
#include <netdb.h>
#include <cstring>

namespace chucho
{

void host::get_base_impl()
{
    struct utsname info;
    uname(&info);
    base_ = info.nodename;
}

void host::get_full_impl()
{
    get_base_impl();
    std::string result = base_;
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
    full_ = result;
}

}
