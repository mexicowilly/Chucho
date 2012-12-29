#include <chucho/logger_memento.hpp>

namespace chucho
{

logger_memento::logger_memento(const configurator& cfg)
    : memento(cfg)
{
    set_status_origin("logger_memento");
    set_handler("level", [this] (const std::string& name) { level_ = level::from_text(name); });
    set_handler("name", [this] (const std::string& name) {  name_ = name; });
    set_handler("writer_id", [this] (const std::string& name) { writers_.push_back(cfg_.get_writer(name)); });
    set_handler("writes_to_ancestors", [this] (const std::string& name) { writes_to_ancestors_.reset(new bool(boolean_value(name))); });
}

}
