#include <chucho/numbered_file_roller_memento.hpp>

namespace chucho
{

numbered_file_roller_memento::numbered_file_roller_memento(const configurator& cfg)
    : memento(cfg)
{
    set_status_origin("numbered_file_roller_memento");
    set_handler("min_index", [this] (const std::string& idx) { min_index_.reset(new int(std::stoi(idx))); });
    set_handler("max_index", [this] (const std::string& idx) { max_index_.reset(new int(std::stoi(idx))); });
}

}
