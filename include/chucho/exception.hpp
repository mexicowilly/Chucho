#if !defined(CHUCHO_EXCEPTION_HPP__)
#define CHUCHO_EXCEPTION_HPP__

#if !defined(chucho_shared_EXPORTS)
#error "This header is private"
#endif

#include <chucho/export.hpp>
#include <exception>
#include <string>

namespace chucho
{

class CHUCHO_EXPORT exception : public std::exception
{
public:
    static std::string nested_whats(const std::exception& e);

    exception(const std::string& msg);

    virtual const char* what() const noexcept override;

protected:
    std::string message_;
};

}

#endif
