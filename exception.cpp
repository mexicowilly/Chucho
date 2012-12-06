#include <chucho/exception.hpp>
#include <sstream>

namespace
{

void nested_what_handler(std::ostream& stream, int level, const std::exception& e)
{
    if (level > 1)
        stream << ' ';
    stream << "{ " << level << ": " << e.what() << "}";
    try
    {
        std::rethrow_if_nested(e);
    }
    catch (std::exception& ne)
    {
        nested_what_handler(stream, level + 1, ne);
    }
}

}

namespace chucho
{

exception::exception(const std::string& msg)
    : message_(msg)
{
}

std::string exception::nested_whats(const std::exception& e)
{
    std::ostringstream stream;
    nested_what_handler(stream, 1, e);
    return stream.str();
}

const char* exception::what() const noexcept
{
    return message_.c_str();
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
