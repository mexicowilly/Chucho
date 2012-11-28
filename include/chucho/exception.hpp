#if !defined(CHUCHO_EXCEPTION_HPP__)
#define CHUCHO_EXCEPTION_HPP__

#include <chucho/export.hpp>
#include <exception>
#include <string>

namespace chucho
{

class CHUCHO_EXPORT exception : public std::exception
{
public:
    exception(const std::string& msg);

    virtual const char* what() const noexcept override;

private:
    std::string message_;
};

}

#endif
