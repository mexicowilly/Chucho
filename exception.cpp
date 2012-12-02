#include <chucho/exception.hpp>

namespace chucho
{

exception::exception(const std::string& msg)
    : message_(msg)
{
}

const char* exception::what() const noexcept
{
    return message_.c_str();
}

pattern_exception::pattern_exception(const std::string& msg)
    : exception(msg)
{
}

file_exception::file_exception(const std::string& msg)
    : exception(msg)
{
}

time_file_roller_exception::time_file_roller_exception(const std::string& msg)
    : exception(msg)
{
}

}
