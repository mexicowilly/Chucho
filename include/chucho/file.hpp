#if !defined(CHUCHO_FILE_HPP__)
#define CHUCHO_FILE_HPP__

#if !defined(chucho_EXPORTS)
#error "This header is private"
#endif

#include <chucho/export.hpp>
#include <string>

namespace chucho
{

namespace file
{

CHUCHO_EXPORT extern const char dir_sep;

CHUCHO_EXPORT std::string base_name(const std::string& name);
CHUCHO_EXPORT void create_directories(const std::string& name);
CHUCHO_EXPORT void create_directory(const std::string& name);
CHUCHO_EXPORT std::string directory_name(const std::string& name);
CHUCHO_EXPORT bool exists(const std::string& name);
CHUCHO_EXPORT bool is_fully_qualified(const std::string& name);
CHUCHO_EXPORT void remove(const std::string& name);
CHUCHO_EXPORT void remove_all(const std::string& name);
CHUCHO_EXPORT unsigned long long size(const std::string& name);

}

}

#endif
