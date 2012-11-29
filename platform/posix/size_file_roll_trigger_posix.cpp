#include <chucho/size_file_roll_trigger.hpp>
#include <chucho/file_writer.hpp>
#include <sys/stat.h>

namespace chucho
{

unsigned long long size_file_roll_trigger::size(const std::string& file_name) const
{
    struct stat info;
    if (stat(file_name.c_str(), &info) != 0)
        throw file_exception("Could not get size of " + file_name);
    return info.st_size;
}

}
