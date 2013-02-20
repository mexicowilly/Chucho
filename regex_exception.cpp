#include <chucho/regex_exception.hpp>

namespace chucho
{

regex_exception::regex_exception(const std::string& msg)
    : exception(msg)
{
}

}
