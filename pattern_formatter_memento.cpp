#include <chucho/pattern_formatter_memento.hpp>

namespace chucho
{

pattern_formatter_memento::pattern_formatter_memento(const configurator& cfg)
    : memento(cfg)
{
    set_status_origin("pattern_formatter_memento");
    set_handler("pattern", [this] (const std::string& pat) { pattern_ = pat; });
}

}
