#include <chucho/calendar.hpp>
#include <mutex>
#include <sstream>
#include <vector>
#include <iomanip>

#if !defined(CHUCHO_HAVE_GMTIME_R) || !defined(CHUCHO_HAVE_LOCALTIME_R)

namespace
{

std::mutex calendar_guard;

}

#endif

namespace
{

const std::size_t FORMAT_BUF_SIZE_MAX = 20 * 1024;

}

namespace chucho
{

namespace calendar
{

std::string format(const struct std::tm& cal, const std::string& pattern)
{
    #if defined(CHUCHO_HAVE_PUT_TIME)
    std::ostringstream stream;
    stream << std::put_time(&cal, pattern.c_str());
    return stream.str();
    #else
    std::vector<char> buf(1024);
    std::string result;
    while (true)
    {
        std::size_t rc = std::strftime(&buf[0],
                                       buf.size(),
                                       pattern.c_str(),
                                       &cal);
        if (rc > 0)
        {
            result = &buf[0];
            break;
        }
        if (buf.size() > FORMAT_BUF_SIZE_MAX)
            break;
        buf.resize(buf.size() + 1024);
    }
    return result;
    #endif
}

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
