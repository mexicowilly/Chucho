#include <chucho/filter_memento.hpp>

namespace chucho
{

filter_memento::filter_memento(configurator& cfg)
    : memento(cfg)
{
    set_status_origin("filter_memento");
    cfg.get_security_policy().set_text("filter::name", 256);
    set_handler("name", [this] (const std::string& name) { name_  = validate("filter::name", name); });
}

}