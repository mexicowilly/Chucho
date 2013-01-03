#include <chucho/file_exception.hpp>

namespace chucho
{

file_exception::file_exception(const std::string& msg)
    : exception(msg)
{
}

}
