#if !defined(CHUCHO_CALENDAR_HPP__)
#define CHUCHO_CALENDAR_HPP__

#if !defined(chucho_shared_EXPORTS)
#error "This header is private"
#endif

#include <chucho/export.hpp>
#include <ctime>
#include <string>

namespace chucho
{

namespace calendar
{

CHUCHO_EXPORT std::string format(const struct std::tm& cal,
                                 const std::string& pattern);
CHUCHO_EXPORT struct std::tm get_local(std::time_t t);
CHUCHO_EXPORT struct std::tm get_utc(std::time_t t);

}

}

#endif
