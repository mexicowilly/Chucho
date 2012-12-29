#include <chucho/demangle.hpp>
#if defined(__clang__) || defined(__GNUC__)
#include <cxxabi.h>
#include <stdlib.h>
#endif

namespace chucho
{

namespace demangle
{

std::string get_demangled_name(const std::type_info& info)
{
#if defined(__clang__) || defined(__GNUC__)
    int status;
    char* demangled = ::abi::__cxa_demangle(info.name(), nullptr, nullptr, &status);
    if (status != 0)
        return info.name();
    std::string result(demangled);
    free(demangled);
    return result;
#else
    return info.name();
#endif
}

}

}
