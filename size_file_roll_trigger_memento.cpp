#include <chucho/size_file_roll_trigger_memento.hpp>
#include <chucho/exception.hpp>
#include <cctype>
#include <sstream>
#include <algorithm>

namespace chucho
{

size_file_roll_trigger_memento::size_file_roll_trigger_memento(const configurator& cfg)
    : memento(cfg)
{
    set_status_origin("size_file_roll_trigger_memento");
    set_handler("max_size", std::bind(&size_file_roll_trigger_memento::parse, this, std::placeholders::_1));
}

void size_file_roll_trigger_memento::parse(const std::string& spec)
{
    if (spec.empty() || !std::isdigit(spec[0]))
        throw exception(get_status_origin() + ": The size specification must start with a digit");
    std::istringstream stream(spec);
    unsigned long long tmp;
    stream >> tmp;
    std::string suffix;
    stream >> suffix;
    if (!suffix.empty())
    {
        std::string lower;
        std::locale c_loc("C");
        std::transform(suffix.begin(),
                       suffix.end(),
                       std::back_inserter(lower),
                       [&] (char c) { return std::tolower(c, c_loc); });
        if (lower.length() > 2 ||
            (lower.length() == 2 && lower[1] != 'b') ||
            (lower.length() == 2 && lower[0] == 'b') ||
            (std::string("bkmg").find(lower[0]) == std::string::npos))
        {
            throw exception(get_status_origin() + ": The suffix '" + suffix + "' is invalid (case-insensitive b, k[b], m[b], g[b])");
        }
        switch (lower[0])
        {
        case 'b':
            break;
        case 'k':
            tmp *= 1024;
            break;
        case 'm':
            tmp *= 1024 * 1024;
            break;
        case 'g':
            tmp *= 1024 * 1024 * 1024;
            break;
        }
    }
    max_size_ = tmp;
}

}
