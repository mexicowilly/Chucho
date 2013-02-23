#if !defined(CHUCHO_PRINTF_UTIL_HPP__)
#define CHUCHO_PRINTF_UTIL_HPP__

#if !defined(chucho_EXPORTS)
#error "This header if private"
#endif

#include <string>

namespace chucho
{

namespace printf
{

template <typename rep_type>
inline std::string get_milli_format()
{
}

template <>
inline std::string get_milli_format<long long int>()
{
    return "%03lli";
}

template <>
inline std::string get_milli_format<long int>()
{
    return "%03li";
}

}

}

#endif
