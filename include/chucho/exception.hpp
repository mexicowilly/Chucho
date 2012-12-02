#if !defined(CHUCHO_EXCEPTION_HPP__)
#define CHUCHO_EXCEPTION_HPP__

#if !defined(chucho_EXPORTS)
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
    exception(const std::string& msg);

    virtual const char* what() const noexcept override;

private:
    std::string message_;
};

class CHUCHO_EXPORT pattern_exception : public exception
{
public:
    pattern_exception(const std::string& msg);
};

class CHUCHO_EXPORT file_exception : public exception
{
public:
    file_exception(const std::string& msg);
};

class CHUCHO_EXPORT time_file_roller_exception : public exception
{
public:
    time_file_roller_exception(const std::string& msg);
};

}

#endif
