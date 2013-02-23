#if !defined(CHUCHO_FILE_EXCEPTION_HPP__)
#define CHUCHO_FILE_EXCEPTION_HPP__

#if !defined(chucho_EXPORTS)
#error "This header is private"
#endif

#include <chucho/exception.hpp>

namespace chucho
{

class CHUCHO_EXPORT file_exception : public exception
{
public:
    file_exception(const std::string& msg);
};

}

#endif
