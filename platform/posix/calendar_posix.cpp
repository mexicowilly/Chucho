#include <chucho/calendar.hpp>
#include <mutex>

#if !defined(CHUCHO_HAVE_GMTIME_R) || !defined(CHUCHO_HAVE_LOCALTIME_R)

namespace
{

std::mutex calendar_guard;

}

#endif

namespace chucho
{

namespace calendar
{

struct std::tm get_local(std::time_t t)
{
    #if defined(CHUCHO_HAVE_LOCALTIME_R)
    struct std::tm cal;
    return *::localtime_r(&t, &cal);
    #else
    std::lock_guard<std::mutex> lg(calendar_guard);
    return *std::localtime(t);
    #endif
}

struct std::tm get_utc(std::time_t t)
{
    #if defined(CHUCHO_HAVE_GMTIME_R)
    struct std::tm cal;
    return *::gmtime_r(&t, &cal);
    #else
    std::lock_guard<std::mutex> lg(calendar_guard);
    return *std::gmtime(t);
    #endif
}

}

}
