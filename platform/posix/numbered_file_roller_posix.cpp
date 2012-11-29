#include <chucho/numbered_file_roller.hpp>
#include <unistd.h>

namespace chucho
{

bool numbered_file_roller::exists(const std::string& file_name) const
{
    return access(file_name.c_str(), F_OK) == 0;
}

}
