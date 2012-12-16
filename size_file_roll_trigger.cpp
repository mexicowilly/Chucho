#include <chucho/size_file_roll_trigger.hpp>
#include <chucho/file.hpp>

namespace chucho
{

size_file_roll_trigger::size_file_roll_trigger(unsigned long long max_size)
    : max_size_(max_size)
{
    set_status_origin("size_file_roll_trigger");
}

bool size_file_roll_trigger::is_triggered(const std::string& active_file, const event& e)
{
    return file::exists(active_file) && file::size(active_file) >= max_size_;
}

}
