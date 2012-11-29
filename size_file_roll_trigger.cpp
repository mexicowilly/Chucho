#include <chucho/size_file_roll_trigger.hpp>

namespace chucho
{

size_file_roll_trigger::size_file_roll_trigger(unsigned long long max_size)
    : max_size_(max_size)
{
}

bool size_file_roll_trigger::is_triggered(const std::string& active_file, const event& e)
{
    return size(active_file) >= max_size_;
}

}
