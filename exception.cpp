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

}
