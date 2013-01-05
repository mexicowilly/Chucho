#if !defined(CHUCHO_DEMANGLE_HPP__)
#define CHUCHO_DEMANGLE_HPP__

#if !defined(chucho_shared_EXPORTS)
#error "This header is private"
#endif

#include <string>

namespace chucho
{

namespace demangle
{

std::string get_demangled_name(const std::type_info& info);

}

}

#endif
