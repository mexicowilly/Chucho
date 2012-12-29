#include <chucho/level_threshold_filter_memento.hpp>
#include <chucho/exception.hpp>

namespace chucho
{

level_threshold_filter_memento::level_threshold_filter_memento(const configurator& cfg)
    : memento(cfg)
{
    set_status_origin("level_threshold_filter_memento");
    set_handler("level", [this] (const std::string& name) { level_ = level::from_text(name); });
}

}
