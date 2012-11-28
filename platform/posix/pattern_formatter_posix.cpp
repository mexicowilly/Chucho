#include <chucho/pattern_formatter.hpp>
#include <sys/utsname.h>
#include <sys/resource.h>
#include <unistd.h>

namespace chucho
{

pattern_formatter::base_host_piece::base_host_piece(justification just,
                                                    std::size_t min_width,
                                                    std::size_t max_width)
    : piece(just, min_width, max_width)
{
    struct utsname info;
    uname(&info);
    name_ = info.nodename;
}

std::string pattern_formatter::milliseconds_since_start_piece::get_text_impl(const event& evt) const
{
    std::string result;
    struct rusage info;
    if (getrusage(RUSAGE_SELF, &info) == 0)
        result = std::to_string(info.ru_utime.tv_sec * 1000 + info.ru_utime.tv_usec / 1000);
    return result;
}

std::string pattern_formatter::pid_piece::get_text_impl(const event& evt) const
{
    return std::to_string(getpid());
}

}
