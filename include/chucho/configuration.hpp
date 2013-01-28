#if !defined(CHUCHO_CONFIGURATION_HPP__)
#define CHUCHO_CONFIGURATION_HPP__

#include <chucho/export.hpp>
#include <string>

namespace chucho
{

namespace configuration
{

enum class style
{
    AUTOMATIC,
    MANUAL,
    OFF
};

CHUCHO_EXPORT bool allow_default();
CHUCHO_EXPORT const std::string& get_file_name();
CHUCHO_EXPORT style get_style();
void perform();
CHUCHO_EXPORT void set_allow_default(bool allow);
CHUCHO_EXPORT void set_file_name(const std::string& name);
CHUCHO_EXPORT void set_style(style stl);

}

}

#endif
