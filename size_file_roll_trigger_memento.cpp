#include <chucho/size_file_roll_trigger_memento.hpp>

namespace chucho
{

size_file_roll_trigger_memento::size_file_roll_trigger_memento(const configurator& cfg)
    : memento(cfg)
{
    set_handler("max_size", [this] (const std::string& val) { max_size_ = std::stoull(val); });
}

}
