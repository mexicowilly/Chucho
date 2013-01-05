#include <chucho/time_file_roller_memento.hpp>

namespace chucho
{

time_file_roller_memento::time_file_roller_memento(const configurator& cfg)
    : memento(cfg)
{
    set_status_origin("tile_file_roller_memento");
    set_handler("file_name_pattern", [this] (const std::string& val) { file_name_pattern_ = val; });
    set_handler("max_history", [this] (const std::string& val) { max_history_ = std::stoul(val); });
}

}
