#if !defined(CHUCHO_REGEX_EXCEPTION_HPP__)
#define CHUCHO_REGEX_EXCEPTION_HPP__

#if !defined(chucho_EXPORTS)
#error "This header is private"
#endif

#include <chucho/exception.hpp>

namespace chucho
{

class CHUCHO_EXPORT regex_exception : public exception
{
public:
    regex_exception(const std::string& msg);
};

}

#endif

