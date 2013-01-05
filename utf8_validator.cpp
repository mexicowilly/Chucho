#include <chucho/utf8_validator.hpp>
#include <chucho/exception.hpp>
#include "utf8.h"
#include <string>

namespace chucho
{

namespace utf8
{

void validate(std::istream& stream)
{
    std::string line;
    int cur = 0;
    while (std::getline(stream, line))
    {
        ++cur;
        auto invalid = ::utf8::find_invalid(line.begin(), line.end());
        if (invalid != line.end())
        {
            throw exception("Found invalid UTF-8 at line " + std::to_string(cur) + ", column " +
                std::to_string(std::distance(line.begin(), invalid)));
        }
    }
}

}

}
